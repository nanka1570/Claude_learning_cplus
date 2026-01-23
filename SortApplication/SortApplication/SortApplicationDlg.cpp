// SortApplicationDlg.cpp : 実装ファイル

#include "pch.h"
#include "framework.h"
#include "SortApplication.h"
#include "SortApplicationDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <tchar.h>

#include <algorithm>
#include <random>
#include <chrono>

#include "CBubbleSortDlg.h"
#include "CQuickSortDlg.h"
#include "GraphDlg.h"

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

	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	g_hWnd = m_hWnd;
	m_ctrlSortTab.InsertItem(0, _T("バブルソート"));
	m_ctrlSortTab.InsertItem(1, _T("クイックソート"));
	m_tabs.emplace_back(new CBubbleSortDlg(0, &m_ctrlSortTab));
	m_tabs.emplace_back(new CQuickSortDlg(1, &m_ctrlSortTab));
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
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
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


//tabの切り替えをする関数
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


//受け取ったメッセージによって、ソート・昇順と降順を切り替える
LRESULT CSortApplicationDlg::OnSortSwitch(WPARAM wParam, LPARAM lParam)
{
	//どちらのソートが選ばれているかメッセージを受け取る
	sortType = static_cast<CGraphDlg::SORTTYPEORDER>(wParam);

	return 0;
}


//ボタンが押されたときに選択された並べ替えを行う
//ボタンを押されたときにグラフの表示ダイアログを開く
void CSortApplicationDlg::OnBnClickedSort()
{
#if 0
	if (m_pDialog != nullptr) {
		m_pDialog->ShowWindow(SW_HIDE);
	}
	m_pDialog = new CGraphDlg(this, sortType, m_sortNum);
	m_pDialog->Create(IDD_GRAPH_DIALOG);
	m_pDialog->ShowWindow(SW_SHOW);
	m_pDialog->PostMessage(WM_VIEWSORT, 0, 0);
	//SortSwitch(sortType);
#else
	if (m_pDialog == nullptr) {
		m_pDialog = new CGraphDlg(this, sortType, m_sortNum);
		m_pDialog->Create(IDD_GRAPH_DIALOG);
		m_pDialog->ShowWindow(SW_SHOW);
	}
	m_pDialog->PostMessage(WM_VIEWSORT, 0, 0);
#endif
}


//１から昇順に挿入された配列を生成し、配列をランダムで入れ替える
void CSortApplicationDlg::RandomNumGenerate()
{
	//RandomNumに値が入っていたら削除する
	m_randomNum.clear();

	//テキストボックスに入力された数字を取得
	auto inputNumLength = GetDlgItemInt(IDC_EDIT_INPUT_NUM);
	//入力された数字を長さとして配列を作成する
	for (int i = 1; i <= inputNumLength; ++i) {
		m_randomNum.push_back(i);
	}

	//配列をランダムに並べ替える
	std::random_device rd;		//ハードウェアに基づいたランダムな数値をrandom_deviceクラスで生成
	std::mt19937 generator(rd());		//random_deviceクラスをシード値として、mt19937で乱数を生成
	std::shuffle(m_randomNum.begin(), m_randomNum.end(), generator);		//乱数を使い配列をシャッフルする

	//ランダムに並べ替えた数値を出力
	m_randomText = (CStatic*)GetDlgItem(IDC_EDIT_RANDOM_NUM);
	m_sortText = (CStatic*)GetDlgItem(IDC_EDIT_SORT_NUM);

	//SortNumに初期値を代入
	m_sortNum = m_randomNum;

	NumOutput(m_randomNum, m_randomText);
	//NumOutput(RandomNum, sortText);

	return;
}


//int型の配列を文字列にしてテキストボックスに出力
void CSortApplicationDlg::NumOutput(const std::vector<int>& array, CStatic* text)
{
	//RandomNum[]をテキストに変換
	CString NumText{};
	for (const auto num : array) {
		NumText.AppendFormat(_T("%d "), num);
	}

	//末尾の空白をトリミングする
	NumText.Trim(_T(" "));


	//出力のテキストボックスに配列を表示する
	text->SetWindowText(NumText);
	text->RedrawWindow();

	return;
}