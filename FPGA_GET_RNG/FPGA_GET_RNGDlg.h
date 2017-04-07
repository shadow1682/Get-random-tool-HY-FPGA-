
// FPGA_GET_RNGDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "dbt.h"	

// CFPGA_GET_RNGDlg �Ի���
class CFPGA_GET_RNGDlg : public CDialog
{
// ����
public:
	CFPGA_GET_RNGDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FPGA_GET_RNG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	HANDLE hThread1;										//�߳�1�߳̾��
	HANDLE hThread2;											//�߳�2�߳̾��
	HANDLE hThread3;											//�߳�3�߳̾��

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	BOOL runstate;

	CString m_strCurPath;
	CString m_SECPath;

	LRESULT OnDeviceChange(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonSavepath();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedCheckAutosave();
	void savebuffer(void);
	afx_msg void OnBnClickedCheckClosehex();
//	afx_msg void OnMenuDevlink();
//	afx_msg void OnMenuClose();
	afx_msg void OnBnClickedCheckSmall();
	void SaveSEC();
	CString m_nstrASC;
	CString m_nstrSEC;
	CString ReturnPath();
//	CRichEditCtrl m_richedit_rng_data;
	CEdit m_edit_rng_path;
//	afx_msg void OnBnClickedButtonEditClear();
	int m_int_delay;
//	CButton m_ctrl_auto_clr;
//	CEdit m_edit_ctrl_rng_data;
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
