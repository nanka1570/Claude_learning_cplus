// CImageDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "MFCPracticeByClaude.h"
#include "CImageDlg.h"


// CImageDlg

IMPLEMENT_DYNAMIC(CImageDlg, CWnd)

CImageDlg::CImageDlg()
{

}

CImageDlg::~CImageDlg()
{
}


BEGIN_MESSAGE_MAP(CImageDlg, CWnd)
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CImageDlg メッセージ ハンドラー




LRESULT CImageDlg::OnRotate(WPARAM wParam, LPARAM lParam)
{
	BUTTONS button = static_cast<BUTTONS>(wParam);

	switch (button)
	{
		case BUTTON_DEFAULT:
			m_rotationAngle = 0;
			break;
		case BUTTON_90:
			m_rotationAngle = (m_rotationAngle + 90) % 360;
			break;
		case BUTTON_180:
			m_rotationAngle = (m_rotationAngle + 180) % 360;
			break;
		case BUTTON_270:
			m_rotationAngle = (m_rotationAngle + 270) % 360;
			break;
		default:
			break;
	}
	resizeBitmap(m_image.GetWidth(), m_image.GetHeight());

	return 0;

}


void CImageDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_image.IsNull())
		return;
	//if (!m_ctrlImage)
	//	return;
	if (m_ctrlImage.GetsafeHwnd() == NULL)
		return;
	resizeBitmap(cx, cy);
}


void CImageDlg::resizeBitmap(int width, int height)
{
	if (m_isResizing == true)
	{
		return;
	}
	m_isResizing = true;

	int displayWidth = width;
	int displayHeight = height;
	
	if (m_rotationAngle == 90 || m_rotationAngle == 270)
	{
		displayWidth = height;
		displayHeight = width;
	}

	CRect windowRect;
	GetWindowRect(windowRect);
	MoveWindow(windowRect.left, windowRect.top, displayWidth, displayHeight);

	m_isResizing = false;
}