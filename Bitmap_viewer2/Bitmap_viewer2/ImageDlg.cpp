#include "pch.h"
#include "Resource.h"
#include "Bitmap_viewer2Dlg.h"
#include "ImageDlg.h"
#include <vector>
#include <Gdiplus.h>
#include <Gdiplusimaging.h>

IMPLEMENT_DYNAMIC(CImageDlg, CDialogEx)

CImageDlg::CImageDlg(CWnd* pParent)
    : CDialogEx(IDD_IMAGE_DIALOG, pParent)
{
}

CImageDlg::~CImageDlg()
{
}

BEGIN_MESSAGE_MAP(CImageDlg, CDialogEx)
    ON_MESSAGE(CBitmapviewer2Dlg::WM_DRAWBITMAP, OnDrawBitmap)
    ON_MESSAGE(WM_ROTATEBITMAP, OnRotateBitmap)
    ON_MESSAGE(WM_SAVEBITMAP, OnSaveBitmap)
    ON_WM_SIZING()
    ON_WM_SIZE()
    ON_WM_NCHITTEST()
END_MESSAGE_MAP()

void CImageDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BITMAP2, m_ctrlBitmap2);
}

BOOL CImageDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_ctrlBitmap2.SetBitmap(nullptr);
    m_imageView.Destroy();
    m_image.Destroy();
    m_imageTemp.Destroy();

    m_pointsAngle[0] = { 0, 0 };
    m_pointsAngle[1] = { 1, 0 };
    m_pointsAngle[2] = { 1, 1 };
    m_pointsAngle[3] = { 0, 1 };

    DrawBitmap();

    return TRUE;
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

void CImageDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
    CDialogEx::OnSizing(fwSide, pRect);
}

void CImageDlg::setBitmapFile(std::filesystem::path filename)
{
    bitmapFilename = filename;
}

void CImageDlg::DrawBitmap()
{
    PostMessage(CBitmapviewer2Dlg::WM_DRAWBITMAP, 0, 0);
}

LRESULT CImageDlg::OnDrawBitmap(WPARAM wParam, LPARAM lParam)
{
    m_image.Load(bitmapFilename.native().c_str());

    const auto srcWidth = m_image.GetWidth();
    const auto srcHeight = m_image.GetHeight();

    const auto targetWidth = srcWidth;
    const auto targetHeight = srcHeight;

    if (!m_imageView.IsNull())
        m_imageView.Destroy();

    m_imageView.Create(targetWidth, targetHeight, m_image.GetBPP());
    HDC hDCView = m_imageView.GetDC();
    m_image.StretchBlt(hDCView, 0, 0, targetWidth, targetHeight, SRCCOPY);
    m_imageView.ReleaseDC();

    resizeBitmap(targetWidth, targetHeight);

    return 0;
}

void CImageDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    if (cx <= 10 || cy <= 10)
        return;
    if (m_image.IsNull())
        return;

    if (!m_isResizing)
    {
        const auto srcWidth = m_image.GetWidth();
        const auto srcHeight = m_image.GetHeight();

        // ★ 追加: アスペクト比を計算
        const double aspectRatio = static_cast<double>(srcWidth) / static_cast<double>(srcHeight);

        // ★ 追加: アスペクト比を維持したサイズを計算
        int targetWidth = cx;
        int targetHeight = cy;

        if (static_cast<double>(cx) / static_cast<double>(cy) > aspectRatio)
        {
            // ウィンドウが横長 → 高さに合わせる
            targetWidth = static_cast<int>(cy * aspectRatio);
            targetHeight = cy;
        }
        else
        {
            // ウィンドウが縦長 → 幅に合わせる
            targetWidth = cx;
            targetHeight = static_cast<int>(cx / aspectRatio);
        }

        if (!m_imageView.IsNull())
            m_imageView.Destroy();

        m_imageView.Create(targetWidth, targetHeight, m_image.GetBPP());
        HDC hDCView = m_imageView.GetDC();
        m_image.StretchBlt(hDCView, 0, 0, targetWidth, targetHeight, SRCCOPY);
        m_imageView.ReleaseDC();

        resizeBitmap(cx, cy);
        ShowWindow(SW_SHOW);
    }
}

void CImageDlg::resizeBitmap(const int width, const int height)
{
    if (width <= 10 || height <= 10)
        return;

    if (m_image.IsNull())
        return;

    m_isResizing = true;

    CRect windowRectBitmap;
    CRect clientRectBitmap;
    GetWindowRect(windowRectBitmap);
    GetClientRect(clientRectBitmap);
    ClientToScreen(clientRectBitmap);

    const auto borderWidth = (windowRectBitmap.Width() - clientRectBitmap.Width());
    const auto borderHeight = (windowRectBitmap.Height() - clientRectBitmap.Height());

    m_ctrlBitmap2.ShowWindow(SW_HIDE);
    m_ctrlBitmap2.SetBitmap(nullptr);

    ScreenToClient(clientRectBitmap);
    m_ctrlBitmap2.MoveWindow(0, 0, width, height);

    if (!m_imageTemp.IsNull())
        m_imageTemp.Destroy();

    m_imageTemp.Create(width, height, m_imageView.GetBPP());
    HDC hDCTemp = m_imageTemp.GetDC();
    m_imageView.StretchBlt(hDCTemp, 0, 0, width, height, SRCCOPY);
    m_imageTemp.ReleaseDC();

    if (!m_imageView.IsNull())
        m_imageView.Destroy();

    m_imageView.Create(width, height, m_imageTemp.GetBPP());
    HDC hDCView = m_imageView.GetDC();
    m_imageTemp.BitBlt(hDCView, 0, 0, width, height, 0, 0, SRCCOPY);
    m_imageView.ReleaseDC();

    m_ctrlBitmap2.SetBitmap(m_imageView);
    m_ctrlBitmap2.ShowWindow(SW_SHOW);

    MoveWindow(windowRectBitmap.left, windowRectBitmap.top, width + borderWidth, height + borderHeight);

    m_isResizing = false;
}

LRESULT CImageDlg::OnRotateBitmap(WPARAM wParam, LPARAM lParam)
{
    BUTTONS button = static_cast<BUTTONS>(wParam);
    DrawRotate(button);
    return 0;
}

LRESULT CImageDlg::OnSaveBitmap(WPARAM wParam, LPARAM lParam)
{
    m_imageView.Save(m_strReceiveSaveFilename.GetString(), Gdiplus::ImageFormatBMP);
    return 0;
}

// ★ 修正後の DrawRotate
void CImageDlg::DrawRotate(const BUTTONS button)
{
    const auto viewWidth = m_imageView.GetWidth();
    const auto viewHeight = m_imageView.GetHeight();
    const auto viewBPP = m_imageView.GetBPP();

    auto rotateWidth = viewWidth;
    auto rotateHeight = viewHeight;

    auto points{ RotateAngle(button) };

    if (!m_bHorizontal) {
        std::swap(rotateWidth, rotateHeight);
    }

    for (auto& [x, y] : points)
    {
        x = static_cast<LONG>(x * (rotateWidth));
        y = static_cast<LONG>(y * (rotateHeight));

        switch (button) {
        case BUTTON_180:
            --x;
            --y;
            break;
        case BUTTON_HORIZONTAL_REVERSE:
            --y;
            break;
        case BUTTON_VERTICAL_REVERSE:
            --x;
            break;
        default:
            break;
        }
    }

    // ★ 修正: 常に Destroy してから Create する
    m_ctrlBitmap2.SetBitmap(nullptr);

    if (!m_imageView.IsNull())
        m_imageView.Destroy();

    m_imageView.Create(rotateWidth, rotateHeight, m_image.GetBPP());

    if (m_imageView.IsNull())
        return;

    HDC hDCView = m_imageView.GetDC();
    m_image.PlgBlt(hDCView, points.data());
    m_imageView.ReleaseDC();

    resizeBitmap(rotateWidth, rotateHeight);
}

std::array<POINT, 3> CImageDlg::RotateAngle(const BUTTONS button)
{
    POINT temp;
    switch (button) {
    case BUTTON_90:
        temp = m_pointsAngle[3];
        m_pointsAngle[3] = m_pointsAngle[2];
        m_pointsAngle[2] = m_pointsAngle[1];
        m_pointsAngle[1] = m_pointsAngle[0];
        m_pointsAngle[0] = temp;
        m_bHorizontal = !m_bHorizontal;
        break;
    case BUTTON_180:
        std::swap(m_pointsAngle[0], m_pointsAngle[2]);
        std::swap(m_pointsAngle[1], m_pointsAngle[3]);
        break;
    case BUTTON_270:
        temp = m_pointsAngle[0];
        m_pointsAngle[0] = m_pointsAngle[1];
        m_pointsAngle[1] = m_pointsAngle[2];
        m_pointsAngle[2] = m_pointsAngle[3];
        m_pointsAngle[3] = temp;
        m_bHorizontal = !m_bHorizontal;
        break;
    case BUTTON_HORIZONTAL_REVERSE:
        std::swap<POINT>(m_pointsAngle[0], m_pointsAngle[1]);
        std::swap<POINT>(m_pointsAngle[2], m_pointsAngle[3]);
        break;
    case BUTTON_VERTICAL_REVERSE:
        std::swap<POINT>(m_pointsAngle[0], m_pointsAngle[3]);
        std::swap<POINT>(m_pointsAngle[1], m_pointsAngle[2]);
        break;
    default:
        break;
    }
    const std::array<POINT, 3> points{ m_pointsAngle[0], m_pointsAngle[1], m_pointsAngle[3] };
    return points;
}


LRESULT CImageDlg::OnNcHitTest(CPoint point)
{
	LRESULT result = CDialogEx::OnNcHitTest(point);

    if (result == HTCLIENT)
    {
        return HTCAPTION;
    }
    return result;
}
