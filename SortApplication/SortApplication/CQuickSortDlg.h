#pragma once

// CQuickSortDlg ダイアログ

class CQuickSortDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CQuickSortDlg)

public:
	explicit CQuickSortDlg(const int tabIndex, CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CQuickSortDlg() noexcept;

	// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QUICK_SORT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog() override;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedRadioQuickAsc();
	afx_msg void OnBnClickedRadioQuickDesc();

protected:
	void RadioButtonExecuteJudgeQuick();

protected:
	int m_tabIndex;
};
