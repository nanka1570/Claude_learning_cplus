#pragma once
#include "afxdialogex.h"
#include <vector>
#include <random>


class CSortApplicationDlg;	//変更

// GraphDlg ダイアログ

class CGraphDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGraphDlg)

public:
	enum SORTENUM
	{
		SORT_BUBBLE_ASC,
		SORT_BUBBLE_DESC,
		SORT_QUICK_ASC,
		SORT_QUICK_DESC
	};
	CGraphDlg(CWnd* pParent, SORTENUM sortType, std::vector<int> SortNum);   // 標準コンストラクター
	virtual ~CGraphDlg();

	// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRAPH_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

	LRESULT OnViewSort(WPARAM wParam, LPARAM lParam);

	void viewSortGraph();

protected:
	//void NumOutput(std::vector<int> array, CStatic* text);	//変更
	SORTENUM m_nSortType;
	//const std::vector<int> m_nRandomNum;
	std::vector<int> m_nSortNum;
	CSortApplicationDlg* m_pParent;

	//バブルソート・クイックソート・昇順・降順を切り替える
	void SortSwitch(SORTENUM sortType);
	//昇順・降順にバブルソートで並べ替える関数
	std::vector<int> BubbleSort(SORTENUM sortOrder);

	int swapCount = 0;
	CStatic* sortText{};
};