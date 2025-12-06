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
	ON_BN_CLICKED(IDOK, &CSubDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSubDlg::OnBnClickedCancel)
	ON_MESSAGE(MainDlg::WM_MYMESSAGE, &CSubDlg::OnMyMessage)
END_MESSAGE_MAP()

// CSubDlg メッセージ ハンドラー
LRESULT CSubDlg::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox(_T("受信しました！"));
	return 0;
}
void CSubDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnOK();
}

void CSubDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}
