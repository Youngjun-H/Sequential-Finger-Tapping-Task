
// SRTTDlg.h: 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "SerialCom.h"
#include "opencv2/opencv.hpp"
#include "mmsystem.h"
#pragma comment (lib,"winmm")

using namespace std;
using namespace cv;

#define	STORE_DATA_FILE	_T("SRTT_Data_")

// 버튼 시리얼 번호
#define	CLICK_L1	1
#define	CLICK_L2	2
#define	CLICK_L3	3
#define	CLICK_L4	4
#define	CLICK_L5	5

#define	CLICK_R1	1
#define	CLICK_R2	2
#define	CLICK_R3	3
#define	CLICK_R4	4
#define	CLICK_R5	5

// 이미지 번호
#define	PIC_1ST		  1
#define	PIC_2ND		  2
#define	PIC_3RD		  3
#define	PIC_4TH		  4
#define	PIC_5TH		  5
#define PIC_END		  6
#define PIC_BASELINE  7
#define PIC_BLANK     8
#define PIC_READY     9

// 손방향 선택

#define MODE_LEFT   0
#define MODE_RIGHT  1

// Point of stimulation 선택

#define MODE_POS_1  0
#define MODE_POS_3  1
#define MODE_POS_5  2

// A_pre ~ C_post 선택
#define MODE_1      0
#define MODE_2      1
#define MODE_3      2
#define MODE_4		3
#define MODE_5		4
#define MODE_6		5

// 선택된 테스트 모드
#define MODE_L_1_1     0
#define MODE_L_3_1     1
#define MODE_L_5_1     2
#define MODE_L_1_2     3
#define MODE_L_3_2     4
#define MODE_L_5_2     5
#define MODE_L_1_3     6
#define MODE_L_3_3     7
#define MODE_L_5_3     8
#define MODE_L_1_4     9
#define MODE_L_3_4     10
#define MODE_L_5_4     11
#define MODE_L_1_5     12
#define MODE_L_3_5     13
#define MODE_L_5_5     14
#define MODE_L_1_6     15
#define MODE_L_3_6     16
#define MODE_L_5_6     17

#define MODE_R_1_1     18
#define MODE_R_3_1     19
#define MODE_R_5_1     20
#define MODE_R_1_2     21
#define MODE_R_3_2     22
#define MODE_R_5_2     23
#define MODE_R_1_3     24
#define MODE_R_3_3     25
#define MODE_R_5_3     26
#define MODE_R_1_4     27
#define MODE_R_3_4     28
#define MODE_R_5_4     29
#define MODE_R_1_5     30
#define MODE_R_3_5     31
#define MODE_R_5_5     32
#define MODE_R_1_6     33
#define MODE_R_3_6     34
#define MODE_R_5_6     35


#define ON  1
#define OFF 0

#define REST_1ST 19

void CALLBACK TimerProc(UINT m_nTimerID, UINT uiMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);


// CSRTTDlg 대화 상자
class CSRTTDlg : public CDialogEx
{
// 생성입니다.
public:
	CSRTTDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SRTT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 시리얼통신 관련
	CSerialCom* m_Comm;
	LRESULT OnSerialReceive(WPARAM length, LPARAM lpara);
	LRESULT OnThreadClosed(WPARAM length, LPARAM lpara);
	CString strBaudRate;
	BOOL comport_state;	
	CString m_str_comport;
	CString SerialRxDATA_buf;
	CString SerialRxDATA;

	// 버튼 관련
	int prs_btn_L;
	int prs_btn_R;
	int ref_btn;
	int ref_rest;
	CButton m_connectkey;

	void OnBnClickedBtnConnect();
	CEdit m_EditOut;

	void OnBnClickedBtnStart();
	CButton ctr_Start;
	

	// 이미지 파일 관련
	int m_NextInst;
	int m_NextInst_part;

	Mat m_matImage;
	Mat m_matImage_baseline = imread("Baseline.png", IMREAD_UNCHANGED);
	Mat m_matImage_blank = imread("Blank.png", IMREAD_UNCHANGED);
	Mat m_matImage_ready = imread("Ready.png", IMREAD_UNCHANGED);
	Mat m_matImage_1 = imread("pic_1st.png", IMREAD_UNCHANGED);
	Mat m_matImage_2 = imread("pic_2nd.png", IMREAD_UNCHANGED);
	Mat m_matImage_3 = imread("pic_3rd.png", IMREAD_UNCHANGED);
	Mat m_matImage_4 = imread("pic_4th.png", IMREAD_UNCHANGED);
	Mat m_matImage_5 = imread("pic_5th.png", IMREAD_UNCHANGED);
	Mat m_matImage_end = imread("Rest.png", IMREAD_UNCHANGED);
	CStatic m_picture;
	BITMAPINFO *m_pBitmapInfo; // Bitmap 정보를 담고 있는 구조체.
	void CreateBitmapInfo(int w, int h, int bpp); // Bitmap 정보를 생성하는 함수
	void DrawImage(); // 그리는 작업을 수행하는 함수
	void Display_Image(int num);

	//테스트 실행 관련 함수

	void Start_Test_LEFT(int order);
	void Start_Test_RIGHT(int order);
	void Mode_LEFT_P1_Apre(int next);
	void Mode_LEFT_P3_Apre(int next);
	void Mode_LEFT_P5_Apre(int next);
	void Mode_LEFT_P1_Apost(int next);
	void Mode_LEFT_P3_Apost(int next);
	void Mode_LEFT_P5_Apost(int next);
	void Mode_LEFT_P1_Bpre(int next);
	void Mode_LEFT_P3_Bpre(int next);
	void Mode_LEFT_P5_Bpre(int next);
	void Mode_LEFT_P1_Bpost(int next);
	void Mode_LEFT_P3_Bpost(int next);
	void Mode_LEFT_P5_Bpost(int next);
	void Mode_LEFT_P1_Cpre(int next);
	void Mode_LEFT_P3_Cpre(int next);
	void Mode_LEFT_P5_Cpre(int next);
	void Mode_LEFT_P1_Cpost(int next);
	void Mode_LEFT_P3_Cpost(int next);
	void Mode_LEFT_P5_Cpost(int next);

	void Mode_RIGHT_P1_Apre(int next);
	void Mode_RIGHT_P3_Apre(int next);
	void Mode_RIGHT_P5_Apre(int next);
	void Mode_RIGHT_P1_Apost(int next);
	void Mode_RIGHT_P3_Apost(int next);
	void Mode_RIGHT_P5_Apost(int next);
	void Mode_RIGHT_P1_Bpre(int next);
	void Mode_RIGHT_P3_Bpre(int next);
	void Mode_RIGHT_P5_Bpre(int next);
	void Mode_RIGHT_P1_Bpost(int next);
	void Mode_RIGHT_P3_Bpost(int next);
	void Mode_RIGHT_P5_Bpost(int next);
	void Mode_RIGHT_P1_Cpre(int next);
	void Mode_RIGHT_P3_Cpre(int next);
	void Mode_RIGHT_P5_Cpre(int next);
	void Mode_RIGHT_P1_Cpost(int next);
	void Mode_RIGHT_P3_Cpost(int next);
	void Mode_RIGHT_P5_Cpost(int next);


	int m_blank_order;
	int resp_time;
	int test_time;
	int m_prs_order_count; //테스트 순서
	//int prs_btn_time[30];
	//int pressed_btn[30];
	//int prssed_btn_ref[30];
	int prs_btn_time;
	int pressed_btn;
	int prssed_btn_ref;
	int m_resptime[1000];
	CString m_errbtn[1000];
	int m_prbtn[1000];

	int num_interrupt;
	int prs_btn_time_inter;
	int pressed_btn_inter;


	//분류해야하는 변수들
	UINT m_nTimerID;
	void MenuHide();
	int rest;	
	int btn_cnt;
	int err_btn;	
	int btn_count;
	int time;	

	CString	s_SaveFile;
	void OnBnClickedBtnSave();

	// Radio 버튼	
	int Mode_RL;
	int Mode_POS;
	int Mode_ABC;
	void RadioButtonClick_RL(UINT ID);
	void RadioButtonClick_POS(UINT ID);
	void RadioButtonClick_ABC(UINT ID);
	
	BOOL m_mode_rl;
	UINT m_mode_pos;
	UINT m_mode_abc;
	int m_mode_selected;
	CStatic m_groupbox_1;
	CStatic m_groupbox_2;
	CStatic m_groupbox_3;

	CButton m_radiobtn_1;
	CButton m_radiobtn_2;
	CButton m_radiobtn_3;
	CButton m_radiobtn_4;
	CButton m_radiobtn_5;
	CButton m_radiobtn_6;
	CButton m_radiobtn_7;
	CButton m_radiobtn_8;
	CButton m_radiobtn_9;
	CButton m_radiobtn_10;
	CButton m_radiobtn_11;
	
};
