// DDNSpod.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDDNSpodApp:
// �йش����ʵ�֣������ DDNSpod.cpp
//

class CDDNSpodApp : public CWinApp
{
public:
	CDDNSpodApp();

protected:
	HANDLE m_hMutex; //ֻ����һ��ʵ���Ļ������

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CDDNSpodApp theApp;