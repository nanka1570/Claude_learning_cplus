// ImageDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "Resource.h"
#include "Bitmap_viewer1Dlg.h"
//#include "AboutDlg.h"
//#include "afxdialogex.h"
#include "ImageDlg.h"
#include <vector>
#include <Gdiplus.h>
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
	ON_BN_CLICKED(IDC_BUTTON_BITMAPFILE_SAVE, &CImageDlg::OnBnClickedButtonBitmapfileSave)
	ON_BN_CLICKED(IDC_BUTTON_FLIP_UPSIDE_DOWN, &CImageDlg::OnBnClickedButtonFlipUpsideDown)
	ON_BN_CLICKED(IDC_BUTTON_FLIP_LEFT_RIGHT, &CImageDlg::OnBnClickedButtonFlipLeftRight)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE_RIGHT, &CImageDlg::OnBnClickedButtonRotateRight)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE_LEFT, &CImageDlg::OnBnClickedButtonRotateLeft)
END_MESSAGE_MAP()

//ビットマップの表示に必要な値をセットする
void CImageDlg::setBitmap(int m_offset, int m_width, int m_height, int m_bits, uint8_t* pImage)
{
	//m_offset = this.m_offset;
	m_newOffset = m_offset;
	m_newWidth = m_width;
	m_newHeight = m_height;
	m_newBits = m_bits;
	m_pImage = pImage;

	printf("");
}

//Bitmap_viewer1Dlg.cppからもらったファイルをセットする
void CImageDlg::setBitmapFile(std::filesystem::path filename)
{
	bitmapFilename = filename;
	return;
}

LRESULT CImageDlg::OnDrawBitmap(WPARAM wParam, LPARAM lParam)
{
	//1つ目
	 //オリジナル
	m_image.Load(bitmapFilename.native().c_str());
	// m_ctrlBitmap2.SetBitmap(m_image);

	if (!m_imageView.IsNull())
	{
		m_imageView.Destroy();
	}
	srcWidth = m_image.GetWidth();
	srcHeight = m_image.GetHeight();
	targetWidth = srcWidth;
	targetHeight = srcHeight;
	bpp = m_image.GetBPP();
	//描画するキャンバスを用意 Create(x, y, bpp, 0)
	m_imageView.Create(targetWidth, targetHeight, bpp, 0);

	//POINT points[3] = {
	 // { 0, 0 },
	 // { targetWidth - 1, 0 },
	 // { 0, targetHeight - 1 }
	 //};
	POINT points[3] = {};
	points[0] = { 0, 0 };
	points[1] = { targetWidth - 1, 0 };
	points[2] = { 0, targetHeight - 1 };

	m_image.PlgBlt(m_imageView.GetDC(), points);
	//m_image.BitBlt(m_imageView.GetDC(), points);
	m_imageView.ReleaseDC();

	//m_image.Destroy();
	m_ctrlBitmap2.SetBitmap(m_imageView);

	return 0;
	printf("");
}

//初期化
BOOL CImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ShowWindow(SW_SHOW);

	DrawBitmap();

	printf("");
	return TRUE;
}

//DrawBitmap使用
void CImageDlg::DrawBitmap()
{
	PostMessage(CBitmapviewer1Dlg::WM_DRAWBITMAP, 0, 0);
	return;
}

//「名前を付けて保存」する
void CImageDlg::OnBnClickedButtonBitmapfileSave()
{
	CFileDialog dlg(FALSE);
	if (dlg.DoModal() == IDOK)
	{
		m_strSaveBitmapFilename = dlg.GetPathName();

		UpdateData(FALSE);

		ImageSave();
	}
	printf("");
}

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
	if (button == BUTTON_90 || button == BUTTON_270 || button == BUTTON_VERTICAL_REVERSE) {
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
		points[0] = { 0, 1 };
		points[1] = { 0, 0 };
		points[2] = { 1, 1 };
	}
	else if (button == BUTTON_270) {
		points[0] = { 1, 0 };
		points[1] = { 1, 1 };
		points[2] = { 0, 0 };
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
		points[i].x *= (newWidth - 1);
		points[i].y *= (newHeight - 1);
	}
	m_imageView.PlgBlt(hDC, points);
	m_imageTemp.ReleaseDC();

	if (srcWidth != newWidth) {
		m_imageView.Destroy();
		m_imageView.Create(newWidth, newHeight, m_imageTemp.GetBPP());
	}

	hDC = m_imageView.GetDC();
	//m_imageTemp.PlgBlt(hDC, nullptr);
	m_imageView.ReleaseDC();

	m_ctrlBitmap2.SetBitmap(m_imageView);

	//m_imageTemp.Destroy();
}

//bool CImageDlg::BitmapJudge()
//{
// return false;
//}

//上下反転ボタン
void CImageDlg::OnBnClickedButtonFlipUpsideDown()
{
	DrawRotate(BUTTON_HORIZONTAL_REVERSE);
}

//左右反転ボタン
void CImageDlg::OnBnClickedButtonFlipLeftRight()
{
	DrawRotate(BUTTON_VERTICAL_REVERSE);
}

//右に90°回転
void CImageDlg::OnBnClickedButtonRotateRight()
{
	DrawRotate(BUTTON_90);
}

//左に90°回転
void CImageDlg::OnBnClickedButtonRotateLeft()
{
	DrawRotate(BUTTON_270);
}

void CImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BITMAP2, m_ctrlBitmap2);
}

void CImageDlg::OnOK()
{
	CDialog::OnOK();
}

void CImageDlg::OnCancel()
{
	CDialog::OnCancel();
}

void CImageDlg::PostNcDestroy()
{

	CDialogEx::PostNcDestroy();
	delete this;
}

