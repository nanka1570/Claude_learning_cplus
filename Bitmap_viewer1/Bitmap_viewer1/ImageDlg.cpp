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

		if (m_image.IsNull())
			return;
		if (m_ctrlBitmap2.GetSafeHwnd() == NULL)
			return;
		resizeBitmap(cx, cy, m_currentButton);
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
	// オリジナルをロード
	m_image.Load(bitmapFilename.native().c_str());

	// 回転状態をリセット
	m_currentButton = BUTTON_DEFAULT;

	// 元画像のサイズでリサイズ（回転なし）
	resizeBitmap(m_image.GetWidth(), m_image.GetHeight(), BUTTON_DEFAULT);

	ShowWindow(SW_SHOW);

	return 0;
}


LRESULT CImageDlg::OnDrawRotate(WPARAM wParam, LPARAM lParam)
{
	BUTTONS button = static_cast<BUTTONS>(wParam);

	// 現在の回転状態を更新
	m_currentButton = button;

	// 現在のウィンドウサイズを取得
	CRect rect;
	GetClientRect(&rect);

	// 回転を適用してリサイズ
	resizeBitmap(rect.Width(), rect.Height(), button);

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


void CImageDlg::resizeBitmap(const int width, const int height, const BUTTONS button)
{
    if (m_image.IsNull())
        return;
    if (width <= 0 || height <= 0)
        return;

    // ウィンドウサイズを調整
    CRect windowRect, clientRect;
    GetWindowRect(windowRect);
    GetClientRect(clientRect);
    ClientToScreen(clientRect);

    const auto borderWidth = windowRect.Width() - clientRect.Width();
    const auto borderHeight = windowRect.Height() - clientRect.Height();

    MoveWindow(windowRect.left, windowRect.top, width + borderWidth, height + borderHeight, FALSE);

    ScreenToClient(clientRect);
    m_ctrlBitmap2.MoveWindow(clientRect.left, clientRect.top, width, height, FALSE);

    // 回転後のサイズを計算
    int newWidth = width;
    int newHeight = height;

    if (button == BUTTON_90 || button == BUTTON_270) {
        // 90度/270度回転は縦横入れ替え
        newWidth = height;
        newHeight = width;
    }

    // m_imageTemp を作成
    if (!m_imageTemp.IsNull())
        m_imageTemp.Destroy();
    m_imageTemp.Create(newWidth, newHeight, m_image.GetBPP());

    HDC hDCTemp = m_imageTemp.GetDC();

    // 回転/反転の座標を設定
    POINT points[3] = {};

    switch (button) {
    case BUTTON_DEFAULT:
        // 回転なし：StretchBlt を使用
        m_image.StretchBlt(hDCTemp, 0, 0, newWidth, newHeight, SRCCOPY);
        m_imageTemp.ReleaseDC();
        break;

    case BUTTON_90:
        points[0] = { newWidth - 1, 0 };
        points[1] = { newWidth - 1, newHeight - 1 };
        points[2] = { 0, 0 };
        m_image.PlgBlt(hDCTemp, points);
        m_imageTemp.ReleaseDC();
        break;

    case BUTTON_180:
        points[0] = { newWidth - 1, newHeight - 1 };
        points[1] = { 0, newHeight - 1 };
        points[2] = { newWidth - 1, 0 };
        m_image.PlgBlt(hDCTemp, points);
        m_imageTemp.ReleaseDC();
        break;

    case BUTTON_270:
        points[0] = { 0, newHeight - 1 };
        points[1] = { 0, 0 };
        points[2] = { newWidth - 1, newHeight - 1 };
        m_image.PlgBlt(hDCTemp, points);
        m_imageTemp.ReleaseDC();
        break;

    case BUTTON_HORIZONTAL_REVERSE:
        points[0] = { 0, newHeight - 1 };
        points[1] = { newWidth - 1, newHeight - 1 };
        points[2] = { 0, 0 };
        m_image.PlgBlt(hDCTemp, points);
        m_imageTemp.ReleaseDC();
        break;

    case BUTTON_VERTICAL_REVERSE:
        points[0] = { newWidth - 1, 0 };
        points[1] = { 0, 0 };
        points[2] = { newWidth - 1, newHeight - 1 };
        m_image.PlgBlt(hDCTemp, points);
        m_imageTemp.ReleaseDC();
        break;

    default:
        m_image.StretchBlt(hDCTemp, 0, 0, newWidth, newHeight, SRCCOPY);
        m_imageTemp.ReleaseDC();
        break;
    }

    // m_imageView を作成してコピー
    if (!m_imageView.IsNull())
        m_imageView.Destroy();
    m_imageView.Create(newWidth, newHeight, m_imageTemp.GetBPP());

    HDC hDCView = m_imageView.GetDC();
    m_imageTemp.BitBlt(hDCView, 0, 0, SRCCOPY);
    m_imageView.ReleaseDC();

    // 表示
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

//void CImageDlg::DrawRotate(const BUTTONS button)
//{
//	const auto srcWidth = m_imageView.GetWidth();
//	const auto srcHeight = m_imageView.GetHeight();
//	auto newWidth = 0;
//	auto newHeight = 0;
//
//	//画像が横になる場合
//	if (button == BUTTON_90 || button == BUTTON_270) {
//		newWidth = srcHeight;
//		newHeight = srcWidth;
//	}
//	else {
//		newWidth = srcWidth;
//		newHeight = srcHeight;
//	}
//
//	//画像が横になる場合は横長のキャンバスにする
//	if (srcWidth != newWidth) {
//		m_imageTemp.Destroy();
//	}
//	if (m_imageTemp.IsNull()) {
//		m_imageTemp.Create(newWidth, newHeight, m_imageView.GetBPP());
//	}
//#if 0
//	POINT points[3] = { {0, 0}, {1, 0}, {0, 1} }; //デフォルト（0）
//	POINT points[3] = { {0, 1}, {0, 0}, {1, 1} }; //90
//	POINT points[3] = { {1, 1}, {0, 1}, {1, 0} }; //180
//	POINT points[3] = { {1, 0}, {1, 1}, {0, 0} }; //270
//	POINT points[3] = { {0, 1}, {1, 1}, {0, 0} }; //H
//	POINT points[3] = { {1, 0}, {0, 0}, {1, 1} }; //V
//#endif
//
//	auto hDC = m_imageTemp.GetDC();
//	POINT points[3] = {};
//	if (button == BUTTON_90) {
//		points[0] = { 1, 0 };
//		points[1] = { 1, 1 };
//		points[2] = { 0, 0 };
//
//	}
//	else if (button == BUTTON_270) {
//		points[0] = { 0, 1 };
//		points[1] = { 0, 0 };
//		points[2] = { 1, 1 };
//	}
//	else if (button == BUTTON_HORIZONTAL_REVERSE) {
//		points[0] = { 0, 1 };
//		points[1] = { 1, 1 };
//		points[2] = { 0, 0 };
//	}
//	else if (button == BUTTON_VERTICAL_REVERSE) {
//		points[0] = { 1, 0 };
//		points[1] = { 0, 0 };
//		points[2] = { 1, 1 };
//	}
//
//	for (int i = 0; i < 3; ++i) {
//		points[i].x *= newWidth;
//		points[i].y *= newHeight;
//
//		switch (button) {
//		case BUTTON_180:
//			points[i].x -= 1;
//			points[i].y -= 1;
//			break;
//		case BUTTON_HORIZONTAL_REVERSE:
//			points[i].y -= 1;
//			break;
//		case BUTTON_VERTICAL_REVERSE:
//			points[i].x -= 1;
//			break;
//		default:
//			break;
//		}
//	}
//	m_imageView.PlgBlt(hDC, points);
//	m_imageTemp.ReleaseDC();
//
//	if (srcWidth != newWidth) {
//		m_imageView.Destroy();
//		m_imageView.Create(newWidth, newHeight, m_imageTemp.GetBPP());
//	}
//
//	resizeBitmap(newWidth, newHeight);
//	//hDC = m_imageView.GetDC();
//	//m_imageTemp.BitBlt(hDC, 0, 0, SRCCOPY);
//	//m_imageView.ReleaseDC();
//
//	//m_ctrlBitmap2.SetBitmap(m_imageView);
//}
