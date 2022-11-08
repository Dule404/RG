
// Tanagram-RG17727.h : main header file for the Tanagram-RG17727 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTanagramRG17727App:
// See Tanagram-RG17727.cpp for the implementation of this class
//

class CTanagramRG17727App : public CWinApp
{
public:
	CTanagramRG17727App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTanagramRG17727App theApp;
