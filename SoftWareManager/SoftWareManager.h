
// SoftWareManager.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSoftWareManagerApp:
// �йش����ʵ�֣������ SoftWareManager.cpp
//

class CSoftWareManagerApp : public CWinAppEx
{
public:
	CSoftWareManagerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSoftWareManagerApp theApp;