// WinProgressApp.h: interface for the CWinProgressApp class.
//
// HEADER ver. 2.0
// written by Andrey Demski
// 2001
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINPROGRESSAPP_H__226235BB_8DE2_4CE7_8F2B_339D9A1C36C4__INCLUDED_)
#define AFX_WINPROGRESSAPP_H__226235BB_8DE2_4CE7_8F2B_339D9A1C36C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWinProgressApp : public CWinApp  
{
// ProgressBar
public:
	void LockProgressBar( BOOL bLock = TRUE ); // Locks progress bar for changing show mode and ranges
	void EnableProgressBar(BOOL bEnable = TRUE); // Enables/disables progress bar during calculations
	void ShowProgress( BOOL bShow = TRUE );
	void SetProgressRange( int nStart, int nFinish );
	void IncrementProgressBar();

	BOOL GetProgressIsEnabled() {  return m_bShowProgress;  };

	void AdjustProgressPosition();

// Overrides
public:
	virtual BOOL InitInstance();

// Construction/Destruction
public:
	CWinProgressApp();
	virtual ~CWinProgressApp();

// ProgressBar Attributes
protected:
	CProgressCtrl* m_pProgressBar;
	BOOL m_bShowProgress;
	BOOL m_bProgressIsLocked;
	int m_nProgressUnitsTotal;
	int m_nProgressUnitsCurrent;
	int m_nProgressPos;

protected:
	virtual void CreateProgressBar();
	virtual RECT GetProgressRect();
	virtual CWnd* GetProgressParentWnd(); // should be overridden in child class
};


#endif // !defined(AFX_WINPROGRESSAPP_H__226235BB_8DE2_4CE7_8F2B_339D9A1C36C4__INCLUDED_)
