
// SoftWareManagerDlg.h : ͷ�ļ�
//

#pragma once

#include "FastListCtrl.h"
#include <vector>
using namespace std;
//�����Ϣ�ṹ��
typedef struct _SOFTINFO
{
	TCHAR szSoftName[50];                //�������
	TCHAR szSoftVer[50];                //����汾��
	TCHAR strSoftVenRel[50];            //�����������
	TCHAR szSoftData[20];                //�����װ����
	TCHAR strSoftInsPath[MAX_PATH];        //�����װ·��
	TCHAR strSoftUniPath[MAX_PATH];        //���ж��·��
}SOFTINFO, *PSOFTINFO;
#define Win32PATH _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall")
#define Win64PATH _T("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall")


// CSoftWareManagerDlg �Ի���
class CSoftWareManagerDlg : public CDialog
{
// ����
public:
	CSoftWareManagerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SOFTWAREMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:
	CFastListCtrl m_ControlList;
	//��ѡ������
	int m_Select;
	CString strTemp;
	//������������Ϣ
	vector<SOFTINFO> m_vecSoftInfo;

	BOOL CSoftWareManagerDlg::Is64();
	afx_msg void OnBnClickedBtnFolder();
	afx_msg void OnBnClickedBtnUninstall();
	afx_msg void OnBnClickedBtnRefresh();
	void CSoftWareManagerDlg::GetSoftwareInfo();
};
