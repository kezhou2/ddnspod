//DNSpod API�ķ�װ��ʵ��

#pragma once

#include <afxinet.h>
#include "stdafx.h"
#include "tinyxml.h"
#include "DDNSpodAPI.h"

//dnspod֧��HTTPS 
//HTTPSͨ�Ųο�http://support.microsoft.com/kb/168151/

/*
����DNSpod�ṩ��APIҪ��Ĳ�����domain id����������DNSpod�е����
��˱������Ȼ��������Ӧ��domain id
������Dnspod����Ψһ�ԣ�����domain id���������������ǲ��㹻��
record_idͬ��Ҳ�Ǽ�¼��Ӧ��id��Ҳ��Ҫ���ҷ��صļ�¼�б���ܵõ�
*/

CDNSpodAPI::CDNSpodAPI()
{
	ServerURL=_T("www.dnspod.com");
	nPort=80;
	login_email="";
	login_password="";
	bLoginSuccess=FALSE;
	return_format=_T("xml");
	APIVersion="";
	tmp = new TiXmlDocument();
}

CDNSpodAPI::CDNSpodAPI(CString URL,WORD port,CString account, CString password, CString format)
{
	ServerURL=URL;
	nPort=port;
	login_email=account;
	login_password=password;
	bLoginSuccess=FALSE;
	return_format=_T("xml"); //��֧��xml
	APIVersion="";
	tmp = new TiXmlDocument();
}

CDNSpodAPI::~CDNSpodAPI()
{
	delete tmp;
}

//�Ƿ�ƥ�����°汾API���ڼ����Ӧ�����ȵ���һ��GetAPIVersion()
BOOL CDNSpodAPI::APIVerMatch(CString version)
{
	if (APIVersion==version)
		return TRUE;
	else
		return FALSE;
}

/********��ȡAPI�汾��*******/
/*
URL: /API/Info.Version
����: POST
����:
login_email - �û��ʺ�
login_password - �û�����
format {json,xml} - ���ص����ݸ�ʽ��֧��json��xml.
��Ӧ����:
-1 ��½ʧ��
1 �ɹ�
*/
/***************************/
TiXmlDocument CDNSpodAPI::GetAPIVersion()
{
	CInternetSession session(_T("MySession"));
	CHttpConnection *pServer=NULL;
	CHttpFile *pFile=NULL;
	DWORD retcode;
	char szServerMsg[102400]; //�������ݻ�����
	DWORD dwFileSize=0;
	CString paraStr;
	paraStr=_T("/API/Info.Version");
	CString strFormData;
	strFormData=_T("login_email=");
	strFormData+=login_email;
	strFormData+=_T("&login_password=");
	strFormData+=login_password;
	strFormData+=_T("&format=");
	strFormData+=return_format;
	//unicode -> ansi
	CStringA strFormDataA(strFormData);
	//::AfxMessageBox(strFormData);

	CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded");
	try
	{
		pServer=session.GetHttpConnection(ServerURL,nPort,login_email,login_password);
		pFile=pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST,paraStr,NULL,1,NULL,NULL,INTERNET_FLAG_EXISTING_CONNECT);
		pFile->AddRequestHeaders(strHeaders);
		pFile->AddRequestHeaders(_T("User-Agent: DDNSpod 0.4"));
		pFile->SendRequest(strHeaders,(LPVOID)(LPCSTR)strFormDataA,strFormDataA.GetLength()); //���ͱ�����
		pFile->QueryInfoStatusCode(retcode);

		UINT nSize=pFile->Read(szServerMsg,1024);
		while (nSize>0)
		{
			dwFileSize +=nSize;
			nSize=pFile->Read(szServerMsg+dwFileSize,1024);
		}
		szServerMsg[dwFileSize]='\0';
		//::AfxMessageBox(CString(szServerMsg));
	}
	catch (CInternetException *e)
	{
	}
	pFile->Close();
	pServer->Close();
	delete pFile;
	delete pServer;

	TiXmlDocument *temp = new TiXmlDocument;
	//�������ؽ��
	temp->Parse(szServerMsg,NULL,TIXML_ENCODING_UTF8);
	if (temp->Error())
	{
		bLoginSuccess=FALSE;                           //login fail
		return *temp;                                  //
	}

	TiXmlHandle hRoot(temp);
	TiXmlElement *pElem;

	pElem=hRoot.FirstChild("dnspod").FirstChild("status").FirstChild("code").ToElement();
	if (!pElem)
	{
		bLoginSuccess=FALSE;                           //login fail
		return *temp;
	}
	if (atoi(pElem->GetText())!=1)
	{
		bLoginSuccess=FALSE;                           //login fail
		return *temp;
	}
	TiXmlElement *pAPIVersionNode;
	pAPIVersionNode=hRoot.FirstChild("dnspod").FirstChild("status").FirstChild("message").ToElement();
	if (!pAPIVersionNode)
	{
		bLoginSuccess=FALSE;
		return *temp;
	}

	APIVersion=CString(pAPIVersionNode->GetText());
	bLoginSuccess=TRUE;
	//temp->SaveFile("result.xml");
	return *temp;
}

/**********��������*********/
/*
URL: /API/Domain.Create
����: POST
����:
login_email - �û��ʺ�
login_password - �û�����
format {json,xml} - ���ص����ݸ�ʽ��֧��json��xml.
domain - ����, û�� WWW, �� dnspod.com
��Ӧ����:
-1 ��½ʧ��
1 �ɹ�
2 ֻ����POST����
3 δ֪����
4 ������Ч
5 �����Ѵ���
*/
/***************************/
TiXmlDocument CDNSpodAPI::DomainCreate(CString Domain)
{
	CInternetSession session(_T("MySession"));
	CHttpConnection *pServer=NULL;
	CHttpFile *pFile=NULL;
	DWORD retcode;
	char szServerMsg[102400]; //�������ݻ�����
	DWORD dwFileSize=0;
	CString paraStr;
	paraStr=_T("/API/Domain.Create");
	CString strFormData;
	strFormData=_T("login_email=");
	strFormData+=login_email;
	strFormData+=_T("&login_password=");
	strFormData+=login_password;
	strFormData+=_T("&format=");
	strFormData+=return_format;
	strFormData+=_T("&domain=");
	strFormData+=Domain;
	//unicode -> ansi
	CStringA strFormDataA(strFormData);
	//::AfxMessageBox(strFormData);

	CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded");
	try
	{
		pServer=session.GetHttpConnection(ServerURL,nPort,login_email,login_password);
		pFile=pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST,paraStr,NULL,1,NULL,NULL,INTERNET_FLAG_EXISTING_CONNECT);
		pFile->AddRequestHeaders(strHeaders);
		pFile->AddRequestHeaders(_T("User-Agent: DDNSpod 0.4"));
		pFile->SendRequest(strHeaders,(LPVOID)(LPCSTR)strFormDataA,strFormDataA.GetLength()); //���ͱ�����
		pFile->QueryInfoStatusCode(retcode);
		
		UINT nSize=pFile->Read(szServerMsg,1024);
		while (nSize>0)
		{
			dwFileSize +=nSize;
			nSize=pFile->Read(szServerMsg+dwFileSize,1024);
		}
		szServerMsg[dwFileSize]='\0';
		//::AfxMessageBox(CString(szServerMsg));
	}
	catch (CInternetException *e)
	{
	}
	pFile->Close();
	pServer->Close();
	delete pFile;
	delete pServer;

	TiXmlDocument *temp = new TiXmlDocument;
	//�������ؽ��
	temp->Parse(szServerMsg,NULL,TIXML_ENCODING_UTF8);
	if (temp->Error())
	{
		bLoginSuccess=FALSE;                           //login fail
		return *temp;                                  //
	}

	TiXmlHandle hRoot(temp);
	TiXmlElement *pElem;

	pElem=hRoot.FirstChild("dnspod").FirstChild("status").FirstChild("code").ToElement();
	if (!pElem)
	{
		bLoginSuccess=FALSE;                           //login fail
		return *temp;
	}
	if (atoi(pElem->GetText())!=1)
	{
		bLoginSuccess=FALSE;                           //����ʧ��
		return *temp;
	}

	bLoginSuccess=TRUE;
	//temp->SaveFile("result.xml");
	return *temp;
}

/********���������б�********/
/*
URL: /API/Domain.List
����: POST
����:
login_email - �û��ʺ�
login_password - �û�����
format {json,xml} - ���ص����ݸ�ʽ��֧��json��xml.
grade - �û����࣬����ΪFree��Express��Extra��Ultra���� //ע�����跢�͸ò���
��Ӧ����:
-1 ��½ʧ��
1 �ɹ�
2 ֻ����POST����
3 δ֪����
4 û������
*/
/***************************/
TiXmlDocument CDNSpodAPI::DomainList()
{
	CInternetSession session(_T("MySession"));
	CHttpConnection *pServer=NULL;
	CHttpFile *pFile=NULL;
	DWORD retcode;
	char szServerMsg[102400]; //�������ݻ�����
	DWORD dwFileSize=0;
	CString paraStr;
	paraStr=_T("/API/Domain.List");
	CString strFormData;
	strFormData=_T("login_email=");
	strFormData+=login_email;
	strFormData+=_T("&login_password=");
	strFormData+=login_password;
	strFormData+=_T("&format=");
	strFormData+=return_format;
	//unicode -> ansi
	CStringA strFormDataA(strFormData);
	//::AfxMessageBox(strFormData);

	CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded");
	try
	{
		pServer=session.GetHttpConnection(ServerURL,nPort,login_email,login_password);
		pFile=pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST,paraStr,NULL,1,NULL,NULL,INTERNET_FLAG_EXISTING_CONNECT);
		pFile->AddRequestHeaders(strHeaders);
		pFile->AddRequestHeaders(_T("User-Agent: DDNSpod 0.4"));
		pFile->SendRequest(strHeaders,(LPVOID)(LPCSTR)strFormDataA,strFormDataA.GetLength()); //���ͱ�����
		pFile->QueryInfoStatusCode(retcode);
		
		UINT nSize=pFile->Read(szServerMsg,1024);
		while (nSize>0)
		{
			dwFileSize +=nSize;
			nSize=pFile->Read(szServerMsg+dwFileSize,1024);
		}
		szServerMsg[dwFileSize]='\0';
		//::AfxMessageBox(CString(szServerMsg));
	}
	catch (CInternetException *e)
	{
	}
	pFile->Close();
	pServer->Close();
	delete pFile;
	delete pServer;

	TiXmlDocument *temp = new TiXmlDocument;
	//�������ؽ��
	temp->Parse(szServerMsg,NULL,TIXML_ENCODING_UTF8);
	if (temp->Error())
	{
		bLoginSuccess=FALSE;                           //login fail
		return *tmp;                                   //null xml document
	}
	//temp->SaveFile("domain.xml");
	TiXmlHandle hRoot(temp);
	TiXmlElement *pElem;
	/*
	TiXmlHandle hXmlHandle(0);
	pElem=hRoot.FirstChildElement("dnspod").Element(); //<dnspod>
	if (!pElem)
	{
		bLoginSuccess=FALSE;                           //login fail
		return *temp;
	}
	hXmlHandle=TiXmlHandle(pElem);
	pElem=hXmlHandle.FirstChild("status").Element();   //<status>
	if (!pElem)
	{
		bLoginSuccess=FALSE;                           //login fail
		return *temp;
	}
	hXmlHandle=TiXmlHandle(pElem);
	pElem=hXmlHandle.FirstChild("code").Element();     //<code>
	if (!pElem)
	{
		bLoginSuccess=FALSE;                           //login fail
		return *temp;
	}
	if (atoi(pElem->GetText())!=1)
	{
		bLoginSuccess=FALSE;                           //login fail
		return *temp;
	}
	*/
	pElem=hRoot.FirstChild("dnspod").FirstChild("status").FirstChild("code").ToElement();
	if (!pElem)
	{
		bLoginSuccess=FALSE;                           //login fail
		return *temp;
	}
	if (atoi(pElem->GetText())!=1)
	{
		bLoginSuccess=FALSE;                           //login fail
		return *temp;
	}

	bLoginSuccess=TRUE;
	return *temp;
}

/**********ɾ������*********/
/*
URL: /API/Domain.Remove
����: POST
����:
login_email - �û��ʺ�
login_password - �û�����
format {json,xml} - ���ص����ݸ�ʽ��֧��json��xml.
domain_id - ����ID, �� 12345
��Ӧ����:
-1 ��½ʧ��
1 �ɹ�
2 ֻ����POST����
3 δ֪����
4 ����ID����
5 ����������
6 ������������
 */
/***************************/
TiXmlDocument CDNSpodAPI::DomainRemove(int DomainID)
{
	CInternetSession session(_T("MySession"));
	CHttpConnection *pServer=NULL;
	CHttpFile *pFile=NULL;
	DWORD retcode;
	char szServerMsg[102400]; //�������ݻ�����
	DWORD dwFileSize=0;
	CString paraStr;
	paraStr=_T("/API/Domain.Remove");
	CString strFormData;
	strFormData=_T("login_email=");
	strFormData+=login_email;
	strFormData+=_T("&login_password=");
	strFormData+=login_password;
	strFormData+=_T("&format=");
	strFormData+=return_format;
	strFormData+=_T("&domain_id=");
	CString domianid;
	domianid.Format(_T("%d"),DomainID);
	strFormData+=domianid;
	//unicode -> ansi
	CStringA strFormDataA(strFormData);
	//::AfxMessageBox(strFormData);

	CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded");
	try
	{
		pServer=session.GetHttpConnection(ServerURL,nPort,login_email,login_password);
		pFile=pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST,paraStr,NULL,1,NULL,NULL,INTERNET_FLAG_EXISTING_CONNECT);
		pFile->AddRequestHeaders(strHeaders);
		pFile->AddRequestHeaders(_T("User-Agent: DDNSpod 0.4"));
		pFile->SendRequest(strHeaders,(LPVOID)(LPCSTR)strFormDataA,strFormDataA.GetLength()); //���ͱ�����
		pFile->QueryInfoStatusCode(retcode);
		
		UINT nSize=pFile->Read(szServerMsg,1024);
		while (nSize>0)
		{
			dwFileSize +=nSize;
			nSize=pFile->Read(szServerMsg+dwFileSize,1024);
		}
		szServerMsg[dwFileSize]='\0';
		//::AfxMessageBox(CString(szServerMsg));
	}
	catch (CInternetException *e)
	{
	}
	pFile->Close();
	pServer->Close();
	delete pFile;
	delete pServer;

	TiXmlDocument *temp = new TiXmlDocument;
	//�������ؽ��
	temp->Parse(szServerMsg,NULL,TIXML_ENCODING_UTF8);
	if (temp->Error())
	{
		bLoginSuccess=FALSE;                           //login fail
		return *temp;                                  //
	}

	TiXmlHandle hRoot(temp);
	TiXmlElement *pElem;

	pElem=hRoot.FirstChild("dnspod").FirstChild("status").FirstChild("code").ToElement();
	if (!pElem)
	{
		bLoginSuccess=FALSE;                           //login fail
		return *temp;
	}
	if (atoi(pElem->GetText())!=1)
	{
		bLoginSuccess=FALSE;                           //ɾ��ʧ��
		return *temp;
	}

	bLoginSuccess=TRUE;
	//temp->SaveFile("result.xml");
	return *temp;
}

/********��������״̬*******/
/*
URL: /API/Domain.Status
����: POST
����:
login_email - �û��ʺ�
login_password - �û�����
format {json,xml} - ���ص����ݸ�ʽ��֧��json��xml.
domain_id - ����ID, �� 12345
status {enable,disable} - ״̬
��Ӧ����:
-1 ��½ʧ��
1 �ɹ�
2 ֻ����POST����
3 δ֪����
4 ����ID����
5 ����������
6 ������������
*/
/***************************/
TiXmlDocument CDNSpodAPI::DomainStatus(int DomainID,BOOL DomainEnabled)
{
	CInternetSession session(_T("MySession"));
	CHttpConnection *pServer=NULL;
	CHttpFile *pFile=NULL;
	DWORD retcode;
	char szServerMsg[102400]; //�������ݻ�����
	DWORD dwFileSize=0;
	CString paraStr;
	paraStr=_T("/API/Domain.Status");
	CString strFormData;
	strFormData=_T("login_email=");
	strFormData+=login_email;
	strFormData+=_T("&login_password=");
	strFormData+=login_password;
	strFormData+=_T("&format=");
	strFormData+=return_format;
	strFormData+=_T("&domain_id=");
	CString domianid;
	domianid.Format(_T("%d"),DomainID);
	strFormData+=domianid;
	strFormData+=_T("&status=");
	if (DomainEnabled)
		strFormData+=_T("enable");
	else
		strFormData+=_T("disable");
	//unicode -> ansi
	CStringA strFormDataA(strFormData);
	//::AfxMessageBox(strFormData);

	CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded");
	try
	{
		pServer=session.GetHttpConnection(ServerURL,nPort,login_email,login_password);
		pFile=pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST,paraStr,NULL,1,NULL,NULL,INTERNET_FLAG_EXISTING_CONNECT);
		pFile->AddRequestHeaders(strHeaders);
		pFile->AddRequestHeaders(_T("User-Agent: DDNSpod 0.4"));
		pFile->SendRequest(strHeaders,(LPVOID)(LPCSTR)strFormDataA,strFormDataA.GetLength()); //���ͱ�����
		pFile->QueryInfoStatusCode(retcode);
		
		UINT nSize=pFile->Read(szServerMsg,1024);
		while (nSize>0)
		{
			dwFileSize +=nSize;
			nSize=pFile->Read(szServerMsg+dwFileSize,1024);
		}
		szServerMsg[dwFileSize]='\0';
		//::AfxMessageBox(CString(szServerMsg));
	}
	catch (CInternetException *e)
	{
	}
	pFile->Close();
	pServer->Close();
	delete pFile;
	delete pServer;

	TiXmlDocument *temp = new TiXmlDocument;
	//�������ؽ��
	temp->Parse(szServerMsg,NULL,TIXML_ENCODING_UTF8);
	if (temp->Error())
	{
		bLoginSuccess=FALSE;                           //login fail
		return *temp;                                  //
	}

	TiXmlHandle hRoot(temp);
	TiXmlElement *pElem;

	pElem=hRoot.FirstChild("dnspod").FirstChild("status").FirstChild("code").ToElement();
	if (!pElem)
	{
		bLoginSuccess=FALSE;                           //login fail
		return *temp;
	}
	if (atoi(pElem->GetText())!=1)
	{
		bLoginSuccess=FALSE;                           //����״̬����ʧ��
		return *temp;
	}

	bLoginSuccess=TRUE;
	//temp->SaveFile("result.xml");
	return *temp;
}

/**********������¼*********/
/*
URL: /API/Record.Create
����: POST
����:
 login_email - �û��ʺ�
 login_password - �û�����
 format {json,xml} - ���ص����ݸ�ʽ��֧��json��xml.
 domain_id - ����ID, �� 12345
 sub_domain - ������¼, �� www
 record_type {'A','CNAME','MX','URL','NS','TXT','AAAA'} - ��¼����
 record_line {'default', 'tel', 'cnc', 'edu', 'cmc', 'foreign'} - ��¼��·����Ϊ��Ĭ�ϡ����š���ͨ�����������ƶ������⣨cmc/foreign����VIP��Ч��
 value - ��¼ֵ, �� IP:200.200.200.200, CNAME: cname.dnspod.com., MX: mail.dnspod.com.
 mx {1-20} - MX���ȼ�, ����¼������ MX ʱ��Ч����Χ1-20
 ttl {1-604800} - TTL����Χ1-604800
��Ӧ����:
 -1 ��½ʧ��
 1 �ɹ�
 2 ֻ����POST����
 3 δ֪����
 4 ����ID����
 5 ����������
 6 ������������
 7 ȱ�ٲ������߲�������
 8 ������¼����
 9 ��¼ֵ����
 10 ����Ϊ@��¼����NS����
 11 TTL ������� 1
 12 MX ������� 1
 12 ��¼�ظ����ߺ� CNAME ��¼��ͻ
 */
/***************************/
TiXmlDocument CDNSpodAPI::RecordCreate(int DomainID,CString SubDomain,CString record_type,CString record_line,CString RecordValue,int MX,int TTL)
{
	if (!bLoginSuccess)
		return *tmp;

	CInternetSession session(_T("MySession"));
	CHttpConnection *pServer=NULL;
	CHttpFile *pFile=NULL;
	DWORD retcode;
	char szServerMsg[102400]; //�������ݻ�����
	DWORD dwFileSize=0;
	CString paraStr;
	paraStr=_T("/API/Record.Create");
	CString strFormData;
	strFormData=_T("login_email=");
	strFormData+=login_email;
	strFormData+=_T("&login_password=");
	strFormData+=login_password;
	strFormData+=_T("&format=");
	strFormData+=return_format;
	strFormData+=_T("&domain_id=");
	CString tmpStr;
	tmpStr.Format(_T("%d"),DomainID);
	strFormData+=tmpStr;
	strFormData+=_T("&sub_domain=");
	strFormData+=SubDomain;
	strFormData+=_T("&record_type=");
	strFormData+=record_type;
	strFormData+=_T("&record_line=");
	strFormData+=record_line;
	strFormData+=_T("&value=");
	strFormData+=RecordValue;
	strFormData+=_T("&mx=");
	tmpStr.Format(_T("%d"),MX);
	strFormData+=tmpStr;
	strFormData+=_T("&ttl=");
	tmpStr.Format(_T("%d"),TTL);
	strFormData+=tmpStr;
	//unicode -> ansi
	CStringA strFormDataA(strFormData);
	//::AfxMessageBox(strFormData);

	CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded");
	try
	{
		pServer=session.GetHttpConnection(ServerURL,nPort,login_email,login_password);
		pFile=pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST,paraStr,NULL,1,NULL,NULL,INTERNET_FLAG_EXISTING_CONNECT);
		pFile->AddRequestHeaders(strHeaders);
		pFile->AddRequestHeaders(_T("User-Agent: DDNSpod 0.4"));
		pFile->SendRequest(strHeaders,(LPVOID)(LPCSTR)strFormDataA,strFormDataA.GetLength()); //���ͱ�����
		pFile->QueryInfoStatusCode(retcode);
		
		UINT nSize=pFile->Read(szServerMsg,1024);
		while (nSize>0)
		{
			dwFileSize +=nSize;
			nSize=pFile->Read(szServerMsg+dwFileSize,1024);
		}
		szServerMsg[dwFileSize]='\0';
		//::AfxMessageBox(CString(szServerMsg));
	}
	catch (CInternetException *e)
	{
	}
	pFile->Close();
	pServer->Close();
	delete pFile;
	delete pServer;

	TiXmlDocument *temp = new TiXmlDocument;
	//�������ؽ��
	temp->Parse(szServerMsg,NULL,TIXML_ENCODING_UTF8);
	if (temp->Error())
	{
		bLoginSuccess=FALSE;                           //login fail
		//return *temp;                                //
	}
	
	return *temp;
}

/********���������б�*******/
/*
URL: /API/Record.List
����: POST
����:
login_email - �û��ʺ�
login_password - �û�����
format {json,xml} - ���ص����ݸ�ʽ��֧��json��xml.
domain_id - ����ID, �� 12345
��Ӧ����:
-1 ��½ʧ��
1 �ɹ�
2 ֻ����POST����
3 δ֪����
4 ����ID����
5 ����������
6 ������������
7 û�м�¼
 */
/***************************/
TiXmlDocument CDNSpodAPI::RecordList(int DomainID)
{
	if (!bLoginSuccess)
		return *tmp;

	CInternetSession session(_T("MySession"));
	CHttpConnection *pServer=NULL;
	CHttpFile *pFile=NULL;
	DWORD retcode;
	char szServerMsg[102400]; //�������ݻ�����
	DWORD dwFileSize=0;
	CString paraStr;
	paraStr=_T("/API/Record.List");
	CString strFormData;
	strFormData=_T("login_email=");
	strFormData+=login_email;
	strFormData+=_T("&login_password=");
	strFormData+=login_password;
	strFormData+=_T("&format=");
	strFormData+=return_format;
	strFormData+=_T("&domain_id=");
	CString domainid;
	domainid.Format(_T("%d"),DomainID);
	strFormData+=domainid;
	//unicode -> ansi
	CStringA strFormDataA(strFormData);
	//::AfxMessageBox(strFormData);

	CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded");
	try
	{
		pServer=session.GetHttpConnection(ServerURL,nPort,login_email,login_password);
		pFile=pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST,paraStr,NULL,1,NULL,NULL,INTERNET_FLAG_EXISTING_CONNECT);
		pFile->AddRequestHeaders(strHeaders);
		pFile->AddRequestHeaders(_T("User-Agent: DDNSpod 0.4"));
		pFile->SendRequest(strHeaders,(LPVOID)(LPCSTR)strFormDataA,strFormDataA.GetLength()); //���ͱ�����
		pFile->QueryInfoStatusCode(retcode);
		
		UINT nSize=pFile->Read(szServerMsg,1024);
		while (nSize>0)
		{
			dwFileSize +=nSize;
			nSize=pFile->Read(szServerMsg+dwFileSize,1024);
		}
		szServerMsg[dwFileSize]='\0';
		//::AfxMessageBox(CString(szServerMsg));
		/*
		CFile myfile;
		myfile.Open(_T("record.xml"),CFile::modeCreate|CFile::modeWrite);
		myfile.Write(szServerMsg,dwFileSize);
		myfile.Close();
		*/
	}
	catch (CInternetException *e)
	{
	}
	pFile->Close();
	pServer->Close();
	delete pFile;
	delete pServer;

	TiXmlDocument *temp = new TiXmlDocument;
	//�������ؽ��
	temp->Parse(szServerMsg,NULL,TIXML_ENCODING_UTF8);
	if (temp->Error())
	{
		bLoginSuccess=FALSE;                           //login fail
		//return *temp;                                //
	}
	
	//temp->SaveFile("record.xml");
	return *temp;
}

/********�޸�����***********/
/*
URL: /API/Record.Modify
����: POST
����:
login_email - �û��ʺ�
login_password - �û�����
format {json,xml} - ���ص����ݸ�ʽ��֧��json��xml.
domain_id - ����ID, �� 12345
record_id - ��¼ID, �� 1234567
sub_domain - ������¼, �� www
record_type {'A','CNAME','MX','URL','NS','TXT','AAAA'} - ��¼����
record_line {'default', 'tel', 'cnc', 'edu', 'cmc', 'foreign'} - ��¼��·����Ϊ��Ĭ�ϡ����š���ͨ�����������ƶ������⣨cmc/foreign����VIP��Ч��
value - ��¼ֵ, �� IP:200.200.200.200, CNAME: cname.dnspod.com., MX: mail.dnspod.com.
mx {1-20} - MX���ȼ�, ����¼������ MX ʱ��Ч����Χ1-20
ttl {1-604800} - TTL����Χ1-604800
��Ӧ����:
-1 ��½ʧ��
1 �ɹ�
2 ֻ����POST����
3 δ֪����
4 ����ID����
5 ����������
6 ������������
7 ȱ�ٲ������߲�������
8 ������¼����
9 ��¼ֵ����
10 ����Ϊ@��¼����NS����
11 TTL ������� 1
12 MX ������� 1
12 ��¼�ظ����ߺ� CNAME ��¼��ͻ
*/
/***************************/
TiXmlDocument CDNSpodAPI::RecordModify(int DomainID,int RecordID,CString SubDomain,CString record_type,CString record_line,CString RecordValue,int MX, int TTL)
{
	if (!bLoginSuccess)
		return *tmp;

	CInternetSession session(_T("MySession"));
	CHttpConnection *pServer=NULL;
	CHttpFile *pFile=NULL;
	DWORD retcode;
	char szServerMsg[102400]; //�������ݻ�����
	DWORD dwFileSize=0;
	CString paraStr;
	paraStr=_T("/API/Record.Modify");
	CString strFormData;
	strFormData=_T("login_email=");
	strFormData+=login_email;
	strFormData+=_T("&login_password=");
	strFormData+=login_password;
	strFormData+=_T("&format=");
	strFormData+=return_format;
	strFormData+=_T("&domain_id=");
	CString tmpStr;
	tmpStr.Format(_T("%d"),DomainID);
	strFormData+=tmpStr;
	strFormData+=_T("&record_id=");
	tmpStr.Format(_T("%d"),RecordID);
	strFormData+=tmpStr;
	strFormData+=_T("&sub_domain=");
	strFormData+=SubDomain;
	strFormData+=_T("&record_type=");
	strFormData+=record_type;
	strFormData+=_T("&record_line=");
	strFormData+=record_line;
	strFormData+=_T("&value=");
	strFormData+=RecordValue;
	strFormData+=_T("&mx=");
	tmpStr.Format(_T("%d"),MX);
	strFormData+=tmpStr;
	strFormData+=_T("&ttl=");
	tmpStr.Format(_T("%d"),TTL);
	strFormData+=tmpStr;
	//unicode -> ansi
	CStringA strFormDataA(strFormData);
	//::AfxMessageBox(strFormData);

	CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded");
	try
	{
		pServer=session.GetHttpConnection(ServerURL,nPort,login_email,login_password);
		pFile=pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST,paraStr,NULL,1,NULL,NULL,INTERNET_FLAG_EXISTING_CONNECT);
		pFile->AddRequestHeaders(strHeaders);
		pFile->AddRequestHeaders(_T("User-Agent: DDNSpod 0.4"));
		pFile->SendRequest(strHeaders,(LPVOID)(LPCSTR)strFormDataA,strFormDataA.GetLength()); //���ͱ�����
		pFile->QueryInfoStatusCode(retcode);
		
		UINT nSize=pFile->Read(szServerMsg,1024);
		while (nSize>0)
		{
			dwFileSize +=nSize;
			nSize=pFile->Read(szServerMsg+dwFileSize,1024);
		}
		szServerMsg[dwFileSize]='\0';
		//::AfxMessageBox(CString(szServerMsg));
	}
	catch (CInternetException *e)
	{
	}
	pFile->Close();
	pServer->Close();
	delete pFile;
	delete pServer;

	TiXmlDocument *temp = new TiXmlDocument;
	//�������ؽ��
	temp->Parse(szServerMsg,NULL,TIXML_ENCODING_UTF8);
	if (temp->Error())
	{
		bLoginSuccess=FALSE;                           //login fail
		//return *temp;                                //
	}
	
	return *temp;
}

/********ɾ����¼***********/
/*
URL: /API/Record.Remove
����: POST
����:
login_email - �û��ʺ�
login_password - �û�����
format {json,xml} - ���ص����ݸ�ʽ��֧��json��xml.
domain_id - ����ID, �� 12345
record_id - ��¼ID, �� 1234567
��Ӧ����:
-1 ��½ʧ��
1 �ɹ�
2 ֻ����POST����
3 δ֪����
4 ȱ�ٲ������߲�������
5 ����������
6 ������������
7 ��¼������
*/
/***************************/
TiXmlDocument CDNSpodAPI::RecordRemove(int DomainID, int RecordID)
{
	if (!bLoginSuccess)
		return *tmp;

	CInternetSession session(_T("MySession"));
	CHttpConnection *pServer=NULL;
	CHttpFile *pFile=NULL;
	DWORD retcode;
	char szServerMsg[102400]; //�������ݻ�����
	DWORD dwFileSize=0;
	CString paraStr;
	paraStr=_T("/API/Record.Remove");
	CString strFormData;
	strFormData=_T("login_email=");
	strFormData+=login_email;
	strFormData+=_T("&login_password=");
	strFormData+=login_password;
	strFormData+=_T("&format=");
	strFormData+=return_format;
	strFormData+=_T("&domain_id=");
	CString id;
	id.Format(_T("%d"),DomainID);
	strFormData+=id;
	strFormData+=_T("&record_id=");
	id.Format(_T("%d"),RecordID);
	strFormData+=id;
	//unicode -> ansi
	CStringA strFormDataA(strFormData);
	//::AfxMessageBox(strFormData);

	CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded");
	try
	{
		pServer=session.GetHttpConnection(ServerURL,nPort,login_email,login_password);
		pFile=pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST,paraStr,NULL,1,NULL,NULL,INTERNET_FLAG_EXISTING_CONNECT);
		pFile->AddRequestHeaders(strHeaders);
		pFile->AddRequestHeaders(_T("User-Agent: DDNSpod 0.4"));
		pFile->SendRequest(strHeaders,(LPVOID)(LPCSTR)strFormDataA,strFormDataA.GetLength()); //���ͱ�����
		pFile->QueryInfoStatusCode(retcode);
		
		UINT nSize=pFile->Read(szServerMsg,1024);
		while (nSize>0)
		{
			dwFileSize +=nSize;
			nSize=pFile->Read(szServerMsg+dwFileSize,1024);
		}
		szServerMsg[dwFileSize]='\0';
		//::AfxMessageBox(CString(szServerMsg));
	}
	catch (CInternetException *e)
	{
	}
	pFile->Close();
	pServer->Close();
	delete pFile;
	delete pServer;

	TiXmlDocument *temp = new TiXmlDocument;
	//�������ؽ��
	temp->Parse(szServerMsg,NULL,TIXML_ENCODING_UTF8);
	if (temp->Error())
	{
		bLoginSuccess=FALSE;                           //login fail
		return *temp;                                  //
	}

	TiXmlHandle hRoot(temp);
	TiXmlElement *pElem;

	pElem=hRoot.FirstChild("dnspod").FirstChild("status").FirstChild("code").ToElement();
	if (!pElem)
	{
		bLoginSuccess=FALSE;                           //login fail
		return *temp;
	}
	if (atoi(pElem->GetText())!=1)
	{
		bLoginSuccess=FALSE;                           //ɾ��ʧ��
		return *temp;
	}

	//temp->SaveFile("result.xml");
	return *temp;
}

/********���ü�¼״̬*******/
/*
URL: /API/Record.Status
����: POST
����:
login_email - �û��ʺ�
login_password - �û�����
format {json,xml} - ���ص����ݸ�ʽ��֧��json��xml.
domain_id - ����ID, �� 12345
record_id - ��¼ID, �� 1234567
status {enable,disable} - ״̬
��Ӧ����:
-1 ��½ʧ��
1 �ɹ�
2 ֻ����POST����
3 δ֪����
4 ȱ�ٲ������߲�������
5 ����������
6 ������������
7 ��¼������
*/
/***************************/
TiXmlDocument CDNSpodAPI::RecordStatus(int DomainID, int RecordID,BOOL RecordEnabled)
{
	if (!bLoginSuccess)
		return *tmp;

	CInternetSession session(_T("MySession"));
	CHttpConnection *pServer=NULL;
	CHttpFile *pFile=NULL;
	DWORD retcode;
	char szServerMsg[102400]; //�������ݻ�����
	DWORD dwFileSize=0;
	CString paraStr;
	paraStr=_T("/API/Record.Status");
	CString strFormData;
	strFormData=_T("login_email=");
	strFormData+=login_email;
	strFormData+=_T("&login_password=");
	strFormData+=login_password;
	strFormData+=_T("&format=");
	strFormData+=return_format;
	strFormData+=_T("&domain_id=");
	CString id;
	id.Format(_T("%d"),DomainID);
	strFormData+=id;
	strFormData+=_T("&record_id=");
	id.Format(_T("%d"),RecordID);
	strFormData+=id;
	strFormData+=_T("&status=");
	if (RecordEnabled)
		strFormData+=_T("enable");
	else
		strFormData+=_T("disable");
	//unicode -> ansi
	CStringA strFormDataA(strFormData);
	//::AfxMessageBox(strFormData);

	CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded");
	try
	{
		pServer=session.GetHttpConnection(ServerURL,nPort,login_email,login_password);
		pFile=pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST,paraStr,NULL,1,NULL,NULL,INTERNET_FLAG_EXISTING_CONNECT);
		pFile->AddRequestHeaders(strHeaders);
		pFile->AddRequestHeaders(_T("User-Agent: DDNSpod 0.4"));
		pFile->SendRequest(strHeaders,(LPVOID)(LPCSTR)strFormDataA,strFormDataA.GetLength()); //���ͱ�����
		pFile->QueryInfoStatusCode(retcode);
		
		UINT nSize=pFile->Read(szServerMsg,1024);
		while (nSize>0)
		{
			dwFileSize +=nSize;
			nSize=pFile->Read(szServerMsg+dwFileSize,1024);
		}
		szServerMsg[dwFileSize]='\0';
		//::AfxMessageBox(CString(szServerMsg));
	}
	catch (CInternetException *e)
	{
	}
	pFile->Close();
	pServer->Close();
	delete pFile;
	delete pServer;

	TiXmlDocument *temp = new TiXmlDocument;
	//�������ؽ��
	temp->Parse(szServerMsg,NULL,TIXML_ENCODING_UTF8);
	if (temp->Error())
	{
		bLoginSuccess=FALSE;                           //login fail
		return *temp;                                  //
	}

	TiXmlHandle hRoot(temp);
	TiXmlElement *pElem;

	pElem=hRoot.FirstChild("dnspod").FirstChild("status").FirstChild("code").ToElement();
	if (!pElem)
	{
		bLoginSuccess=FALSE;                           //login fail
		return *temp;
	}
	if (atoi(pElem->GetText())!=1)
	{
		bLoginSuccess=FALSE;                           //��¼״̬����ʧ��
		return *temp;
	}

	//temp->SaveFile("result.xml");
	return *temp;
}

//����������ID
int CDNSpodAPI::GetDomainID(TiXmlDocument domainlist, CString DomainValue)
{
	if (!bLoginSuccess)
		return 0;
	
	//����domianlist
	TiXmlHandle hRoot(&domainlist);
	TiXmlElement *pElem;
	pElem=hRoot.FirstChild("dnspod").FirstChild("domains").ToElement();
	if (!pElem)
	{
		bLoginSuccess=FALSE;                           //login fail
		return 0;
	}
	
	//��������
	TiXmlElement *pSubElem;
	pSubElem=pElem->FirstChildElement("domain");
	if (!pSubElem)
	{
		return 0;
	}
	//����
	for(const TiXmlElement *sub_tag =pElem->FirstChildElement(); sub_tag; sub_tag = sub_tag->NextSiblingElement())
	{
		CString DomainName(sub_tag->FirstChildElement("name")->GetText());
		if (DomainName==DomainValue)
			return atoi(sub_tag->FirstChildElement("id")->GetText());
	} 

	return 0;
}

//���ؼ�¼��ID
int CDNSpodAPI::GetRecordID(TiXmlDocument recordlist, CString RecordValue,CString RecordType)
{
	if (!bLoginSuccess)
		return 0;
	
	//����recordlist
	TiXmlHandle hRoot(&recordlist);
	TiXmlElement *pElem;
	pElem=hRoot.FirstChild("dnspod").FirstChild("records").ToElement();
	if (!pElem)
	{
		bLoginSuccess=FALSE;                           //login fail
		return 0;
	}
	
	//��������
	TiXmlElement *pSubElem;
	pSubElem=pElem->FirstChildElement("record");
	if (!pSubElem)
	{
		return 0;
	}
	//����
	for(const TiXmlElement *sub_tag =pElem->FirstChildElement(); sub_tag; sub_tag = sub_tag->NextSiblingElement())
	{
		CString RecordName(sub_tag->FirstChildElement("name")->GetText());
		if (RecordName==RecordValue)
		{
			CString recordtype(sub_tag->FirstChildElement("type")->GetText()); //��¼���ͣ�����ƾ��¼�����ǲ��㹻ɸѡ����¼ID��
			if (recordtype==RecordType)
			{
				return atoi(sub_tag->FirstChildElement("id")->GetText());
			}
		}
	} 

	return 0;
}

BOOL CDNSpodAPI::bLogin()
{
	return bLoginSuccess;
}

void CDNSpodAPI::SetModeNoChecking(BOOL bNoChecking)
{
	bLoginSuccess=bNoChecking;
}