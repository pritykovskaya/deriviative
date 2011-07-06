#pragma once


// SampleDialog dialog

class SampleDialog : public CDialog
{
	DECLARE_DYNAMIC(SampleDialog)

public:
	SampleDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~SampleDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int size;
};
