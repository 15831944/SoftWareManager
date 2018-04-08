
// SoftWareManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SoftWareManager.h"
#include "SoftWareManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BOOL OpenFolderAndSelectFile(TCHAR* szPath);

// CSoftWareManagerDlg �Ի���




CSoftWareManagerDlg::CSoftWareManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoftWareManagerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);	
}

void CSoftWareManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ControlList);
}

BEGIN_MESSAGE_MAP(CSoftWareManagerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_Folder, &CSoftWareManagerDlg::OnBnClickedBtnFolder)
	ON_BN_CLICKED(IDC_BTN_Uninstall, &CSoftWareManagerDlg::OnBnClickedBtnUninstall)
	ON_BN_CLICKED(IDC_BTN_Refresh, &CSoftWareManagerDlg::OnBnClickedBtnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CSoftWareManagerDlg ��Ϣ�������

BOOL CSoftWareManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_Select = -1;
	m_ControlList.AddColumn(5,
		_T("ID"),
		_T("����"),
		_T("������"),
		_T("��װʱ��"),
		_T("�汾"));
	m_ControlList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	GetSoftwareInfo();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSoftWareManagerDlg::OnPaint()
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
HCURSOR CSoftWareManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSoftWareManagerDlg::OnBnClickedBtnFolder()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CListCtrl *ListCtrl = NULL;
    if (m_ControlList.IsWindowVisible())
        ListCtrl = &m_ControlList;
    POSITION Pos  = ListCtrl->GetFirstSelectedItemPosition();
    while(Pos)
    {
        m_Select = ListCtrl->GetNextSelectedItem(Pos);
        if (m_vecSoftInfo[m_Select].strSoftInsPath[0])
        {
            OpenFolderAndSelectFile(m_vecSoftInfo[m_Select].strSoftInsPath);
        }
		else if (m_vecSoftInfo[m_Select].strSoftUniPath[0])
		{
			OpenFolderAndSelectFile(m_vecSoftInfo[m_Select].strSoftUniPath);
		}
    }
}


void CSoftWareManagerDlg::OnBnClickedBtnUninstall()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
 
    CListCtrl    *ListCtrl = NULL;
    if (m_ControlList.IsWindowVisible())
        ListCtrl = &m_ControlList;
    POSITION Pos  = ListCtrl->GetFirstSelectedItemPosition();
    while(Pos)
    {
        m_Select = ListCtrl->GetNextSelectedItem(Pos);
        if (m_vecSoftInfo[m_Select].strSoftUniPath[0])
        {
             ShellExecute(NULL, _T("open"),m_vecSoftInfo[m_Select].strSoftUniPath, NULL, NULL, SW_SHOWNORMAL);
        }
    }
    
}


void CSoftWareManagerDlg::OnBnClickedBtnRefresh()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    GetSoftwareInfo();
}


//************************************************************
// ��������:    GetSoftwareInfo
// ����˵��:    ��ȡ�����Ϣ
// ��    ��:    cyxvc
// ʱ    ��:    2015/11/10
// �� ��    ֵ:    void
//************************************************************
void CSoftWareManagerDlg::GetSoftwareInfo()
{
    m_ControlList.DeleteAllItems();
    m_vecSoftInfo.clear();
    SOFTINFO stcSoftInfo = {0};
    HKEY RootKey;            // ����
    LPCTSTR lpSubKey;        // �Ӽ�����
    HKEY hkResult;           // ��Ҫ�򿪼��ľ�� 
    HKEY hkRKey;
    LONG lReturn;            // ��¼��ȡע����Ƿ�ɹ�
    CString strBuffer;
    CString strMidReg;

    DWORD index = 0;
    TCHAR szKeyName[255] = { 0 };        // ע���������
    DWORD dwKeyLen = 255;
    DWORD dwNameLen = 255;
    DWORD dwType = 0;

    RootKey = HKEY_LOCAL_MACHINE;
    lpSubKey = Is64() ? Win64PATH : Win32PATH;

    lReturn = RegOpenKeyEx(RootKey, lpSubKey, 0, 
        KEY_ALL_ACCESS, &hkResult);
    if (lReturn == ERROR_SUCCESS)
    {
        DWORD index = 0;
        DWORD ListIndex = 0;
		TCHAR szIDbuf[32]={0};
        while (ERROR_NO_MORE_ITEMS != 
            RegEnumKeyEx(hkResult, index, szKeyName, &dwKeyLen, 
            0, NULL, NULL, NULL))
        {
            strBuffer.Format(_T("%s"), szKeyName);
            if (!strBuffer.IsEmpty())
            {
                strMidReg = (CString)lpSubKey + _T("\\") + strBuffer;

                if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, strMidReg, 
                    0, KEY_ALL_ACCESS, &hkRKey) == ERROR_SUCCESS)
                {
                    RegQueryValueEx(hkRKey, _T("DisplayName"), 
                        0, &dwType, (LPBYTE)stcSoftInfo.szSoftName, &dwNameLen);
                    dwNameLen = 255;

                    RegQueryValueEx(hkRKey, _T("DisplayVersion"), 
                        0, &dwType, (LPBYTE)stcSoftInfo.szSoftVer, &dwNameLen);
                    dwNameLen = 255;

                    RegQueryValueEx(hkRKey, _T("InstallLocation"), 
                        0, &dwType, (LPBYTE)stcSoftInfo.strSoftInsPath, &dwNameLen);
                    dwNameLen = 255;

                    RegQueryValueEx(hkRKey, _T("Publisher"), 
                        0, &dwType, (LPBYTE)stcSoftInfo.strSoftVenRel, &dwNameLen);
                    dwNameLen = 255;

                    RegQueryValueEx(hkRKey, _T("UninstallString"), 
                        0, &dwType, (LPBYTE)stcSoftInfo.strSoftUniPath, &dwNameLen);
                    dwNameLen = 255;

                    RegQueryValueEx(hkRKey, _T("InstallDate"),
                        0, &dwType, (LPBYTE)stcSoftInfo.szSoftData, &dwNameLen);
                    if (stcSoftInfo.szSoftData[0])
                    {
                        stcSoftInfo.szSoftData[9] = stcSoftInfo.szSoftData[7];
                        stcSoftInfo.szSoftData[8] = stcSoftInfo.szSoftData[6];
                        stcSoftInfo.szSoftData[7] = '-';
                        stcSoftInfo.szSoftData[6] = stcSoftInfo.szSoftData[5];
                        stcSoftInfo.szSoftData[5] = stcSoftInfo.szSoftData[4];
                        stcSoftInfo.szSoftData[4] = '-';
                    }
                    dwNameLen = 255;

                    //��������
                    if (stcSoftInfo.szSoftName[0]!='\0')
                    {
                        m_vecSoftInfo.push_back(stcSoftInfo);

						_itot(ListIndex+1,szIDbuf,10);
                        m_ControlList.AddItem(ListIndex++, 5,
							szIDbuf,
                            stcSoftInfo.szSoftName,
                            stcSoftInfo.strSoftVenRel,
                            stcSoftInfo.szSoftData,
                            stcSoftInfo.szSoftVer);
                    }

                    index++;
                }
                dwKeyLen = 255;
                memset(szKeyName, 0, 255);
                memset(&stcSoftInfo, 0, sizeof(SOFTINFO));
            }
        }
        RegCloseKey(hkResult);
    }
}


//************************************************************
// ��������:    Is64
// ����˵��:    �ж�ϵͳ�Ƿ���64λ
// ��    ��:    cyxvc
// ʱ    ��:    2015/11/10
// �� ��    ֵ:    BOOL
//************************************************************
BOOL CSoftWareManagerDlg::Is64()
{
    SYSTEM_INFO si;
    GetNativeSystemInfo(&si);
    if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
        si.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_IA64)
    {
        //64 λ����ϵͳ
        return TRUE;
    }
    else
    {
        // 32 λ����ϵͳ
        return FALSE;
    }
    return FALSE;
}

//************************************************************
// ��������:    On32776
// ����˵��:    ���ļ�����λ��
// ��    ��:    cyxvc
// ʱ    ��:    2015/11/10
// �� ��    ֵ:    void
//************************************************************
BOOL OpenFolderAndSelectFile(TCHAR* szPath)
{
	BOOL bRet = FALSE;
	::CoInitialize(NULL);
	do
	{
		LPSHELLFOLDER pDesktopFolder;
		if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
		{
			LPITEMIDLIST     pidl;
			ULONG             chEaten;
			ULONG             dwAttributes;
			HRESULT             hr;

			hr = pDesktopFolder->ParseDisplayName(
				NULL, NULL, szPath, &chEaten, &pidl, &dwAttributes);
			if (FAILED(hr))
			{
				pDesktopFolder->Release();
				break;;
			}
			LPCITEMIDLIST pidlFolder = pidl;

			hr = SHOpenFolderAndSelectItems(pidl, 0, NULL, 0);
			pDesktopFolder->Release();
			if (hr == S_OK)
			{
				bRet = TRUE;
			}
		}
	}
	while(0);
	::CoUninitialize();
    return bRet;
}