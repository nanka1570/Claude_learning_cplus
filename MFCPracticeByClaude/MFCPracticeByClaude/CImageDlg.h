#pragma once
#include <afxwin.h>

// CImageDlg

class CImageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageDlg)

public:
	CImageDlg();
	virtual ~CImageDlg();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	int m_rotationAngle = 0;
	bool m_isResizing = false;

	CImage m_image;
public:
	enum BUTTONS
	{
	BUTTON_DEFAULT = 0,
	BUTTON_90,
	BUTTON_180,
	BUTTON_270
	};
	void resizeBitmap(int width, int height);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnRotate(WPARAM wParam, LPARAM lParam);
};


