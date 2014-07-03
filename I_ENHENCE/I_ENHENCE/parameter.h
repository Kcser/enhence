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

//С���任����
struct Wavelet_Trans
{
	string Name;
	//С������
	int Level;
	//С���任���
	double Enc_Wave[3][3];
	//С����ǿ���֣��������С���任�Ĳ�Σ��������С���任��ǿ�ķ���
	//0,1,2�ֱ����ˮƽ����ֱ���Խ���������
};

//����Ҷ�任����
struct Four_Trans
{
	double Threshold;
	//���ָ�Ƶ���Ƶ����ֵ
	double H_enhence;
	//��Ƶ��ǿϵ��
	double L_enhence;
	//��Ƶ��ǿϵ��
};

//�����ڲ����ݽṹ
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
	unsigned long       bfSize;       //�ļ���С�����ֽ�Ϊ��λ
	unsigned short int  bfReserverd1; //λͼ�ļ������֣�����Ϊ0 
	unsigned short int  bfReserverd2; //λͼ�ļ������֣�����Ϊ0 
	unsigned long       bfbfOffBits;  //λͼ�ļ�ͷ�����ݵ�ƫ���������ֽ�Ϊ��λ
}BitMapFileHeader; 
typedef  struct  INFO_HEADER
{ 
	long biSize;                        //�ýṹ��С���ֽ�Ϊ��λ
	long  biWidth;                     //ͼ�ο��������Ϊ��λ
	long  biHeight;                     //ͼ�θ߶�������Ϊ��λ
	short int  biPlanes;               //Ŀ���豸�ļ��𣬱���Ϊ1 
	short int  biBitcount;             //��ɫ��ȣ�ÿ����������Ҫ��λ��
	long  biCompression;        //λͼ��ѹ������
	long  biSizeImage;              //λͼ�Ĵ�С�����ֽ�Ϊ��λ
	long  biXPelsPermeter;       //λͼˮƽ�ֱ��ʣ�ÿ��������
	long  biYPelsPermeter;       //λͼ��ֱ�ֱ��ʣ�ÿ��������
	long  biClrUsed;            //λͼʵ��ʹ�õ���ɫ���е���ɫ��
	long  biClrImportant;       //λͼ��ʾ��������Ҫ����ɫ��
}BitInfoHeader; 