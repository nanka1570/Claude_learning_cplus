// CQuickSortDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "Resource.h"
#include "SortApplication.h"
#include "SortApplicationDlg.h"
#include "CQuickSortDlg.h"


// CQuickSortDlg ダイアログ

IMPLEMENT_DYNAMIC(CQuickSortDlg, CDialogEx)

CQuickSortDlg::CQuickSortDlg(const int tabIndex, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_QUICK_SORT_DIALOG, pParent)
	, m_tabIndex{ tabIndex }
{
	Create(IDD_QUICK_SORT_DIALOG, pParent);
}

CQuickSortDlg::~CQuickSortDlg()
{
}

void CQuickSortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CQuickSortDlg::OnInitDialog()
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


BEGIN_MESSAGE_MAP(CQuickSortDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_QUICK_ASC, &CQuickSortDlg::OnBnClickedRadioQuickAsc)
	ON_BN_CLICKED(IDC_RADIO_QUICK_DESC, &CQuickSortDlg::OnBnClickedRadioQuickDesc)
END_MESSAGE_MAP()


// CQuickSortDlg メッセージ ハンドラー

void CQuickSortDlg::OnBnClickedRadioQuickAsc()
{
	RadioButtonExecuteJudgeQuick();
}

void CQuickSortDlg::OnBnClickedRadioQuickDesc()
{
	RadioButtonExecuteJudgeQuick();
}

void CQuickSortDlg::RadioButtonExecuteJudgeQuick()
{
	// ラジオボタンの状態取得
	int radioSelection = GetCheckedRadioButton(IDC_RADIO_QUICK_ASC, IDC_RADIO_QUICK_DESC);

	CGraphDlg::SORTTYPEORDER sortType{};

	switch (radioSelection) {
	case IDC_RADIO_QUICK_ASC:
		sortType = CGraphDlg::SORT_QUICK_ASC;
		break;
	case IDC_RADIO_QUICK_DESC:
		sortType = CGraphDlg::SORT_QUICK_DESC;
		break;
	default:
		break;
	}
	GetParent()->GetParent()->PostMessage(CSortApplicationDlg::WM_SORTSWITCH, sortType, 0);

}
