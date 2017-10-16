//=================== Математические вычисления ======================

#include "math.h"

#define M_PI ((float)3.141592653589793)
#define M_PI12 (M_PI/12.F)
#define M_PI6 (M_PI/6.F)
#define M_PI2 (M_PI/2.F)
/* square root of 3 */
#define SQRT3 ((float)1.732050807569)

//------------- Вычисление арктангенса -----------------
float Arctan(float x)
{
  int sta=0,sp=0;
  float x2,a;
  // проверяем знак аргумента
  if(x<0.F) {x=-x;sta|=1;}
  /* check up the invertation */
  if(x>1.F) {x=1.F/x;sta|=2;}
  /* process shrinking the domain until x<PI/12 */
  while(x>M_PI12) {
    sp++; a=x+SQRT3; a=1.F/a; x*=SQRT3; x-=1.F; x*=a;
  }
  /* calculation core */
  x2=x*x; a=x2+1.4087812F; a=0.55913709F/a; a+=0.60310579F;
  a-=0.05160454F*x2; a*=x;
  /* process until sp=0 */
  while(sp>0) {a+=M_PI6;sp--;}
  /* invertation took place */
  if(sta&2) a=M_PI2-a;
  /* sign change took place */
  if(sta&1) a=-a;
  return(a);
}
//--------------------------------------------------------------------

/* 4 iterations needed for the single precision */
#define ITNUM 4

//------------ Вычисление квадратного корня -----------------
float Sqroot(float x)
{
  int sp=0,i,inv=0;
  float a,b;
  if(x<=0.F) return(0.F);
  /* argument less than 1 : invert it */
  if(x<1.F) {x=1.F/x;inv=1;}
  /* process series of division by 16 until argument is <16 */
  while(x>16.F) {sp++;x/=16.F;}
  /* initial approximation */
  a=2.F;
  /* Newtonian algorithm */
  for(i=ITNUM;i>0;i--)
  {
    b=x/a; a+=b; a*=0.5F;
  }
  /* multiply result by 4 : as much times as divisions by 16 took place */
  while(sp>0) {sp--;a*=4.F;}
  /* invert result for inverted argument */
  if(inv) a=1.F/a;
  return(a);
}
//--------------------------------------------------------------------