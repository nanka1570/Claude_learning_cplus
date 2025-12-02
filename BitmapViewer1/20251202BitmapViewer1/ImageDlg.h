#pragma once
#include "afxdialogex.h"
#include <vector>
#include <filesystem>

class CImageDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CImageDlg)

public:
    explicit CImageDlg(CWnd* pParent = nullptr);
    virtual ~CImageDlg();

public:
    void setBitmap(int m_offset, int m_width, int m_height, int m_bits, uint8_t* pImage);
    void setBitmapFile(std::filesystem::path filename);
    void DrawBitmap();
    void ImageSave();

    // ★回転・反転関数
    void flipUpsideDown();
    void flipLeftRight();
    void rotateRight();
    void RotateLeft();

    // ★状態判定関数
    bool IsDefault();
    bool IsUnder();
    bool IsLeft();

public:
    virtual BOOL OnInitDialog();

public:
    int m_newOffset = 0;
    int m_newWidth = 0;
    int m_newHeight = 0;
    int m_newBits = 0;
    uint8_t* m_pImage = nullptr;
    std::filesystem::path bitmapFilename;

public:
    int srcWidth = 0;
    int srcHeight = 0;
    int targetWidth = 0;
    int targetHeight = 0;
    int bpp = 0;

    POINT points[3];  // ★変換座標を保持

public:
    CString m_strSaveBitmapFilename;

protected:
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_IMAGE_DIALOG };
#endif

protected:
    CStatic m_ctrlBitmap2{};
    CImage m_image{};
    CImage m_image2{};
    CImage m_image3{};  // ★上下反転用

protected:
    virtual void DoDataExchange(CDataExchange* pDX) override;
    virtual void OnOK() override;
    virtual void OnCancel() override;
    virtual void PostNcDestroy();  // ★モードレス用

protected:
    afx_msg LRESULT OnDrawBitmap(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()

public:
    // ★ボタンイベントハンドラ
    afx_msg void OnBnClickedButtonBitmapfileSave();
    afx_msg void OnBnClickedButtonFlipUpsideDown();
    afx_msg void OnBnClickedButtonFlipLeftRight();
    afx_msg void OnBnClickedButtonRotateRight();
    afx_msg void OnBnClickedButtonRotateLeft();
};