#pragma once


// RowSettings dialog

class RowSettings : public CDialog
{
	DECLARE_DYNAMIC(RowSettings)

public:
	RowSettings(CWnd* pParent = NULL);   // standard constructor
	virtual ~RowSettings();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_len;
public:
	afx_msg void OnEnChangeStart();
public:
	int m_start;
public:
	double m_noise_var;
public:
	double m_tan_mean;
public:
	double m_tan_var;
public:
	double m_lambda;
};
