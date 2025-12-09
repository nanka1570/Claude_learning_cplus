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


void CSubDlg::DrawImage()
{
	CImage image;

	image.Create(100, 100, 24);

	HDC hDC = image.GetDC();

	image.ReleaseDC();
	if (!image.IsNull())
		image.Destroy();
	//image.BitBlt(hDC, 200, 200, SRCCOPY);
	image.Create(200, 200, 24);

	HDC hDC2 = image.GetDC();

	image.ReleaseDC();
}


void CSubDlg::ResizeImage(int newWidth, int newHeight)
{
	if (!m_image.IsNull())
	{
		m_image.Destroy();
	}
	m_image.Create(newWidth, newHeight, 24);

	HDC hDC = m_image.GetDC();
	
	m_image.BitBlt(hDC, 0, 0, SRCCOPY);
	
	m_image.ReleaseDC();

}
