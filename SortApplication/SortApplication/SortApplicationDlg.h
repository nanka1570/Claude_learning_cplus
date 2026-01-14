// SortApplicationDlg.h : ヘッダー ファイル
#pragma once
#include <vector>
#include "GraphDlg.h"

// CSortApplicationDlg ダイアログ
class CSortApplicationDlg : public CDialogEx
{
public:
    CSortApplicationDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_SORTAPPLICATION_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

public:
    static const UINT_PTR WM_SORTSWITCH = WM_USER + 100;
    static const UINT_PTR WM_VIEWSORT = WM_USER + 101;

public:
    static HWND g_hWnd;
    void NumOutput(std::vector<int> array, CStatic* text);  //指定したフォーマットで配列を出力する関数

public:
    CStatic* sortText = {};
    CGraphDlg::SORTENUM sortType{};

protected:
    void OnSysCommand(UINT nID, LPARAM lParam);
    void OnPaint();

protected:
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();

protected:
    HICON m_hIcon;
    afx_msg
        HCURSOR OnQueryDragIcon();
    LRESULT OnSortSwitch(WPARAM wParam, LPARAM lParam);
    void OnBnClickedRandom();
    void OnBnClickedSort();
    void OnTcnSelChangeTab(NMHDR* pNMHDR, LRESULT* pResult);
    DECLARE_MESSAGE_MAP()

protected:
    void RandomNumGenerate();   //ランダムな配列を生成する関数

protected:
    CTabCtrl m_ctrlSortTab{};
    CStatic* m_randomText = {};
    CEdit* m_ctrlEditSwapCount = {};

protected:
    std::vector<int> RandomNum{};
    std::vector<int> SortNum{};
    std::vector<CWnd*> m_tabs{};
    const int waitingTime = 30;

};