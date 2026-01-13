// CBubbleSortDlg.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹

#include "pch.h"
#include "Resource.h"
#include "SortApplication.h"
#include "SortApplicationDlg.h"
#include "BubbleSortDlg.h"

IMPLEMENT_DYNAMIC(CBubbleSortDlg, CDialogEx)

CBubbleSortDlg::CBubbleSortDlg(const int tabIndex, CWnd* pParent /*=nullptr*/)
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

void CBubbleSortDlg::OnBnClickedRadioBubbleAsc()
{
	RadioButtonExecuteJudge();
}

void CBubbleSortDlg::OnBnClickedRadioBubbleDesc()
{
	RadioButtonExecuteJudge();
}

void CBubbleSortDlg::RadioButtonExecuteJudge()
{
	int radioSelection = GetCheckedRadioButton(IDC_RADIO_BUBBLE_ASC, IDC_RADIO_BUBBLE_DESC);

	CSortApplicationDlg::SORTENUM sortType{};

	switch (radioSelection) {
	case IDC_RADIO_BUBBLE_ASC:
		sortType = CSortApplicationDlg::SORT_BUBBLE_ASC;
		break;
	case IDC_RADIO_BUBBLE_DESC:
		sortType = CSortApplicationDlg::SORT_BUBBLE_DESC;
		break;
	default:
		break;
	}

	GetParent()->GetParent()->PostMessage(CSortApplicationDlg::WM_SORTSWITCH, sortType, 0);
}