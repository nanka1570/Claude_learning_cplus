#pragma once

// CBubbleSortDlg ダイアログ

class CBubbleSortDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBubbleSortDlg)

public:
	explicit CBubbleSortDlg(const int tabIndex, CWnd* pParent = nullptr);
	virtual ~CBubbleSortDlg() noexcept;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BUBBLE_SORT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog() override;

protected:
	int m_tabIndex;
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedRadioBubbleAsc();
	afx_msg void OnBnClickedRadioBubbleDesc();
	void RadioButtonExecuteJudge();
};