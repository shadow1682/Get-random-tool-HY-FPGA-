
// FPGA_GET_RNGDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FPGA_GET_RNG.h"
#include "FPGA_GET_RNGDlg.h"
#include "afxdialogex.h"
#include "stdlib.h"
#include "stdio.h"
#include "io.h"
#include "Transformers.h"
#include "USB_Driver.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int Check_AUTOSAVE = 0,Check_CLOSEHEX = 0, Check_SEC = 0;
CString DEMO_NAME = "FPGAȡ�������libusb��V3.1";
CString str_hex = _T(""),file_name_ASC = _T(""), file_name_HEX = _T("");
CString SEC_File = _T(""), strSECpath = _T("");
CString str_mASC, str_mSEC;
int ifile = 0,iSEC = 0;
float bin_KB = 0;

DWORD WINAPI ThreadFun(LPVOID pthread);
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CFPGA_GET_RNGDlg �Ի���




CFPGA_GET_RNGDlg::CFPGA_GET_RNGDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFPGA_GET_RNGDlg::IDD, pParent)
	, m_nstrASC(_T(""))
	, m_nstrSEC(_T(""))
	, m_int_delay(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFPGA_GET_RNGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_ASC, m_nstrASC);
	DDX_Text(pDX, IDC_EDIT_SEC, m_nstrSEC);
	DDX_Control(pDX, IDC_EDIT_RNG_PATH, m_edit_rng_path);
	//  DDX_Control(pDX, IDC_RICHEDIT2_RNG_DATA, m_richedit_rng_data);
	DDX_Text(pDX, IDC_EDIT_DLY_SET, m_int_delay);
	DDV_MinMaxInt(pDX, m_int_delay, 500, 10000000);
	//  DDX_Control(pDX, IDC_CHECK_AUTO_CLR, m_ctrl_auto_clr);
	//  DDX_Control(pDX, IDC_EDIT_RNG_DATA, m_edit_ctrl_rng_data);
}

BEGIN_MESSAGE_MAP(CFPGA_GET_RNGDlg, CDialog)
	ON_MESSAGE(WM_DEVICECHANGE, OnDeviceChange)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CFPGA_GET_RNGDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_SAVEPATH, &CFPGA_GET_RNGDlg::OnBnClickedButtonSavepath)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CFPGA_GET_RNGDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_CHECK_AUTOSAVE, &CFPGA_GET_RNGDlg::OnBnClickedCheckAutosave)
	ON_BN_CLICKED(IDC_CHECK_CLOSEHEX, &CFPGA_GET_RNGDlg::OnBnClickedCheckClosehex)
//	ON_COMMAND(ID_MENU_DEVLINK, &CFPGA_GET_RNGDlg::OnMenuDevlink)
//	ON_COMMAND(ID_MENU_CLOSE, &CFPGA_GET_RNGDlg::OnMenuClose)
	ON_BN_CLICKED(IDC_CHECK_SMALL, &CFPGA_GET_RNGDlg::OnBnClickedCheckSmall)
//	ON_BN_CLICKED(IDC_BUTTON_EDIT_CLEAR, &CFPGA_GET_RNGDlg::OnBnClickedButtonEditClear)
//	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

static const GUID GUID_DEVINTERFACE_LIST[] =
{
	// GUID_DEVINTERFACE_USB_DEVICE
	{ 0xA5DCBF10, 0x6530, 0x11D2,{ 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } },

	// GUID_DEVINTERFACE_DISK
	//{ 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } },

	// GUID_DEVINTERFACE_HID, 
	//{ 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } },

	// GUID_NDIS_LAN_CLASS
	//{ 0xad498944, 0x762f, 0x11d0, { 0x8d, 0xcb, 0x00, 0xc0, 0x4f, 0xc3, 0x35, 0x8c } }

};


// CFPGA_GET_RNGDlg ��Ϣ�������

BOOL CFPGA_GET_RNGDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	/*��USB����ע�ᣬΪ�Ժ�����Ϣ��׼��*/

	if (USBScanDev(1) == 1 && USBOpenDev(0) == 0)
	{
		this->SetWindowText(DEMO_NAME + "*�豸������");
		((CButton*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON_CLEAR))->EnableWindow(TRUE);
	}
	else
	{
		this->SetWindowText(DEMO_NAME + "*�豸�ѶϿ�");
		((CButton*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_CLEAR))->EnableWindow(FALSE);
	}

	//ע�����¼�
	HDEVNOTIFY hDevNotify;
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
	ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
	NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	for (int i = 0; i<sizeof(GUID_DEVINTERFACE_LIST) / sizeof(GUID); i++)
	{
		NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_LIST[i];
		hDevNotify = RegisterDeviceNotification(this->GetSafeHwnd(), &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
		if (!hDevNotify)
		{
			return FALSE;
		}
	}

	/************************************/

	m_strCurPath = CFPGA_GET_RNGDlg::ReturnPath();
	m_edit_rng_path.SetWindowText(m_strCurPath);

	SetDlgItemText(IDC_EDIT_ASC,"random_file_ASC");
	SetDlgItemText(IDC_EDIT_SEC, "random");
	SetDlgItemText(IDC_EDIT_DLY_SET, "5000"); 

	((CButton*)GetDlgItem(IDC_CHECK_CLOSEHEX))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_SMALL))->SetCheck(TRUE);

	Check_CLOSEHEX = 1;
	Check_SEC = 1;

	runstate = FALSE;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

LRESULT CFPGA_GET_RNGDlg::OnDeviceChange(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case DBT_DEVICEARRIVAL:
	{
		if (USBScanDev(1) == 1 && USBOpenDev(0) == 0)
		{
			((CButton*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_BUTTON_CLEAR))->EnableWindow(TRUE);
			this->SetWindowText(DEMO_NAME + "*�豸������");
			break;
		}
	}
	case DBT_DEVICEREMOVECOMPLETE:
	{
		if (USBScanDev(1) == 0 || USBOpenDev(0) == 1)
		{
			DWORD code;
			if (GetExitCodeThread(hThread1, &code) || code == STILL_ACTIVE)
			{
				TerminateThread(hThread1,0);
				CloseHandle(hThread1);
			}

			USBCloseDev(0);
			((CButton*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_BUTTON_CLEAR))->EnableWindow(FALSE);
			this->SetWindowText(DEMO_NAME + "�豸�ѶϿ�");
			break;
		}
	}
	}
	return LRESULT();
}

void CFPGA_GET_RNGDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFPGA_GET_RNGDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFPGA_GET_RNGDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString CFPGA_GET_RNGDlg::ReturnPath()
{
	CString    sPath;
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	sPath.ReleaseBuffer();
	int    nPos;
	nPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(nPos);
	return    sPath;
}


int m_persent=0;

void CFPGA_GET_RNGDlg::OnBnClickedButtonStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UpdateData(TRUE);

	if (m_nstrASC == m_nstrSEC)
	{
		AfxMessageBox("ע�⣡�ļ����ظ���");
		return;
	}

	if (runstate == FALSE)
	{
		CString now_path;
		now_path = m_strCurPath + "\\" + file_name_ASC;
		if (PathFileExists(now_path)) DeleteFile(now_path);

		now_path = m_strCurPath + "\\" + file_name_HEX;
		if (PathFileExists(now_path)) DeleteFile(now_path);

		DWORD ThreadID;
		DWORD code;

		//UpdateData(FALSE);

		((CEdit*)GetDlgItem(IDC_EDIT_ASC))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_SEC))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_SAVEPATH))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_SMALL))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_CLEAR))->EnableWindow(TRUE);

		//this->SetWindowText(DEMO_NAME + "--�������ȡ��...");

		if (!GetExitCodeThread(hThread1, &code) || (code != STILL_ACTIVE))
		{
			hThread1 = CreateThread(NULL, 0, ThreadFun, this, 0, &ThreadID);//��������ʼ�߳�
		}
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_EDIT_ASC))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_SEC))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_SAVEPATH))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_SMALL))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_CLEAR))->EnableWindow(TRUE);

		//����Ƿ�������

		if (Check_SEC == 1)
		{
			if (m_nstrASC != str_mASC || m_nstrSEC!= str_mSEC)
			{
				bin_KB = 0;
				ifile = 0;
				iSEC = 0;
			}
		}
		else
		{
			if (m_nstrASC != str_mASC)
			{
				bin_KB = 0;
				ifile = 0;
				iSEC = 0;
			}
		}

		//����ļ��Ƿ����
		CString now_path_asc, now_path_hex;
		now_path_asc = m_strCurPath + "\\" + file_name_ASC;
		now_path_hex = m_strCurPath + "\\" + file_name_HEX;

		if (Check_CLOSEHEX == 0)
		{
			if (!PathFileExists(now_path_asc) || !PathFileExists(now_path_hex))
			{
				DeleteFile(now_path_asc);
				DeleteFile(now_path_hex);
				bin_KB = 0;
				ifile = 0;
				iSEC = 0;
			}
		}
		else
		{
			if (!PathFileExists(now_path_asc))
			{
				DeleteFile(now_path_asc);
				bin_KB = 0;
				ifile = 0;
				iSEC = 0;
			}
		}

		//this->SetWindowText(DEMO_NAME + "--�������ȡ��...");

		runstate = FALSE;
		ResumeThread(hThread1);
	}
}

#define RECVBUF_LEN 64*1024

unsigned char recv_buffer[RECVBUF_LEN] = { 0 };
unsigned char SECrecvbuf[RECVBUF_LEN * 2] = { 0 };


DWORD WINAPI ThreadFun(LPVOID pArg)						//�߳���ں���
{
	CFPGA_GET_RNGDlg *cDlg = (CFPGA_GET_RNGDlg *)pArg;
	CEdit *pEdit = (CEdit *)pArg;

	while(1)
	{
		int irecv = 0;
		int mSEC = 0;
		CString Data = _T(""),str_trs = _T(""),str_SECfile = _T(""), str_ifile = _T("");

		str_hex = _T("");

		str_ifile.Format("%d", ifile);
		strSECpath = "random_data" + str_ifile;

		if (Check_SEC == 1) cDlg->m_SECPath = cDlg->m_strCurPath + "\\" + strSECpath;

		while (bin_KB < 128 * 1025)
		{
			//���ݽ���
			while (1)
			{
				memset(recv_buffer, 0, RECVBUF_LEN);

				if (USBBulkReadData(0, EP1_IN, (char*)recv_buffer, RECVBUF_LEN, cDlg->m_int_delay) != RECVBUF_LEN)
				{
					memset(recv_buffer, 0, RECVBUF_LEN);
					if (cDlg->MessageBox("�������ȡ����ͣ��", "����", MB_RETRYCANCEL) == IDRETRY) continue;
					else
					{
						cDlg->GetDlgItem(IDC_CHECK_SMALL)->EnableWindow(TRUE);
						cDlg->GetDlgItem(IDC_BUTTON_SAVEPATH)->EnableWindow(TRUE);
						cDlg->GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
						cDlg->GetDlgItem(IDC_EDIT_ASC)->EnableWindow(TRUE);
						cDlg->GetDlgItem(IDC_EDIT_SEC)->EnableWindow(TRUE);

						cDlg->runstate = FALSE;

						ifile += 1;
						bin_KB = 0;
						iSEC = 0;
						return true;
					}
				}
				else break;
			}

			while (irecv < RECVBUF_LEN)//������ת��Ϊ�ַ���
			{
				str_trs = transResult(&recv_buffer[irecv], 64);

				str_hex += str_trs;

				irecv += 64;
			}

			//��������

			file_name_ASC = cDlg->m_nstrASC + str_ifile + ".txt";
			file_name_HEX = "random_file_HEX" + str_ifile + ".txt";//HEX��ʾ����


			//������ʾ

			Data.Format("%.2f", bin_KB / 1024);//��������ʾ
			Data += "MB";
			pEdit->GetDlgItem(IDC_STATIC_DATA)->SetWindowText("ASC�����Ѵ棺" + Data + ",��ǰ�ĵ�:(" + file_name_ASC + ")");
			cDlg->savebuffer();//�������ݺ���
			bin_KB += 64;

			//�����ͼ챣�淽��

			if (Check_SEC == 1)
			{
				memcpy(&SECrecvbuf[RECVBUF_LEN*(mSEC / 64)], recv_buffer, RECVBUF_LEN);
				mSEC += 64;

				if (mSEC == 128)
				{
					str_SECfile.Format("%d", iSEC);
					SEC_File = cDlg->m_nstrSEC + str_SECfile;
					pEdit->GetDlgItem(IDC_STATIC_SEC)->SetWindowText("�����ͼ췽�����ڱ��棺" + SEC_File);

					cDlg->SaveSEC();//�������ݺ���
					iSEC++;
					mSEC = 0;
				}
			}
			//��ջ���
			irecv = 0;
			str_hex = _T("");

		}

		if (Check_AUTOSAVE == 1)
		{
			bin_KB = 0;
			ifile += 1;
			iSEC = 0;
			continue;
		}
		else
		{
			pEdit->GetDlgItem(IDC_STATIC_SEC)->SetWindowText("�����ͼ�������ɼ����");
			pEdit->GetDlgItem(IDC_STATIC_DATA)->SetWindowText("����ɲɼ����ļ���С" + Data);
			cDlg->GetDlgItem(IDC_CHECK_SMALL)->EnableWindow(TRUE);
			cDlg->GetDlgItem(IDC_BUTTON_SAVEPATH)->EnableWindow(TRUE);
			cDlg->GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			cDlg->GetDlgItem(IDC_EDIT_ASC)->EnableWindow(FALSE);
			cDlg->GetDlgItem(IDC_EDIT_SEC)->EnableWindow(FALSE);

			ifile += 1;
			bin_KB = 0;
			iSEC = 0;
			return true;
		}
	}
	return true;
}


void CFPGA_GET_RNGDlg::OnBnClickedButtonSavepath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	static char displayname[MAX_PATH];
	static char path[MAX_PATH];
    LPITEMIDLIST pidlBrowse;    // PIDL selected by user 
	BROWSEINFO bi;
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = displayname;
	bi.lpszTitle = "��ѡ��Ҫ����������ݵ��ļ���";
	bi.ulFlags = BIF_EDITBOX ;
	bi.lpfn = NULL;
	pidlBrowse=SHBrowseForFolder( &bi);
	if(pidlBrowse!=NULL)
	{
		SHGetPathFromIDList(pidlBrowse,path);
	}
	CString str=path;  //�õ�·��
	if(str.IsEmpty()) return;  //���û��ѡ�񣬾ͷ���
	m_strCurPath=str;	//����·���༭���Ӧ����
	m_SECPath = m_strCurPath;
	m_edit_rng_path.SetWindowText(m_strCurPath);

}


void CFPGA_GET_RNGDlg::OnBnClickedButtonClear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	
	((CEdit*)GetDlgItem(IDC_EDIT_ASC))->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_SEC))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_SMALL))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_BUTTON_CLEAR))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_SAVEPATH))->EnableWindow(TRUE);

	//this->SetWindowText(DEMO_NAME + "--����ͣ��ȡ�����");

	GetDlgItemText(IDC_EDIT_ASC, str_mASC);
	GetDlgItemText(IDC_EDIT_SEC, str_mSEC);

	runstate = TRUE;
	SuspendThread(hThread1);
}


void CFPGA_GET_RNGDlg::OnBnClickedCheckAutosave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(((CButton*)GetDlgItem(IDC_CHECK_AUTOSAVE))->GetCheck()) Check_AUTOSAVE = 1;
	else
	{
		Check_AUTOSAVE = 0;
	}
}


void CFPGA_GET_RNGDlg::savebuffer(void)
{
	if(Check_CLOSEHEX == 1)
	{
		CFile m_rFile;
		LPCSTR	lpszPath =m_strCurPath;// "����·��";
		SetCurrentDirectory( lpszPath );

		if(!m_rFile.Open(file_name_ASC,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite ))
		{
			AfxMessageBox( "������¼�ļ�ʧ�ܣ�");
			return;
		}
		m_rFile.SeekToEnd();
		m_rFile.Write((LPCTSTR)recv_buffer, RECVBUF_LEN);

		m_rFile.Close();
	}
	else
	{

		CFile m_rFile;
		LPCSTR	lpszPath =m_strCurPath;// "����·��";
		SetCurrentDirectory( lpszPath );

		CFile m_rFile_hex;
		LPCSTR	lpszPath_hex =m_strCurPath;// "����·��";
		SetCurrentDirectory( lpszPath_hex );
		if(!m_rFile_hex.Open(file_name_HEX,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite )) 
		{
			AfxMessageBox( "������¼�ļ�ʧ�ܣ�");
			return;
		}
		m_rFile_hex.SeekToEnd();
		m_rFile_hex.Write((LPCTSTR)str_hex,str_hex.GetLength());

		if(!m_rFile.Open(file_name_ASC,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite ))
		{
			AfxMessageBox( "������¼�ļ�ʧ�ܣ�");
			return;
		}
		m_rFile.SeekToEnd();
		m_rFile.Write((LPCTSTR)recv_buffer, RECVBUF_LEN);

		m_rFile.Close();
		m_rFile_hex.Close();
	}
}


void CFPGA_GET_RNGDlg::OnBnClickedCheckClosehex()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(((CButton*)GetDlgItem(IDC_CHECK_CLOSEHEX))->GetCheck()) Check_CLOSEHEX = 1;
	else Check_CLOSEHEX = 0;
}

void CFPGA_GET_RNGDlg::OnBnClickedCheckSmall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (((CButton*)GetDlgItem(IDC_CHECK_SMALL))->GetCheck())
	{
		GetDlgItem(IDC_STATIC_SEC)->SetWindowText("�ѿ��������ͼ챣�淽��");
		Check_SEC = 1;
	}
	else
	{
		GetDlgItem(IDC_STATIC_SEC)->SetWindowText("δ���������ͼ챣�淽��");
		Check_SEC = 0;
	}
}


void CFPGA_GET_RNGDlg::SaveSEC()
{

	if (!PathFileExists(m_SECPath))//�ļ��в������򴴽�
	{
		CreateDirectory(m_SECPath, NULL);
	}

	CString now_path;
	now_path = m_SECPath + "\\" + SEC_File;
	if (PathFileExists(now_path)) DeleteFile(now_path);

	CFile m_rFile;
	LPCSTR	lpszPath = m_SECPath;// "����·��";
	SetCurrentDirectory(lpszPath);

	if (!m_rFile.Open(SEC_File, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		AfxMessageBox("������¼�ļ�ʧ�ܣ�");
		return;
	}
	m_rFile.Write((LPCTSTR)SECrecvbuf, RECVBUF_LEN*2);

	m_rFile.Close();

	
}