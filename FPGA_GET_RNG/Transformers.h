// Transformers.h : Transformers DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTransformersApp
// �йش���ʵ�ֵ���Ϣ������� Transformers.cpp
//

class CTransformersApp : public CWinApp
{
public:
	CTransformersApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

/****************************************************************
�������ܣ�10����---->16����
����ת����unsigned int(Dec)->CString(Hex)
����ȡֵ��Χ��
          HEX��0x00000000-0xFFFFFFFF
		  DEC��0-4294967295
*****************************************************************/
CString __stdcall DecimalToHex(int iDecimal);//0-4294967295



/****************************************************************
�������ܣ�2����---->10����
����ת����CString(Bin)->CString(Dec)
����ȡֵ��Χ��
          BIN��0-111111111111111111111111111111111
		  DEC��0-4294967295
*****************************************************************/
CString __stdcall BinToDec(CString strBin);



/****************************************************************
�������ܣ�2����---->16����
����ת����CString(Bin)->CString(Hex)
����ȡֵ��Χ��
          BIN��0-111111111111111111111111111111111
		  HEX��0-4294967295
*****************************************************************/
CString __stdcall BinToHex(CString strBin);



/****************************************************************
�������ܣ�16����---->10����
����ת����CString(Hex)->CString(Dec)
����ȡֵ��Χ��
          HEX��0x00000000-0xFFFFFFFF
		  DEC��0-4294967295
*****************************************************************/
CString __stdcall HexToDec(CString strHex);



/****************************************************************
�������ܣ�16����---->2����
����ת����CString(Hex)->CString(Bin)
����ȡֵ��Χ��
          HEX��0x00000000-0xFFFFFFFF
		  BIN��0-111111111111111111111111111111111
*****************************************************************/
CString __stdcall HexToBin(CString strHex);



/****************************************************************
�������ܣ�10����---->2����
����ת����CString(Dec)->CString(Bin)
����ȡֵ��Χ��
          BIN��0-111111111111111111111111111111111
		  DEC��0-4294967295
*****************************************************************/
CString __stdcall DecToBin(CString strDec);



int __stdcall hexToint(unsigned char sHex[],int *iInt);
unsigned char* __stdcall intTohex(int iData,unsigned char* sHex);


CString __stdcall transResult(unsigned char* Response_buffer,int buffer_len);//��ʮ����������ת��Ϊ���ݼ���ո��CString����

int __stdcall transCStringbuffer(CString strbuffer,unsigned char *ucbuffer);//����ȡ��CString����ת��Ϊunsigned charʮ����������