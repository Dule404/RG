
// Bitmape-RG17727.h : main header file for the Bitmape-RG17727 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CBitmapeRG17727App:
// See Bitmape-RG17727.cpp for the implementation of this class
//

class CBitmapeRG17727App : public CWinApp
{
public:
	CBitmapeRG17727App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBitmapeRG17727App theApp;
