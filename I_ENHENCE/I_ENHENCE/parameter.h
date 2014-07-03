//by hzs 2014/6/29
#include<stdlib.h>
#include<iostream>
#include<stdint.h>
#include<stdio.h>
#include "fftw32\fftw3.h"

using namespace std;

#define W_LENGTH 20
#define HEIGHT 320
#define WIDTH 256

//小波变换部分
struct Wavelet_Trans
{
	string Name;
	//小波名称
	int Level;
	//小波变换层次
	double Enc_Wave[3][3];
	//小波增强部分，纵向代表小波变换的层次，横向代表小波变换增强的方向
	//0,1,2分别代表水平，垂直，对角三个方向
};

//傅里叶变换部分
struct Four_Trans
{
	double Threshold;
	//区分高频与低频的阈值
	double H_enhence;
	//高频增强系数
	double L_enhence;
	//低频增强系数
};

//函数内部数据结构
struct Wavelet
{
	int Length1;
	int Length2;
	double F_wavelet[W_LENGTH];
	double M_wavelet[W_LENGTH];
	uint32_t a;
};

typedef  struct  MAP_HEADER
{ 
	unsigned long       bfSize;       //文件大小，以字节为单位
	unsigned short int  bfReserverd1; //位图文件保留字，必须为0 
	unsigned short int  bfReserverd2; //位图文件保留字，必须为0 
	unsigned long       bfbfOffBits;  //位图文件头到数据的偏移量，以字节为单位
}BitMapFileHeader; 
typedef  struct  INFO_HEADER
{ 
	long biSize;                        //该结构大小，字节为单位
	long  biWidth;                     //图形宽度以象素为单位
	long  biHeight;                     //图形高度以象素为单位
	short int  biPlanes;               //目标设备的级别，必须为1 
	short int  biBitcount;             //颜色深度，每个象素所需要的位数
	long  biCompression;        //位图的压缩类型
	long  biSizeImage;              //位图的大小，以字节为单位
	long  biXPelsPermeter;       //位图水平分辨率，每米像素数
	long  biYPelsPermeter;       //位图垂直分辨率，每米像素数
	long  biClrUsed;            //位图实际使用的颜色表中的颜色数
	long  biClrImportant;       //位图显示过程中重要的颜色数
}BitInfoHeader; 