// derivative.h : main header file for the derivative application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "WinProgressApp.h"

// CderivativeApp:
// See derivative.cpp for the implementation of this class
//

class CderivativeApp : public CWinProgressApp
{
public:
	CderivativeApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
protected:
	virtual CWnd* GetProgressParentWnd();
};

extern CderivativeApp theApp;