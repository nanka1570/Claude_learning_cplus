#pragma once
#include "afxdialogex.h"
#include <vector>
#include <filesystem>
#include <array>

class CImageDlg : public CDialogEx
{
public:
    explicit CImageDlg(CWnd* pParent = nullptr);
    virtual ~CImageDlg();
    DECLARE_DYNAMIC(CImageDlg)

public:
    enum BUTTONS {
        BUTTON_DEFAULT = 0,
        BUTTON_90,
        BUTTON_180,
        BUTTON_270,
        BUTTON_HORIZONTAL_REVERSE,
        BUTTON_VERTICAL_REVERSE,
        BUTTON_MAX
    };

public:
    void DrawBitmap();
    void setBitmapFile(std::filesystem::path filename);

public:
    virtual BOOL OnInitDialog();

protected:
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_IMAGE_DIALOG };
#endif

protected:
    CStatic m_ctrlBitmap2{};
    CImage m_image{};
    CImage m_imageView{};
    CImage m_imageTemp{};

protected:
    virtual void DoDataExchange(CDataExchange* pDX) override;
    virtual void OnOK() override;
    virtual void OnCancel() override;
    virtual void PostNcDestroy();

protected:
    afx_msg LRESULT OnDrawBitmap(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnRotateBitmap(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnSaveBitmap(WPARAM wParam, LPARAM lParam);
    void DrawRotate(const BUTTONS button);
    DECLARE_MESSAGE_MAP()

protected:
    void resizeBitmap(const int width, const int height);

protected:
    afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
    afx_msg void OnSize(UINT nType, int cx, int cy);

public:
    static const UINT_PTR WM_ROTATEBITMAP = WM_USER + 101;
    static const UINT_PTR WM_SAVEBITMAP = WM_USER + 102;
    CString m_strReceiveSaveFilename;

protected:
    std::filesystem::path bitmapFilename;
    std::array<POINT, 4> m_pointsAngle{};
    std::array<POINT, 3> RotateAngle(const BUTTONS button);
    bool m_bHorizontal{ true };
    bool m_isResizing = false;
public:
    afx_msg LRESULT OnNcHitTest(CPoint point);
};