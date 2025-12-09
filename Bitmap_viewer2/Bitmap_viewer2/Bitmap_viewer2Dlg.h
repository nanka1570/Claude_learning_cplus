#pragma once
#include <vector>
#include <filesystem>

class CImageDlg;

class CBitmapviewer2Dlg : public CDialogEx
{
public:
    explicit CBitmapviewer2Dlg(CWnd* pParent = nullptr);
    virtual ~CBitmapviewer2Dlg() = default;

public:
    static const UINT_PTR WM_DRAWBITMAP = WM_USER + 100;

    CString m_strEditBitmapFilename{};
    std::filesystem::path filename;
    CString m_strSaveBitmapFilename;
    CImageDlg* m_pDialog = nullptr;

protected:
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_BITMAP_VIEWER2_DIALOG };
#endif

protected:
    HICON m_hIcon{ nullptr };
    std::vector<uint8_t> m_bitmapImage{};
    int m_offset = 0;
    int m_width = 0;
    int m_height = 0;
    int m_planes = 1;
    int m_bits = 0;

protected:
    virtual void DoDataExchange(CDataExchange* pDX) override;
    virtual BOOL OnInitDialog() override;
    virtual void OnOK() override;
    virtual void OnCancel() override;

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedBitmapFileOpen();
    afx_msg void OnBnClickedButtonBitmapfileSave();
    afx_msg void OnBnClickedButtonFlipUpsideDown();
    afx_msg void OnBnClickedButtonFlipLeftRight();
    afx_msg void OnBnClickedButtonRotateRight();
    afx_msg void OnBnClickedButtonRotateLeft();

protected:
    bool LoadBitmap();
};