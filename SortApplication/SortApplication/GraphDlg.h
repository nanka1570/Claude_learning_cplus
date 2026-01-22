#pragma once
#include "afxdialogex.h"
#include <vector>
#include <random>


class CSortApplicationDlg;
// GraphDlg ダイアログ

class CGraphDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGraphDlg)

public:
	enum SORTTYPEORDER
	{
		SORT_BUBBLE_ASC,
		SORT_BUBBLE_DESC,
		SORT_QUICK_ASC,
		SORT_QUICK_DESC
	};
	CGraphDlg(CWnd* pParent, SORTTYPEORDER sortType, const std::vector<int>& SortNum);   // 標準コンストラクター
	virtual ~CGraphDlg();

	// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRAPH_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()


protected:
	LRESULT OnViewSort(WPARAM wParam, LPARAM lParam);

protected:
	SORTTYPEORDER m_nSortType;					//SORTENUMのインスタンス
	CSortApplicationDlg* m_pParent;			//SortApplicationDlgのポインタ
	CStatic* m_sortText{};					//テキストの配列を表示するエディットボックス

	CStatic m_ctrlViewGraph;				//ピクチャコントロールのポインタ
	CImage m_imageGraph{};					//グラフ本体

protected:
	void SortSwitch(SORTTYPEORDER sortType);		//バブルソート・クイックソート・昇順・降順を切り替える
	void BubbleSort(SORTTYPEORDER sortOrder);	//昇順・降順にバブルソートで並べ替える関数
	void ViewSortGraph();					//グラフを表示する関数
	void DrawSortGraph(SORTTYPEORDER sortType);					//グラフを描く関数

protected:
	const int WAITING_TIME = 0;			//視覚的に分かりやすく表示するため、待ち時間が入る変数

protected:
	std::vector<int> m_nSortNum;			//ソートされた配列が入る変数
	int m_swapCount = 0;						//ソートで入れ替えた回数が入る変数

	std::vector<CImage> m_barImages;  // 各棒のビットマップ
	int m_nBarWidth = 0;              // 棒の幅
	int m_nMaxHeight = 0;             // 最大の高さ
};