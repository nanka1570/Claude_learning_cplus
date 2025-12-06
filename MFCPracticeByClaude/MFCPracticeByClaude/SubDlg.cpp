#include "pch.h"
#include "framework.h"
#include "MFCPracticeByClaude.h"
#include "mainDlg.h"
#include "SubDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSubDlg ダイアログ
IMPLEMENT_DYNAMIC(CSubDlg, CDialogEx)
CSubDlg::CSubDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SUBDIALOG, pParent)
{
}
CSubDlg::~CSubDlg()
{
}

void CSubDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSubDlg, CDialogEx)
	ON_MESSAGE(MainDlg::WM_MYMESSAGE, &CSubDlg::OnMyMessage)
END_MESSAGE_MAP()

// CSubDlg メッセージ ハンドラー
LRESULT CSubDlg::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox(_T("受信しました！"));
	return 0;
}