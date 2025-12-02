#include "pch.h"
#include "Resource.h"
#include "Bitmap_viewer1Dlg.h"
#include "ImageDlg.h"
#include <vector>
#include <Gdiplus.h>       // ★保存機能に必要
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
    ON_MESSAGE(CBitmapviewer1Dlg::WM_DRAWBITMAP, OnDrawBitmap)
    ON_BN_CLICKED(IDC_BUTTON_BITMAPFILE_SAVE, &CImageDlg::OnBnClickedButtonBitmapfileSave)
    ON_BN_CLICKED(IDC_BUTTON_FLIP_UPSIDE_DOWN, &CImageDlg::OnBnClickedButtonFlipUpsideDown)  // ★
    ON_BN_CLICKED(IDC_BUTTON_FLIP_LEFT_RIGHT, &CImageDlg::OnBnClickedButtonFlipLeftRight)    // ★
    ON_BN_CLICKED(IDC_BUTTON_ROTATE_RIGHT, &CImageDlg::OnBnClickedButtonRotateRight)          // ★
    ON_BN_CLICKED(IDC_BUTTON_ROTATE_LEFT, &CImageDlg::OnBnClickedButtonRotateLeft)            // ★
END_MESSAGE_MAP()

void CImageDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BITMAP2, m_ctrlBitmap2);
}

void CImageDlg::setBitmap(int m_offset, int m_width, int m_height, int m_bits, uint8_t* pImage)
{
    m_newOffset = m_offset;
    m_newWidth = m_width;
    m_newHeight = m_height;
    m_newBits = m_bits;
    m_pImage = pImage;
}

void CImageDlg::setBitmapFile(std::filesystem::path filename)
{
    bitmapFilename = filename;
    return;
}

// ★最初の描画処理
LRESULT CImageDlg::OnDrawBitmap(WPARAM wParam, LPARAM lParam)
{
    // オリジナル画像を読み込み
    m_image.Load(bitmapFilename.native().c_str());

    srcWidth = m_image.GetWidth();
    srcHeight = m_image.GetHeight();
    targetWidth = srcWidth;   // ★等倍表示
    targetHeight = srcHeight; // ★等倍表示
    bpp = m_image.GetBPP();

    // ★描画用キャンバス作成
    m_image2.Create(targetWidth, targetHeight, bpp, 0);

    // ★デフォルトの座標を設定
    points[0] = { 0, 0 };
    points[1] = { targetWidth - 1, 0 };
    points[2] = { 0, targetHeight - 1 };

    // ★変換して描画
    m_image.PlgBlt(m_image2.GetDC(), points);
    m_image2.ReleaseDC();

    // ★Picture Controlに設定
    m_ctrlBitmap2.SetBitmap(m_image2);

    // ★描画後に破棄（次回のために）
    m_image2.Destroy();

    return 0;
}

BOOL CImageDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    ShowWindow(SW_SHOW);
    DrawBitmap();
    return TRUE;
}

void CImageDlg::DrawBitmap()
{
    PostMessage(CBitmapviewer1Dlg::WM_DRAWBITMAP, 0, 0);
    return;
}

// ★保存ボタン
void CImageDlg::OnBnClickedButtonBitmapfileSave()
{
    CFileDialog dlg(FALSE, _T("bmp"), NULL,
        OFN_OVERWRITEPROMPT, _T("Bitmap Files (*.bmp)|*.bmp||"));

    if (dlg.DoModal() == IDOK)
    {
        m_strSaveBitmapFilename = dlg.GetPathName();
        UpdateData(FALSE);
        ImageSave();
    }
}

void CImageDlg::ImageSave()
{
    // ★現在表示中の画像を再作成して保存
    if (m_image2.IsNull())
    {
        m_image2.Create(targetWidth, targetHeight, bpp, 0);
        m_image.PlgBlt(m_image2.GetDC(), points);
        m_image2.ReleaseDC();
    }

    m_image2.Save(m_strSaveBitmapFilename.GetString(), Gdiplus::ImageFormatBMP);
    return;
}

// ★==================== 回転・反転機能 ====================

// ★上下反転
void CImageDlg::flipUpsideDown()
{
    if (m_image2.IsNull())
    {
        m_image2.Create(targetWidth, targetHeight, bpp, 0);

        // ★現在の状態を判定して座標を設定
        if (IsDefault())
        {
            // デフォルト → 上下反転
            points[0] = { targetWidth - 1, targetHeight - 1 };
            points[1] = { 0, targetHeight - 1 };
            points[2] = { targetWidth - 1, 0 };
        }
        else if (IsUnder())
        {
            // 上下反転済み → デフォルトに戻す
            points[0] = { 0, 0 };
            points[1] = { targetWidth - 1, 0 };
            points[2] = { 0, targetHeight - 1 };
        }

        m_image.PlgBlt(m_image2.GetDC(), points);
        m_image2.ReleaseDC();
        m_ctrlBitmap2.SetBitmap(m_image2);
        m_image2.Destroy();
    }
    else
    {
        m_image2.Destroy();
    }
}

// ★左右反転
void CImageDlg::flipLeftRight()
{
    if (m_image2.IsNull())
    {
        m_image2.Create(targetWidth, targetHeight, bpp, 0);

        if (IsDefault())
        {
            // デフォルト → 左右反転
            points[0] = { targetWidth - 1, 0 };
            points[1] = { 0, 0 };
            points[2] = { targetWidth - 1, targetHeight - 1 };
        }
        else if (IsLeft())
        {
            // 左右反転済み → デフォルトに戻す
            points[0] = { 0, 0 };
            points[1] = { targetWidth - 1, 0 };
            points[2] = { 0, targetHeight - 1 };
        }

        m_image.PlgBlt(m_image2.GetDC(), points);
        m_image2.ReleaseDC();
        m_ctrlBitmap2.SetBitmap(m_image2);
        m_image2.Destroy();
    }
    else
    {
        m_image2.Destroy();
    }
}

// ★右に90度回転
void CImageDlg::rotateRight()
{
    if (m_image2.IsNull())
    {
        // ★回転時は縦横を入れ替える
        m_image2.Create(targetHeight, targetWidth, bpp, 0);

        POINT rotatePoints[3] = {
            { targetHeight - 1, 0 },
            { targetHeight - 1, targetWidth - 1 },
            { 0, 0 }
        };

        m_image.PlgBlt(m_image2.GetDC(), rotatePoints);
        m_image2.ReleaseDC();
        m_ctrlBitmap2.SetBitmap(m_image2);
        m_image2.Destroy();
    }
    else
    {
        m_image2.Destroy();
    }
}

// ★左に90度回転
void CImageDlg::RotateLeft()
{
    if (m_image2.IsNull())
    {
        // ★回転時は縦横を入れ替える
        m_image2.Create(targetHeight, targetWidth, bpp, 0);

        POINT rotatePoints[3] = {
            { 0, targetWidth - 1 },
            { 0, 0 },
            { targetHeight - 1, targetWidth - 1 }
        };

        m_image.PlgBlt(m_image2.GetDC(), rotatePoints);
        m_image2.ReleaseDC();
        m_ctrlBitmap2.SetBitmap(m_image2);
        m_image2.Destroy();
    }
    else
    {
        m_image2.Destroy();
    }
}

// ★==================== 状態判定関数 ====================

bool CImageDlg::IsDefault()
{
    return (points[0].x == 0 && points[0].y == 0
        && points[1].x == targetWidth - 1 && points[1].y == 0
        && points[2].x == 0 && points[2].y == targetHeight - 1);
}

bool CImageDlg::IsUnder()
{
    return (points[0].x == targetWidth - 1 && points[0].y == targetHeight - 1
        && points[1].x == 0 && points[1].y == targetHeight - 1
        && points[2].x == targetWidth - 1 && points[2].y == 0);
}

bool CImageDlg::IsLeft()
{
    return (points[0].x == targetWidth - 1 && points[0].y == 0
        && points[1].x == 0 && points[1].y == 0
        && points[2].x == targetWidth - 1 && points[2].y == targetHeight - 1);
}

// ★==================== ボタンイベント ====================

void CImageDlg::OnBnClickedButtonFlipUpsideDown()
{
    flipUpsideDown();
}

void CImageDlg::OnBnClickedButtonFlipLeftRight()
{
    flipLeftRight();
}

void CImageDlg::OnBnClickedButtonRotateRight()
{
    rotateRight();
}

void CImageDlg::OnBnClickedButtonRotateLeft()
{
    RotateLeft();
}

// ★モードレス用
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
    delete this;  // ★モードレスは自分で削除
}