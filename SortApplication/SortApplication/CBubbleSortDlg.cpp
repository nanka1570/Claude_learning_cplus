// CBubbleSortDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "Resource.h"
#include "SortApplicationDlg.h"
#include "CBubbleSortDlg.h"
#include "GraphDlg.h"


// CBubbleSortDlg ダイアログ

IMPLEMENT_DYNAMIC(CBubbleSortDlg, CDialogEx)

CBubbleSortDlg::CBubbleSortDlg(const int tabIndex, CWnd* pParent /*= nullptr */ )
	: CDialogEx(IDD_BUBBLE_SORT_DIALOG, pParent)
	, m_tabIndex{ tabIndex }
{
	Create(IDD_BUBBLE_SORT_DIALOG, pParent);
}

CBubbleSortDlg::~CBubbleSortDlg()
{
}

void CBubbleSortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CBubbleSortDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect parentClient{};
	::GetClientRect(*m_pParentWnd, parentClient);
	CRect itemRect{};
	((CTabCtrl*)m_pParentWnd)->GetItemRect(m_tabIndex, itemRect);
	parentClient.top += itemRect.Height();
	MoveWindow(parentClient);

	return TRUE;
}


BEGIN_MESSAGE_MAP(CBubbleSortDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_BUBBLE_ASC, &CBubbleSortDlg::OnBnClickedRadioBubbleAsc)
	ON_BN_CLICKED(IDC_RADIO_BUBBLE_DESC, &CBubbleSortDlg::OnBnClickedRadioBubbleDesc)
END_MESSAGE_MAP()


// CBubbleSortDlg メッセージ ハンドラー
void CBubbleSortDlg::OnBnClickedRadioBubbleAsc()
{
	//SortApplicationDlg.cppにSORT_BUBBLE_ASCを使うように、メッセージを送る
	//PostMessage(CSortApplicationDlg::WM_SORTSWITCH, CSortApplicationDlg::SORT_BUBBLE_ASC, 0);
	RadioButtonExecuteJudge();
}


void CBubbleSortDlg::OnBnClickedRadioBubbleDesc()
{
	//SortApplicationDlg.cppにSORT_BUBBLE_DESCを使うように、メッセージを送る
	//PostMessage(CSortApplicationDlg::WM_SORTSWITCH, CSortApplicationDlg::SORT_BUBBLE_DESC, 0);
	RadioButtonExecuteJudge();
}


//昇順・降順どちらのラジオボタンが押されたかメッセージを送信する関数
void CBubbleSortDlg::RadioButtonExecuteJudge()
{

	// ラジオボタンの状態取得
	int radioSelection = GetCheckedRadioButton(IDC_RADIO_BUBBLE_ASC, IDC_RADIO_BUBBLE_DESC);

	CGraphDlg::SORTENUM sortType{};

	switch (radioSelection) {
	case IDC_RADIO_BUBBLE_ASC:
		sortType = CGraphDlg::SORT_BUBBLE_ASC;
		break;
	case IDC_RADIO_BUBBLE_DESC:
		sortType = CGraphDlg::SORT_BUBBLE_DESC;
		break;
	default:
		break;
	}

	//CWnd* pParent = GetParent();
	//if (pParent == nullptr)
	//	return;

	// 親ダイアログにメッセージ送信
	//GetParent()->PostMessage(CSortApplicationDlg::WM_SORTSWITCH, sortType, 0);
	GetParent()->GetParent()->PostMessage(CSortApplicationDlg::WM_SORTSWITCH, sortType, 0);
	//::PostMessage(CSortApplicationDlg::g_hWnd, CSortApplicationDlg::WM_SORTSWITCH, sortType, 0);

}