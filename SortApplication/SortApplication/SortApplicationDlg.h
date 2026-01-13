// SortApplicationDlg.h : ヘッダー ファイル
#pragma once
#include <vector>

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
	enum SORTENUM
	{
		SORT_BUBBLE_ASC,
		SORT_BUBBLE_DESC,
		SORT_QUICK_ASC,
		SORT_QUICK_DESC
	};

public:
	static const UINT_PTR WM_SORTSWITCH = WM_USER + 100;
	static const UINT_PTR WM_NUMOUTPUT = WM_USER + 101;
	static const UINT_PTR WM_VIEWSORT = WM_USER + 102;
	static HWND g_hWnd;
	//指定したフォーマットで配列を出力する関数
	void NumOutput(std::vector<int> array, CStatic* text);

protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	void OnSysCommand(UINT nID, LPARAM lParam);
	void OnPaint();
	LRESULT OnSortSwitch(WPARAM wParam, LPARAM lParam);
	LRESULT OnNumOutPut(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	//ランダムな配列を生成する関数
	void RandomNumGenerate();
	//バブルソート・クイックソート・昇順・降順を切り替える
	void SortSwitch(SORTENUM sortType);
	//昇順・降順にバブルソートで並べ替える関数
	std::vector<int> BubbleSort(SORTENUM sortOrder);

protected:
	//std::vector<uint8_t> RandomNum{};
	std::vector<int> RandomNum{};
	std::vector<int> SortNum{};

protected:
	CTabCtrl m_ctrlSortTab{};
	std::vector<CWnd*> m_tabs{};

protected:
	afx_msg void OnBnClickedRandom();
	afx_msg void OnBnClickedSort();
	afx_msg void OnTcnSelChangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();

protected:
	CStatic* randomText = {};
	CStatic* sortText = {};
	int swapCount = 0;

public:
	SORTENUM sortType{};

protected:
	CEdit* m_ctrlEditSwapCount = {};
};