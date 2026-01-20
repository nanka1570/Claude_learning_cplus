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
#if 0
	//ランダムな値を表示
	m_sortText = (CStatic*)GetDlgItem(IDC_EDIT_RANDOM_NUM);
	//CSortApplicationDlgのNumOutput関数を使って出力
	m_pParent->NumOutput(m_nSortNum, m_sortText);
	Sleep(1000);
	//バブルソートをテキスト表示
	//SortSwitch(m_nSortType);
#else
	//グラフを表示する
	//DrawSortGraph(m_nSortType);
	SortSwitch(m_nSortType);
#endif
}


void CGraphDlg::DrawSortGraph(SORTENUM sortType)
{
	//m_nSortNumを使ってグラフを表示する
	//x軸がi
	//y軸がm_nSortNum[i]

	auto maxIterator = std::max_element(m_nSortNum.begin(), m_nSortNum.end());	//配列の中で一番値が大きいイテレータを取得
	auto maxIndex = std::distance(m_nSortNum.begin(), maxIterator);				//そのイテレータのインデックスを取得
	auto maxValue = m_nSortNum[maxIndex];										//一番大きい値をheightに代入

	//m_nSortNumの初期値（ランダム数列）をグラフにできるようにする

	//クライアント領域のwidth、heightを求める
	CRect clientRect;
	GetClientRect(clientRect);
	auto clientWidth = clientRect.Width();
	auto clientHeight = clientRect.Height();

	if (!m_imageGraph.IsNull())
		m_imageGraph.Destroy();

	m_imageGraph.Create(clientWidth, clientHeight, 24);		//グラフ本体
	HDC hDCMain = m_imageGraph.GetDC();				//描くメインの紙

	CBrush brushMain;
	brushMain.CreateSolidBrush(RGB(255, 255, 255));	//メインの紙の背景色を生成
	//SetBkColor(hDCMain, RGB(255, 0, 0));

	CRect rectMain = { 0, 0, clientWidth, clientHeight };	//背景を描画する位置
	//hDCMain（紙）のrectMain（0, 0, クライアントの幅, クライアントの高さ）の位置をbrushMain（白）で塗りつぶす
	FillRect(hDCMain, rectMain, brushMain);


	CImage m_imageBar;
	auto length = m_nSortNum.size();				//配列の長さを取得
	auto barWidth = clientWidth / length;
	CBrush brushBar;
	brushBar.CreateSolidBrush(RGB(0, 100, 255));
	CRect rectBar;

	for (auto i = 0; i < length; ++i) {
		auto barHeight = m_nSortNum[i] * clientHeight / maxValue;	//棒の高さを計算
		if (barWidth < 1) barWidth = 1;  // ← 追加
		int left = i * barWidth;			//左	
		int top = clientHeight - barHeight;	//上
		int right = left + barWidth;		//右	
		int bottom = clientHeight;			//下
		rectBar = { left, top, right, bottom };

		//m_imageBar.Create(barWidth, barHeight, 24);
		//HDC hDCBar = m_imageBar.GetDC();
		FillRect(hDCMain, rectBar, brushBar);
		//
	}

	m_imageGraph.ReleaseDC();

	m_ctrlViewGraph.SetBitmap(m_imageGraph);	//ピクチャコントロールに出力

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
#if 0
				Sleep(WAITING_TIME);											//入れ替わっていることが確認できるように待ち時間を設定する
				m_pParent->NumOutput(m_nSortNum, m_sortText);					//SortApplicationDlgのNumOutput()で出力する
#else
				DrawSortGraph(m_nSortType);									//グラフを描く関数を呼び出す
#endif
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
			for (auto baseIndex = 0; baseIndex < lastIndex; ++baseIndex) {		//baseIndexが基点でlastIndexまで探索する
				auto nextIndex = baseIndex + 1;									//nextIndexにbaseIndexの次の値を代入する
				if (m_nSortNum[baseIndex] < m_nSortNum[nextIndex]) {			//基点(baseIndex)が次点(nextIndex)より大きいとき
					std::swap(m_nSortNum[baseIndex], m_nSortNum[nextIndex]);	//基点と次点を入れ替える
					m_swapCount += 1;
				}
				auto excludeBegin = std::chrono::high_resolution_clock::now();	//除外処理終了時間
#if 0
				Sleep(WAITING_TIME);											//入れ替わっていることが確認できるように待ち時間を設定する
				m_pParent->NumOutput(m_nSortNum, m_sortText);					//SortApplicationDlgのNumOutput()で出力する
#else
				DrawSortGraph(m_nSortType);									//グラフを描く関数を呼び出す
#endif
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