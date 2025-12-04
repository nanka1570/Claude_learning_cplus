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

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);

//追加
protected:
	void updateBitmapWindow();
	//// ★リサイズ用：初期サイズを記録
	//CRect m_rectInitialClient{};      // クライアント領域の初期サイズ
	//bool m_bInitialized = false;      // 初期化完了フラグ

	//// ★コントロール情報を保持する構造体
	//struct ControlInfo {
	//	UINT nID;           // コントロールID
	//	CRect rectInitial;  // 初期位置
	//	double moveX;       // X方向移動率（0.0～1.0）
	//	double moveY;       // Y方向移動率（0.0～1.0）
	//	double sizeX;       // X方向サイズ変化率（0.0～1.0）
	//	double sizeY;       // Y方向サイズ変化率（0.0～1.0）
	//};
	//std::vector<ControlInfo> m_controls{};

	//// ★ヘルパー関数
	//void UpdateControlPositions();
	//void RegisterControl(UINT nID, double moveX, double moveY, double sizeX, double sizeY);
//protected:
//	CRect m_rectInitialDialog{};	//ダイアログの初期サイズ
//	CRect m_rectInitialBitmap{};	//ビットマップ表示用スタティックコントロールの初期サイズ
//	std::vector<std::pair<UINT, CRect>> m_initialControlRects{}; //コントロールの初期サイズとIDのペアの配列
//
//	//拡大率
//	double m_zoomRation = 1.0;
	//protected:
	//	bool m_bImageLoaded = false;  // 画像読み込み完了フラグ
};