//
// Copyright (c) 1995 Crescent Division of Progress Software Corporation
//

#include <math.h>
#include "probdist.h"

const double Eps = 1e-15;

int fequal( double a, double b )
{
	return  (fabs(a-b) < Eps ) ? 1 : 0;
}

int fcompare( double a, double b )
{
	double d = a-b;
	int res;

	if( fabs(d) < Eps )
		res = 0;
	else if( d < 0.0 )
		res = -1;
	else
		res = 1;
	return res;
}
//
// Normal distribution
//

void NORMAL(int IFLAG, double &X, double &PROB)
{
/*    ' Normal distribution subroutine

	' Input

	'  IFLAG = type of computation
	'      1 = given x, compute probability
	'      2 = given probability, compute x

	' Output (or input)

	'  X     = x value
	'  PROB  = probability; 0 < PROB < 1
*/
	double x1;

	switch ( IFLAG ) {
	case 1:
	   //  N(x)
		x1 = fabs(X);

		if (x1 > 7)
			PROB = 0;
		else {
			PROB = 1 + x1 * (0.049867347 + x1 * (0.0211410061 + x1 *
				(0.0032776263 + x1 * (0.0000380036 + x1 *
				(0.0000488906 + x1 * 0.000005383)))));
			PROB = 0.5 * pow( PROB, -16);
		}

		if (X < 0.0)  PROB = 1.0 - PROB;
		break;
	case 2:
		// N(p)
		if( fequal(PROB,0.5) ) {
			X = 0.0;
			return;
		}

		x1 = (PROB > 0.5) ? 1.0 - PROB : PROB;

		if( fequal(x1,0.05) )
			X = 1.64485;
		else if( fequal(x1,0.025) )
			X = 1.95996;
		else if( fequal(x1,0.01) )
			X = 2.32635;
		else if( fequal(x1,0.005) )
			X = 2.57583;
		else {
			x1 = -log(4.0 * x1 * (1.0 - x1));
			X = (-3.231081277E-09 * x1 + 8.360937017E-08) * x1 - 0.00000104527497;
			X = (X * x1 + 0.000005824238515) * x1 + 0.000006841218299;
			X = ((X * x1 - 0.0002250947176) * x1 - 0.000836435359) * x1 + 0.03706987906;
			X = X * x1 + 1.570796288;
			X = sqrt(x1 * X);
		}

	   if( PROB > 0.5 ) X = -X;
	}

}

double pNormal(double x)
{
	double prob;
	NORMAL(1,x,prob);
	return 1.0-prob;
}

double xNormal(double prob)
{
	double x, p=1.0-prob;
	NORMAL(2,x,p);
	return x;
}

//
//  Chi-2 distribution
//

void CHI( int IFLAG, double N, double &X, double &PROB )
{
/*	' Chi-squared distribution subroutine

	' Input

	'  IFLAG = type of computation
	'      1 = given x, compute probability
	'      2 = given probability, compute x
	'  N     = degrees of freedom; N >= 1

	' Output (or input)

	'  X     = x value; X >=0
	'  PROB  = probability; 0 < PROB <= 1

	' NOTE: requires subroutine pNORMAL.BAS
*/
	int i;
	switch( IFLAG ) {
	case 1:
	//	Chi(x)

		double X1, X3;
		double QF, QP, QX0, QX1, QX2, QX3;
		int QPIndex, iN;

		if ( fequal(X,0.0) ) {
		  PROB = 1.0;
		  return;
		}

		if( N > 40 ) {
			X3 = 2.0 / (9.0 * N);
			X1 = (pow( X/N, 0.3333333333) - 1.0 + X3 ) / sqrt(X3);
			NORMAL(1, X1, PROB);
			return;
		}

		iN = N;
		QPIndex = 2 - iN + 2 * (iN / 2);

		X3 = sqrt(X);

		if( QPIndex != 1) {
			PROB = exp(-X / 2);
			QF = PROB / 2;
		}
		else {
			NORMAL(1, X3, PROB);
			PROB = 2.0 * PROB;
			QF = 0.3989422804 * exp(-X / 2.0) / X3;
		}

		for( i= QPIndex; i < iN;  i += 2 ) {
			QF = QF * X / i;
			PROB = PROB + 2.0 * QF;
		}
		return;
	case 2:
	//	Chi(p)

		if( N == 1) {
			X1 = PROB;
			X1 = X1 / 2;
			NORMAL(2, X, X1);
			X = X * X;
			return;
		}
		else if (N == 2) {
		  X = -2 * log(PROB);
		  return;
		}

		QX1 = 0;
		QX2 = 1;
		QX3 = 0.5;
		QP = PROB;

		do {
			X = 1.0 / QX3 - 1.0;

			CHI(1, N, X, PROB);

			if (PROB <= QP)
				QX1 = QX3;
			else
				QX2 = QX3;

			QX0 = QX3;
			QX3 = (QX1 + QX2) / 2;
		} while ( fabs(QX3 - QX0) > (0.00001 * QX3));

		X = 1 / QX3 - 1;
		PROB = QP;
		return;
	}
}

double pChi(double x, int n)
{
	double prob;
	CHI(1,(double)n,x,prob);
	return 1.0-prob;
}

double xChi(double prob, int n)
{
	double x, p=1.0-prob;
	CHI(2,(double)n,x,p);
	return x;
}

