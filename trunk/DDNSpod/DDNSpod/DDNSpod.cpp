// DDNSpod.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "DDNSpod.h"
#include "DDNSpodDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDDNSpodApp

BEGIN_MESSAGE_MAP(CDDNSpodApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDDNSpodApp ����

CDDNSpodApp::CDDNSpodApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDDNSpodApp ����

CDDNSpodApp theApp;


// CDDNSpodApp ��ʼ��

BOOL CDDNSpodApp::InitInstance()
{
	m_hMutex = CreateMutex(NULL, TRUE, _T("DDNSpod")); 
	// ����Ƿ��Ѿ�����Mutex
	// ����Ѿ�����������ֹ���̵�����
	if ((m_hMutex != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS)) 
	{
		ReleaseMutex(m_hMutex);
		MessageBox(NULL, _T("DDNSpod�Ѿ�����"), _T("����"), MB_OK);
		return FALSE;
	}
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("DDNSpod"));

	CDDNSpodDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//  �Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int CDDNSpodApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (m_hMutex != NULL)
	{
		ReleaseMutex(m_hMutex);
		CloseHandle(m_hMutex);
	}

	return CWinApp::ExitInstance();
}
