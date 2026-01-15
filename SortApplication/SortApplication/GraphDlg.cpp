// GraphDlg.cpp : 実装ファイル
//
#include "pch.h"
#include "SortApplication.h"
#include "SortApplicationDlg.h"
#include "afxdialogex.h"
#include "GraphDlg.h"
#include <chrono>
#include <algorithm>
#include <vector>


// GraphDlg ダイアログ
IMPLEMENT_DYNAMIC(CGraphDlg, CDialogEx)

CGraphDlg::CGraphDlg(CWnd* pParent, SORTENUM sortType, std::vector<int> sortNum)
	: CDialogEx(IDD_GRAPH_DIALOG, pParent)
	, m_nSortType(sortType)
	, m_nSortNum(sortNum)
	, m_pParent((CSortApplicationDlg*)pParent)
{

}

CGraphDlg::~CGraphDlg()
{
}

void CGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GRAPH, m_ctrlViewGraph);
}


BEGIN_MESSAGE_MAP(CGraphDlg, CDialogEx)
	ON_MESSAGE(CSortApplicationDlg::WM_VIEWSORT, OnViewSort)
END_MESSAGE_MAP()
// GraphDlg メッセージ ハンドラー


LRESULT CGraphDlg::OnViewSort(WPARAM wParam, LPARAM lParam)
{
	ViewSortGraph();
	return 0;
}


//グラフを表示する
void CGraphDlg::ViewSortGraph()
{
	//ランダムな値を表示
	m_sortText = (CStatic*)GetDlgItem(IDC_EDIT_RANDOM_NUM);
	//CSortApplicationDlgのNumOutput関数を使って出力
	m_pParent->NumOutput(m_nSortNum, m_sortText);
	Sleep(1000);

	//バブルソートをテキスト表示
	//SortSwitch(m_nSortType);
	//グラフを表示する
	DrawSortGraph(m_nSortType);
}


void CGraphDlg::DrawSortGraph(SORTENUM sortType)
{
	//m_nSortNumを使ってグラフを表示する
	//x軸がi
	//y軸がm_nSortNum[i]

	auto length = m_nSortNum.size();				//配列の長さを取得
	auto width = length;							//配列の長さを図形の幅としてwidthに代入
	auto maxIterator = std::max_element(m_nSortNum.begin(), m_nSortNum.end());	//配列の中で一番値が大きいイテレータを取得
	auto maxIndex = std::distance(m_nSortNum.begin(), maxIterator);				//そのイテレータのインデックスを取得
	auto height = m_nSortNum[maxIndex];											//一番大きい値をheightに代入

	//m_nSortNumの初期値をまずグラフにできるようにする

	//クライアント領域のwidth、heightを求める
	CRect clientRect;
	GetClientRect(clientRect);
	auto targetWidth = clientRect.Width();
	auto taregetHeight = clientRect.Height();

	if (!m_imageGraph.IsNull())
		m_imageGraph.Destroy();

	m_imageGraph.Create(targetWidth, taregetHeight, 16);		//グラフ本体
	HDC hDC = m_imageGraph.GetDC();				//描く紙
	//RECT rect = { 0, 0, 100, 100 };
	HBRUSH brushWhite = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(hDC, &clientRect, brushWhite);
	DeleteObject(brushWhite);

	for (int i = 0; i < 5; ++i) {
		CImage barImage;
		barImage.Create(width, height, 16);

		HDC hDCBar = barImage.GetDC();
		RECT barRect = { 0, 0, width, height };
		HBRUSH brushBlue = CreateSolidBrush(RGB(0, 0, 255));
		FillRect(hDCBar, &barRect, brushBlue);
		barImage.ReleaseDC();

		barImage.BitBlt(hDC, i * 10, 200, SRCCOPY);

		barImage.Destroy();
	}

	

	m_imageGraph.ReleaseDC();
	m_ctrlViewGraph.SetBitmap(m_imageGraph);	//ピクチャコントロールに出力
	m_imageGraph.Destroy();

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
	auto arrayLength = m_nSortNum.size();
	//時間を計測するための変数
	auto begin{ std::chrono::high_resolution_clock::now() };
	auto end{ std::chrono::high_resolution_clock::now() };
	auto totalExclude{ 0LL };

	switch (sortOrder) {
	case SORT_BUBBLE_ASC:		//昇順のバブルソート
		begin = std::chrono::high_resolution_clock::now();						//処理開始時間
		for (arrayLength; arrayLength > 0; --arrayLength) {						//配列の長さ分繰り返す
			auto lastIndex = arrayLength - 1;									//探索範囲を縮める
			for (auto baseIndex = 0; baseIndex < lastIndex; ++baseIndex) {		//baseIndexが基点でlastIndexまで探索する
				auto nextIndex = baseIndex + 1;									//nextIndexにbaseIndexの次の値を代入する
				if (m_nSortNum[baseIndex] > m_nSortNum[nextIndex]) {			//基点(baseIndex)が次点(nextIndex)より大きいとき
					std::swap(m_nSortNum[baseIndex], m_nSortNum[nextIndex]);	//基点と次点を入れ替える
					m_swapCount += 1;											//入れ替えた回数を数える
				}
				auto excludeBegin = std::chrono::high_resolution_clock::now();	//除外処理開始時間
				Sleep(WAITING_TIME);											//入れ替わっていることが確認できるように待ち時間を設定する
				m_pParent->NumOutput(m_nSortNum, m_sortText);					//SortApplicationDlgのNumOutput()で出力する
				auto excludeEnd = std::chrono::high_resolution_clock::now();	//除外処理終了時間
				//除外時間の合計をループが回るたびに蓄積
				totalExclude += std::chrono::duration_cast<std::chrono::milliseconds>(excludeEnd - excludeBegin).count();
			}
		}
		end = std::chrono::high_resolution_clock::now();	//処理終了時間
		break;
	case SORT_BUBBLE_DESC:		//降順のバブルソート
		begin = std::chrono::high_resolution_clock::now();	//処理開始時間
		for (arrayLength; arrayLength > 0; --arrayLength) {						//配列の長さ分繰り返す
			auto lastIndex = arrayLength - 1;									//探索範囲を縮める
			for (int baseIndex = lastIndex; baseIndex > 0; --baseIndex) {		//baseIndexが基点でlastIndexまで探索する
				int nextIndex = baseIndex - 1;									//nextIndexにbaseIndexの次の値を代入する
				if (m_nSortNum[baseIndex] > m_nSortNum[nextIndex]) {			//基点(baseIndex)が次点(nextIndex)より大きいとき
					std::swap(m_nSortNum[baseIndex], m_nSortNum[nextIndex]);	//基点と次点を入れ替える
					m_swapCount += 1;
				}
				auto excludeBegin = std::chrono::high_resolution_clock::now();	//除外処理終了時間
				Sleep(WAITING_TIME);
				m_pParent->NumOutput(m_nSortNum, m_sortText);
				auto excludeEnd = std::chrono::high_resolution_clock::now();	//除外処理終了時間
				//除外時間の合計をループが回るたびに蓄積
				totalExclude += std::chrono::duration_cast<std::chrono::milliseconds>(excludeEnd - excludeBegin).count();
			}
		}
		end = std::chrono::high_resolution_clock::now();	//処理終了時間
		break;
	default:
		break;
	}
	//時間を計算
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	duration -= totalExclude;
	// 時間を表示
	CString timeText;
	timeText.Format(_T("%d ms"), duration);

	CEdit* timeEdit = (CEdit*)GetDlgItem(IDC_EDIT_SORT_TIME);
	if (timeEdit != nullptr) {
		timeEdit->SetWindowText(timeText);
	}
	return;
}