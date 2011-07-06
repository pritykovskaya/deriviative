// derivativeDoc.cpp : implementation of the CderivativeDoc class
//

#include "stdafx.h"
#include "derivative.h"
#include "R32M.H"
#include "modelling.h"
#include "derivativeDoc.h"
#include "StrategyBuilder.h"
#include "histogram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CderivativeDoc

IMPLEMENT_DYNCREATE(CderivativeDoc, CDocument)

BEGIN_MESSAGE_MAP(CderivativeDoc, CDocument)
	ON_COMMAND(ID_SETTINGS_ROW, &CderivativeDoc::OnSettingsRow)
	ON_COMMAND(ID_SETTINGS_STRATEGY, &CderivativeDoc::OnSettingsStrategy)
END_MESSAGE_MAP()


// CderivativeDoc construction/destruction

CderivativeDoc::CderivativeDoc() :i(0), sample_size(25)
{
	draw_hist = false;
	rninit(time(0));
	m_base.set_everything(200, 150, 0, 0.27, 0, 2);
	m_test.set_everything(200, 150, 0, 0.27, 0, 2);

	m_base.model_noise();
	m_base.model_trend();
	vector<double> row = m_base.model_row();

	StrategyBuilder strategy(row);
	best_parameters = strategy.tune();
	change_points_build = strategy.get_res();
	profit_fun_build = strategy.build_profit_fun(change_points_build);

	m_test.model_noise();
	m_test.model_trend();

	StrategyBuilder test_strategy(m_test.model_row());
	test_strategy.make_strategy(best_parameters.first, best_parameters.second);
	change_points_test = test_strategy.get_res(); 

	profit_fun_test = test_strategy.build_profit_fun(change_points_test);
	// TODO: add one-time construction code here

	pair<double, double> res_build = strategy.test_strategy();
	pair<double, double> res_test = test_strategy.test_strategy();
		
	m_base.profit = res_build.first;
	m_base.drawdown = res_build.second;
	
	m_test.profit = res_test.first;
	m_test.drawdown = res_test.second;
}

CderivativeDoc::~CderivativeDoc()
{
}

BOOL CderivativeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	i = 100;
	return TRUE;
}




// CderivativeDoc serialization

void CderivativeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CderivativeDoc diagnostics

#ifdef _DEBUG
void CderivativeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CderivativeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CderivativeDoc commands

int CderivativeDoc::GetI(void)
{
	return i;
}

void CderivativeDoc::OnSettingsRow()
{
	RowSettings d;
	d.m_len = m_base.get_len();
	d.m_lambda = m_base.get_lambda();
	d.m_noise_var = m_base.get_noise_var();
	d.m_start = m_base.get_start_point();
	d.m_tan_mean = m_base.get_tang_mean();
	d.m_tan_var = m_base.get_tang_var();

	if (d.DoModal() == IDOK) {
		draw_hist = false;
		m_base.set_everything(d.m_len, d.m_start, d.m_noise_var, d.m_lambda, d.m_tan_mean, d.m_tan_var);	
		m_test.set_everything(d.m_len, d.m_start, d.m_noise_var, d.m_lambda, d.m_tan_mean, d.m_tan_var);			
		
		m_base.model_noise();
		m_base.model_trend();
		vector<double> row = m_base.model_row();

		StrategyBuilder strategy(row);
		best_parameters = strategy.tune();
		change_points_build = strategy.get_res();
		profit_fun_build = strategy.build_profit_fun(change_points_build);

		m_test.model_noise();
		m_test.model_trend();

		StrategyBuilder test_strategy(m_test.model_row());
		test_strategy.make_strategy(best_parameters.first, best_parameters.second);
		change_points_test = test_strategy.get_res(); 

		profit_fun_test = test_strategy.build_profit_fun(change_points_test);

		pair<double, double> res_build = strategy.test_strategy();
		pair<double, double> res_test = test_strategy.test_strategy();
		
		m_base.profit = res_build.first;
		m_base.drawdown = res_build.second;
	
		m_test.profit = res_test.first;
		m_test.drawdown = res_test.second;
	}

	UpdateAllViews(0);
	// TODO: Add your command handler code here
}

void CderivativeDoc::OnSettingsStrategy()
{

	SampleDialog d;
	d.size = sample_size;
	if (d.DoModal() == IDOK) {
		draw_hist = true;
		sample_size = d.size;

		CWinProgressApp* pApp = (CWinProgressApp*)AfxGetApp();
		pApp->SetProgressRange( 0, sample_size);//Числа для примера
		pApp->ShowProgress();

		for(int i = 0; i < sample_size; ++i) {
			
			m_base.model_noise();
			m_base.model_trend();
			
			StrategyBuilder strategy(m_base.model_row());
			best_parameters = strategy.tune();
			strategy.make_strategy(best_parameters.first, best_parameters.second);
			
			pair<double, double> tmp = strategy.test_strategy();
			profit_sample_build.push_back(tmp.first);
			drawdown_sample_build.push_back(tmp.second);

			m_test.model_noise();
			m_test.model_trend();

			StrategyBuilder test_strategy(m_test.model_row());
			test_strategy.make_strategy(best_parameters.first, best_parameters.second);
			
			tmp = test_strategy.test_strategy();
			profit_sample_test.push_back(tmp.first);
			drawdown_sample_test.push_back(tmp.second);

			pApp->IncrementProgressBar();
		}

		double sum_build = 0;
		for (int i = 0; i < profit_sample_build.size(); ++i) {
			sum_build += profit_sample_build[i];
		}
		avg_profit_build = sum_build / profit_sample_build.size();
		
		double sum_test = 0;
		for (int i = 0; i < profit_sample_test.size(); ++i) {
			sum_test += profit_sample_test[i];
		}
		avg_profit_test = sum_test / profit_sample_test.size();

		pApp->ShowProgress( FALSE );
	}


	UpdateAllViews(0);
	// TODO: Add your command handler code here
}
