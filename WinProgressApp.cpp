// WinProgressApp.cpp: implementation of the CWinProgressApp class.
//
// CPP ver. 2.0
// written by Andrey Demski
// 2001
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinProgressApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CWinProgressApp Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWinProgressApp::CWinProgressApp()
{
	m_pProgressBar = NULL;
	m_bShowProgress = TRUE;
	m_bProgressIsLocked = FALSE;
}

CWinProgressApp::~CWinProgressApp()
{
	if ( m_pProgressBar != NULL )
		delete m_pProgressBar;
}

//////////////////////////////////////////////////////////////////////
// Overrides

BOOL CWinProgressApp::InitInstance()
{
	CreateProgressBar();
	return CWinApp::InitInstance();
} 

/////////////////////////////////////////////////////////////////////////////
// Progress bar

void CWinProgressApp::CreateProgressBar()
{
	if ( m_pProgressBar != NULL )
		delete m_pProgressBar;

	m_pProgressBar = new CProgressCtrl;
	m_pProgressBar->Create( WS_CHILD | PBS_SMOOTH, CRect(0,0,0,0), GetProgressParentWnd(), 1);
	m_pProgressBar->SetStep( 1 );
	m_pProgressBar->SetRange( 0, 100 );
	AdjustProgressPosition();
	ShowProgress( FALSE );
}

CWnd* CWinProgressApp::GetProgressParentWnd()
{
	return GetMainWnd();
}

void CWinProgressApp::EnableProgressBar(BOOL bEnable)
{
	m_bShowProgress = bEnable;
}

void CWinProgressApp::LockProgressBar(BOOL bLock)
{
	m_bProgressIsLocked = bLock;
}

void CWinProgressApp::AdjustProgressPosition()
{
	if ( m_pProgressBar == NULL || ! m_bShowProgress)
		return;

	m_pProgressBar->MoveWindow( &GetProgressRect() );
}

RECT CWinProgressApp::GetProgressRect()
{
	RECT rectProgress;
	GetProgressParentWnd()->GetWindowRect( &rectProgress );
	rectProgress.bottom -= rectProgress.top;
	rectProgress.top = 0;
	rectProgress.right -= rectProgress.left;
	rectProgress.left = 0;
	return rectProgress;
}

void CWinProgressApp::ShowProgress(BOOL bShow)
{
	if ( m_pProgressBar == NULL || ! m_bShowProgress || m_bProgressIsLocked )
		return;

	if ( bShow )
	{
		AdjustProgressPosition();
		m_pProgressBar->ShowWindow( SW_SHOW );
		GetProgressParentWnd()->BeginWaitCursor();
	}
	else
	{	
		m_pProgressBar->ShowWindow( SW_HIDE );
		GetProgressParentWnd()->EndWaitCursor();
	}
}

void CWinProgressApp::SetProgressRange(int nStart, int nFinish)
{
	if ( m_pProgressBar != NULL && m_bShowProgress && (! m_bProgressIsLocked) )
	{
		m_nProgressPos = 0;
		m_nProgressUnitsCurrent = 0;
		m_nProgressUnitsTotal = nFinish - nStart;
		m_pProgressBar->SetPos( 0 );
	}
}

void CWinProgressApp::IncrementProgressBar()
{
	if ( m_pProgressBar != NULL && m_bShowProgress )
	{
		int nNewProgressPos = 100 * (++m_nProgressUnitsCurrent) / m_nProgressUnitsTotal;
		if ( m_nProgressPos < nNewProgressPos )
			m_pProgressBar->SetPos( m_nProgressPos = nNewProgressPos );
	}
}

