// derivativeView.h : interface of the CderivativeView class
//


#pragma once
#include "histogram.h"

class CderivativeView : public CView
{
protected: // create from serialization only
	CderivativeView();
	DECLARE_DYNCREATE(CderivativeView)

// Attributes
public:
	CderivativeDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CderivativeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	void DrawTraj(CDC* pDC, Model m, bool base);
	void DrawRow(CDC* pDC, int rx, int ry, int rw, int rh, vector<double> pre_x, vector<double> y, double max, bool draw_mark, bool is_build);
	void DrawMark(int x1, int y1, bool to_buy); 
	vector<int> Scale(vector<double> v, int dim1, double dim2);
	vector<int> Scale_for_Profit(vector<double> v, int new_min, int new_max, int rh, int rw, bool is_base);
	void CderivativeView::DrawProfit(CDC* pDC, int rx, int ry, int rw, int rh, vector<double> pre_x, vector<double> v, bool is_base);

	void CderivativeView::DrawHistogram(CDC* pDC, Histogram histogram, CRect &rect);
	void CderivativeView::DrawHistograms(CDC* pDC);
};

#ifndef _DEBUG  // debug version in derivativeView.cpp
inline CderivativeDoc* CderivativeView::GetDocument() const
   { return reinterpret_cast<CderivativeDoc*>(m_pDocument); }
#endif

