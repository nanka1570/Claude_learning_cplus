#pragma once
// SubDlg ダイアログ
class CSubDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSubDlg);
public:
	CSubDlg(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CSubDlg();
	
	// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUBDIALOG };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	DECLARE_MESSAGE_MAP()
	LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
