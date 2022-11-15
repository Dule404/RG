
// TransformacijaRG17727.h : main header file for the TransformacijaRG17727 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTransformacijaRG17727App:
// See TransformacijaRG17727.cpp for the implementation of this class
//

class CTransformacijaRG17727App : public CWinApp
{
public:
	CTransformacijaRG17727App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTransformacijaRG17727App theApp;
