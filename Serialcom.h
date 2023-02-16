#pragma once

#define MAXBUFF			50000
#define InBuffSize		50000
#define OutBuffSize		50000
#define ASCII_XON		0x11	
#define ASCII_XOFF		0x13
#define WM_SERIALRECEIVE	(WM_USER+1)		// 수신메시지
#define WM_SERIALCLOSE		(WM_USER+2)		// 종료메시지


class CSerialCom :
	public CCmdTarget
{

public:
	CSerialCom();
	~CSerialCom();

public:
	HANDLE m_hComDev;
	HWND m_hWnd;
	BOOL m_bIsOpenned;
	CString m_sComPort;
	CString m_sBaudRate;
	CString m_sParity;
	CString m_sDataBit;
	CString m_sStopBit;
	BOOL m_bFlowChk;
	OVERLAPPED m_OLR;
	OVERLAPPED m_OLW;
	char m_sInBuff[MAXBUFF * 2];
	int		m_nLength;
	CEvent* m_pEvent;

public:
	void	Clear();
	int		SerialReceive(LPSTR inbuff, int len);
	BOOL	SerialSend(LPCTSTR outbuff, int len);
	BOOL	Create(HWND hWnd);
	void	HandleClose();
	void	Close();
	void	ResetSerial();
	CSerialCom(CString port, CString baudrate, CString parity, CString databit, CString stopbit);
};