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
	static HWND g_hWnd;
	void NumOutput(const std::vector<int>& array, CStatic* text);	//指定したフォーマットで配列を出力する関数

protected:
	HICON m_hIcon;
	HCURSOR OnQueryDragIcon();
	LRESULT OnSortSwitch(WPARAM wParam, LPARAM lParam);

protected:
	afx_msg
		void OnSysCommand(UINT nID, LPARAM lParam);
	void OnPaint();
	void OnBnClickedRandom();
	void OnBnClickedSort();
	void OnTcnSelChangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();


protected:
	CTabCtrl m_ctrlSortTab{};			//タブコントロール
	CStatic* m_randomText = {};			//並べ替え前のエディットボックス
	CStatic* m_sortText = {};				//並べ替え後のエディットボックス
	CEdit* m_ctrlEditSwapCount = {};	//入れ替えた回数を表示するエディットボックス

protected:
	CGraphDlg::SORTTYPEORDER sortType{};		//SORTENUMのインスタンス

protected:
	void RandomNumGenerate();			//ランダムな配列を生成する関数

protected:
	std::vector<int> m_randomNum{};		//並べ替え前（ランダム）の配列を入れる変数
	std::vector<int> m_sortNum{};		//並べ替え後（バブルソート or クイックソート）した配列を入れる変数
	std::vector<CWnd*> m_tabs{};		//タブ配列を入れる変数
};