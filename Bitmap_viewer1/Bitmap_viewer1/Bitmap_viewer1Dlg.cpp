#include "pch.h"
#include "Bitmap_viewer1.h"
#include "Bitmap_viewer1Dlg.h"
#include "AboutDlg.h"
#include <vector>
#include "ImageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBitmapviewer1Dlg ダイアログ
CBitmapviewer1Dlg::CBitmapviewer1Dlg(CWnd* pParent)
    : CDialogEx(IDD_BITMAP_VIEWER1_DIALOG, pParent)
    , m_strEditBitmapFilename(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBitmapviewer1Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_BITMAP_FILENAME, m_strEditBitmapFilename);
}

BEGIN_MESSAGE_MAP(CBitmapviewer1Dlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_BITMAPFILE_OPEN, &CBitmapviewer1Dlg::OnBnClickedBitmapFileOpen)
    ON_BN_CLICKED(IDC_BUTTON_ROTATE_LEFT, &CBitmapviewer1Dlg::OnBnClickedButtonRotateLeft)
    ON_BN_CLICKED(IDC_BUTTON_ROTATE_RIGHT, &CBitmapviewer1Dlg::OnBnClickedButtonRotateRight)
    ON_BN_CLICKED(IDC_BUTTON_FLIP_UPSIDE_DOWN, &CBitmapviewer1Dlg::OnBnClickedButtonFlipUpsideDown)
    ON_BN_CLICKED(IDC_BUTTON_FLIP_LEFT_RIGHT, &CBitmapviewer1Dlg::OnBnClickedButtonFlipLeftRight)
    ON_BN_CLICKED(IDC_BUTTON_BITMAPFILE_SAVE, &CBitmapviewer1Dlg::OnBnClickedButtonBitmapfileSave)
END_MESSAGE_MAP()

// CBitmapviewer1Dlg メッセージ ハンドラー
BOOL CBitmapviewer1Dlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    SetIcon(m_hIcon, TRUE);   // 大きいアイコンの設定
    SetIcon(m_hIcon, FALSE);  // 小さいアイコンの設定

    return TRUE;
}

void CBitmapviewer1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

void CBitmapviewer1Dlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
    printf("");
}

HCURSOR CBitmapviewer1Dlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

// OKボタンのイベントハンドラー
void CBitmapviewer1Dlg::OnOK()
{
    // 読み込むことができるビットマップがあればダイアログに表示する
    if (LoadBitmap())
    {
        // ImageDlgのダイアログを作成する（モードレス）
        if (nullptr == m_pDialog)
        {
            m_pDialog = new CImageDlg(this);
            m_pDialog->setBitmapFile(filename);
            m_pDialog->Create(IDD_IMAGE_DIALOG, this);
            m_pDialog->ShowWindow(SW_SHOW);
        }
    }
    return;
    printf("");
}

// キャンセルボタンのイベントハンドラー
void CBitmapviewer1Dlg::OnCancel()
{
    CDialogEx::OnCancel();
}

// ファイル選択ダイアログのイベントハンドラー
void CBitmapviewer1Dlg::OnBnClickedBitmapFileOpen()
{
    CFileDialog dlg(TRUE);
    // ダイアログボックスでファイルが選択される
    if (dlg.DoModal() == IDOK)
    {
        // 選択されたフルパスを取得
        m_strEditBitmapFilename = dlg.GetPathName();
        UpdateData(FALSE);
    }
}

// ファイルを読み込み
bool CBitmapviewer1Dlg::LoadBitmap()
{
    filename = m_strEditBitmapFilename.GetString();

    // ファイルの存在確認
    if (std::filesystem::exists(filename) && !std::filesystem::is_directory(filename))
    {
        CFile file;
        // m_strEditBitmapFilenameに入力されているパスを読み取り専用で開くことに成功した場合
        if (file.Open(m_strEditBitmapFilename, CFile::modeRead | CFile::typeBinary))
        {
            const size_t size = std::filesystem::file_size(filename);

            if ((sizeof(BITMAPFILEHEADER) + sizeof(BITMAPCOREHEADER)) < size)
            {
                m_bitmapImage.resize(size, 0);

                file.Read(m_bitmapImage.data(), size);
                file.Close();

                // キャスト
                BITMAPFILEHEADER* pFileHeader = (BITMAPFILEHEADER*)m_bitmapImage.data();
                m_offset = pFileHeader->bfOffBits;
                uint8_t* pBitmapData = m_bitmapImage.data() + m_offset;

                BITMAPINFOHEADER* pInfoHeader_ = (BITMAPINFOHEADER*)(m_bitmapImage.data() + sizeof(BITMAPFILEHEADER));
                const auto infoHeaderSize = pInfoHeader_->biSize;

                // 初代のバージョン「BITMAPINFOHEADER」とサイズが一致するか
                if (infoHeaderSize == sizeof(BITMAPINFOHEADER))
                {
                    BITMAPINFOHEADER* pInfoHeader = pInfoHeader_;
                    m_width = pInfoHeader->biWidth;
                    m_height = pInfoHeader->biHeight;
                    m_planes = pInfoHeader->biPlanes;
                    m_bits = pInfoHeader->biBitCount;
                }
                else if (infoHeaderSize == 52)
                {
                }
                else if (infoHeaderSize == 56)
                {
                }
                // バージョン４の「BITMAPV4HEADER」とサイズが一致するか
                else if (infoHeaderSize == sizeof(BITMAPV4HEADER))
                {
                    BITMAPV4HEADER* pInfoHeader = (BITMAPV4HEADER*)pInfoHeader_;
                    m_width = pInfoHeader->bV4Width;
                    m_height = pInfoHeader->bV4Height;
                    m_planes = pInfoHeader->bV4Planes;
                    m_bits = pInfoHeader->bV4BitCount;
                }
                // バージョン５の「BITMAPV5HEADER」とサイズが一致するか
                else if (infoHeaderSize == sizeof(BITMAPV5HEADER))
                {
                    BITMAPV5HEADER* pInfoHeader = (BITMAPV5HEADER*)pInfoHeader_;
                    m_width = pInfoHeader->bV5Width;
                    m_height = pInfoHeader->bV5Height;
                    m_planes = pInfoHeader->bV5Planes;
                    m_bits = pInfoHeader->bV5BitCount;
                }
                else
                {
                }

                if (m_width > 0 && m_height > 0)
                {
                    return true;
                }
            }
        }
    }
    return false;
    printf("");
}


void CBitmapviewer1Dlg::OnBnClickedButtonFlipUpsideDown()
{
    if (m_pDialog != nullptr)
    {
        m_pDialog->PostMessage(WM_ROTATEBITMAP, CImageDlg::BUTTON_HORIZONTAL_REVERSE, 0);
        //DrawRotate(BUTTON_HORIZONTAL_REVERSE);
    }
}

void CBitmapviewer1Dlg::OnBnClickedButtonFlipLeftRight()
{
    if (m_pDialog != nullptr)
    {
        m_pDialog->PostMessage(WM_ROTATEBITMAP, CImageDlg::BUTTON_VERTICAL_REVERSE, 0);
        //DrawRotate(BUTTON_VERTICAL_REVERSE);
    }
}

void CBitmapviewer1Dlg::OnBnClickedButtonRotateRight()
{
    if (m_pDialog != nullptr)
    {
        m_pDialog->PostMessage(WM_ROTATEBITMAP, CImageDlg::BUTTON_90, 0);
        //DrawRotate(BUTTON_90);
    }
}

void CBitmapviewer1Dlg::OnBnClickedButtonRotateLeft()
{
    if (m_pDialog != nullptr)
    {
        m_pDialog->PostMessage(WM_ROTATEBITMAP, CImageDlg::BUTTON_270, 0);
        //DrawRotate(BUTTON_270);
    }
}


void CBitmapviewer1Dlg::OnBnClickedButtonBitmapfileSave()
{
    if (m_pDialog != nullptr)
    {
		m_pDialog->PostMessage(WM_SAVEBITMAPFILE, 0, 0);
    }
}
