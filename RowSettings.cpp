// RowSettings.cpp : implementation file
//

#include "stdafx.h"
#include "derivative.h"
#include "RowSettings.h"


// RowSettings dialog

IMPLEMENT_DYNAMIC(RowSettings, CDialog)

RowSettings::RowSettings(CWnd* pParent /*=NULL*/)
	: CDialog(RowSettings::IDD, pParent)
	, m_len(0)
	, m_start(0)
	, m_noise_var(0)
	, m_tan_mean(0)
	, m_tan_var(0)
	, m_lambda(0)
{

}

RowSettings::~RowSettings()
{
}

void RowSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LEN, m_len);
	DDV_MinMaxInt(pDX, m_len, 0, 10000);
	DDX_Text(pDX, IDC_START, m_start);
	DDV_MinMaxInt(pDX, m_start, 0, 1000000);
	DDX_Text(pDX, IDC_NOISE, m_noise_var);
	DDV_MinMaxDouble(pDX, m_noise_var, 0, 1000);
	DDX_Text(pDX, IDC_TAN_MEAN, m_tan_mean);
	DDV_MinMaxDouble(pDX, m_tan_mean, -100, 100);
	DDX_Text(pDX, IDC_TAN_VAR, m_tan_var);
	DDV_MinMaxDouble(pDX, m_tan_var, 0, 100);
	DDX_Text(pDX, IDC_LAMBDA, m_lambda);
	DDV_MinMaxDouble(pDX, m_lambda, 0, 100);
}


BEGIN_MESSAGE_MAP(RowSettings, CDialog)
	ON_EN_CHANGE(IDC_START, &RowSettings::OnEnChangeStart)
END_MESSAGE_MAP()


// RowSettings message handlers

void RowSettings::OnEnChangeStart()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
