#pragma once
#include "afxdialogex.h"

//追加
#include <vector>
#include <filesystem>

// CImageDlg ダイアログ

class CImageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageDlg)

public:
	enum BUTTONS {
		BUTTON_DEFAULT = 0,
		BUTTON_90,
		BUTTON_180,
		BUTTON_270,
		BUTTON_HORIZONTAL_REVERSE,
		BUTTON_VERTICAL_REVERSE
	};
	//int coordinate(button){}
	//
public:
	void DrawRotate(const BUTTONS button);
	void DrawFlip(const BUTTONS button) {};

	//protected:
	 //bool BitmapJudge();

public:
	explicit CImageDlg(CWnd* pParent = nullptr);  // 標準コンストラクター
	virtual ~CImageDlg();

public:
	void setBitmap(int m_offset, int m_width, int m_height, int m_bits, uint8_t* pImage);
	void setBitmapFile(std::filesystem::path filename);
	void DrawBitmap();
	void ImageSave();

	//void flipUpsideDown();
	 //void flipLeftRight();
	 //void rotateRight();
	 //void RotateLeft();

public:
	virtual BOOL OnInitDialog();

public:
	int m_newOffset = 0;
	int m_newWidth = 0;
	int m_newHeight = 0;
	int m_newBits = 0;
	uint8_t* m_pImage = nullptr;
	std::filesystem::path bitmapFilename;

public:
	int srcWidth = 0;
	int srcHeight = 0;
	int targetWidth = 0;
	int targetHeight = 0;
	int bpp = 0;

public:
	CString m_strSaveBitmapFilename;

	// ダイアログ データ
protected:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGE_DIALOG };
#endif

protected:
	CStatic m_ctrlBitmap2{};
	//オリジナルのビットマップイメージ
	CImage m_image{};
	//複製したビットマップイメージ
	CImage m_imageView{};
	//回転や反転するために複製したビットマップイメージ
	CImage m_imageTemp{};

	//POINT points[3] = {};
	 //POINT points;
	 //std::vector<std::vector<int>> points{};

	//protected:
	// bool IsDefault();
	// bool IsUnder();
	// bool IsLeft();

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;   // DDX/DDV サポート

	virtual void OnOK() override;
	virtual void OnCancel() override;
	virtual void PostNcDestroy();

protected:
	afx_msg //ビットマップをダイアログに表示する関数
		LRESULT OnDrawBitmap(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	//int widthSrc = 0;
	//int heightSrc = 0;

public:
	afx_msg void OnBnClickedButtonBitmapfileSave();
	afx_msg void OnBnClickedButtonFlipUpsideDown();
	afx_msg void OnBnClickedButtonFlipLeftRight();
	afx_msg void OnBnClickedButtonRotateRight();
	afx_msg void OnBnClickedButtonRotateLeft();

};