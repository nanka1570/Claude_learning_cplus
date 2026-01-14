// GraphDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "afxdialogex.h"
#include "SortApplication.h"	//変更
#include "SortApplicationDlg.h"
#include "GraphDlg.h"
#include <chrono>
#include <algorithm>
#include <vector>


// GraphDlg ダイアログ

IMPLEMENT_DYNAMIC(CGraphDlg, CDialogEx)

CGraphDlg::CGraphDlg(CWnd* pParent, SORTENUM sortType, std::vector<int> SortNum)
	: CDialogEx(IDD_GRAPH_DIALOG, pParent)
	, m_nSortType(sortType)
	, m_nSortNum(SortNum)
	, m_pParent((CSortApplicationDlg*)pParent)
{

}

CGraphDlg::~CGraphDlg()
{
}

void CGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGraphDlg, CDialogEx)
	ON_MESSAGE(CSortApplicationDlg::WM_VIEWSORT, OnViewSort)
END_MESSAGE_MAP()
// GraphDlg メッセージ ハンドラー


LRESULT CGraphDlg::OnViewSort(WPARAM wParam, LPARAM lParam)
{
	viewSortGraph();
	return 0;
}


//グラフを表示する
void CGraphDlg::viewSortGraph()
{
	//ランダムな値を表示
	//CStatic* m_sortText{};
	m_sortText = (CStatic*)GetDlgItem(IDC_EDIT_RANDOM_NUM);
	//CSortApplicationDlgのOutputNumbers関数を使って出力
	m_pParent->OutputNumbers(m_nSortNum, m_sortText);
	Sleep(1000);

	//バブルソート
	SortSwitch(m_nSortType);


}


//ソートを判断する関数
void CGraphDlg::SortSwitch(SORTENUM sortType)
{
	switch (sortType) {
	case SORT_BUBBLE_ASC:
		BubbleSort(SORT_BUBBLE_ASC);
		break;
	case SORT_BUBBLE_DESC:
		BubbleSort(SORT_BUBBLE_DESC);
		break;
	case SORT_QUICK_ASC:
		break;
	case SORT_QUICK_DESC:
		break;
	default:
		break;
	}
}


//バブルソートで並べ替える関数
void CGraphDlg::BubbleSort(SORTENUM sortOrder)
{
	//ソートする配列の長さをarrayLengthに代入
	auto arrayLength = m_nSortNum.size();	//変更
	

	//auto begin{ std::chrono::system_clock::now() };			//処理開始時間
	//auto end{ std::chrono::system_clock::now() };			//処理終了時間
	//auto excludeBegin{ std::chrono::system_clock::now() };	//除外処理開始時間
	//auto excludeEnd{ std::chrono::system_clock::now() };	//除外処理終了時間
	//auto totalExclude{ std::chrono::duration_cast<std::chrono::milliseconds>(excludeEnd - excludeBegin).count() };	//合計除外時間
	auto begin = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
	auto totalExclude = 0LL;  //変更

	auto duration{ (std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) - totalExclude };	//ソートアルゴリズムの経過時間

	switch (sortOrder) {
	case SORT_BUBBLE_ASC:		//昇順のバブルソート
		begin = std::chrono::high_resolution_clock::now();
		for (arrayLength; arrayLength > 0; --arrayLength) {
				auto lastIndex = arrayLength - 1;
			for (int baseIndex = 0; baseIndex < lastIndex; ++baseIndex) {
				auto nextIndex = baseIndex + 1;
				if (m_nSortNum[baseIndex] > m_nSortNum[nextIndex]) {
					std::swap(m_nSortNum[baseIndex], m_nSortNum[nextIndex]);
					m_swapCount += 1;
				}
				//PostMessage(WM_OutputNumbers, 0, 0);
				auto excludeBegin = std::chrono::high_resolution_clock::now();
				Sleep(WAITING_TIME);
				m_pParent->OutputNumbers(m_nSortNum, m_sortText);	//変更
				auto excludeEnd = std::chrono::high_resolution_clock::now();
				//除外時間の合計をループが回るたびに蓄積
				totalExclude += std::chrono::duration_cast<std::chrono::milliseconds>(excludeEnd - excludeBegin).count();
			}
		}
		end = std::chrono::high_resolution_clock::now();
		break;
	case SORT_BUBBLE_DESC:		//降順のバブルソート
		begin = std::chrono::high_resolution_clock::now();
		for (arrayLength; arrayLength > 0; --arrayLength) {
			auto lastIndex = arrayLength - 1;
			for (int baseIndex = lastIndex; baseIndex > 0; --baseIndex) {
				int nextIndex = baseIndex - 1;
				if (m_nSortNum[baseIndex] > m_nSortNum[nextIndex]) {
					std::swap(m_nSortNum[baseIndex], m_nSortNum[nextIndex]);
				}
				auto excludeBegin = std::chrono::high_resolution_clock::now();
				Sleep(WAITING_TIME);
				m_pParent->OutputNumbers(m_nSortNum, m_sortText);	//変更
				auto excludeEnd = std::chrono::high_resolution_clock::now();
				//除外時間の合計をループが回るたびに蓄積
				totalExclude += std::chrono::duration_cast<std::chrono::milliseconds>(excludeEnd - excludeBegin).count();
			}
		}
		end = std::chrono::high_resolution_clock::now();
		break;
	default:
		break;
	}
	//時間を計算
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	duration -= totalExclude;
	// 時間を表示
	CString timeText;
	timeText.Format(_T("%d ms"), duration);

	CEdit* timeEdit = (CEdit*)GetDlgItem(IDC_EDIT_SORT_TIME);
	if (timeEdit != nullptr) {
		timeEdit->SetWindowText(timeText);
	}
	return m_nSortNum;
}