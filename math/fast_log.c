/*************************************************************************
	> File Name: fast_log.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月22日 星期二 17时31分34秒
    > Version     : 
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>


#define LOG2F_COE1 ((float)1.23149591368684)
#define LOG2F_COE2 ((float)-4.11852516267426)
#define LOG2F_COE3 ((float)6.02197014179219)
#define LOG2F_COE4 ((float)-3.13396450166353)

/* ----------------------------------------------------------------------
* ** Fast approximation to the log2() function.  It uses a two step
* ** process.  First, it decomposes the floating-point number into
* ** a fractional component F and an exponent E.  The fraction component
* ** is used in a polynomial approximation and then the exponent added
* ** to the result.  A 3rd order polynomial is used and the result
* ** when computing db20() is accurate to 7.984884e-003 dB.
* ** frexpf 将float 分解为指数e，基数 f ，log2（x）= log2（2^e * f）
* ** --> = log2(2^e) + log2(f) = e + log2(f) log2(f) 泰勒展开求近似值
* ** ------------------------------------------------------------------- */
static float log2f_approx(float input)
{
   float out_data = 0;
   float f;
   int   e;
   
   f = frexpf(fabsf(input),&e);

    //printf("%d  %f \n",e , f);
   
    //out_data = coe1*f*f*f + coe2*f*f + coe3*f + coe4 + e
    //out_data = LOG2F_COE1*f*f*f + LOG2F_COE2*f*f + LOG2F_COE3*f + LOG2F_COE4 +e;
    out_data  = LOG2F_COE1;
    out_data *= f;
    out_data += LOG2F_COE2;
    out_data *= f;
    out_data += LOG2F_COE3;
    out_data *= f;
    out_data += LOG2F_COE4 + e;

   return out_data;
}

static float log10f_approx(float value)
{
    return log2f_approx(value)*0.3010299956639812;  //log10 = log2(x)/log2(10);
}

void main(void)
{
   int i;
   float x = 0.0001;
   for(i=(1 <<16);i< (1<<25);i++)
    {   
        printf("approx %f \n",log10f_approx(100));
        printf("log    %f \n",log10f(100));
        x += 0.001;

    }


}

