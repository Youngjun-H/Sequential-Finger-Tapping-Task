
// SRTTDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SRTT.h"
#include "SRTTDlg.h"
#include "afxdialogex.h"
#include "mmsystem.h"
#include <iostream>
#include <ctime>


#pragma comment (lib,"winmm")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSRTTDlg 대화 상자



CSRTTDlg::CSRTTDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SRTT_DIALOG, pParent)
	, m_mode_pos(0)
	, m_mode_abc(0)	
	, m_mode_rl(MODE_LEFT)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSRTTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditOut);
	DDX_Control(pDX, IDC_PIC, m_picture);
	DDX_Control(pDX, IDC_BTN_START, ctr_Start);
	DDX_Control(pDX, IDC_BTN_CONNECT, m_connectkey);

	DDX_Radio(pDX, IDC_RADIO3, (int&)m_mode_pos);
	DDX_Radio(pDX, IDC_RADIO6, (int&)m_mode_abc);
	DDX_Radio(pDX, IDC_RADIO1, m_mode_rl);

	DDX_Control(pDX, IDC_GB_1, m_groupbox_1);
	DDX_Control(pDX, IDC_GB_2, m_groupbox_2);
	DDX_Control(pDX, IDC_GB_3, m_groupbox_3);

	DDX_Control(pDX, IDC_RADIO1, m_radiobtn_1);
	DDX_Control(pDX, IDC_RADIO2, m_radiobtn_2);
	DDX_Control(pDX, IDC_RADIO3, m_radiobtn_3);
	DDX_Control(pDX, IDC_RADIO4, m_radiobtn_4);
	DDX_Control(pDX, IDC_RADIO5, m_radiobtn_5);
	DDX_Control(pDX, IDC_RADIO6, m_radiobtn_6);
	DDX_Control(pDX, IDC_RADIO7, m_radiobtn_7);
	DDX_Control(pDX, IDC_RADIO8, m_radiobtn_8);
	DDX_Control(pDX, IDC_RADIO9, m_radiobtn_9);
	DDX_Control(pDX, IDC_RADIO10, m_radiobtn_10);
	DDX_Control(pDX, IDC_RADIO11, m_radiobtn_11);	
}

BEGIN_MESSAGE_MAP(CSRTTDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SERIALCLOSE, &CSRTTDlg::OnThreadClosed)
	ON_MESSAGE(WM_SERIALRECEIVE, &CSRTTDlg::OnSerialReceive)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CSRTTDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_START, &CSRTTDlg::OnBnClickedBtnStart)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, RadioButtonClick_RL)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO3, IDC_RADIO5, RadioButtonClick_POS)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO6, IDC_RADIO11, RadioButtonClick_ABC)
END_MESSAGE_MAP()


// CSRTTDlg 메시지 처리기

BOOL CSRTTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_str_comport = _T("COM10");
	strBaudRate = _T("115200");
	comport_state = FALSE;
	GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(_T("Connect"));

	//m_OpStat = OPSTAT_STOP;
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSRTTDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSRTTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSRTTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CSRTTDlg::OnThreadClosed(WPARAM length, LPARAM lpara)
{
	//overlapped i/o 핸들을닫는다.
	((CSerialCom*)lpara)->HandleClose();
	delete ((CSerialCom*)lpara);

	return 0;
}

LRESULT CSRTTDlg::OnSerialReceive(WPARAM length, LPARAM lpara)
{
	
	char data[20000];
	int count;


	if (m_Comm)
	{
		m_Comm->SerialReceive(data, length);	// length 길이만큼 데이터를 받음
		count = (int)length;
		
		while (count > 0)
		{
			SerialRxDATA_buf = data;
			count--;
			AfxExtractSubString(SerialRxDATA, SerialRxDATA_buf, 0, ',');
		}

		//--시리얼 통신 제대로 되는지 메세지박스로 확인--//
		/*MessageBox(SerialRxDATA);
		SerialRxDATA_buf = "";
		SerialRxDATA = "";*/

		if (SerialRxDATA == '1') {
			prs_btn_L = CLICK_L1;
			prs_btn_R = CLICK_R5;
		}
		else if (SerialRxDATA == '2') {
			prs_btn_L = CLICK_L2;
			prs_btn_R = CLICK_R4;
		}
		else if (SerialRxDATA == '3') {
			prs_btn_L = CLICK_L3;
			prs_btn_R = CLICK_R3;
		}
		else if (SerialRxDATA == '4') {
			prs_btn_L = CLICK_L4;
			prs_btn_R = CLICK_R2;
		}
		else if (SerialRxDATA == '5') {
			prs_btn_L = CLICK_L5;
			prs_btn_R = CLICK_R1;
		}
	}
	return 0;
}


void CSRTTDlg::OnBnClickedBtnConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (comport_state)
	{
		if (m_Comm)		// COM PORT 가 존재하면
		{
			m_Comm->Close();
			m_Comm = NULL;
			//AfxMessageBox(_T("COM 포트 닫힘"));
			m_EditOut.SetWindowTextA(_T("Disconnected"));
			comport_state = FALSE;
			GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(_T("Connect"));
		}
	}
	else
	{
		m_Comm = new CSerialCom(_T("\\\\.\\") + m_str_comport, strBaudRate, _T("NONE"), _T("8 Bit"), _T("1 Bit"));

		if (m_Comm->Create(GetSafeHwnd()) != 0)		// 통신포트를 열고 윈도우의 핸들을 넘김
		{
			//AfxMessageBox(_T("COM 포트 열림"));
			m_EditOut.SetWindowTextA(_T("Connected"));
			comport_state = TRUE;

			m_matImage = m_matImage_baseline;
			CreateBitmapInfo(m_matImage.cols, m_matImage.rows, m_matImage.channels() * 8);
			DrawImage();

			GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(_T("Disconnect"));

		}
		else
		{
			AfxMessageBox(_T("PORT 설정을 다시 확인해주세요!"));
		}
	}
}

void CSRTTDlg::CreateBitmapInfo(int w, int h, int bpp)
{
	if (m_pBitmapInfo != NULL)
	{
		delete[]m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}

	if (bpp == 8)
		m_pBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else // 24 or 32bit
		m_pBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO)];

	m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBitmapInfo->bmiHeader.biPlanes = 1;
	m_pBitmapInfo->bmiHeader.biBitCount = bpp;
	m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;
	m_pBitmapInfo->bmiHeader.biSizeImage = 0;
	m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biClrUsed = 0;
	m_pBitmapInfo->bmiHeader.biClrImportant = 0;

	if (bpp == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			m_pBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbReserved = 0;
		}
	}

	m_pBitmapInfo->bmiHeader.biWidth = w;
	m_pBitmapInfo->bmiHeader.biHeight = -h;
}

void CSRTTDlg::DrawImage()
{
	CClientDC dc(GetDlgItem(IDC_PIC));

	CRect rect;
	GetDlgItem(IDC_PIC)->GetClientRect(&rect);
	
	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, m_matImage.cols, m_matImage.rows, m_matImage.data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

}

void CSRTTDlg::Display_Image(int num) {
	switch (num)
	{
	case PIC_1ST:
		m_matImage = m_matImage_1;
		CreateBitmapInfo(m_matImage.cols, m_matImage.rows, m_matImage.channels() * 8);
		DrawImage();
		ref_btn = CLICK_L1;
		break;
	case PIC_2ND:
		m_matImage = m_matImage_2;
		CreateBitmapInfo(m_matImage.cols, m_matImage.rows, m_matImage.channels() * 8);
		DrawImage();
		ref_btn = CLICK_L2;
		break;
	case PIC_3RD:
		m_matImage = m_matImage_3;
		CreateBitmapInfo(m_matImage.cols, m_matImage.rows, m_matImage.channels() * 8);
		DrawImage();
		ref_btn = CLICK_L3;
		break;
	case PIC_4TH:
		m_matImage = m_matImage_4;
		CreateBitmapInfo(m_matImage.cols, m_matImage.rows, m_matImage.channels() * 8);
		DrawImage();
		ref_btn = CLICK_L4;
		break;
	case PIC_5TH:
		m_matImage = m_matImage_5;
		CreateBitmapInfo(m_matImage.cols, m_matImage.rows, m_matImage.channels() * 8);
		DrawImage();
		ref_btn = CLICK_L5;
		break;
	case PIC_END:
		m_matImage = m_matImage_end;
		CreateBitmapInfo(m_matImage.cols, m_matImage.rows, m_matImage.channels() * 8);
		DrawImage();
		ref_rest = 20000 / 10;
		break;
	case PIC_BASELINE:
		m_matImage = m_matImage_baseline;
		CreateBitmapInfo(m_matImage.cols, m_matImage.rows, m_matImage.channels() * 8);
		DrawImage();
		break;
	case PIC_BLANK:
		m_matImage = m_matImage_blank;
		CreateBitmapInfo(m_matImage.cols, m_matImage.rows, m_matImage.channels() * 8);
		DrawImage();
		break;
	case PIC_READY:
		m_matImage = m_matImage_ready;
		CreateBitmapInfo(m_matImage.cols, m_matImage.rows, m_matImage.channels() * 8);
		DrawImage();
		break;
	}
}

void CSRTTDlg::RadioButtonClick_RL(UINT ID)
{
	UpdateData(TRUE);

	switch (m_mode_rl)
	{
	case MODE_LEFT:
		AfxMessageBox(_T("Mode LEFT is selected!"));
		Mode_RL = MODE_LEFT;
		break;
	case MODE_RIGHT:
		AfxMessageBox(_T("Mode RIGHT is selected!"));
		Mode_RL = MODE_RIGHT;
		break;
	}
}

void CSRTTDlg::RadioButtonClick_POS(UINT ID)
{
	UpdateData(TRUE);

	switch (m_mode_pos)
	{
	case MODE_POS_1:
		AfxMessageBox(_T("'1st' is selected!"));
		Mode_POS = MODE_POS_1;
		break;
	case MODE_POS_3:
		AfxMessageBox(_T("'3rd' is selected!"));
		Mode_POS = MODE_POS_3;
		break;
	case MODE_POS_5:
		AfxMessageBox(_T("'5th' is selected!"));
		Mode_POS = MODE_POS_5;
		break;
	}
}

void CSRTTDlg::RadioButtonClick_ABC(UINT ID)
{
	UpdateData(TRUE);

	switch (m_mode_abc)
	{
	case MODE_1:
		AfxMessageBox(_T("Mode A-pre is selected!"));
		Mode_ABC = MODE_1;
		break;
	case MODE_2:
		AfxMessageBox(_T("Mode A-post is selected!"));
		Mode_ABC = MODE_2;
		break;
	case MODE_3:
		AfxMessageBox(_T("Mode B-pre is selected!"));
		Mode_ABC = MODE_3;
		break;
	case MODE_4:
		AfxMessageBox(_T("Mode B-post is selected!"));
		Mode_ABC = MODE_4;
		break;
	case MODE_5:
		AfxMessageBox(_T("Mode C-pre is selected!"));
		Mode_ABC = MODE_5;
		break;
	case MODE_6:
		AfxMessageBox(_T("Mode C-post is selected!"));
		Mode_ABC = MODE_6;
		break;
	}
}

void CSRTTDlg::OnBnClickedBtnStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	ctr_Start.SetWindowText(_T("STOP"));

	m_NextInst = 1;
	m_prs_order_count = 1;
	m_NextInst_part = 0;
	resp_time = 0;
	test_time = 0;

	prs_btn_L = 0;
	prs_btn_R = 0;
	rest = 0;	
	btn_cnt = 0;
	err_btn = 0;	
	m_blank_order = OFF;
	btn_count = 0;
	time = 0;

	/*for (int i = 0; i < 30; i++) {  //05.18수정
		prs_btn_time[i] = 0;
		pressed_btn[i] = 0;
		prssed_btn_ref[i] = 0;
	}*/

	//__전체화면 & 메뉴숨기기__//
	MenuHide();
	Sleep(10);
	ShowWindow(SW_SHOWMAXIMIZED);
	CRect rc;
	GetWindowRect(&rc);
	m_picture.SetWindowPos(NULL, rc.left + 550, rc.top + 300, 0, 0, SWP_NOSIZE);
	COLORREF clBkColor = GetSysColor(COLOR_3DFACE);
	SetBackgroundColor(RGB(0, 0, 0));

	m_nTimerID = timeSetEvent(10, 1, TimerProc, (DWORD_PTR)this, TIME_PERIODIC); //0.001 초를 주기로 하면 시간이 맞지않음!
	
	UpdateData(FALSE);
}

void CALLBACK TimerProc(UINT m_nTimerID, UINT uiMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	CSRTTDlg* pView;
	pView = (CSRTTDlg*)dwUser;

	pView->time++;

	if (pView->Mode_RL == MODE_LEFT) {
		if (pView->Mode_POS == MODE_POS_1) {
			if (pView->Mode_ABC == MODE_1) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_LEFT(MODE_L_1_1);
				}
			}
			else if (pView->Mode_ABC == MODE_2) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_LEFT(MODE_L_1_2);
				}
			}
			else if (pView->Mode_ABC == MODE_3) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_LEFT(MODE_L_1_3);
				}
			}
			else if (pView->Mode_ABC == MODE_4) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_LEFT(MODE_L_1_4);
				}
			}
			else if (pView->Mode_ABC == MODE_5) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_LEFT(MODE_L_1_5);
				}
			}
			else if (pView->Mode_ABC == MODE_6) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_LEFT(MODE_L_1_6);
				}
			}
		}
		else if (pView->Mode_POS == MODE_POS_3) {
			if (pView->Mode_ABC == MODE_1) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_LEFT(MODE_L_3_1);
				}
			}
			else if (pView->Mode_ABC == MODE_2) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_LEFT(MODE_L_3_2);
				}
			}
			else if (pView->Mode_ABC == MODE_3) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_LEFT(MODE_L_3_3);
				}
			}
			else if (pView->Mode_ABC == MODE_4) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_LEFT(MODE_L_3_4);
				}
			}
			else if (pView->Mode_ABC == MODE_5) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_LEFT(MODE_L_3_5);
				}
			}
			else if (pView->Mode_ABC == MODE_6) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_LEFT(MODE_L_3_6);
				}
			}
		}
		else if (pView->Mode_POS == MODE_POS_5) {
			if (pView->Mode_ABC == MODE_1) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_LEFT(MODE_L_5_1);
				}
			}
			else if (pView->Mode_ABC == MODE_2) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_LEFT(MODE_L_5_2);
				}
			}
			else if (pView->Mode_ABC == MODE_3) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_LEFT(MODE_L_5_3);
				}
			}
			else if (pView->Mode_ABC == MODE_4) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_LEFT(MODE_L_5_4);
				}
			}
			else if (pView->Mode_ABC == MODE_5) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_LEFT(MODE_L_5_5);
				}
			}
			else if (pView->Mode_ABC == MODE_6) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_LEFT(MODE_L_5_6);
				}
			}
		}
	}
	else if (pView->Mode_RL == MODE_RIGHT) {
		if (pView->Mode_POS == MODE_POS_1) {
			if (pView->Mode_ABC == MODE_1) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_RIGHT(MODE_R_1_1);
				}
			}
			else if (pView->Mode_ABC == MODE_2) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_RIGHT(MODE_R_1_2);
				}
			}
			else if (pView->Mode_ABC == MODE_3) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_RIGHT(MODE_R_1_3);
				}
			}
			else if (pView->Mode_ABC == MODE_4) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_RIGHT(MODE_R_1_4);
				}
			}
			else if (pView->Mode_ABC == MODE_5) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_RIGHT(MODE_R_1_5);
				}
			}
			else if (pView->Mode_ABC == MODE_6) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_RIGHT(MODE_R_1_6);
				}
			}
		}
		else if (pView->Mode_POS == MODE_POS_3) {
			if (pView->Mode_ABC == MODE_1) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_RIGHT(MODE_R_3_1);
				}
			}
			else if (pView->Mode_ABC == MODE_2) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_RIGHT(MODE_R_3_2);
				}
			}
			else if (pView->Mode_ABC == MODE_3) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_RIGHT(MODE_R_3_3);
				}
			}
			else if (pView->Mode_ABC == MODE_4) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_RIGHT(MODE_R_3_4);
				}
			}
			else if (pView->Mode_ABC == MODE_5) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_RIGHT(MODE_R_3_5);
				}
			}
			else if (pView->Mode_ABC == MODE_6) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_RIGHT(MODE_R_3_6);
				}
			}
		}
		else if (pView->Mode_POS == MODE_POS_5) {
			if (pView->Mode_ABC == MODE_1) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_RIGHT(MODE_R_5_1);
				}
			}
			else if (pView->Mode_ABC == MODE_2) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_RIGHT(MODE_R_5_2);
				}
			}
			else if (pView->Mode_ABC == MODE_3) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_RIGHT(MODE_R_5_3);
				}
			}
			else if (pView->Mode_ABC == MODE_4) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_RIGHT(MODE_R_5_4);
				}
			}
			else if (pView->Mode_ABC == MODE_5) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_RIGHT(MODE_R_5_5);
				}
			}
			else if (pView->Mode_ABC == MODE_6) {
				if (pView->time > 0 && pView->time <= 3000 / 10) {
					pView->Display_Image(PIC_BLANK);
				}
				else if (pView->time > 3000 / 10 && pView->time <= 13000 / 10) {
					pView->Display_Image(PIC_READY);
				}
				else if (pView->time > 13000 / 10) {
					pView->Start_Test_RIGHT(MODE_R_5_6);
				}
			}
		}

	}	
}

void CSRTTDlg::Start_Test_LEFT(int order) {

	test_time++;
	
	
	if (test_time > 540000 / 10) { //540000 / 10 //테스트를 마무리 하는 부분 //92000
		Display_Image(PIC_BASELINE);
		timeKillEvent(m_nTimerID);		
		AfxMessageBox(_T("Test complete!"));
		OnBnClickedBtnSave();
		::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
	}
	else if (m_NextInst_part == 4) { //같은 패턴 4번 실행 후 동작
		
		Display_Image(PIC_END);
		resp_time++;

		if (resp_time == ref_rest) {
			resp_time = 0;
			m_NextInst_part = 0;
			prs_btn_L = 0; // rest 시간 동안 누른 버튼값을 초기화
		}
	}	
	else { //테스트 진행 부분.
		
		if (order == MODE_L_1_1) {
			Mode_LEFT_P1_Apre(m_NextInst);
		}
		else if (order == MODE_L_3_1){
			Mode_LEFT_P3_Apre(m_NextInst);		
		}
		else if (order == MODE_L_5_1) {
			Mode_LEFT_P5_Apre(m_NextInst);
		}
		else if (order == MODE_L_1_2) {
			Mode_LEFT_P1_Apost(m_NextInst);
		}
		else if (order == MODE_L_3_2) {
			Mode_LEFT_P3_Apost(m_NextInst);
		}
		else if (order == MODE_L_5_2) {
			Mode_LEFT_P5_Apost(m_NextInst);
		}
		else if (order == MODE_L_1_3) {
			Mode_LEFT_P1_Bpre(m_NextInst);
		}
		else if (order == MODE_L_3_3) {
			Mode_LEFT_P3_Bpre(m_NextInst);
		}
		else if (order == MODE_L_5_3) {
			Mode_LEFT_P5_Bpre(m_NextInst);
		}
		else if (order == MODE_L_1_4) {
			Mode_LEFT_P1_Bpost(m_NextInst);
		}
		else if (order == MODE_L_3_4) {
			Mode_LEFT_P3_Bpost(m_NextInst);
		}
		else if (order == MODE_L_5_4) {
			Mode_LEFT_P5_Bpost(m_NextInst);
		}
		else if (order == MODE_L_1_5) {
			Mode_LEFT_P1_Cpre(m_NextInst);
		}
		else if (order == MODE_L_3_5) {
			Mode_LEFT_P3_Cpre(m_NextInst);
		}
		else if (order == MODE_L_5_5) {
			Mode_LEFT_P5_Cpre(m_NextInst);
		}
		else if (order == MODE_L_1_6) {
			Mode_LEFT_P1_Cpost(m_NextInst);
		}
		else if (order == MODE_L_3_6) {
			Mode_LEFT_P3_Cpost(m_NextInst);
		}
		else if (order == MODE_L_5_6) {
			Mode_LEFT_P5_Cpost(m_NextInst);
		}
			   		

		resp_time++;

		if (m_blank_order == OFF) {

			if (!(prs_btn_L == 0)) {
				prs_btn_time = resp_time * 10;
				pressed_btn = prs_btn_L;
				
				if (prs_btn_L == ref_btn) {
					num_interrupt = ON;
					prs_btn_time_inter = resp_time * 10;
					pressed_btn_inter = prs_btn_L;
				}

				btn_cnt++;
				prs_btn_L = 0;
			}

			if (num_interrupt == ON) {
				m_resptime[m_prs_order_count - 1] = prs_btn_time_inter;
				m_prbtn[m_prs_order_count - 1] = pressed_btn_inter;
			}
			else {
				m_resptime[m_prs_order_count - 1] = prs_btn_time;
				m_prbtn[m_prs_order_count - 1] = pressed_btn;  // 수정함 05.18
			}			

			if (resp_time == 600 / 10) {

				
				if (!(pressed_btn == 0)) {  // 수정함 05.18
					if (num_interrupt == ON) {
						m_errbtn[m_prs_order_count - 1] = "C";
					}
					else {
						m_errbtn[m_prs_order_count - 1] = "E";
					}					
				}
				else {
					m_errbtn[m_prs_order_count - 1] = "NR";
				}

				m_blank_order = ON;
				m_NextInst++;
				m_prs_order_count++;
				resp_time = 0;

				num_interrupt = OFF;
									
				btn_cnt = 0;
				
			}
		}
		else if (m_blank_order == ON) {
			if (resp_time == 400 / 10) {
				prs_btn_time_inter = 0; //반응시간동안의 마지막에 누른 정보를 초기화
				pressed_btn_inter = 0;
				prs_btn_time = 0;
				pressed_btn = 0;


				m_blank_order = OFF;
				m_NextInst++;
				resp_time = 0;
				prs_btn_L = 0; // blank 시간 동안 누른 버튼값을 초기화
				prs_btn_R = 0;
			}
		}


		if (m_NextInst == 19) {
			m_NextInst = 1;
			m_NextInst_part++;
		}
	}	
}

void CSRTTDlg::Start_Test_RIGHT(int order) {

	test_time++;

	if (test_time > 540000 / 10) { //540000 / 10 //테스트를 마무리 하는 부분 //39000
		Display_Image(PIC_BASELINE);
		timeKillEvent(m_nTimerID);
		AfxMessageBox(_T("Test complete!"));
		OnBnClickedBtnSave();
		::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
	}
	else if (m_NextInst_part == 4) { //같은 패턴 4번 실행 후 동작

		Display_Image(PIC_END);
		resp_time++;

		if (resp_time == ref_rest) {
			resp_time = 0;
			m_NextInst_part = 0;
			prs_btn_R = 0; // blank 시간 동안 누른 버튼값을 초기화
		}
	}
	else { //테스트 진행 부분

		if (order == MODE_R_1_1) {
			Mode_RIGHT_P1_Apre(m_NextInst);
		}
		else if (order == MODE_R_3_1) {
			Mode_RIGHT_P3_Apre(m_NextInst);
		}
		else if (order == MODE_R_5_1) {
			Mode_RIGHT_P5_Apre(m_NextInst);
		}
		else if (order == MODE_R_1_2) {
			Mode_RIGHT_P1_Apost(m_NextInst);
		}
		else if (order == MODE_R_3_2) {
			Mode_RIGHT_P3_Apost(m_NextInst);
		}
		else if (order == MODE_R_5_2) {
			Mode_RIGHT_P5_Apost(m_NextInst);
		}
		else if (order == MODE_R_1_3) {
			Mode_RIGHT_P1_Bpre(m_NextInst);
		}
		else if (order == MODE_R_3_3) {
			Mode_RIGHT_P3_Bpre(m_NextInst);
		}
		else if (order == MODE_R_5_3) {
			Mode_RIGHT_P5_Bpre(m_NextInst);
		}
		else if (order == MODE_R_1_4) {
			Mode_RIGHT_P1_Bpost(m_NextInst);
		}
		else if (order == MODE_R_3_4) {
			Mode_RIGHT_P3_Bpost(m_NextInst);
		}
		else if (order == MODE_R_5_4) {
			Mode_RIGHT_P5_Bpost(m_NextInst);
		}
		else if (order == MODE_R_1_5) {
			Mode_RIGHT_P1_Cpre(m_NextInst);
		}
		else if (order == MODE_R_3_5) {
			Mode_RIGHT_P3_Cpre(m_NextInst);
		}
		else if (order == MODE_R_5_5) {
			Mode_RIGHT_P5_Cpre(m_NextInst);
		}
		else if (order == MODE_R_1_6) {
			Mode_RIGHT_P1_Cpost(m_NextInst);
		}
		else if (order == MODE_R_3_6) {
			Mode_RIGHT_P3_Cpost(m_NextInst);
		}
		else if (order == MODE_R_5_6) {
			Mode_RIGHT_P5_Cpost(m_NextInst);
		}

		resp_time++;

		if (m_blank_order == OFF) {

			if (!(prs_btn_R == 0)) {
				prs_btn_time = resp_time * 10;
				pressed_btn = prs_btn_R;
				prssed_btn_ref = prs_btn_L;

				if (prs_btn_L == ref_btn) { //reference 가 왼손에 맞춰져 있기때문에 왼손번호로 해야함.
					num_interrupt = ON;
					prs_btn_time_inter = resp_time * 10;
					pressed_btn_inter = prs_btn_R;
				}
								
				btn_cnt++;
				prs_btn_R = 0;
				prs_btn_L = 0;
			}

			if (num_interrupt == ON) {
				m_resptime[m_prs_order_count - 1] = prs_btn_time_inter;
				m_prbtn[m_prs_order_count - 1] = pressed_btn_inter;
			}
			else {
				m_resptime[m_prs_order_count - 1] = prs_btn_time;
				m_prbtn[m_prs_order_count - 1] = pressed_btn;   // 수정함 05.18
				prssed_btn_ref = prssed_btn_ref;//코드 작성시 햇갈릴까봐 한번더 언급
			}		
			
			
			if (resp_time == 600 / 10) {

				if (!(prssed_btn_ref == 0)) {
					if (num_interrupt == ON) {
						m_errbtn[m_prs_order_count - 1] = "C";
					}
					else {
						m_errbtn[m_prs_order_count - 1] = "E";
					}					
				}
				else {
					m_errbtn[m_prs_order_count - 1] = "NR";
				}
							   				
				m_blank_order = ON;
				m_NextInst++;
				m_prs_order_count++;
				resp_time = 0;

				num_interrupt = OFF;

				btn_cnt = 0;
			}
		}
		else if (m_blank_order == ON) {
			if (resp_time == 400 / 10) {
				prs_btn_time_inter = 0; //반응시간동안의 마지막에 누른 정보를 초기화
				pressed_btn_inter = 0;
				prs_btn_time = 0;
				pressed_btn = 0;

				m_blank_order = OFF;
				m_NextInst++;
				resp_time = 0;
				prs_btn_R = 0; // blank 시간 동안 누른 버튼값을 초기화
				prs_btn_L = 0;
			}
		}


		if (m_NextInst == 19) {
			m_NextInst = 1;
			m_NextInst_part++;
		}
	}
}

void CSRTTDlg::MenuHide() {

	m_connectkey.ShowWindow(SW_HIDE);
	ctr_Start.ShowWindow(SW_HIDE);
	m_EditOut.ShowWindow(SW_HIDE);
	m_groupbox_1.ShowWindow(SW_HIDE);
	m_groupbox_2.ShowWindow(SW_HIDE);
	m_groupbox_3.ShowWindow(SW_HIDE);

	m_radiobtn_1.ShowWindow(SW_HIDE);
	m_radiobtn_2.ShowWindow(SW_HIDE);
	m_radiobtn_3.ShowWindow(SW_HIDE);
	m_radiobtn_4.ShowWindow(SW_HIDE);
	m_radiobtn_5.ShowWindow(SW_HIDE);
	m_radiobtn_6.ShowWindow(SW_HIDE);
	m_radiobtn_7.ShowWindow(SW_HIDE);
	m_radiobtn_8.ShowWindow(SW_HIDE);
	m_radiobtn_9.ShowWindow(SW_HIDE);
	m_radiobtn_10.ShowWindow(SW_HIDE);
	m_radiobtn_11.ShowWindow(SW_HIDE);
}

void CSRTTDlg::OnBnClickedBtnSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strPath, strTime;
	CTime t = CTime::GetCurrentTime();
	strTime.Format(_T("_%02d%02d_%02d%02d"), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute());
	int nLen = (int)GetModuleFileName(AfxGetInstanceHandle(), strPath.GetBuffer(256), 256);
	strPath.ReleaseBuffer(nLen); int nFind = strPath.ReverseFind('\\'); if (nFind >= 0) strPath = strPath.Left(nFind);


	s_SaveFile = STORE_DATA_FILE + strTime + _T(".csv");

	CFile file;
	CString strLine;

	if (file.Open(s_SaveFile, CFile::modeWrite | CFile::modeCreate)) {
		strLine.Format(_T("%s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; \r\n"),
			_T("MODE"), _T("RESP01"), _T("RESP02"), _T("RESP03"), _T("RESP04"), _T("RESP05"), _T("RESP06"), _T("RESP07"), _T("RESP08"), _T("RESP09"), _T("RESP10"),
			_T("RESP11"), _T("RESP12"), _T("RESP13"), _T("RESP14"), _T("RESP15"), _T("RESP16"), _T("RESP17"), _T("RESP18"), _T("RESP19"), _T("RESP20"),
			_T("RESP21"), _T("RESP22"), _T("RESP23"), _T("RESP24"), _T("RESP25"), _T("RESP26"), _T("RESP27"), _T("RESP28"), _T("RESP29"), _T("RESP30"), _T("RESP31"), 
			_T("RESP32"), _T("RESP33"), _T("RESP34"), _T("RESP35"), _T("RESP36"));

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; \n"),
			Mode_RL, m_resptime[0], m_resptime[1], m_resptime[2], m_resptime[3], m_resptime[4], m_resptime[5], m_resptime[6], m_resptime[7], m_resptime[8],
			m_resptime[9], m_resptime[10], m_resptime[11], m_resptime[12], m_resptime[13], m_resptime[14], m_resptime[15], m_resptime[16], m_resptime[17],
			m_resptime[18], m_resptime[19], m_resptime[20], m_resptime[21], m_resptime[22], m_resptime[23], m_resptime[24], m_resptime[25], m_resptime[26],
			m_resptime[27], m_resptime[28], m_resptime[29], m_resptime[30], m_resptime[31], m_resptime[32], m_resptime[33], m_resptime[34], m_resptime[35]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; \n"),
			Mode_POS, m_errbtn[0], m_errbtn[1], m_errbtn[2], m_errbtn[3], m_errbtn[4], m_errbtn[5], m_errbtn[6], m_errbtn[7], m_errbtn[8], m_errbtn[9],
			m_errbtn[10], m_errbtn[11], m_errbtn[12], m_errbtn[13], m_errbtn[14], m_errbtn[15], m_errbtn[16], m_errbtn[17], m_errbtn[18],
			m_errbtn[19], m_errbtn[20], m_errbtn[21], m_errbtn[22], m_errbtn[23], m_errbtn[24], m_errbtn[25], m_errbtn[26], m_errbtn[27],
			m_errbtn[28], m_errbtn[29], m_errbtn[30], m_errbtn[31], m_errbtn[32], m_errbtn[33], m_errbtn[34], m_errbtn[35]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; \n"),
			Mode_ABC, m_prbtn[0], m_prbtn[1], m_prbtn[2], m_prbtn[3], m_prbtn[4], m_prbtn[5], m_prbtn[6], m_prbtn[7], m_prbtn[8],
			m_prbtn[9], m_prbtn[10], m_prbtn[11], m_prbtn[12], m_prbtn[13], m_prbtn[14], m_prbtn[15], m_prbtn[16], m_prbtn[17],
			m_prbtn[18], m_prbtn[19], m_prbtn[20], m_prbtn[21], m_prbtn[22], m_prbtn[23], m_prbtn[24], m_prbtn[25], m_prbtn[26],
			m_prbtn[27], m_prbtn[28], m_prbtn[29], m_prbtn[30], m_prbtn[31], m_prbtn[32], m_prbtn[33], m_prbtn[34], m_prbtn[35]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format("\n");
		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; \n"),
			_T("MODE"), _T("RESP37"), _T("RESP38"), _T("RESP39"), _T("RESP40"), _T("RESP41"), _T("RESP42"), _T("RESP43"), _T("RESP44"), _T("RESP45"), _T("RESP46"),
			_T("RESP47"), _T("RESP48"), _T("RESP49"), _T("RESP50"), _T("RESP51"), _T("RESP52"), _T("RESP53"), _T("RESP54"), _T("RESP55"), _T("RESP56"),
			_T("RESP57"), _T("RESP58"), _T("RESP59"), _T("RESP60"), _T("RESP61"), _T("RESP62"), _T("RESP63"), _T("RESP64"), _T("RESP65"), _T("RESP66"), _T("RESP67"),
			_T("RESP68"), _T("RESP69"), _T("RESP70"), _T("RESP71"), _T("RESP72"));

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; \n"),
			Mode_RL, m_resptime[36], m_resptime[37], m_resptime[38], m_resptime[39], m_resptime[40], m_resptime[41], m_resptime[42], m_resptime[43], m_resptime[44], m_resptime[45],
			m_resptime[46], m_resptime[47], m_resptime[48], m_resptime[49], m_resptime[50], m_resptime[51], m_resptime[52], m_resptime[53], m_resptime[54],
			m_resptime[55], m_resptime[56], m_resptime[57], m_resptime[58], m_resptime[59], m_resptime[60], m_resptime[61], m_resptime[62], m_resptime[63],
			m_resptime[64], m_resptime[65], m_resptime[66], m_resptime[67], m_resptime[68], m_resptime[69], m_resptime[70], m_resptime[71]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; \n"),
			Mode_POS, m_errbtn[36], m_errbtn[37], m_errbtn[38], m_errbtn[39], m_errbtn[40], m_errbtn[41], m_errbtn[42], m_errbtn[43], m_errbtn[44], m_errbtn[45],
			m_errbtn[46], m_errbtn[47], m_errbtn[48], m_errbtn[49], m_errbtn[50], m_errbtn[51], m_errbtn[52], m_errbtn[53], m_errbtn[54],
			m_errbtn[55], m_errbtn[56], m_errbtn[57], m_errbtn[58], m_errbtn[59], m_errbtn[60], m_errbtn[61], m_errbtn[62], m_errbtn[63],
			m_errbtn[64], m_errbtn[65], m_errbtn[66], m_errbtn[67], m_errbtn[68], m_errbtn[69], m_errbtn[70], m_errbtn[71]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; \n"),
			Mode_ABC, m_prbtn[36], m_prbtn[37], m_prbtn[38], m_prbtn[39], m_prbtn[40], m_prbtn[41], m_prbtn[42], m_prbtn[43], m_prbtn[44], m_prbtn[45],
			m_prbtn[46], m_prbtn[47], m_prbtn[48], m_prbtn[49], m_prbtn[50], m_prbtn[51], m_prbtn[52], m_prbtn[53], m_prbtn[54],
			m_prbtn[55], m_prbtn[56], m_prbtn[57], m_prbtn[58], m_prbtn[59], m_prbtn[60], m_prbtn[61], m_prbtn[62], m_prbtn[63],
			m_prbtn[64], m_prbtn[65], m_prbtn[66], m_prbtn[67], m_prbtn[68], m_prbtn[69], m_prbtn[70], m_prbtn[71]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format("\n");
		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; \r\n"),
			_T("MODE"), _T("RESP73"), _T("RESP74"), _T("RESP75"), _T("RESP76"), _T("RESP77"), _T("RESP78"), _T("RESP79"), _T("RESP80"), _T("RESP81"), _T("RESP82"), 
			_T("RESP83"), _T("RESP84"), _T("RESP85"), _T("RESP86"), _T("RESP87"), _T("RESP88"), _T("RESP89"), _T("RESP90"), _T("RESP91"), _T("RESP92"), 
			_T("RESP93"), _T("RESP94"), _T("RESP95"), _T("RESP96"), _T("RESP97"), _T("RESP98"),	_T("RESP99"), _T("RESP100"), _T("RESP101"), _T("RESP102"), _T("RESP103"),
			_T("RESP104"), _T("RESP105"), _T("RESP106"), _T("RESP107"), _T("RES108"));

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; \n"),
			Mode_RL, m_resptime[72], m_resptime[73], m_resptime[74], m_resptime[75], m_resptime[76], m_resptime[77], m_resptime[78], m_resptime[79], m_resptime[80], m_resptime[81],
			m_resptime[82], m_resptime[83], m_resptime[84], m_resptime[85], m_resptime[86], m_resptime[87], m_resptime[88], m_resptime[89], m_resptime[90],
			m_resptime[91], m_resptime[92], m_resptime[93], m_resptime[94], m_resptime[95], m_resptime[96], m_resptime[97], m_resptime[98], m_resptime[99],
			m_resptime[100], m_resptime[101], m_resptime[102], m_resptime[103], m_resptime[104], m_resptime[105], m_resptime[106], m_resptime[107]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; \n"),
			Mode_POS, m_errbtn[72], m_errbtn[73], m_errbtn[74], m_errbtn[75], m_errbtn[76], m_errbtn[77], m_errbtn[78], m_errbtn[79], m_errbtn[80], m_errbtn[81],
			m_errbtn[82], m_errbtn[83], m_errbtn[84], m_errbtn[85], m_errbtn[86], m_errbtn[87], m_errbtn[88], m_errbtn[89], m_errbtn[90],
			m_errbtn[91], m_errbtn[92], m_errbtn[93], m_errbtn[94], m_errbtn[95], m_errbtn[96], m_errbtn[97], m_errbtn[98], m_errbtn[99],
			m_errbtn[100], m_errbtn[101], m_errbtn[102], m_errbtn[103], m_errbtn[104], m_errbtn[105], m_errbtn[106], m_errbtn[107]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; \n"),
			Mode_ABC, m_prbtn[72], m_prbtn[73], m_prbtn[74], m_prbtn[75], m_prbtn[76], m_prbtn[77], m_prbtn[78], m_prbtn[79], m_prbtn[80], m_prbtn[81],
			m_prbtn[82], m_prbtn[83], m_prbtn[84], m_prbtn[85], m_prbtn[86], m_prbtn[87], m_prbtn[88], m_prbtn[89], m_prbtn[90],
			m_prbtn[91], m_prbtn[92], m_prbtn[93], m_prbtn[94], m_prbtn[95], m_prbtn[96], m_prbtn[97], m_prbtn[98], m_prbtn[99],
			m_prbtn[100], m_prbtn[101], m_prbtn[102], m_prbtn[103], m_prbtn[104], m_prbtn[105], m_prbtn[106], m_prbtn[107]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format("\n");
		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; \r\n"),
			_T("MODE"), _T("RESP109"), _T("RESP110"), _T("RESP111"), _T("RESP112"), _T("RESP113"), _T("RESP114"), _T("RESP115"), _T("RESP116"), _T("RESP117"), _T("RESP118"), 
			_T("RESP119"), _T("RESP120"), _T("RESP121"), _T("RESP122"), _T("RESP123"), _T("RESP124"), _T("RESP125"), _T("RESP126"), _T("RESP127"), _T("RESP128"), 
			_T("RESP129"), _T("RESP130"), _T("RESP131"), _T("RESP132"), _T("RESP133"), _T("RESP134"), _T("RESP135"), _T("RESP136"), _T("RESP137"), _T("RESP138"), 
			_T("RESP139"), _T("RESP140"), _T("RESP141"), _T("RESP142"), _T("RESP143"), _T("RESP144"));

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; \n"),
			Mode_RL, m_resptime[108], m_resptime[109], m_resptime[110], m_resptime[111], m_resptime[112], m_resptime[113], m_resptime[114], m_resptime[115], m_resptime[116], m_resptime[117],
			m_resptime[118], m_resptime[119], m_resptime[120], m_resptime[121], m_resptime[122], m_resptime[123], m_resptime[124], m_resptime[125], m_resptime[126],
			m_resptime[127], m_resptime[128], m_resptime[129], m_resptime[130], m_resptime[131], m_resptime[132], m_resptime[133], m_resptime[134], m_resptime[135],
			m_resptime[136], m_resptime[137], m_resptime[138], m_resptime[139], m_resptime[140], m_resptime[141], m_resptime[142], m_resptime[143]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; \n"),
			Mode_POS, m_errbtn[108], m_errbtn[109], m_errbtn[110], m_errbtn[111], m_errbtn[112], m_errbtn[113], m_errbtn[114], m_errbtn[115], m_errbtn[116], m_errbtn[117],
			m_errbtn[118], m_errbtn[119], m_errbtn[120], m_errbtn[121], m_errbtn[122], m_errbtn[123], m_errbtn[124], m_errbtn[125], m_errbtn[126],
			m_errbtn[127], m_errbtn[128], m_errbtn[129], m_errbtn[130], m_errbtn[131], m_errbtn[132], m_errbtn[133], m_errbtn[134], m_errbtn[135],
			m_errbtn[136], m_errbtn[137], m_errbtn[138], m_errbtn[139], m_errbtn[140], m_errbtn[141], m_errbtn[142], m_errbtn[143]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; \n"),
			Mode_ABC, m_prbtn[108], m_prbtn[109], m_prbtn[110], m_prbtn[111], m_prbtn[112], m_prbtn[113], m_prbtn[114], m_prbtn[115], m_prbtn[116], m_prbtn[117],
			m_prbtn[118], m_prbtn[119], m_prbtn[120], m_prbtn[121], m_prbtn[122], m_prbtn[123], m_prbtn[124], m_prbtn[125], m_prbtn[126],
			m_prbtn[127], m_prbtn[128], m_prbtn[129], m_prbtn[130], m_prbtn[131], m_prbtn[132], m_prbtn[133], m_prbtn[134], m_prbtn[135],
			m_prbtn[136], m_prbtn[137], m_prbtn[138], m_prbtn[139], m_prbtn[140], m_prbtn[141], m_prbtn[142], m_prbtn[143]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format("\n");
		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; \r\n"),
			_T("MODE"), _T("RESP145"), _T("RESP146"), _T("RESP147"), _T("RESP148"), _T("RESP149"), _T("RESP150"), _T("RESP151"), _T("RESP152"), _T("RESP153"), _T("RESP154"),
			_T("RESP155"), _T("RESP156"), _T("RESP157"), _T("RESP158"), _T("RESP159"), _T("RESP160"), _T("RESP161"), _T("RESP162"), _T("RESP163"), _T("RESP164"),
			_T("RESP165"), _T("RESP166"), _T("RESP167"), _T("RESP168"), _T("RESP169"), _T("RESP170"), _T("RESP171"), _T("RESP172"), _T("RESP173"), _T("RESP174"),
			_T("RESP175"), _T("RESP176"), _T("RESP177"), _T("RESP178"), _T("RESP179"), _T("RESP180"));

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; \n"),
			Mode_RL, m_resptime[144], m_resptime[145], m_resptime[146], m_resptime[147], m_resptime[148], m_resptime[149], m_resptime[150], m_resptime[151], m_resptime[152], m_resptime[153],
			m_resptime[154], m_resptime[155], m_resptime[156], m_resptime[157], m_resptime[158], m_resptime[159], m_resptime[160], m_resptime[161], m_resptime[162],
			m_resptime[163], m_resptime[164], m_resptime[165], m_resptime[166], m_resptime[167], m_resptime[168], m_resptime[169], m_resptime[170], m_resptime[171],
			m_resptime[172], m_resptime[173], m_resptime[174], m_resptime[175], m_resptime[176], m_resptime[177], m_resptime[178], m_resptime[179]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; \n"),
			Mode_POS, m_errbtn[144], m_errbtn[145], m_errbtn[146], m_errbtn[147], m_errbtn[148], m_errbtn[149], m_errbtn[150], m_errbtn[151], m_errbtn[152], m_errbtn[153],
			m_errbtn[154], m_errbtn[155], m_errbtn[156], m_errbtn[157], m_errbtn[158], m_errbtn[159], m_errbtn[160], m_errbtn[161], m_errbtn[162],
			m_errbtn[163], m_errbtn[164], m_errbtn[165], m_errbtn[166], m_errbtn[167], m_errbtn[168], m_errbtn[169], m_errbtn[170], m_errbtn[171],
			m_errbtn[172], m_errbtn[173], m_errbtn[174], m_errbtn[175], m_errbtn[176], m_errbtn[177], m_errbtn[178], m_errbtn[179]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; \n"),
			Mode_ABC, m_prbtn[144], m_prbtn[145], m_prbtn[146], m_prbtn[147], m_prbtn[148], m_prbtn[149], m_prbtn[150], m_prbtn[151], m_prbtn[152], m_prbtn[153],
			m_prbtn[154], m_prbtn[155], m_prbtn[156], m_prbtn[157], m_prbtn[158], m_prbtn[159], m_prbtn[160], m_prbtn[161], m_prbtn[162],
			m_prbtn[163], m_prbtn[164], m_prbtn[165], m_prbtn[166], m_prbtn[167], m_prbtn[168], m_prbtn[169], m_prbtn[170], m_prbtn[171],
			m_prbtn[172], m_prbtn[173], m_prbtn[174], m_prbtn[175], m_prbtn[176], m_prbtn[177], m_prbtn[178], m_prbtn[179]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format("\n");
		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; \r\n"),
			_T("MODE"), _T("RESP181"), _T("RESP182"), _T("RESP183"), _T("RESP184"), _T("RESP185"), _T("RESP186"), _T("RESP187"), _T("RESP188"), _T("RESP189"), _T("RESP190"),
			_T("RESP191"), _T("RESP192"), _T("RESP193"), _T("RESP194"), _T("RESP195"), _T("RESP196"), _T("RESP197"), _T("RESP198"), _T("RESP199"), _T("RESP200"),
			_T("RESP201"), _T("RESP202"), _T("RESP203"), _T("RESP204"), _T("RESP205"), _T("RESP206"), _T("RESP207"), _T("RESP208"), _T("RESP209"), _T("RESP210"),
			_T("RESP211"), _T("RESP212"), _T("RESP213"), _T("RESP214"), _T("RESP215"), _T("RESP216"));

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; \n"),
			Mode_RL, m_resptime[180], m_resptime[181], m_resptime[182], m_resptime[183], m_resptime[184], m_resptime[185], m_resptime[186], m_resptime[187], m_resptime[188], m_resptime[189],
			m_resptime[190], m_resptime[191], m_resptime[192], m_resptime[193], m_resptime[194], m_resptime[195], m_resptime[196], m_resptime[197], m_resptime[198],
			m_resptime[199], m_resptime[200], m_resptime[201], m_resptime[202], m_resptime[203], m_resptime[204], m_resptime[205], m_resptime[206], m_resptime[207],
			m_resptime[208], m_resptime[209], m_resptime[210], m_resptime[211], m_resptime[212], m_resptime[213], m_resptime[214], m_resptime[215]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; \n"),
			Mode_POS, m_errbtn[180], m_errbtn[181], m_errbtn[182], m_errbtn[183], m_errbtn[184], m_errbtn[185], m_errbtn[186], m_errbtn[187], m_errbtn[188], m_errbtn[189],
			m_errbtn[190], m_errbtn[191], m_errbtn[192], m_errbtn[193], m_errbtn[194], m_errbtn[195], m_errbtn[196], m_errbtn[197], m_errbtn[198],
			m_errbtn[199], m_errbtn[200], m_errbtn[201], m_errbtn[202], m_errbtn[203], m_errbtn[204], m_errbtn[205], m_errbtn[206], m_errbtn[207],
			m_errbtn[208], m_errbtn[209], m_errbtn[210], m_errbtn[211], m_errbtn[212], m_errbtn[213], m_errbtn[214], m_errbtn[215]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; \n"),
			Mode_ABC, m_prbtn[180], m_prbtn[181], m_prbtn[182], m_prbtn[183], m_prbtn[184], m_prbtn[185], m_prbtn[186], m_prbtn[187], m_prbtn[188], m_prbtn[189],
			m_prbtn[190], m_prbtn[191], m_prbtn[192], m_prbtn[193], m_prbtn[194], m_prbtn[195], m_prbtn[196], m_prbtn[197], m_prbtn[198],
			m_prbtn[199], m_prbtn[200], m_prbtn[201], m_prbtn[202], m_prbtn[203], m_prbtn[204], m_prbtn[205], m_prbtn[206], m_prbtn[207],
			m_prbtn[208], m_prbtn[209], m_prbtn[210], m_prbtn[211], m_prbtn[212], m_prbtn[213], m_prbtn[214], m_prbtn[215]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format("\n");
		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; \r\n"),
			_T("MODE"), _T("RESP217"), _T("RESP218"), _T("RESP219"), _T("RESP220"), _T("RESP221"), _T("RESP222"), _T("RESP223"), _T("RESP224"), _T("RESP225"), _T("RESP226"),
			_T("RESP227"), _T("RESP228"), _T("RESP229"), _T("RESP230"), _T("RESP231"), _T("RESP232"), _T("RESP233"), _T("RESP234"), _T("RESP235"), _T("RESP236"),
			_T("RESP237"), _T("RESP238"), _T("RESP239"), _T("RESP240"), _T("RESP241"), _T("RESP242"), _T("RESP243"), _T("RESP244"), _T("RESP245"), _T("RESP246"),
			_T("RESP247"), _T("RESP248"), _T("RESP249"), _T("RESP250"), _T("RESP251"), _T("RESP252"));

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; \n"),
			Mode_RL, m_resptime[216], m_resptime[217], m_resptime[218], m_resptime[219], m_resptime[220], m_resptime[221], m_resptime[222], m_resptime[223], m_resptime[224], m_resptime[225],
			m_resptime[226], m_resptime[227], m_resptime[228], m_resptime[229], m_resptime[230], m_resptime[231], m_resptime[232], m_resptime[233], m_resptime[234],
			m_resptime[235], m_resptime[236], m_resptime[237], m_resptime[238], m_resptime[239], m_resptime[240], m_resptime[241], m_resptime[242], m_resptime[243],
			m_resptime[244], m_resptime[245], m_resptime[246], m_resptime[247], m_resptime[248], m_resptime[249], m_resptime[250], m_resptime[251]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; \n"),
			Mode_POS, m_errbtn[216], m_errbtn[217], m_errbtn[218], m_errbtn[219], m_errbtn[220], m_errbtn[221], m_errbtn[222], m_errbtn[223], m_errbtn[224], m_errbtn[225],
			m_errbtn[226], m_errbtn[227], m_errbtn[228], m_errbtn[229], m_errbtn[230], m_errbtn[231], m_errbtn[232], m_errbtn[233], m_errbtn[234],
			m_errbtn[235], m_errbtn[236], m_errbtn[237], m_errbtn[238], m_errbtn[239], m_errbtn[240], m_errbtn[241], m_errbtn[242], m_errbtn[243],
			m_errbtn[244], m_errbtn[245], m_errbtn[246], m_errbtn[247], m_errbtn[248], m_errbtn[249], m_errbtn[250], m_errbtn[251]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; \n"),
			Mode_ABC, m_prbtn[216], m_prbtn[217], m_prbtn[218], m_prbtn[219], m_prbtn[220], m_prbtn[221], m_prbtn[222], m_prbtn[223], m_prbtn[224], m_prbtn[225],
			m_prbtn[226], m_prbtn[227], m_prbtn[228], m_prbtn[229], m_prbtn[230], m_prbtn[231], m_prbtn[232], m_prbtn[233], m_prbtn[234],
			m_prbtn[235], m_prbtn[236], m_prbtn[237], m_prbtn[238], m_prbtn[239], m_prbtn[240], m_prbtn[241], m_prbtn[242], m_prbtn[243],
			m_prbtn[244], m_prbtn[245], m_prbtn[246], m_prbtn[247], m_prbtn[248], m_prbtn[249], m_prbtn[250], m_prbtn[251]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format("\n");
		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; \r\n"),
			_T("MODE"), _T("RESP253"), _T("RESP254"), _T("RESP255"), _T("RESP256"),	_T("RESP257"), _T("RESP258"), _T("RESP259"), _T("RESP260"), _T("RESP261"), _T("RESP262"), 
			_T("RESP263"), _T("RESP264"), _T("RESP265"), _T("RESP266"), _T("RESP267"), _T("RESP268"), _T("RESP269"), _T("RESP270"), _T("RESP271"), _T("RESP272"), 
			_T("RESP273"), _T("RESP274"), _T("RESP275"), _T("RESP276"), _T("RESP277"), _T("RESP278"), _T("RESP279"), _T("RESP280"), _T("RESP281"), _T("RESP282"), 
			_T("RESP283"), _T("RESP284"), _T("RESP285"), _T("RESP286"), _T("RESP287"), _T("RESP288"));

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; \n"),
			Mode_RL, m_resptime[252], m_resptime[253], m_resptime[254], m_resptime[255], m_resptime[256], m_resptime[257], m_resptime[258], m_resptime[259], m_resptime[260],
			m_resptime[261], m_resptime[262], m_resptime[263], m_resptime[264], m_resptime[265], m_resptime[266], m_resptime[267], m_resptime[268], m_resptime[269],
			m_resptime[270], m_resptime[271], m_resptime[272], m_resptime[273], m_resptime[274], m_resptime[275], m_resptime[276], m_resptime[277], m_resptime[278],
			m_resptime[279], m_resptime[280], m_resptime[281], m_resptime[282], m_resptime[283], m_resptime[284], m_resptime[285], m_resptime[286], m_resptime[287]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; \n"),
			Mode_POS, m_errbtn[252], m_errbtn[253], m_errbtn[254], m_errbtn[255], m_errbtn[256], m_errbtn[257], m_errbtn[258], m_errbtn[259], m_errbtn[260],
			m_errbtn[261], m_errbtn[262], m_errbtn[263], m_errbtn[264], m_errbtn[265], m_errbtn[266], m_errbtn[267], m_errbtn[268], m_errbtn[269],
			m_errbtn[270], m_errbtn[271], m_errbtn[272], m_errbtn[273], m_errbtn[274], m_errbtn[275], m_errbtn[276], m_errbtn[277], m_errbtn[278],
			m_errbtn[279], m_errbtn[280], m_errbtn[281], m_errbtn[282], m_errbtn[283], m_errbtn[284], m_errbtn[285], m_errbtn[286], m_errbtn[287]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; \n"),
			Mode_ABC, m_prbtn[252], m_prbtn[253], m_prbtn[254], m_prbtn[255], m_prbtn[256], m_prbtn[257], m_prbtn[258], m_prbtn[259], m_prbtn[260],
			m_prbtn[261], m_prbtn[262], m_prbtn[263], m_prbtn[264], m_prbtn[265], m_prbtn[266], m_prbtn[267], m_prbtn[268], m_prbtn[269],
			m_prbtn[270], m_prbtn[271], m_prbtn[272], m_prbtn[273], m_prbtn[274], m_prbtn[275], m_prbtn[276], m_prbtn[277], m_prbtn[278],
			m_prbtn[279], m_prbtn[280], m_prbtn[281], m_prbtn[282], m_prbtn[283], m_prbtn[284], m_prbtn[285], m_prbtn[286], m_prbtn[287]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format("\n");
		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; \r\n"),
			_T("MODE"), _T("RESP289"), _T("RESP290"), _T("RESP291"), _T("RESP292"), _T("RESP293"), _T("RESP294"), _T("RESP295"), _T("RESP296"), _T("RESP297"), _T("RESP298"), 
			_T("RESP299"), _T("RESP300"), _T("RESP301"), _T("RESP302"), _T("RESP303"), _T("RESP304"), _T("RESP305"), _T("RESP306"), _T("RESP307"), _T("RESP308"), 
			_T("RESP309"), _T("RESP310"), _T("RESP311"), _T("RESP312"), _T("RESP313"), _T("RESP314"), _T("RESP315"), _T("RESP316"), _T("RESP317"), _T("RESP318"), 
			_T("RESP319"), _T("RESP320"), _T("RESP321"), _T("RESP322"), _T("RESP323"), _T("RESP324"));

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; \n"),
			Mode_RL, m_resptime[288], m_resptime[289], m_resptime[290], m_resptime[291], m_resptime[292], m_resptime[293], m_resptime[294], m_resptime[295], m_resptime[296],
			m_resptime[297], m_resptime[298], m_resptime[299], m_resptime[300], m_resptime[301], m_resptime[302], m_resptime[303], m_resptime[304], m_resptime[305],
			m_resptime[306], m_resptime[307], m_resptime[308], m_resptime[309], m_resptime[310], m_resptime[311], m_resptime[312], m_resptime[313], m_resptime[314],
			m_resptime[315], m_resptime[316], m_resptime[317], m_resptime[318], m_resptime[319], m_resptime[320], m_resptime[321], m_resptime[322], m_resptime[323]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; \n"),
			Mode_POS, m_errbtn[288], m_errbtn[289], m_errbtn[290], m_errbtn[291], m_errbtn[292], m_errbtn[293], m_errbtn[294], m_errbtn[295], m_errbtn[296],
			m_errbtn[297], m_errbtn[298], m_errbtn[299], m_errbtn[300], m_errbtn[301], m_errbtn[302], m_errbtn[303], m_errbtn[304], m_errbtn[305],
			m_errbtn[306], m_errbtn[307], m_errbtn[308], m_errbtn[309], m_errbtn[310], m_errbtn[311], m_errbtn[312], m_errbtn[313], m_errbtn[314],
			m_errbtn[315], m_errbtn[316], m_errbtn[317], m_errbtn[318], m_errbtn[319], m_errbtn[320], m_errbtn[321], m_errbtn[322], m_errbtn[323]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; \n"),
			Mode_ABC, m_prbtn[288], m_prbtn[289], m_prbtn[290], m_prbtn[291], m_prbtn[292], m_prbtn[293], m_prbtn[294], m_prbtn[295], m_prbtn[296],
			m_prbtn[297], m_prbtn[298], m_prbtn[299], m_prbtn[300], m_prbtn[301], m_prbtn[302], m_prbtn[303], m_prbtn[304], m_prbtn[305],
			m_prbtn[306], m_prbtn[307], m_prbtn[308], m_prbtn[309], m_prbtn[310], m_prbtn[311], m_prbtn[312], m_prbtn[313], m_prbtn[314],
			m_prbtn[315], m_prbtn[316], m_prbtn[317], m_prbtn[318], m_prbtn[319], m_prbtn[320], m_prbtn[321], m_prbtn[322], m_prbtn[323]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format("\n");
		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; \r\n"),
			_T("MODE"), _T("RESP325"), _T("RESP326"), _T("RESP327"), _T("RESP328"), _T("RESP329"), _T("RESP330"), _T("RESP331"), _T("RESP332"), _T("RESP333"), _T("RESP334"),
			_T("RESP335"), _T("RESP336"), _T("RESP337"), _T("RESP338"), _T("RESP339"), _T("RESP340"), _T("RESP341"), _T("RESP342"), _T("RESP343"), _T("RESP344"),
			_T("RESP345"), _T("RESP346"), _T("RESP347"), _T("RESP348"), _T("RESP349"), _T("RESP350"), _T("RESP351"), _T("RESP352"), _T("RESP353"), _T("RESP354"),
			_T("RESP355"), _T("RESP356"), _T("RESP357"), _T("RESP358"), _T("RESP359"), _T("RESP360"));

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; \n"),
			Mode_RL, m_resptime[324], m_resptime[325], m_resptime[326], m_resptime[327], m_resptime[328], m_resptime[329], m_resptime[330], m_resptime[331], m_resptime[332],
			m_resptime[333], m_resptime[334], m_resptime[335], m_resptime[336], m_resptime[337], m_resptime[338], m_resptime[339], m_resptime[340], m_resptime[341],
			m_resptime[342], m_resptime[343], m_resptime[344], m_resptime[345], m_resptime[346], m_resptime[347], m_resptime[348], m_resptime[349], m_resptime[350],
			m_resptime[351], m_resptime[352], m_resptime[353], m_resptime[354], m_resptime[355], m_resptime[356], m_resptime[357], m_resptime[358], m_resptime[359]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s; \n"),
			Mode_POS, m_errbtn[324], m_errbtn[325], m_errbtn[326], m_errbtn[327], m_errbtn[328], m_errbtn[329], m_errbtn[330], m_errbtn[331], m_errbtn[332],
			m_errbtn[333], m_errbtn[334], m_errbtn[335], m_errbtn[336], m_errbtn[337], m_errbtn[338], m_errbtn[339], m_errbtn[340], m_errbtn[341],
			m_errbtn[342], m_errbtn[343], m_errbtn[344], m_errbtn[345], m_errbtn[346], m_errbtn[347], m_errbtn[348], m_errbtn[349], m_errbtn[350],
			m_errbtn[351], m_errbtn[352], m_errbtn[353], m_errbtn[354], m_errbtn[355], m_errbtn[356], m_errbtn[357], m_errbtn[358], m_errbtn[359]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		strLine.Format(_T("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; \n"),
			Mode_ABC, m_prbtn[324], m_prbtn[325], m_prbtn[326], m_prbtn[327], m_prbtn[328], m_prbtn[329], m_prbtn[330], m_prbtn[331], m_prbtn[332],
			m_prbtn[333], m_prbtn[334], m_prbtn[335], m_prbtn[336], m_prbtn[337], m_prbtn[338], m_prbtn[339], m_prbtn[340], m_prbtn[341],
			m_prbtn[342], m_prbtn[343], m_prbtn[344], m_prbtn[345], m_prbtn[346], m_prbtn[347], m_prbtn[348], m_prbtn[349], m_prbtn[350],
			m_prbtn[351], m_prbtn[352], m_prbtn[353], m_prbtn[354], m_prbtn[355], m_prbtn[356], m_prbtn[357], m_prbtn[358], m_prbtn[359]);

		file.Write(strLine, sizeof(TCHAR)*strLine.GetLength());

		file.Close();
	}

	AfxMessageBox(_T("Saved!"));
}

////////////////////////////////////////   TEST 실행 함수   ///////////////////////////////////////////////////

/////// 왼쪽 ///////

void CSRTTDlg::Mode_LEFT_P1_Apre(int next) {

	if (next == 1) {
		Display_Image(PIC_2ND);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_1ST);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_4TH);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_3RD);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_5TH);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_1ST);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_3RD);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_2ND);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_4TH);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}	
}

void CSRTTDlg::Mode_LEFT_P3_Apre(int next) {

	if (next == 1) {
		Display_Image(PIC_1ST);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_4TH);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_3RD);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_2ND);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_5TH);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_1ST);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_2ND);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_4TH);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_3RD);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_LEFT_P5_Apre(int next) {

	if (next == 1) {
		Display_Image(PIC_2ND);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_5TH);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_1ST);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_4TH);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_3RD);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_2ND);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_4TH);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_1ST);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_3RD);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_LEFT_P1_Apost(int next) {

	if (next == 1) {
		Display_Image(PIC_3RD);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_2ND);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_4TH);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_1ST);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_2ND);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_5TH);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_1ST);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_4TH);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_3RD);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_LEFT_P3_Apost(int next) {

	if (next == 1) {
		Display_Image(PIC_5TH);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_1ST);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_4TH);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_2ND);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_3RD);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_4TH);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_2ND);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_3RD);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_4TH);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_LEFT_P5_Apost(int next) {

	if (next == 1) {
		Display_Image(PIC_3RD);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_2ND);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_1ST);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_5TH);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_4TH);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_2ND);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_3RD);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_1ST);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_4TH);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_LEFT_P1_Bpre(int next) {

	if (next == 1) {
		Display_Image(PIC_3RD);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_4TH);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_1ST);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_5TH);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_2ND);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_3RD);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_1ST);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_2ND);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_4TH);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_LEFT_P3_Bpre(int next) {

	if (next == 1) {
		Display_Image(PIC_5TH);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_1ST);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_2ND);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_3RD);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_4TH);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_3RD);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_1ST);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_2ND);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_4TH);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_LEFT_P5_Bpre(int next) {

	if (next == 1) {
		Display_Image(PIC_5TH);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_4TH);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_1ST);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_2ND);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_3RD);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_4TH);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_2ND);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_1ST);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_3RD);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_LEFT_P1_Bpost(int next) {

	if (next == 1) {
		Display_Image(PIC_1ST);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_2ND);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_4TH);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_1ST);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_3RD);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_5TH);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_2ND);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_4TH);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_3RD);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_LEFT_P3_Bpost(int next) {

	if (next == 1) {
		Display_Image(PIC_3RD);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_5TH);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_1ST);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_2ND);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_4TH);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_1ST);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_3RD);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_4TH);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_2ND);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_LEFT_P5_Bpost(int next) {

	if (next == 1) {
		Display_Image(PIC_4TH);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_2ND);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_3RD);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_5TH);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_1ST);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_2ND);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_1ST);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_4TH);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_3RD);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_LEFT_P1_Cpre(int next) {

	if (next == 1) {
		Display_Image(PIC_5TH);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_3RD);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_1ST);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_2ND);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_4TH);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_2ND);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_1ST);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_4TH);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_3RD);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_LEFT_P3_Cpre(int next) {

	if (next == 1) {
		Display_Image(PIC_4TH);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_3RD);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_1ST);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_5TH);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_2ND);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_1ST);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_4TH);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_2ND);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_3RD);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_LEFT_P5_Cpre(int next) {

	if (next == 1) {
		Display_Image(PIC_1ST);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_3RD);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_4TH);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_2ND);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_5TH);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_2ND);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_1ST);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_4TH);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_3RD);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_LEFT_P1_Cpost(int next) {

	if (next == 1) {
		Display_Image(PIC_4TH);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_1ST);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_2ND);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_3RD);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_5TH);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_4TH);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_3RD);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_1ST);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_2ND);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_LEFT_P3_Cpost(int next) {

	if (next == 1) {
		Display_Image(PIC_2ND);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_3RD);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_4TH);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_5TH);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_1ST);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_4TH);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_3RD);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_1ST);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_2ND);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_LEFT_P5_Cpost(int next) {

	if (next == 1) {
		Display_Image(PIC_4TH);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_1ST);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_5TH);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_2ND);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_3RD);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_4TH);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_2ND);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_1ST);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_3RD);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

/////// 오른쪽 ///////

void CSRTTDlg::Mode_RIGHT_P1_Apre(int next) {

	if (next == 1) {
		Display_Image(PIC_4TH);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_5TH);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_2ND);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_3RD);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_1ST);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_5TH);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_3RD);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_4TH);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_2ND);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_RIGHT_P3_Apre(int next) {

	if (next == 1) {
		Display_Image(PIC_5TH);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_2ND);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_3RD);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_4TH);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_1ST);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_5TH);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_4TH);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_2ND);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_3RD);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_RIGHT_P5_Apre(int next) {

	if (next == 1) {
		Display_Image(PIC_4TH);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_1ST);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_5TH);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_2ND);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_3RD);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_4TH);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_2ND);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_5TH);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_3RD);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_RIGHT_P1_Apost(int next) {

	if (next == 1) {
		Display_Image(PIC_3RD);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_4TH);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_2ND);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_5TH);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_4TH);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_1ST);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_5TH);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_2ND);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_3RD);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_RIGHT_P3_Apost(int next) {

	if (next == 1) {
		Display_Image(PIC_1ST);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_5TH);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_2ND);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_4TH);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_3RD);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_2ND);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_4TH);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_3RD);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_2ND);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_RIGHT_P5_Apost(int next) {

	if (next == 1) {
		Display_Image(PIC_3RD);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_4TH);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_5TH);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_1ST);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_2ND);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_4TH);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_3RD);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_5TH);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_2ND);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_RIGHT_P1_Bpre(int next) {

	if (next == 1) {
		Display_Image(PIC_3RD);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_2ND);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_5TH);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_1ST);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_4TH);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_3RD);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_5TH);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_4TH);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_2ND);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_RIGHT_P3_Bpre(int next) {

	if (next == 1) {
		Display_Image(PIC_1ST);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_5TH);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_4TH);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_3RD);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_2ND);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_3RD);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_5TH);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_4TH);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_2ND);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_RIGHT_P5_Bpre(int next) {

	if (next == 1) {
		Display_Image(PIC_1ST);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_2ND);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_5TH);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_4TH);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_3RD);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_2ND);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_4TH);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_5TH);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_3RD);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_RIGHT_P1_Bpost(int next) {

	if (next == 1) {
		Display_Image(PIC_5TH);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_4TH);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_2ND);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_5TH);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_3RD);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_1ST);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_4TH);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_2ND);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_3RD);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_RIGHT_P3_Bpost(int next) {

	if (next == 1) {
		Display_Image(PIC_3RD);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_1ST);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_5TH);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_4TH);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_2ND);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_5TH);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_3RD);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_2ND);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_4TH);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_RIGHT_P5_Bpost(int next) {

	if (next == 1) {
		Display_Image(PIC_2ND);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_4TH);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_3RD);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_1ST);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_5TH);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_4TH);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_5TH);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_2ND);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_3RD);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_RIGHT_P1_Cpre(int next) {

	if (next == 1) {
		Display_Image(PIC_1ST);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_3RD);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_5TH);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_4TH);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_2ND);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_4TH);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_5TH);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_2ND);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_3RD);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_RIGHT_P3_Cpre(int next) {

	if (next == 1) {
		Display_Image(PIC_2ND);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_3RD);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_5TH);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_1ST);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_4TH);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_5TH);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_2ND);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_4TH);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_3RD);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_RIGHT_P5_Cpre(int next) {

	if (next == 1) {
		Display_Image(PIC_5TH);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_3RD);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_2ND);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_4TH);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_1ST);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_4TH);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_5TH);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_2ND);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_3RD);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_RIGHT_P1_Cpost(int next) {

	if (next == 1) {
		Display_Image(PIC_2ND);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_5TH);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_4TH);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_3RD);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_1ST);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_2ND);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_3RD);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_5TH);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_4TH);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_RIGHT_P3_Cpost(int next) {

	if (next == 1) {
		Display_Image(PIC_4TH);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_3RD);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_2ND);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_1ST);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_5TH);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_2ND);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_3RD);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_5TH);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_4TH);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}

void CSRTTDlg::Mode_RIGHT_P5_Cpost(int next) {

	if (next == 1) {
		Display_Image(PIC_2ND);
	}
	else if (next == 2) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 3) {
		Display_Image(PIC_5TH);
	}
	else if (next == 4) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 5) {
		Display_Image(PIC_1ST);
	}
	else if (next == 6) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 7) {
		Display_Image(PIC_4TH);
	}
	else if (next == 8) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 9) {
		Display_Image(PIC_3RD);
	}
	else if (next == 10) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 11) {
		Display_Image(PIC_2ND);
	}
	else if (next == 12) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 13) {
		Display_Image(PIC_4TH);
	}
	else if (next == 14) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 15) {
		Display_Image(PIC_5TH);
	}
	else if (next == 16) {
		Display_Image(PIC_BLANK);
	}
	else if (next == 17) {
		Display_Image(PIC_3RD);
	}
	else if (next == 18) {
		Display_Image(PIC_BLANK);
	}
}