// derivativeView.cpp : implementation of the CderivativeView class
//

#include "stdafx.h"
#include "derivative.h"
#include "stdlib.h"
#include <sstream>

#include "derivativeDoc.h"
#include "derivativeView.h"

#include "histogram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <vector>
using namespace std;
// CderivativeView

IMPLEMENT_DYNCREATE(CderivativeView, CView)

BEGIN_MESSAGE_MAP(CderivativeView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CderivativeView construction/destruction

CderivativeView::CderivativeView()
{
	// TODO: add construction code here

}

CderivativeView::~CderivativeView()
{
}

BOOL CderivativeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CderivativeView drawing

void CderivativeView::OnDraw(CDC* pDC)
{	
	CderivativeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	//int i = pDoc->GetI();
	//pDC->TextOut(i, i, CString("Hello"));
	if (!pDoc->draw_hist) {
		Model m_base = pDoc->m_base;
		DrawTraj(pDC, m_base, true);

		Model m_test = pDoc->m_test;
		DrawTraj(pDC, m_test, false);
	} else {
		DrawHistograms(pDC);
	}
}

vector<int>  CderivativeView::Scale(vector<double> v, int dim1, double dim2){
	double coef = dim1 / dim2;
	vector<int> res;
	for (int i = 0; i < v.size(); ++i) {
		int transformed = v[i] * coef;
		if (transformed > dim1) {
			transformed = dim1;
		}
		res.push_back(transformed);
	}
	return (res);
}

vector<int> CderivativeView::Scale_for_Profit(vector<double> v, int new_min, int new_max, int rh, int rw, bool is_base) {
	vector<int> tmp;
	double max = v[0];
	double min = v[0];
	for (int i = 0; i < v.size(); ++i) {
		if (max < v[i]) {
			max = v[i]; 
		}
		if (min > v[i]) {
			min = v[i];
		}
	}
	double k = (new_max - new_min) / (max - min);
	double shift = new_max - k * max;

	for (int i = 0; i < v.size(); ++i) {
		tmp.push_back(k * v[i] + shift);
	}
	
	CClientDC dc(this);
	CFont font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT)); // zero out structure
	lf.lfHeight = 16; // request a 12-pixel-height font
	//strcpy(lf.lfFaceName, "Arial"); // request a face name "Arial"
	font.CreateFontIndirect(&lf); // create the font
	CFont* def_font = dc.SelectObject(&font);
	
	int h1;
	int h2;
	if (is_base == true) {
		h1 = 0.1 * rh;
		h2 = rh;
	} else {
		h1 = 1.4 * rh ;
		h2 = 2.2 * rh;
	}
	string buffer;
	std::ostringstream s;
	s << max;
	buffer = s.str();
	dc.TextOutW(1.35 * rw, h1, CString(buffer.c_str()));
	
	std::ostringstream s1;
	s1 << min;
	buffer = s1.str();
	dc.TextOutW(1.35 * rw, h2, CString(buffer.c_str()));
	
	dc.SelectObject(def_font);
	font.DeleteObject();
	return(tmp);
}

void CderivativeView::DrawMark(int x1, int y1, bool to_buy) 
{
	CClientDC dc(this);
	
	
	CPen pen; 
	if (to_buy == true) {
		pen.CreatePen( PS_SOLID, 1, RGB(0, 0, 0));
	} else {
		pen.CreatePen( PS_SOLID, 1, RGB(255, 0, 0));
	}
	CPen* pOldPen = (CPen *) dc.SelectObject( &pen );
	dc.MoveTo(x1, y1);
	dc.LineTo(x1 - 3, y1 - 7);
	dc.LineTo(x1 + 3, y1 - 7);
	dc.LineTo(x1, y1);
		
	dc.SelectObject(pOldPen);
	
}


void CderivativeView::DrawProfit(CDC* pDC, int rx, int ry, int rw, int rh, vector<double> pre_x, vector<double> v, bool is_base) {


	CderivativeDoc* pDoc = GetDocument();
	int len = pre_x.size();
	
	vector<int> x = Scale(pre_x, rw, len - 1);
	vector<int> y = Scale_for_Profit(v, rh * 0.1, rh * 0.9, rh, rw, is_base);

	pDC->MoveTo(rx, ry + rh - y[0]);
	for (int i = 1; i < len; ++i) {
		pDC -> LineTo(rx + x[i], ry + rh - y[i]);
	}
	
}


void CderivativeView::DrawRow(CDC* pDC, int rx, int ry, int rw, int rh, vector<double> pre_x, vector<double> v, double max, bool draw_mark, bool is_build) {

	CderivativeDoc* pDoc = GetDocument();
	int len = pre_x.size();
	
	double min = v[0];
	for (int i = 1; i < v.size(); ++i) {
		min = min > v[i] ? v[i] : min;
	}
	vector<int> x = Scale(pre_x, rw, len - 1);
	vector<int> y = Scale(v, rh * 0.9, max);
	
	vector<int> change_points;
	if (is_build == true) {
		for (int i = 0; i < pDoc->change_points_build.size(); ++i) {
			change_points.push_back(pDoc->change_points_build[i]);
		}
	} else {
		for (int i = 0; i < pDoc->change_points_test.size(); ++i) {
			change_points.push_back(pDoc->change_points_test[i]);
		}
	}

	pDC->MoveTo(rx, ry + rh - y[0]);
	for (int i = 1; i < len; ++i) {
		pDC -> LineTo(rx + x[i], ry + rh - y[i]);
		if (draw_mark == true) {
			if (change_points[i] == 1) {
					DrawMark(rx + x[i], ry + rh - y[i], true);
			}
			if (change_points[i] == -1) {
					DrawMark(rx + x[i], ry + rh - y[i], false);
			}
		}
	}
	if (draw_mark == true) {
		CFont font;
		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT)); // zero out structure
		lf.lfHeight = 16; // request a 12-pixel-height font
		//strcpy(lf.lfFaceName, "Arial"); // request a face name "Arial"
		font.CreateFontIndirect(&lf); // create the font
		CFont* def_font = pDC->SelectObject(&font);
		
		int h1;
		int h2;
		if (is_build == true) {
			h1 = 0.1 * rh;
			h2 = rh;
		} else {
			h1 = 1.39 * rh ;
			h2 = 2.2 * rh;
		}
		string buffer;
		std::ostringstream s;
		s << max;
		buffer = s.str();
		pDC->TextOutW(0.21 * rw, h1, CString(buffer.c_str()));
		
		std::ostringstream s1;
		s1 << min;
		buffer = s1.str();
		pDC->TextOutW(0.21 * rw, h2, CString(buffer.c_str()));
		
		pDC->SelectObject(def_font);
		font.DeleteObject();
		
	}
}

void CderivativeView::DrawHistogram(CDC* pDC, Histogram histogram, CRect &rect) {
	int indent = 30;
	
	
	//pDC->Rectangle(rect.left, rect.top, rect.right, rect.bottom);

	CFont font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT)); // zero out structure
	lf.lfHeight = 16; // request a 12-pixel-height font
	//strcpy(lf.lfFaceName, "Arial"); // request a face name "Arial"
	font.CreateFontIndirect(&lf); // create the font
	CFont* def_font = pDC->SelectObject(&font);

	pDC->TextOutW(rect.left, rect.top, CString(histogram.get_name().c_str())); //по умолчанию указанные координаты – это координаты
	
	char buffer[100];
	
	pDC->TextOutW(rect.left + indent / 2, rect.top + indent, CString(itoa(histogram.get_column_max(), buffer, 10))); 
	pDC->TextOutW(rect.left + indent / 2, rect.bottom - indent - 10, CString(itoa(0, buffer, 10))); 

	pDC->TextOutW(rect.right - indent, rect.bottom - indent, CString(itoa(histogram.get_final_point(), buffer, 10))); 
	pDC->TextOutW(rect.left + indent, rect.bottom - indent, CString(itoa(histogram.get_start_point(), buffer, 10))); 

	// верхнего левого угла прямоугольника, в который идет вывод текста. 
	pDC->SelectObject(def_font);
	font.DeleteObject();
	
	int startPointY = rect.bottom - indent;
	int startPointX = rect.left + indent;

	double scaleX = (rect.Width() - indent * 2) / histogram.get_columns().size();
	double scaleY = (rect.Height() - indent * 2) / histogram.get_column_max(); 
	
	for (int i = 0; i < histogram.get_columns().size(); i++) {
		pDC->Rectangle(
			startPointX + scaleX * i, startPointY, 
			startPointX + scaleX * (i + 1), startPointY - scaleY * histogram.get_columns()[i]
		);
	}


	CPen penBlack; // Construct it, then initialize
	// CreatePen возвращает 0, если создать объект не удалось
	penBlack.CreatePen( PS_SOLID, 2, RGB(0,0,0) ); 
	// Select it into the device context
	// Save the old pen at the same time
	CPen* pOldPen = (CPen *) pDC->SelectObject( &penBlack );
	
	pDC->MoveTo(startPointX, startPointY);
	pDC->LineTo(startPointX, rect.top + indent);

	pDC->MoveTo(startPointX, startPointY);
	pDC->LineTo(rect.right - indent, startPointY);

	//Restore the old pen to the device context
	pDC->SelectObject( pOldPen );	
}

void CderivativeView::DrawHistograms(CDC* pDC) {
	CderivativeDoc* pDoc = GetDocument();
	

	CFont font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT)); // zero out structure
	lf.lfHeight = 16; // request a 12-pixel-height font
	font.CreateFontIndirect(&lf); // create the font
	CFont* def_font = pDC->SelectObject(&font);

	char buffer[100];	

	string avg_capital_text = 
		string("Base avg capital: ") + (string) itoa(pDoc->avg_profit_build, buffer, 10) + string("    ") +
		string("Test avg capital: ") + (string) itoa(pDoc->avg_profit_test, buffer, 10);	

	pDC->TextOutW(20, 20, CString(avg_capital_text.c_str())); 
	
	pDC->SelectObject(def_font);
	font.DeleteObject();


	LOGBRUSH logBrush;
	logBrush.lbColor = RGB(243, 255, 32);
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbHatch = HS_CROSS;
	CBrush brush;
	brush.CreateBrushIndirect(&logBrush);

	CBrush*	pOldBrush = (CBrush*) pDC->SelectObject(&brush);
	
	
	CRect rc;
	GetClientRect(&rc);
	
	int const BORDER = 50;
	int width = rc.Width() - BORDER * 2;
	int height = rc.Height() - BORDER * 2;

	int const rowsCount = 2;
	int const columnsCount = 2;
	
	int histWidth = width / columnsCount;
	int histHeight = height / rowsCount;
	
	Histogram histograms[rowsCount][columnsCount] = { 
		{ Histogram(pDoc->profit_sample_build, 6, "Base profit"), 
		  Histogram(pDoc->profit_sample_test, 6, "Test profit")} ,
		
		{ Histogram(pDoc->drawdown_sample_build, 6, "Base Drawdown"), 
		  Histogram(pDoc->drawdown_sample_test, 6, "Test Drawdown")}
	};

	for (int i = 0, left = rc.left + BORDER; i < rowsCount; i++, left += histWidth) {
		for (int j = 0, top = rc.top + BORDER; j < columnsCount; j++, top += histHeight) {
			CRect rect(left, top, left + histWidth, top + histHeight);
			DrawHistogram(pDC, histograms[i][j], rect);
		}
	}

	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();	
}

void CderivativeView::DrawTraj(CDC* pDC, Model m, bool base){

	CderivativeDoc* pDoc = GetDocument();
	vector<double> row = m.get_row();
	vector<double> trend = m.get_trend();

	double max = row[0];
	for (int i = 0; i < row.size(); ++i) {
		if (max < row[i]) {
			max = row[i];
		}
		if (max < trend[i]) {
			max = trend[i];
		}
	}

	vector<double> pre_x;
	for (int i = 0; i < row.size(); ++i) {
		pre_x.push_back(i);
	}
	CPen penBlack; // Construct it, then initialize
	// CreatePen возвращает 0, если создать объект не удалось
	penBlack.CreatePen( PS_SOLID, 2, RGB(0,0,0) ); 
	// Select it into the device context
	// Save the old pen at the same time
	CPen* pOldPen = (CPen *) pDC->SelectObject( &penBlack );
	// Draw with the pen. For example,

	CRect rc;
	GetClientRect(&rc);
	int w = rc.Width();
	int h = rc.Height();
	int rw = 0.6 * w;
	int rh = 0.8 * h/2;

	if (base == true) {
		pDC->Rectangle(0.2 * w, 0.1 * (h/2), 0.2 * w + rw + 1, 0.1 * (h/2) + rh + 5);
		
		DrawRow(pDC, 0.2 * w, 0.1 * (h/2), rw, rh, pre_x, row, max, true, true);
		
		CPen penRed; 	
		penRed.CreatePen( PS_SOLID, 1, RGB(255,0,0) ); 
		pDC->SelectObject( &penRed );
		
		DrawRow(pDC, 0.2 * w, 0.1 * (h/2), rw, rh, pre_x, trend, max, false, false);
		
		CPen penBlue; 	
		penBlue.CreatePen( PS_SOLID, 2, RGB(0, 0, 255) ); 
		pDC->SelectObject( &penBlue );

		DrawProfit(pDC, 0.2 * w, 0.1 * (h/2), rw, rh, pre_x, pDoc->profit_fun_build, true);
		
		pDC->SelectObject( pOldPen );	
	} else {
		pDC->Rectangle(0.2 * w, 0.1 * (h/2) + h/2, 0.2 * w + rw + 1, 0.1 * (h/2) + h/2 + rh + 5);
		
		DrawRow(pDC, 0.2 * w, 0.1 * (h/2) + h/2, rw, rh, pre_x, row, max, true, false);
		
		CPen penRed; 	
		penRed.CreatePen( PS_SOLID, 1, RGB(255,0,0) ); 
		pDC->SelectObject( &penRed );
		
		DrawRow(pDC, 0.2 * w, 0.1 * (h/2) + h/2, rw, rh, pre_x, trend, max, false, false);
		
		CPen penBlue; 	
		penBlue.CreatePen( PS_SOLID, 2, RGB(0, 0, 255) ); 
		pDC->SelectObject( &penBlue );
		
		DrawProfit(pDC, 0.2 * w, 0.1 * (h/2) + h/2, rw, rh, pre_x, pDoc->profit_fun_test, false);
		
		pDC->SelectObject( pOldPen );	
	}
	
	// Restore the old pen to the device context
	pDC->SelectObject( pOldPen );	

	// нарисовать подписи
	CFont font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT)); // zero out structure
	lf.lfHeight = 16; // request a 12-pixel-height font
	//strcpy(lf.lfFaceName, "Arial"); // request a face name "Arial"
	font.CreateFontIndirect(&lf); // create the font
	CFont* def_font = pDC->SelectObject(&font);

	int textStartX = (int) (0.8 * w);
	int textStartY = (int) (base ? 0.1 * h : 0.6 * h);

	pDC->TextOutW(textStartX, textStartY, CString()); //по умолчанию указанные координаты – это координаты
	
	char buffer[100];
	
	string profit_text("Profit: ");
	profit_text = profit_text + (string) itoa(m.profit, buffer, 10);
	
	string drawdown_text("Drawdown: ");
	drawdown_text = drawdown_text + (string) itoa(m.drawdown, buffer, 10);


	pDC->TextOutW(textStartX + 40, textStartY, CString(profit_text.c_str()));
	pDC->TextOutW(textStartX + 40, textStartY + 30, CString(drawdown_text.c_str()));

	if (base == true) {	
		pDC->TextOutW(10, 10, CString("Base"));

		string timeframe_text("Timeframe: ");
		timeframe_text = timeframe_text + (string) itoa(pDoc->best_parameters.first, buffer, 10);
		
		string alpha_text("Alpha: ");
		std::ostringstream s;
		s << pDoc->best_parameters.second;
		alpha_text = alpha_text + s.str();

		pDC->TextOutW(10, 40, CString(timeframe_text.c_str()));
		pDC->TextOutW(10, 70, CString(alpha_text.c_str()));

	} else {
		pDC->TextOutW(10, 10 + h / 2, CString("Test"));
	}
	// верхнего левого угла прямоугольника, в который идет вывод текста. 
	pDC->SelectObject(def_font);
	font.DeleteObject();//	
	
}

// CderivativeView printing

BOOL CderivativeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CderivativeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CderivativeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CderivativeView diagnostics

#ifdef _DEBUG
void CderivativeView::AssertValid() const
{
	CView::AssertValid();
}

void CderivativeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CderivativeDoc* CderivativeView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CderivativeDoc)));
	return (CderivativeDoc*)m_pDocument;
}
#endif //_DEBUG


// CderivativeView message handlers
