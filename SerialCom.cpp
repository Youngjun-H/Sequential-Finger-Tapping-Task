#include "pch.h"
#include "SerialCom.h"

CSerialCom::CSerialCom()
{
}


CSerialCom::~CSerialCom()
{
	if (m_bIsOpenned)
		Close();
	delete m_pEvent;
}

CSerialCom::CSerialCom(CString port, CString baudrate, CString parity, CString databit, CString stopbit)
{
	m_sComPort = port;
	m_sBaudRate = baudrate;
	m_sParity = parity;
	m_sDataBit = databit;
	m_sStopBit = stopbit;
	m_bFlowChk = 0;
	m_bIsOpenned = FALSE;
	m_nLength = 0;
	memset(m_sInBuff, 0, MAXBUFF * 2);
	m_pEvent = new CEvent(FALSE, TRUE);
}

void CSerialCom::ResetSerial()
{
	DCB		dcb;
	DWORD	DErr;
	COMMTIMEOUTS	CommTimeOuts;

	if (!m_bIsOpenned)
		return;

	ClearCommError(m_hComDev, &DErr, NULL);
	SetupComm(m_hComDev, InBuffSize, OutBuffSize);
	PurgeComm(m_hComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

	// Set up for overlapped I/O
	CommTimeOuts.ReadIntervalTimeout = MAXDWORD;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 1000;
	SetCommTimeouts(m_hComDev, &CommTimeOuts);

	memset(&dcb, 0, sizeof(DCB));
	dcb.DCBlength = sizeof(DCB);

	GetCommState(m_hComDev, &dcb);

	dcb.fBinary = TRUE;
	dcb.fParity = TRUE;
	dcb.BaudRate = CBR_115200;
	dcb.Parity = NOPARITY;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;

	SetCommState(m_hComDev, &dcb);
	SetCommMask(m_hComDev, EV_RXCHAR);
}

void CSerialCom::Close()
{
	if (!m_bIsOpenned)
		return;

	m_bIsOpenned = FALSE;
	SetCommMask(m_hComDev, 0);
	EscapeCommFunction(m_hComDev, CLRDTR);
	PurgeComm(m_hComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	Sleep(500);
}

UINT CommThread(LPVOID lpData)
{
	extern short g_nRemoteStatus;
	DWORD	ErrorFlags;
	COMSTAT	ComStat;
	DWORD	EvtMask;
	char	buff[MAXBUFF];
	DWORD	Length;
	int		size;
	int		insize = 0;

	CSerialCom* Comm = (CSerialCom*)lpData;

	while (Comm->m_bIsOpenned)
	{
		EvtMask = 0;
		Length = 0;
		insize = 0;
		memset(buff, '\0', MAXBUFF);
		WaitCommEvent(Comm->m_hComDev, &EvtMask, NULL);
		ClearCommError(Comm->m_hComDev, &ErrorFlags, &ComStat);

		if ((EvtMask & EV_RXCHAR) && ComStat.cbInQue)
		{
			if (ComStat.cbInQue > MAXBUFF)
				size = MAXBUFF;
			else
				size = ComStat.cbInQue;

			do
			{
				ClearCommError(Comm->m_hComDev, &ErrorFlags, &ComStat);

				// overlapped i/o 를 통해 데이터를 읽음 

				if (!ReadFile(Comm->m_hComDev, buff + insize, size, &Length, &(Comm->m_OLR)))
				{
					// Error
					TRACE("Error in ReadFile\n");

					if (GetLastError() == ERROR_IO_PENDING)
					{
						if (WaitForSingleObject(Comm->m_OLR.hEvent, 1000) != WAIT_OBJECT_0)
							Length = 0;
						else
							GetOverlappedResult(Comm->m_hComDev, &(Comm->m_OLR), &Length, FALSE);
					}
					else
						Length = 0;
				}

				insize += Length;
			} while ((Length != 0) && (insize < size));

			ClearCommError(Comm->m_hComDev, &ErrorFlags, &ComStat);

			if (Comm->m_nLength + insize > MAXBUFF * 2)
				insize = (Comm->m_nLength + insize) - MAXBUFF * 2;

			Comm->m_pEvent->ResetEvent();
			memcpy(Comm->m_sInBuff + Comm->m_nLength, buff, insize);
			Comm->m_nLength += insize;
			Comm->m_pEvent->SetEvent();
			LPARAM temp = (LPARAM)Comm;
			SendMessage(Comm->m_hWnd, WM_SERIALRECEIVE, Comm->m_nLength, temp);
		}
	}

	PurgeComm(Comm->m_hComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	LPARAM temp = (LPARAM)Comm;
	SendMessage(Comm->m_hWnd, WM_SERIALCLOSE, 0, temp);

	return 0;
}

void CSerialCom::HandleClose()
{
	CloseHandle(m_hComDev);
	CloseHandle(m_OLR.hEvent);
	CloseHandle(m_OLW.hEvent);
}

BOOL CSerialCom::Create(HWND hWnd)
{
	m_hWnd = hWnd;
	m_hComDev = CreateFile(m_sComPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

	if (m_hComDev != INVALID_HANDLE_VALUE)
		m_bIsOpenned = TRUE;
	else
		return FALSE;

	ResetSerial();

	m_OLW.Offset = 0;
	m_OLW.OffsetHigh = 0;
	m_OLR.Offset = 0;
	m_OLR.OffsetHigh = 0;

	m_OLR.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (m_OLR.hEvent = NULL) {
		CloseHandle(m_OLR.hEvent);
		return FALSE;
	}

	m_OLW.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (m_OLW.hEvent = NULL) {
		CloseHandle(m_OLW.hEvent);
		return FALSE;
	}

	AfxBeginThread(CommThread, (LPVOID)this);
	EscapeCommFunction(m_hComDev, SETDTR);
	return TRUE;
}

BOOL CSerialCom::SerialSend(LPCTSTR outbuff, int len)
{
	BOOL	bRet = TRUE;
	DWORD	ErrorFlags;
	COMSTAT	ComStat;
	DWORD	BytesWritten;
	DWORD	BytesSent = 0;

	ClearCommError(m_hComDev, &ErrorFlags, &ComStat);

	if (!WriteFile(m_hComDev, outbuff, len, &BytesWritten, &m_OLW))
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			if (WaitForSingleObject(m_OLW.hEvent, 1000) != WAIT_OBJECT_0)
				bRet = FALSE;
			else
				GetOverlappedResult(m_hComDev, &m_OLW, &BytesWritten, FALSE);
		}
		else // I/O error
			bRet = FALSE;	// ignore error
	}

	ClearCommError(m_hComDev, &ErrorFlags, &ComStat);

	return bRet;
}

int	CSerialCom::SerialReceive(LPSTR inbuff, int len)
{
	CSingleLock lockObj((CSyncObject*)m_pEvent, FALSE);

	// argument value is not valid

	if (len == 0)
		return -1;
	else if (len > MAXBUFF)
		return -1;

	if (m_nLength == 0)
	{
		inbuff[0] = '\0';
		return 0;
	}
	else if (m_nLength <= len)
	{
		lockObj.Lock();
		memcpy(inbuff, m_sInBuff, m_nLength);
		memset(m_sInBuff, 0, MAXBUFF * 2);

		int tmp = m_nLength;
		m_nLength = 0;
		lockObj.Unlock();
		return tmp;
	}
	else     // m_nLength > len
	{
		lockObj.Lock();
		memcpy(inbuff, m_sInBuff, len);
		memmove(m_sInBuff, m_sInBuff + len, MAXBUFF * 2 - len);

		m_nLength -= len;
		lockObj.Unlock();
		return len;
	}
}

void CSerialCom::Clear()
{
	PurgeComm(m_hComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	memset(m_sInBuff, 0, MAXBUFF * 2);
	m_nLength = 0;
}

