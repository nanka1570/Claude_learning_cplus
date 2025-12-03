#pragma once

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};