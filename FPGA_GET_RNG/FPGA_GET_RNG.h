
// FPGA_GET_RNG.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFPGA_GET_RNGApp:
// �йش����ʵ�֣������ FPGA_GET_RNG.cpp
//

class CFPGA_GET_RNGApp : public CWinApp
{
public:
	CFPGA_GET_RNGApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFPGA_GET_RNGApp theApp;