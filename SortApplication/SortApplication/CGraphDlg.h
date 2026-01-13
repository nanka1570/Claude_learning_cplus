#pragma once
#include "afxdialogex.h"
#include "SortApplicationDlg.h"
#include <vector>
#include <random>

// GraphDlg ダイアログ

class CGraphDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGraphDlg)

public:
	CGraphDlg(CWnd* pParent = nullptr,
		CSortApplicationDlg::SORTENUM sortType = CSortApplicationDlg::SORT_BUBBLE_ASC,
		std::vector<int> RandomNum = {},
		std::vector<int> SortNum = {}); // 標準コンストラクター
	virtual ~CGraphDlg();

	// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRAPH_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

	LRESULT OnViewSort(WPARAM wParam, LPARAM lParam);

	void viewSortGraph();
	//グラフを表示する
	void DrawBarGraph(std::vector<int> data); //追加

protected:
	CSortApplicationDlg::SORTENUM m_sortType;
	std::vector<int> m_RandomNum;              // constを外す、名前をわかりやすく
	std::vector<int> m_SortNum;

	//NumOutput関数を使うために親ウィンドウのポインタを取得
	CSortApplicationDlg* m_pParentDlg;
};