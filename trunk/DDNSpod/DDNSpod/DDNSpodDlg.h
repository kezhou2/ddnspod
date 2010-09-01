// DDNSpodDlg.h : ͷ�ļ�
//

#pragma once
#include "tinyxml.h"

enum UPDATETYPE{NOTAUTO=1,AUTO=2,ONCE=3};
typedef struct CConfig_tag
{
	CString User;                          //�û���
	CString Password;                      //����
	CString URL;                           //DNSpod��ַ
	WORD port;                             //�������˿�
	CString Domain;                        //����
	int DomainID;                          //����ID
	CString A_Record;                      //A��¼
	int A_RecordID;                        //A��¼ID
	CString AAAA_Record;                   //AAAA��¼
	int AAAA_RecordID;                     //AAAA��¼ID
	BOOL A_Record_enabled;                 //
	BOOL AAAA_Record_enabled;              //
	BOOL AutoStart;                        //
	UPDATETYPE UpdateType;                 //
}CConfig;

// CDDNSpodDlg �Ի���
class CDDNSpodDlg : public CDialog
{
// ����
public:
	CDDNSpodDlg(CWnd* pParent = NULL);	// ��׼���캯��
	enum { IDD = IDD_DDNSPOD_DIALOG };  // �Ի�������

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	BOOL CreateConfFile();
	BOOL LoadFromFile(TiXmlDocument *xmlfile);
	BOOL SaveConfFile(BOOL bRegKey);
	BOOL GetRecordList(CString User,CString Password);

// ʵ��
protected:
	HICON m_hIcon;
	CMenu m_menu;                //�˵�
	CString m_confpath;          //�����ļ�·��
	NOTIFYICONDATA m_nd;         //����
	CConfig m_conf;              //����
	BOOL m_bConfLoaded;          //
	TiXmlDocument *AllRecordList;//
	CString ip4;                 //
	CString ip6;                 //
	//CDNSpodAPI DNSpod;         //
	BOOL m_bUsingDefault;        //ʹ��ȱʡ�ʺ�
	CString m_User;              //
	CString m_Password;          //
	BOOL bShow;
	CString DNSpodAPIVer;        //DNSpod�ṩ��API�汾��������ǰ֧�ְ汾1.5

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CConfig GetConfig();
	BOOL GetIP();
	TiXmlDocument *RecordList();
	afx_msg void OnFileExit();
	afx_msg void OnAbout();
	afx_msg void OnFileSetting();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnClickTray(WPARAM wParam,LPARAM lParam); //������Ӧ
	afx_msg void OnTrayShowwindow();
	afx_msg void OnTraySetting();
	afx_msg void OnTrayExit();
	afx_msg void OnTrayAbout();
	afx_msg void OnFileRefresh();
	afx_msg void OnFileSave();
	afx_msg void OnTrayRefresh();
	afx_msg void OnTraySave();
	afx_msg void OnBnClickedCheck();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnFileShowip();
	afx_msg void OnTrayShowip();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
};

