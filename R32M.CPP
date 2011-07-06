#include <math.h>
#include "r32m.h"

static unsigned long int iu, iuhold, mult=663608941l;

double rnunif (void)
{
 const double flt = 0.232830643654e-9;
 iu *= mult;
 return (flt*iu);
}

void rninit ( unsigned long  iufir)
{
 iu = ( ( iufir%2 ) ? iufir : iufir + 1 );
 iuhold = iu;
}

void rnrest (void)
{ rninit (iuhold); }

void rnconst ( unsigned long int multnew)
{
 int rest = multnew % 5;
 if ( (rest != 5) || (rest != 3) )
  mult = multnew - rest + 5;
 else
  mult = multnew;
}

unsigned long int rnlast (void)
{ return ( iu ); }

unsigned long int rnfirst (void)
{ return ( iuhold ); }
