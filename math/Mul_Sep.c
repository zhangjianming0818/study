/*********************************************************************************************************
	> File Name: Mul_Sep.c
	> Author:jimmy 
	> Mail: 
	> Created Time: 2017年08月17日 星期四 09时41分23秒
    > Version     :
    > function    : 用于2个32位有符号整数拆解为高16位 地15位 相剩，在处以扩大倍数 2^Exp_bit 
                    其中高16位建议不要超过15位防止溢出，用于浮点数扩大取整后平方防止溢出
                    exp：

                      (1.0×2^Exp_Bit)*(1.0*2^Exp_Bit)  >> Exp_Bit 保持扩大倍数在2^Exp_Bit
                             a      *  b               >> Exp_Bit 当a×b 大于32位，又不能使用64位时，拆解
 ********************************************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>

#define LOW_BIT 15
#define LOW_BIT_MASK 0x80007fff      //Low 15 bit
#define H_MUL_BIT 30                 //15*2
#define SIG_BIT_MASK 0x80000000

int32_t Multiply_Separate(int32_t src1,int32_t src2,int32_t Exp_Bit)
{
   int sig_bit = 1;
   if((src1&SIG_BIT_MASK)^(src2&SIG_BIT_MASK))
    sig_bit = -1;                                   //get the sign after multiply

   src1 = labs(src1);                               //get absolute value 
   src2 = labs(src2);

   int32_t HightSrc1 = src1 >> LOW_BIT;            //get Hight bit 
   int32_t LowSrc1   = src1&LOW_BIT_MASK;          // get low bit

   int32_t HightSrc2 = src2 >> LOW_BIT;
   int32_t LowSrc2   = src2&LOW_BIT_MASK;
   
   int32_t HighMul  = HightSrc1*HightSrc2;                     //Hight bit multiply
   int32_t MidMul   = HightSrc1*LowSrc2 + HightSrc2*LowSrc1;   // Hight * Low bit
   int32_t LowMul   = LowSrc1*LowSrc2;                         //low bit multiply

   HighMul = HighMul<<(H_MUL_BIT-Exp_Bit);                     //Get real Hight  after divide Exp
   int index = LOW_BIT - Exp_Bit;
   if(index < 0)
    MidMul = MidMul >> (-index);
   else
    MidMul = MidMul << index;                                 //Get mid bit after divide exp 

    LowMul  =  LowMul >> Exp_Bit;

    int32_t result = HighMul + MidMul + LowMul;               // Get multiply result
     
    result = sig_bit*result;                                  // Get real multiply result
    return result;
}



void main(void)
{
    int32_t ret;

    ret = Multiply_Separate(65536,65536,2);
    printf("self %d \n",ret);


}
