
// MainDlg.h : ヘッダー ファイル
//

#pragma once


// MainDlg ダイアログ
class MainDlg : public CDialogEx
{
// コンストラクション
public:
	MainDlg(CWnd* pParent = nullptr);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCPRACTICEBYCLAUDE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	static const UINT WM_MYMESSAGE = WM_USER + 100;
	afx_msg void OnBnClickedButton1();
};
