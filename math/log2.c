/*************************************************************************
	> File Name: log2.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月16日 星期三 13时46分06秒
    > Version     : 
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>

#define LN10 2.302585092994045684017991454
#define LN2  0.6931471805599


float logalgo(float data,int step)
{
   double sum = 1;
   double data2 = data*data;
   double pow   = data2;
   double coe   = 1;
   int cnt = 3 + (step * 2);
    //ln(x-1/x+1) = 2*x*(1+ 1/3*x^2 + 1/5*x^4 +1/7*x^6 + ......) 

   for(int i= 3;i<cnt;i= i+2)
       sum = sum + (coe *=pow)/(i);

   return 2*data*sum;
}


static float Log10f(float value)
{
   float std = value;
   float tran;
   float result;
   int  exp_10 = 0;
   int  exp_2 = 0;

   if(std<0)
      return 1;
   for(;std>1.3333;exp_10++) std /=10;
   for(;std<0.6667;exp_2--)  std *=2;
 
   tran = (std-1)/(std+1);
   
    result = exp_10*LN10 + exp_2*LN2 + logalgo(tran,10);

    return result/LN10;

}


void main(void)
{

  float ret ,value = 65536;
  int i;
  struct timeval tv1,tv2;
  gettimeofday(&tv1,NULL);
  for(i=0;i<2048;i++){
     ret = Log10f(value);
  }
  gettimeofday(&tv2,NULL);

  printf("time %d : %d  \n",tv2.tv_sec-tv1.tv_sec,tv2.tv_usec-tv1.tv_usec);

}
