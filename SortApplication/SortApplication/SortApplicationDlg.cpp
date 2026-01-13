// SortApplicationDlg.cpp : 実装ファイル

#include "pch.h"
#include "framework.h"
#include "SortApplication.h"
#include "SortApplicationDlg.h"
#include "afxdialogex.h"
#include <vector>

#include <algorithm>
#include <random>
#include <chrono>

#include "BubbleSortDlg.h"
//#include "CQuickSortDlg.h"
#include "CGraphDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

HWND CSortApplicationDlg::g_hWnd{ nullptr };

CSortApplicationDlg::CSortApplicationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SORTAPPLICATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSortApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_SORT_CHANGE, m_ctrlSortTab);
}

BEGIN_MESSAGE_MAP(CSortApplicationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_RANDOM, &CSortApplicationDlg::OnBnClickedRandom)
	ON_BN_CLICKED(IDC_BUTTON_SORT, &CSortApplicationDlg::OnBnClickedSort)
	ON_MESSAGE(WM_SORTSWITCH, OnSortSwitch)
	ON_MESSAGE(WM_NUMOUTPUT, OnNumOutPut)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SORT_CHANGE, OnTcnSelChangeTab)
END_MESSAGE_MAP()

BOOL CSortApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	g_hWnd = m_hWnd;
	m_ctrlSortTab.InsertItem(0, _T("バブルソート"));
	m_ctrlSortTab.InsertItem(1, _T("クイックソート"));
	m_tabs.emplace_back(new CBubbleSortDlg(0, &m_ctrlSortTab));
	//m_tabs.emplace_back(new CQuickSortDlg(1, &m_ctrlSortTab));
	m_ctrlSortTab.SetCurSel(0);
	m_tabs.at(0)->ShowWindow(SW_SHOW);

	return TRUE;
}

void CSortApplicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CSortApplicationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CSortApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSortApplicationDlg::OnOK()
{
	/*DONOTHING*/
}

void CSortApplicationDlg::OnCancel()
{
	CDialogEx::OnCancel();
}

void CSortApplicationDlg::OnTcnSelChangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	const auto numTabs = std::min<int>((int)m_ctrlSortTab.GetItemCount(), (int)m_tabs.size());
	const auto curSel = m_ctrlSortTab.GetCurSel();
	for (auto pos{ 0 }; pos < numTabs; ++pos) {
		auto& pTabWnd = m_tabs.at(pos);
		if (nullptr != pTabWnd) {
			pTabWnd->ShowWindow(curSel == pos ? SW_SHOW : SW_HIDE);
		}
	}
	*pResult = 0;
	return;
}

void CSortApplicationDlg::OnBnClickedRandom()
{
	RandomNumGenerate();
}

LRESULT CSortApplicationDlg::OnSortSwitch(WPARAM wParam, LPARAM lParam)
{
	sortType = static_cast<SORTENUM>(wParam);
	return 0;
}

LRESULT CSortApplicationDlg::OnNumOutPut(WPARAM wParam, LPARAM lParam)
{
	NumOutput(SortNum, sortText);
	return 0;
}

void CSortApplicationDlg::OnBnClickedSort()
{
	CGraphDlg* m_pDialog{};

	if (m_pDialog != nullptr)
		m_pDialog->ShowWindow(SW_HIDE);

	m_pDialog = new CGraphDlg(this, sortType, RandomNum, SortNum);
	m_pDialog->Create(IDD_GRAPH_DIALOG);
	m_pDialog->ShowWindow(SW_SHOW);
	m_pDialog->PostMessage(WM_VIEWSORT, 0, 0);
}

void CSortApplicationDlg::SortSwitch(SORTENUM sortType)
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

std::vector<int> CSortApplicationDlg::BubbleSort(SORTENUM sortOrder)
{
	int arrayLength = SortNum.size();
	int lastIndex = arrayLength - 1;

	auto begin{ std::chrono::system_clock::now() };
	auto end{ std::chrono::system_clock::now() };
	auto excludeBegin{ std::chrono::system_clock::now() };
	auto excludeEnd{ std::chrono::system_clock::now() };
	auto totalExclude{ std::chrono::duration_cast<std::chrono::milliseconds>(excludeEnd - excludeBegin).count() };
	auto duration{ (std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) - totalExclude };

	switch (sortOrder) {
	case SORT_BUBBLE_ASC:
		begin = std::chrono::system_clock::now();
		for (arrayLength; arrayLength > 0; --arrayLength) {
			for (int baseIndex = 0; baseIndex < lastIndex; ++baseIndex) {
				int nextIndex = baseIndex + 1;
				if (SortNum[baseIndex] > SortNum[nextIndex]) {
					std::swap(SortNum[baseIndex], SortNum[nextIndex]);
					swapCount += 1;
				}
				excludeBegin = std::chrono::system_clock::now();
				Sleep(50);
				NumOutput(SortNum, sortText);
				excludeEnd = std::chrono::system_clock::now();
				totalExclude += std::chrono::duration_cast<std::chrono::milliseconds>(excludeEnd - excludeBegin).count();
			}
		}
		end = std::chrono::system_clock::now();
		break;
	case SORT_BUBBLE_DESC:
		begin = std::chrono::system_clock::now();
		for (arrayLength; arrayLength > 0; --arrayLength) {
			for (int baseIndex = lastIndex; baseIndex > 0; --baseIndex) {
				int nextIndex = baseIndex - 1;
				if (SortNum[baseIndex] > SortNum[nextIndex]) {
					std::swap(SortNum[baseIndex], SortNum[nextIndex]);
				}
				excludeBegin = std::chrono::system_clock::now();
				Sleep(50);
				NumOutput(SortNum, sortText);
				excludeEnd = std::chrono::system_clock::now();
				totalExclude += std::chrono::duration_cast<std::chrono::milliseconds>(excludeEnd - excludeBegin).count();
			}
		}
		end = std::chrono::system_clock::now();
		break;
	default:
		break;
	}

	duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	duration -= totalExclude;

	CString timeText;
	timeText.Format(_T("%d ms"), duration);

	CEdit* timeEdit = (CEdit*)GetDlgItem(IDC_EDIT_SORT_TIME);
	if (timeEdit != nullptr) {
		timeEdit->SetWindowText(timeText);
	}
	return SortNum;
}

void CSortApplicationDlg::RandomNumGenerate()
{
	RandomNum.clear();

	auto inputNumLength = GetDlgItemInt(IDC_EDIT_INPUT_NUM);
	for (int i = 1; i <= inputNumLength; ++i) {
		RandomNum.push_back(i);
	}

	std::random_device rd;
	std::mt19937 generator(rd());
	std::shuffle(RandomNum.begin(), RandomNum.end(), generator);

	randomText = (CStatic*)GetDlgItem(IDC_EDIT_RANDOM_NUM);
	sortText = (CStatic*)GetDlgItem(IDC_EDIT_SORT_NUM);

	SortNum = RandomNum;

	NumOutput(RandomNum, randomText);
	NumOutput(RandomNum, sortText);

	return;
}

void CSortApplicationDlg::NumOutput(std::vector<int> array, CStatic* text)
{
	CString NumText{};
	for (const auto num : array) {
		NumText.AppendFormat(_T("%d "), num);
	}

	NumText.Trim(_T(" "));

	text->SetWindowText(NumText);
	text->RedrawWindow();

	return;
}