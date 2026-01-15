#pragma once

// CBubbleSortDlg ダイアログ

class CBubbleSortDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBubbleSortDlg)

public:
	explicit CBubbleSortDlg(const int tabIndex, CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CBubbleSortDlg() noexcept;

public:
	void RadioButtonExecuteJudge();
	// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BUBBLE_SORT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog() override;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedRadioBubbleAsc();		//昇順ラジオボタン
	afx_msg void OnBnClickedRadioBubbleDesc();		//降順ラジオボタン

protected:
	int m_tabIndex;		//タブのインデックス
};