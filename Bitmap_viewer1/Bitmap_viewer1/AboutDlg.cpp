#include "pch.h"
#include "Bitmap_viewer1.h"
#include "AboutDlg.h"

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
END_MESSAGE_MAP()

void CAboutDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
}

void CAboutDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
}
