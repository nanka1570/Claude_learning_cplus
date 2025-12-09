#include "pch.h"
#include "Bitmap_viewer2.h"
#include "Bitmap_viewer2Dlg.h"
#include "AboutDlg.h"
#include <vector>
#include "ImageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBitmapviewer2Dlg::CBitmapviewer2Dlg(CWnd* pParent)
    : CDialogEx(IDD_BITMAP_VIEWER2_DIALOG, pParent)
    , m_strEditBitmapFilename(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBitmapviewer2Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_BITMAP_FILENAME, m_strEditBitmapFilename);
}

BEGIN_MESSAGE_MAP(CBitmapviewer2Dlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_BITMAPFILE_OPEN, &CBitmapviewer2Dlg::OnBnClickedBitmapFileOpen)
    ON_BN_CLICKED(IDC_BUTTON_FLIP_UPSIDE_DOWN, &CBitmapviewer2Dlg::OnBnClickedButtonFlipUpsideDown)
    ON_BN_CLICKED(IDC_BUTTON_FLIP_LEFT_RIGHT, &CBitmapviewer2Dlg::OnBnClickedButtonFlipLeftRight)
    ON_BN_CLICKED(IDC_BUTTON_ROTATE_RIGHT, &CBitmapviewer2Dlg::OnBnClickedButtonRotateRight)
    ON_BN_CLICKED(IDC_BUTTON_ROTATE_LEFT, &CBitmapviewer2Dlg::OnBnClickedButtonRotateLeft)
    ON_BN_CLICKED(IDC_BUTTON_BITMAPFILE_SAVE, &CBitmapviewer2Dlg::OnBnClickedButtonBitmapfileSave)
END_MESSAGE_MAP()

BOOL CBitmapviewer2Dlg::OnInitDialog()
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

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    return TRUE;
}

void CBitmapviewer2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBitmapviewer2Dlg::OnPaint()
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
}

HCURSOR CBitmapviewer2Dlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CBitmapviewer2Dlg::OnOK()
{
    if (LoadBitmap())
    {
        if (nullptr == m_pDialog)
        {
            m_pDialog = new CImageDlg(this);
            m_pDialog->setBitmapFile(filename);
            m_pDialog->Create(IDD_IMAGE_DIALOG, this);
            m_pDialog->ShowWindow(SW_SHOW);
        }
    }
}

void CBitmapviewer2Dlg::OnCancel()
{
    CDialogEx::OnCancel();
}

void CBitmapviewer2Dlg::OnBnClickedBitmapFileOpen()
{
    CFileDialog dlg(TRUE);
    if (dlg.DoModal() == IDOK)
    {
        m_strEditBitmapFilename = dlg.GetPathName();
        UpdateData(FALSE);
    }
}

bool CBitmapviewer2Dlg::LoadBitmap()
{
    filename = m_strEditBitmapFilename.GetString();

    if (std::filesystem::exists(filename) && !std::filesystem::is_directory(filename))
    {
        CFile file;
        if (file.Open(m_strEditBitmapFilename, CFile::modeRead | CFile::typeBinary))
        {
            const size_t size = std::filesystem::file_size(filename);

            if ((sizeof(BITMAPFILEHEADER) + sizeof(BITMAPCOREHEADER)) < size)
            {
                m_bitmapImage.resize(size, 0);
                file.Read(m_bitmapImage.data(), size);
                file.Close();

                BITMAPFILEHEADER* pFileHeader = (BITMAPFILEHEADER*)m_bitmapImage.data();
                m_offset = pFileHeader->bfOffBits;

                BITMAPINFOHEADER* pInfoHeader_ = (BITMAPINFOHEADER*)(m_bitmapImage.data() + sizeof(BITMAPFILEHEADER));
                const auto infoHeaderSize = pInfoHeader_->biSize;

                if (infoHeaderSize == sizeof(BITMAPINFOHEADER))
                {
                    BITMAPINFOHEADER* pInfoHeader = pInfoHeader_;
                    m_width = pInfoHeader->biWidth;
                    m_height = pInfoHeader->biHeight;
                    m_planes = pInfoHeader->biPlanes;
                    m_bits = pInfoHeader->biBitCount;
                }
                else if (infoHeaderSize == sizeof(BITMAPV4HEADER))
                {
                    BITMAPV4HEADER* pInfoHeader = (BITMAPV4HEADER*)pInfoHeader_;
                    m_width = pInfoHeader->bV4Width;
                    m_height = pInfoHeader->bV4Height;
                    m_planes = pInfoHeader->bV4Planes;
                    m_bits = pInfoHeader->bV4BitCount;
                }
                else if (infoHeaderSize == sizeof(BITMAPV5HEADER))
                {
                    BITMAPV5HEADER* pInfoHeader = (BITMAPV5HEADER*)pInfoHeader_;
                    m_width = pInfoHeader->bV5Width;
                    m_height = pInfoHeader->bV5Height;
                    m_planes = pInfoHeader->bV5Planes;
                    m_bits = pInfoHeader->bV5BitCount;
                }

                if (m_width > 0 && m_height > 0)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void CBitmapviewer2Dlg::OnBnClickedButtonBitmapfileSave()
{
    CFileDialog dlg(FALSE);
    if (dlg.DoModal() == IDOK)
    {
        m_strSaveBitmapFilename = dlg.GetPathName();
        UpdateData(FALSE);

        if (m_pDialog != nullptr)
        {
            m_pDialog->m_strReceiveSaveFilename = m_strSaveBitmapFilename;
            m_pDialog->PostMessage(CImageDlg::WM_SAVEBITMAP, 0, 0);
        }
    }
}

void CBitmapviewer2Dlg::OnBnClickedButtonFlipUpsideDown()
{
    if (m_pDialog != nullptr)
    {
        m_pDialog->PostMessage(CImageDlg::WM_ROTATEBITMAP, CImageDlg::BUTTON_VERTICAL_REVERSE, 0);
    }
}

void CBitmapviewer2Dlg::OnBnClickedButtonFlipLeftRight()
{
    if (m_pDialog != nullptr)
    {
        m_pDialog->PostMessage(CImageDlg::WM_ROTATEBITMAP, CImageDlg::BUTTON_HORIZONTAL_REVERSE, 0);
    }
}

void CBitmapviewer2Dlg::OnBnClickedButtonRotateRight()
{
    if (m_pDialog != nullptr)
    {
        m_pDialog->PostMessage(CImageDlg::WM_ROTATEBITMAP, CImageDlg::BUTTON_270, 0);
    }
}

void CBitmapviewer2Dlg::OnBnClickedButtonRotateLeft()
{
    if (m_pDialog != nullptr)
    {
        m_pDialog->PostMessage(CImageDlg::WM_ROTATEBITMAP, CImageDlg::BUTTON_90, 0);

    }
}