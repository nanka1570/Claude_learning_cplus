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
		resizeBitmap(cx, cy);
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
	resizeBitmap(m_image.GetWidth(), m_image.GetHeight());

	ShowWindow(SW_SHOW);

	return 0;
}


LRESULT CImageDlg::OnDrawRotate(WPARAM wParam, LPARAM lParam)
{
	BUTTONS button = static_cast<BUTTONS>(wParam);

    // 回転状態を累積
    switch (button) {
    case BUTTON_90:
        m_rotationAngle = (m_rotationAngle + 90) % 360;  // 90度追加
        break;
    case BUTTON_270:
        m_rotationAngle = (m_rotationAngle + 270) % 360; // 270度追加（= -90度）
        break;
    case BUTTON_HORIZONTAL_REVERSE:
        m_flipHorizontal = !m_flipHorizontal;  // トグル（反転/元に戻す）
        break;
    case BUTTON_VERTICAL_REVERSE:
        m_flipVertical = !m_flipVertical;      // トグル
        break;
    }

    // 現在のウィンドウサイズを取得
    CRect rect;
    GetClientRect(&rect);

    // 累積した状態でリサイズ
    resizeBitmap(rect.Width(), rect.Height());

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
    if (m_image.IsNull())
        return;
    if (width <= 0 || height <= 0)
        return;

    // ウィンドウサイズ調整（既存のコード）
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

    if (m_rotationAngle == 90 || m_rotationAngle == 270) {
        newWidth = height;
        newHeight = width;
    }

    // m_imageTemp を作成
    if (!m_imageTemp.IsNull())
        m_imageTemp.Destroy();
    m_imageTemp.Create(newWidth, newHeight, m_image.GetBPP());

    HDC hDCTemp = m_imageTemp.GetDC();

    // 回転の座標を設定
    POINT points[3] = {};

    // 基本座標（回転なし）
    points[0] = { 0, 0 };
    points[1] = { newWidth - 1, 0 };
    points[2] = { 0, newHeight - 1 };

    // 回転を適用
    switch (m_rotationAngle) {
    case 90:
        points[0] = { newWidth - 1, 0 };
        points[1] = { newWidth - 1, newHeight - 1 };
        points[2] = { 0, 0 };
        break;
    case 180:
        points[0] = { newWidth - 1, newHeight - 1 };
        points[1] = { 0, newHeight - 1 };
        points[2] = { newWidth - 1, 0 };
        break;
    case 270:
        points[0] = { 0, newHeight - 1 };
        points[1] = { 0, 0 };
        points[2] = { newWidth - 1, newHeight - 1 };
        break;
    }

    // 反転を適用
    if (m_flipHorizontal) {
        // 上下反転：Y座標を反転
        for (int i = 0; i < 3; ++i) {
            points[i].y = newHeight - 1 - points[i].y;
        }
    }
    if (m_flipVertical) {
        // 左右反転：X座標を反転
        for (int i = 0; i < 3; ++i) {
            points[i].x = newWidth - 1 - points[i].x;
        }
    }

    m_image.PlgBlt(hDCTemp, points);
    m_imageTemp.ReleaseDC();

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
