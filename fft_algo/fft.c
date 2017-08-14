/*********************************************************************************
    > File  Name    : fft.c
    > Function      : fft algo convert time area to frequency area
    > Author        :jimmy
    > Mail          :
    > Created Time  :2017年08月04日 星期五 09时41分26秒
    >Version        :v1.0
 * *******************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include<string.h>

#include"fft.h"


//Complex add
static void Complex_Add(Complex* src1,Complex* src2,Complex* dst)
{
    dst->real = src1->real + src2->real;
    dst->imag = src1->imag + src2->imag;  
}

static void Complex_Sub(Complex* src1,Complex* src2,Complex* dst)
{
    dst->real = src1->real - src2->real;
    dst->imag = src1->imag - src2->imag;
}

static void Complex_Multy(Complex* src1,Complex* src2,Complex* dst)
{
    double real1,real2;
    double imag1,imag2;

    real1 = src1->real;
    real2 = src2->real;
    imag1 = src1->imag;
    imag2 = src2->imag;

    dst->real = real1*real2 - imag1*imag2;
    dst->imag = real1*imag2 + real2*imag1;
}

//Wnk e^-i(2Pi/N)k 
static void Get_WN(double Wk,double N_size,Complex*dst)
{
    //e^-it = cos(it) - i*sin(it)
    double x = 2.0*PI*Wk/N_size;
    dst->real = cos(x);
    dst->imag = -sin(x);
}
//Re-Sort inputdata to FFT
static int FFT_InputData_Remap(double* src,ssize_t size)
{
   ssize_t i;

   if(src==NULL )
   {
       printf("%s: Data Is NULL \n",__func__);
       return -1;
   }
   if(size ==1)
       return 0;

   double *tmp = (double*)calloc(size,sizeof(double));
   if(tmp == NULL)
    {
       printf("%s: alloc memory error \n",__func__);
       return -1;
    }

    for(i=0;i<size;i++)
    {
        if(i%2 == 0)                      //even
          tmp[i/2] = src[i]; 
        else                              //odd
          tmp[(size+i)/2] = src[i];
    }

    for(i=0;i<size;i++)
    {
        src[i] = tmp[i];
    }
    free(tmp);
    tmp = NULL;
    FFT_InputData_Remap(src,size/2);
    FFT_InputData_Remap(src + size/2,size/2);

    return 0;
}

//FFT Function
static int FFT_Convert(double *src,Complex*dst,ssize_t size)
{
    ssize_t i,j;
    unsigned int grade;            //FFT grade 2^grade
    int Wk;                        //WN index e^i*(2*PI*Wk/N)
    int ret,near;
    Complex Wn,temp;                    //FFT Wn

    grade = log2(size);
    
    if(size != (1<<grade))        //size most 2 integer power
    {
        printf("%s: Data Is not the integer power of 2 \n",__func__);
        return -1;
    }
    
    Complex*src_comp = (Complex*)calloc(size,sizeof(Complex));
    if(src_comp == NULL)
    {
        printf("%s: alloc Memory error \n",__func__);
        return -1;
    }

    ret = FFT_InputData_Remap(src,size);
    if(ret == -1){
        printf("%s: remap data error \n",__func__);
        return -1;
    }

    for(i=0;i<size;i++){
       src_comp[i].real = src[i];
       src_comp[i].imag = 0;
    }

    for(i=0;i<grade;i++)
    {
        Wk = 0;
        for(j=0;j<size;j++)
        {
            if((j/(1<<i)%2) == 1)   //find FFT ODD to calculation [K+n/2]
            {
                Get_WN(Wk,size,&Wn);
                Complex_Multy(&src_comp[j],&Wn,&src_comp[j]);    //WN*X2[k]
                Wk += 1<<(grade-i-1);                            //next Wk
                near = j-(1<<i);                                 //[k]
                temp.real  = src_comp[near].real;
                temp.imag  = src_comp[near].imag;
                Complex_Add(&temp,&src_comp[j],&src_comp[near]);         //X1k        = x1k + Wn*x2k
                Complex_Sub(&temp,&src_comp[j],&src_comp[j]);            //X2(k+/n/2) = x1k - Wn*x2k

            }
            else
            {
               Wk = 0;             // part group calculation end 
            }

        }
    }

    for(i=0;i<size;i++)
    {
       dst[i].real = src_comp[i].real;
       dst[i].imag = src_comp[i].imag;
    }
    free(src_comp);
    src_comp = NULL;
    return 0;
}
static void Cal_Amp(double* dst,Complex *src,ssize_t size)
{
    double real,imag;
    ssize_t i;
    dst[0] = src[0].real/(2*size);
    for(i=1;i< size;i++)
    {
       real = src[i].real;
       imag = src[i].imag;
       real = real*real;
       imag = imag*imag;
       real = sqrt(real + imag);
       dst[i] = real/size;
    }

}
void FFT_Interface(void* buffer,ssize_t size)
{
    //int32_t* InputData = buffer;
    ssize_t i,count = size;
    double *data= (double*)buffer;
    if(buffer== NULL || size <=0)
    {
        printf("%s: Parameters error \n",__func__);
        return;
    }
    /*double *data = (double*)calloc(count,sizeof(double));
    if(data == NULL)
    {
        printf("%s: alloc memory error \n",__func__);
        return;
    }*/
    Complex *ret_data = (Complex*)calloc(count,sizeof(Complex));
    if(ret_data == NULL)
    {
        printf("%s: alloc memory error \n",__func__);
        goto err;
    }


    FFT_Convert(data,ret_data,count);


    Cal_Amp(data,ret_data,count/2);
    
    for(i=0;i<count/2;i++){
        if(data[i] > 0.00000001){
            printf("%s: real = %f, imag = %f \n",__func__,ret_data[i].real,ret_data[i].imag);
            printf("%s: result = %f \n",__func__,data[i]);
        }   
    }

    //free(data);
    free(ret_data);
    return;
err:
   // free(data);
    //data = NULL;
    return;
}


