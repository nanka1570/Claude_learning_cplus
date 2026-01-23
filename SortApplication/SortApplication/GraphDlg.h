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

public:
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	DECLARE_MESSAGE_MAP()

protected:
	const double WAITING_TIME = 0;			//視覚的に分かりやすく表示するため、待ち時間が入る変数

protected:
	LRESULT OnViewSort(WPARAM wParam, LPARAM lParam);

protected:
	void SortSwitch(SORTTYPEORDER sortType);	//バブルソートとクイックソートを切り替える
	void BubbleSort(SORTTYPEORDER sortOrder);	//昇順・降順にバブルソートで並べ替える関数
	void QuickSort(SORTTYPEORDER sortOrder);	//昇順・降順にクイックソートで並べ替える関数
	void RecursiveQuickSort(std::vector<int>& array);
	void ViewSortGraph();						//グラフを表示する関数
	void DrawSortGraph(SORTTYPEORDER sortType);	//グラフを描く関数

protected:
	//CStatic* m_sortText{};				//テキストの配列を表示するエディットボックス
	CStatic m_ctrlViewGraph;				//ピクチャコントロール
	CImage m_imageGraph;					//グラフのイメージ

protected:
	SORTTYPEORDER m_nSortType;				//SORTENUMのインスタンス
	std::vector<int> m_nSortNum;			//ソートされた配列が入る変数

protected:
	int m_swapCount = 0;					//ソートで入れ替えた回数が入る変数

};