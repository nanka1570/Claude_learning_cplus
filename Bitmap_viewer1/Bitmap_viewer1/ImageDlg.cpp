// ImageDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "Resource.h"
#include "Bitmap_viewer1Dlg.h"
#include "ImageDlg.h"
#include <vector>
#include <Gdiplusimaging.h>

// CImageDlg ダイアログ
IMPLEMENT_DYNAMIC(CImageDlg, CDialogEx)

CImageDlg::CImageDlg(CWnd* pParent)
	: CDialogEx(IDD_IMAGE_DIALOG, pParent)
{

}

CImageDlg::~CImageDlg()
{
}

BEGIN_MESSAGE_MAP(CImageDlg, CDialogEx)
	ON_MESSAGE(CBitmapviewer1Dlg::WM_DRAWBITMAP, OnDrawBitmap)
	ON_MESSAGE(CBitmapviewer1Dlg::WM_ROTATEBITMAP, OnDrawRotate)
	ON_MESSAGE(CBitmapviewer1Dlg::WM_SAVEBITMAPFILE, OnSaveBitmapFile)
	ON_WM_SIZING()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BITMAP2, m_ctrlBitmap2);
}

//初期化
BOOL CImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ctrlBitmap2.SetBitmap(nullptr);
	m_imageView.Destroy();
	m_image.Destroy();
	m_imageTemp.Destroy();

	DrawBitmap();

	printf("");
	return TRUE;
}

void CImageDlg::OnOK()
{
	//CDialog::OnOK();
}

void CImageDlg::OnCancel()
{
	//CDialog::OnCancel();
}

void CImageDlg::PostNcDestroy()
{

	CDialogEx::PostNcDestroy();
	delete this;
}

void CImageDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
}

void CImageDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (!m_image.IsNull()) {

		resizeBitmap(cx, cy);
		ShowWindow(SW_SHOW);
	}

}


//Bitmap_viewer1Dlg.cppからもらったファイルをセットする
void CImageDlg::setBitmapFile(std::filesystem::path filename)
{
	bitmapFilename = filename;
	return;
}

LRESULT CImageDlg::OnDrawBitmap(WPARAM wParam, LPARAM lParam)
{
	//オリジナルをロード
	m_image.Load(bitmapFilename.native().c_str());
	//if (!m_imageView.IsNull())
	//{
	// m_imageView.Destroy();
	//}
	//m_ctrlBitmap2.SetBitmap(m_image);

	const auto srcWidth = m_image.GetWidth();
	const auto srcHeight = m_image.GetHeight();
	const auto bpp = m_image.GetBPP();

	if (!m_imageView.IsNull()) {
		m_imageView.Destroy();
	}
	const auto targetWidth = srcWidth;
	const auto targetHeight = srcHeight;

	//描画するキャンバスを用意 Create(x, y, bpp, 0)
	m_imageView.Create(targetWidth, targetHeight, bpp);
	hDC = m_imageView.GetDC();
	m_image.BitBlt(hDC, 0, 0, SRCCOPY);
	//m_image.StretchBlt(hDC, 0, 0, targetWidth, targetHeight, SRCCOPY);
	m_imageView.ReleaseDC();

	 //リサイズ
	resizeBitmap(targetWidth, targetHeight);

	m_ctrlBitmap2.SetBitmap(m_imageView);

	ShowWindow(SW_SHOW);

	return 0;
	printf("");
}


LRESULT CImageDlg::OnDrawRotate(WPARAM wParam, LPARAM lParam)
{
	BUTTONS button = static_cast<BUTTONS>(wParam);
	DrawRotate(button);
	return 0;
}

LRESULT CImageDlg::OnSaveBitmapFile(WPARAM wParam, LPARAM lParam)
{
	CFileDialog dlg(FALSE);
	if (dlg.DoModal() == IDOK)
	{
		m_strSaveBitmapFilename = dlg.GetPathName();

		UpdateData(FALSE);

		ImageSave();
	}
	return 0;
}


void CImageDlg::resizeBitmap(const int width, const int height)
{
	if (m_image.IsNull()) {
		return;
	}

	if (width <= 0 || height <= 0) {
		return;
	}

	CRect windowRectBitmap;
	CRect clientRectBitmap;
	GetWindowRect(windowRectBitmap);
	GetClientRect(clientRectBitmap);
	ClientToScreen(clientRectBitmap);

	//左上と幅・高さのWindowとClientの差を求める
	const auto marginLeft = clientRectBitmap.left - windowRectBitmap.left;
	const auto marginTop = clientRectBitmap.top - windowRectBitmap.top;

	const auto borderWidth = (windowRectBitmap.Width() - clientRectBitmap.Width());
	const auto borderHeight = (windowRectBitmap.Height() - clientRectBitmap.Height());

	//ビットマップと同じ大きさでダイアログを表示する
	MoveWindow(windowRectBitmap.left, windowRectBitmap.top, width + borderWidth, height + borderHeight, FALSE);

	ScreenToClient(clientRectBitmap);
	//m_ctrlBitmap2.ShowWindow(SW_HIDE);
	m_ctrlBitmap2.MoveWindow(clientRectBitmap.left, clientRectBitmap.top, width, height, FALSE);

	//ToDo
	//	m_imageTemp.BitBlt(m_image.GetDC(), 0, 0, SRCCOPY);
	//	m_image.ReleaseDC();
	if (!m_imageTemp.IsNull())
	{
		m_imageTemp.Destroy();
	}

	m_imageTemp.Create(width, height, m_image.GetBPP());
	hDC = m_imageTemp.GetDC();
	m_image.StretchBlt(hDC, 0, 0, width, height, SRCCOPY);
	m_imageTemp.ReleaseDC();

	if (!m_imageView.IsNull())
	{
		m_imageView.Destroy();
	}

	m_imageView.Create(width, height, m_imageTemp.GetBPP());
	hDC = m_imageView.GetDC();
	m_imageTemp.BitBlt(hDC, 0, 0, SRCCOPY);
	m_imageView.ReleaseDC();

	//m_imageTemp.PlgBlt(hDC, nullptr);

	m_ctrlBitmap2.SetBitmap(m_imageView);

	m_ctrlBitmap2.ShowWindow(SW_SHOW);
}


//DrawBitmap使用
void CImageDlg::DrawBitmap()
{
	PostMessage(CBitmapviewer1Dlg::WM_DRAWBITMAP, 0, 0);
	return;
}

////「名前を付けて保存」する
//void CImageDlg::OnBnClickedButtonBitmapfileSave()
//{
//	CFileDialog dlg(FALSE);
//	if (dlg.DoModal() == IDOK)
//	{
//		m_strSaveBitmapFilename = dlg.GetPathName();
//
//		UpdateData(FALSE);
//
//		ImageSave();
//	}
//	printf("");
//}

void CImageDlg::ImageSave()
{
	m_imageView.Save(m_strSaveBitmapFilename.GetString(), Gdiplus::ImageFormatBMP);
	return;
}

void CImageDlg::DrawRotate(const BUTTONS button)
{
	const auto srcWidth = m_imageView.GetWidth();
	const auto srcHeight = m_imageView.GetHeight();
	auto newWidth = 0;
	auto newHeight = 0;

	//画像が横になる場合
	if (button == BUTTON_90 || button == BUTTON_270) {
		newWidth = srcHeight;
		newHeight = srcWidth;
	}
	else {
		newWidth = srcWidth;
		newHeight = srcHeight;
	}

	//画像が横になる場合は横長のキャンバスにする
	if (srcWidth != newWidth) {
		m_imageTemp.Destroy();
	}
	if (m_imageTemp.IsNull()) {
		m_imageTemp.Create(newWidth, newHeight, m_imageView.GetBPP());
	}
#if 0
	POINT points[3] = { {0, 0}, {1, 0}, {0, 1} }; //デフォルト（0）
	POINT points[3] = { {0, 1}, {0, 0}, {1, 1} }; //90
	POINT points[3] = { {1, 1}, {0, 1}, {1, 0} }; //180
	POINT points[3] = { {1, 0}, {1, 1}, {0, 0} }; //270
	POINT points[3] = { {0, 1}, {1, 1}, {0, 0} }; //H
	POINT points[3] = { {1, 0}, {0, 0}, {1, 1} }; //V
#endif

	auto hDC = m_imageTemp.GetDC();
	POINT points[3] = {};
	if (button == BUTTON_90) {
		points[0] = { 1, 0 };
		points[1] = { 1, 1 };
		points[2] = { 0, 0 };

	}
	else if (button == BUTTON_270) {
		points[0] = { 0, 1 };
		points[1] = { 0, 0 };
		points[2] = { 1, 1 };
	}
	else if (button == BUTTON_HORIZONTAL_REVERSE) {
		points[0] = { 0, 1 };
		points[1] = { 1, 1 };
		points[2] = { 0, 0 };
	}
	else if (button == BUTTON_VERTICAL_REVERSE) {
		points[0] = { 1, 0 };
		points[1] = { 0, 0 };
		points[2] = { 1, 1 };
	}

	for (int i = 0; i < 3; ++i) {
		points[i].x *= newWidth;
		points[i].y *= newHeight;

		switch (button) {
		case BUTTON_180:
			points[i].x -= 1;
			points[i].y -= 1;
			break;
		case BUTTON_HORIZONTAL_REVERSE:
			points[i].y -= 1;
			break;
		case BUTTON_VERTICAL_REVERSE:
			points[i].x -= 1;
			break; // ← ここでbreakを追加
		default:
			break;
		}
	}
	m_imageView.PlgBlt(hDC, points);
	m_imageTemp.ReleaseDC();

	if (srcWidth != newWidth) {
		m_imageView.Destroy();
		m_imageView.Create(newWidth, newHeight, m_imageTemp.GetBPP());
	}

	hDC = m_imageView.GetDC();
	m_imageTemp.BitBlt(hDC, 0, 0, SRCCOPY);
	m_imageView.ReleaseDC();

	m_ctrlBitmap2.SetBitmap(m_imageView);

	//m_imageTemp.Destroy();
}
