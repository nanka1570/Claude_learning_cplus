// GraphDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "SortApplication.h"
#include "afxdialogex.h"
#include "CGraphDlg.h"

// GraphDlg ダイアログ

IMPLEMENT_DYNAMIC(CGraphDlg, CDialogEx)

CGraphDlg::CGraphDlg(CWnd* pParent, CSortApplicationDlg::SORTENUM sortType, std::vector<int> RandomNum, std::vector<int> SortNum)
	: CDialogEx(IDD_GRAPH_DIALOG, pParent)
	, m_sortType(sortType)
	,m_RandomNum(RandomNum)
	, m_SortNum(SortNum)
	,m_pParentDlg((CSortApplicationDlg*)pParent)	//追加
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
	//CStatic* randomText{};
	//randomText = (CStatic*)GetDlgItem(IDC_EDIT_RANDOM_NUM);
	////コンストラクタのRandomNumを出力したい
	//m_pParentDlg->NumOutput(m_RandomNum, randomText);
	DrawBarGraph(m_RandomNum);
}

void CGraphDlg::DrawBarGraph(std::vector<int> data)
{
	//Pictureコントロールを取得
	CWnd* pPicture = GetDlgItem(IDC_SORT_GRAPH);
	if (pPicture == nullptr) return;

	//HDCを取得
	CDC* pDC = pPicture->GetDC();
	HDC hDC = pDC->m_hDC;

	//Pictureコントロールのクライアント領域を取得
	CRect rect;
	pPicture->GetClientRect(&rect);
	int width = rect.Width();
	int height = rect.Height();

	//背景を白で塗りつぶす
	HBRUSH hBrushWhite = (HBRUSH)GetStockObject(WHITE_BRUSH);
	FillRect(hDC, &rect, hBrushWhite);

	if (data.empty()) {
		pPicture->ReleaseDC(pDC);
		return;
	}

	//最大値を取得
	int maxValue = *std::max_element(data.begin(), data.end());

	//棒の幅を計算
	int barCount = data.size();
	int barWidth = width / barCount;
	int padding = 5;

	//各棒を描画
	for (int i = 0; i < barCount; ++i) {
		int value = data[i];

		//棒の座標を計算
		int left = i * barWidth + padding;
		int right = (i + 1) * barWidth - padding;
		int bottom = height;
		int top = height - (height * value / maxValue);

		//四角形を描画
		Rectangle(hDC, left, top, right, bottom);
	}

	pPicture->ReleaseDC(pDC);
}

