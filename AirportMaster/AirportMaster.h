
// AirportMaster.h : AirportMaster Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CAirportMasterApp:
// �йش����ʵ�֣������ AirportMaster.cpp
//

class CAirportMasterApp : public CWinAppEx
{
public:
	CAirportMasterApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CAirportMasterApp theApp;
