/*************************************************************************
	> File Name: log.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月15日 星期二 09时47分51秒
    > Version     : 
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>
#include<math.h>

#define LN10   2.3025850929940456840179914547
#define LNR    0.2002433314278771112016301167


double log_algo(double data)
{
   double sum = 1;
   double data2 = data*data;
   double pow   = data2;
   double coe   = 1;
    
    //ln(x-1/x+1) = 2*x*(1+ 1/3*x^2 + 1/5*x^4 +1/7*x^6 + ......) 

   for(int i= 3;coe>0.000000000001;i= i+2)
       sum = sum + (coe *=pow)/(i);

   return 2*data*sum;
}


double Log_X(double value)
{
    if(value < 0)
       return -1;

    int k10=0,k1=0;
    double tmp_data;

    for(;value>1;k10++) value /= 10;
    for(;value<=0.1;k10--) value *=10;  //value-> [0.1,1]
    for(;value<0.9047;k1--) value *= 1.2217;

    //lnx = k10*ln10 + k1*ln1 + ln(y-1/y+1) x=10^k10 * 1^K1 * y 
    tmp_data = (value - 1)/(value+1);

    return (LN10*k10 + LNR*k1); //+ log_algo(tmp_data));
}


double Log_10(double value)
{
   return Log_X(value)/LN10;
}


void main()
{
   double i,ret;
   struct timeval tv1,tv2;
   
   gettimeofday(&tv1,NULL);
   for(i=0;i<2048;i++){
       ret = Log_10(65535);
       //ret= log(65536);
    }
   gettimeofday(&tv2,NULL);
   
   printf("sec = %d ,u_sec=%d \n",tv2.tv_sec-tv1.tv_sec,tv2.tv_usec-tv1.tv_usec);

}
