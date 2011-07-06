// derivativeDoc.h : interface of the CderivativeDoc class
//


#pragma once
#include "modelling.h"
#include "RowSettings.h"
#include "SampleDialog.h"

class CderivativeDoc : public CDocument
{
protected: // create from serialization only
	CderivativeDoc();
	DECLARE_DYNCREATE(CderivativeDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CderivativeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	int i;
	int sample_size; 
	Model m_base;
	Model m_test;
	bool draw_hist;
	pair<double, double> best_parameters;
	vector<int> change_points_build;
	vector<int> change_points_test;
	vector<double> profit_fun_build;
	vector<double> profit_fun_test;

	vector<double> profit_sample_build;
	vector<double> drawdown_sample_build;
	double avg_profit_build;
	
	vector<double> profit_sample_test;
	vector<double> drawdown_sample_test;
	double avg_profit_test;

public:
	int GetI(void);
public:
	afx_msg void OnSettingsRow();
public:
	afx_msg void OnSettingsStrategy();
};


