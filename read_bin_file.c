/*************************************************************************
	> File Name: read_bin_file.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月24日 星期四 18时55分25秒
    > Version     : 
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

#define PATH_NAME "./test.bin"
#define BUF_MAX  1024

char data[BUF_MAX] = {0};

ssize_t  read_file(char* path,void *buffer,ssize_t bytes)
{
    int fp;
    ssize_t ret;

    fp = open(path,O_RDONLY);
    if(fp<0)
    {
        printf("%s:open file error\n",__func__);
        return -1;
    }

    ret = read(fp,buffer,bytes);
    if(ret <0)
    {
        printf("%s: read error \n",__func__);
        close(fp);
        return -1;
    }

    close(fp);

    return ret;
}
ssize_t write_bin_file(char *path,void* buffer ,ssize_t bytes)
{
    int fd;
    int ret;

    fd = open(path,O_RDWR|O_CREAT,0666);
    if(fd <0)
    {
        printf("%s: open file failed \n",__func__);
        return -1;
    }

    ret = write(fd,buffer,bytes);
    if(ret < 0)
    {
        printf("%s: write data failed \n",__func__);
        close(fd);
        return -1;
    }

    close(fd);
    return ret;
}

void init_data(int32_t*buffer)
{
   //int count;

   buffer[0] = 384;     //shift_cnt
   buffer[1] = 3;       //filter_num
   buffer[2] = (int32_t)(0.768*1048576);  //filter_gain 0.45*16/10
   buffer[3] = 0;       //filter 0
    buffer[4]= 1048576;
    buffer[5]= -1263248;
    buffer[6]= 2077004;
    buffer[7]= -1028440;
    buffer[8]= 10914;
    buffer[9]= 1048576; //filter 1
    buffer[10] = -2071293;
    buffer[11] = 1022761;
    buffer[12] = 1862749;
    buffer[13] = -844967;
    buffer[14] = 1048576;
    buffer[15] = 1048576; //filter 2
    buffer[16] = -2366814;
    buffer[17] = 1384157;
    buffer[18] = 2015657;
    buffer[19] = -977092;
    buffer[20] = 1048576;

    buffer[21] = 0;        //AlphaAttr pd
    buffer[22] = 1048357;  //AlphaRel
    buffer[23] = 0;        //HoldAttr
    buffer[24] = 192;      //Hold_Rel

    buffer[25] = -11593936; //gf ex treshold
    buffer[26] = 100;       //ratio
    buffer[27] = 5;         //knee
    buffer[28] = -1038090;  //Slope
    buffer[29] = 2621440;   //Width
    buffer[30] = 104857;    //GainCoe

    buffer[31] = -1660551;  //tresholdvol
    buffer[32] = 100;       //ratio
    buffer[33] = 5;         //knee
    buffer[34] = -1038090;  //Slope
    buffer[35] = 2621440;   //Width
    buffer[36] = 104857;    //GainCoe
 
    buffer[37] = 0;         //AlphaAttr
    buffer[38] = 1048357;   //AlphaRel

}

void main(void)
{
  char buf[BUF_MAX];
  int ret,i;

  init_data((int32_t*)data);

    write_bin_file(PATH_NAME,data,39*4);

  ret = read_file(PATH_NAME,buf,BUF_MAX);

  if(ret<0)
    exit(-1);

  int32_t *data = (int32_t*)buf;

  for(i=0;i<ret/4;i++)
    printf("%s: %d = %d \n",__func__,i,data[i]);

}
