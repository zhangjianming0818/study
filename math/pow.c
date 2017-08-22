/*************************************************************************
	> File Name: pow.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月22日 星期二 17时17分48秒
    > Version     : 
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>


float pow10f(float value)
{

    return expf(2.302585092994*value);
}

void main(void)
{

    int  i;
    float x=0.00001;
    for(i=1;i< 1000;i++)
    {
        printf("%f \n",pow10f(x));
        printf("%f \n",pow(10,x));
        x=x + 0.001;
    }

}
