#pragma once


// CFastListCtrl

class CFastListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CFastListCtrl)

public:
	CFastListCtrl();
	virtual ~CFastListCtrl();
	//�ܹ���̬����Ӷ���
	void AddColumn(DWORD dwCount, ...)
	{
		va_list vl;
		va_start(vl, dwCount);
		// ���
		TCHAR* pName = va_arg(vl, TCHAR*);
		InsertColumn(0, pName, 0, 40);
		//����
		pName = va_arg(vl, TCHAR*);
		InsertColumn(1, pName, 0, 200);
		//������
		pName = va_arg(vl, TCHAR*);
		InsertColumn(2, pName, 0, 150);
		//��װʱ��
		pName = va_arg(vl, TCHAR*);
		InsertColumn(3, pName, 0, 80);
		//�汾
		pName = va_arg(vl, TCHAR*);
		InsertColumn(4, pName, 0, 150);
		va_end(vl);
	}
	//�ܹ���̬���һ���е�����
	void AddItem(int n, DWORD dwCount, ...)
	{
		va_list vl;
		va_start(vl, dwCount);
		TCHAR* pName = va_arg(vl, TCHAR*);
		InsertItem(n, pName);
		for (DWORD i = 1; i < dwCount; i++)
		{
			TCHAR* pName = va_arg(vl, TCHAR*);
			SetItemText(n, i, pName);
		}
		va_end(vl);
	}

protected:
	DECLARE_MESSAGE_MAP()
};


