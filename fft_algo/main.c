#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include "fft.h"

#define MAX_LINE 1024
#define AUDIO_CNT 1024
#define INPUT_FILE "./vin.dat"
#define OUTPUT_FILE "./awinic_out.dat"

long int read_data(int32_t *buf,ssize_t size,long int pos)
{
	char read_buf[MAX_LINE];
	int32_t *tmp,read_data;
	int i;
        long int ret;
        FILE*fp;
	
        tmp = buf;

	fp =fopen(INPUT_FILE,"rb");
	if(fp == NULL)
	{
		printf("%s: fopen input file failed \n",__func__);
		return -1;
	}
	
       fseek(fp,pos,SEEK_SET);
       for(i=0;i<size;i++)
       {
       if(fgets(read_buf,MAX_LINE,fp) == NULL){
            printf("%s:read failed",__func__);
			fclose(fp);
            return -1;
        }
        sscanf(read_buf,"%x",&read_data);   //string --> int32_t
        *tmp = read_data;
        tmp++;
        }
        ret = ftell(fp);
        printf("%s:read number = %d \n",__func__,i);
	fclose(fp);
	return ret;
}

int write_data(int32_t *buf,ssize_t size)
{
	ssize_t i;
        FILE*fp;
	
	fp = fopen(OUTPUT_FILE,"ab");
        if(fp == NULL)
	{
		printf("%s: fopen outout file failed \n",__func__);
		return -1;
	}
	
	for(i=0;i<size;i++)
       {
        fprintf(fp,"%08x\n",*buf);  //data --->string
        buf++;
       }
	fclose(fp);
        printf("%s:write number = %ld \n",__func__,i);
	return 0;
}
void add_audio_channel(int32_t *dst,int32_t* src)
{
   int i;
   for(i=0;i<AUDIO_CNT;i++)
   {
      dst[i*2]   = src[i];
      dst[i*2+1] = src[i];
   }

}

void sub_audio_channel(int32_t *dst,int32_t* src)
{
    int i;
    for(i=0;i<AUDIO_CNT;i++)
    {
      dst[i] = src[i*2];
    }

}

void main(void)
{
    ssize_t i,j;
    FILE* fin,*fout;
    int32_t *in_data=NULL;
    double *in_tmp=NULL;


    in_tmp  = (double*)calloc(AUDIO_CNT+1,sizeof(double));
    if(in_tmp== NULL){
       printf("%s: malloc in data failed \n",__func__);
       exit(1);
    }

    for(j=0;j<1024;j++){
        in_tmp[j] =2 + 3*cos(2.0*3.1415926535897932384*50*j/1024 + 2) +1.5*cos(2.0*3.1415926535897932384*75*j/1024 + 2);
        //printf("%s: data %d = %f\n",__func__,j,in_tmp[j]);
    }

    printf("Get data end\n");
    FFT_Interface((void*)in_tmp,AUDIO_CNT);

    free(in_data);
    free(in_tmp);
	
    exit(1);
}
