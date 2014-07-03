#include "stdio.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <malloc.h>

#define INCLUDE_utilityFunBMP 0

#define WIDTHBYTES(bits) (((bits)+31)/32*4)

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;

//λͼ�ļ�ͷ��Ϣ�ṹ����
//���в������ļ�������Ϣ�����ڽṹ����ڴ�ṹ������Ҫ�Ǽ��˵Ļ���������ȷ��ȡ�ļ���Ϣ��
typedef struct tagBITMAPFILEHEADER {
	DWORD bfSize; //�ļ���С
	WORD bfReserved1; //�����֣�������
	WORD bfReserved2; //�����֣�ͬ��
	DWORD bfOffBits; //ʵ��λͼ���ݵ�ƫ���ֽ�������ǰ�������ֳ���֮��
} BITMAPFILEHEADER;

//��ϢͷBITMAPINFOHEADER��Ҳ��һ���ṹ���䶨�����£�
typedef struct tagBITMAPINFOHEADER{
	//public:
	DWORD biSize; //ָ���˽ṹ��ĳ��ȣ�Ϊ40
	LONG biWidth; //λͼ��
	LONG biHeight; //λͼ��
	WORD biPlanes; //ƽ������Ϊ1
	WORD biBitCount; //������ɫλ����������1��2��4��8��16��24���µĿ�����32
	DWORD biCompression; //ѹ����ʽ��������0��1��2������0��ʾ��ѹ��
	DWORD biSizeImage; //ʵ��λͼ����ռ�õ��ֽ���
	LONG biXPelsPerMeter; //X����ֱ���
	LONG biYPelsPerMeter; //Y����ֱ���
	DWORD biClrUsed; //ʹ�õ���ɫ�������Ϊ0�����ʾĬ��ֵ(2^��ɫλ��)
	DWORD biClrImportant; //��Ҫ��ɫ�������Ϊ0�����ʾ������ɫ������Ҫ��
} BITMAPINFOHEADER;


//��ɫ��Palette����Ȼ�������Ƕ���Щ��Ҫ��ɫ���λͼ�ļ����Եġ�24λ��32λ�ǲ���Ҫ��ɫ��ġ�
//���ƺ��ǵ�ɫ��ṹ���������ʹ�õ���ɫ������

typedef struct tagRGBQUAD {
	//public:
	BYTE rgbBlue; //����ɫ����ɫ����
	BYTE rgbGreen; //����ɫ����ɫ����
	BYTE rgbRed; //����ɫ�ĺ�ɫ����
	BYTE rgbReserved; //����ֵ
} RGBQUAD;

/* ��ȡBMP�ļ���غ��� */
/*int exampleMain(void)
{
    BITMAPFILEHEADER bitHead;
    BITMAPINFOHEADER bitInfoHead;
    FILE* pfile;

    char strFile[50];
    printf("����bpm�ļ�:\n");
    scanf("%s",strFile);

    pfile = fopen(strFile,"rb");//���ļ�

    if(pfile!=NULL)
    {
        printf("�ɹ�.\n");
        //��ȡλͼ�ļ�ͷ��Ϣ
        WORD fileType;
        fread(&fileType,1,sizeof(WORD),pfile);
        if(fileType != 0x4d42)
        {
            printf("file is not .bmp file!");
            system("pause");
            return 0;
        }

        fread(&bitHead,1,sizeof(tagBITMAPFILEHEADER),pfile);

        // showBmpHead(&bitHead);
        printf("\n\n");

        //��ȡλͼ��Ϣͷ��Ϣ
        fread(&bitInfoHead,1,sizeof(BITMAPINFOHEADER),pfile);
        // showBmpInforHead(&bitInfoHead);
        printf("\n");
    }
    else
    {
        printf("��ʧ��!\n");
        system("pause");
        return 0;
    }


    tagRGBQUAD *pRgb ;

	if(bitInfoHead.biBitCount < 24)//�е�ɫ��
	{
		//��ȡ��ɫ�̽���Ϣ
		long nPlantNum = long(pow(2,double(bitInfoHead.biBitCount))); // Mix color Plant Number;
		pRgb=(tagRGBQUAD *)malloc(nPlantNum*sizeof(tagRGBQUAD));
		memset(pRgb,0,nPlantNum*sizeof(tagRGBQUAD));
		int num = fread(pRgb,4,nPlantNum,pfile);

		printf("Color Plate Number: %d\n",nPlantNum);

		printf("��ɫ����Ϣ:\n");
		for (int i =0; i<nPlantNum;i++)
		{
			if (i%5==0)
			{
				printf("\n");
			}
			// showRgbQuan(&pRgb[i]);

		}
		printf("\n");

	}

	int width = bitInfoHead.biWidth;
	int height = bitInfoHead.biHeight;
	//�����ڴ�ռ��Դͼ�����ڴ�
	int l_width = WIDTHBYTES(width* bitInfoHead.biBitCount);//����λͼ��ʵ�ʿ�Ȳ�ȷ����Ϊ32�ı���
	BYTE *pColorData=(BYTE *)malloc(height*l_width);
	memset(pColorData,0,height*l_width);
	long nData = height*l_width;

	//��λͼ������Ϣ����������
	fread(pColorData,1,nData,pfile);

	//��λͼ����ת��ΪRGB����
	tagRGBQUAD* dataOfBmp;
	dataOfBmp = (tagRGBQUAD *)malloc(width*height*sizeof(tagRGBQUAD));//���ڱ�������ض�Ӧ��RGB����
	memset(dataOfBmp,0,width*height*sizeof(tagRGBQUAD));

	if(bitInfoHead.biBitCount<24)//�е�ɫ�壬��λͼΪ�����ɫ
	{
		int k;
		int index = 0;
		if (bitInfoHead.biBitCount == 1)
		{
			for(int i=0;i<height;i++)
				for(int j=0;j<width;j++)
				{
					BYTE mixIndex= 0;
					k = i*l_width + j/8;//k:ȡ�ø�������ɫ������ʵ�����������е����
					//j:��ȡ��ǰ���ص���ɫ�ľ���ֵ
					mixIndex = pColorData[k];
					switch(j%8)
					{
						case 0:
						mixIndex = mixIndex<<7;
						mixIndex = mixIndex>>7;
						break;
						case 1:
						mixIndex = mixIndex<<6;
						mixIndex = mixIndex>>7;
						break;
						case 2:
						mixIndex = mixIndex<<5;
						mixIndex = mixIndex>>7;
						break;

						case 3:
						mixIndex = mixIndex<<4;
						mixIndex = mixIndex>>7;
						break;
						case 4:
						mixIndex = mixIndex<<3;
						mixIndex = mixIndex>>7;
						break;

						case 5:
						mixIndex = mixIndex<<2;
						mixIndex = mixIndex>>7;
						break;
						case 6:
						mixIndex = mixIndex<<1;
						mixIndex = mixIndex>>7;
						break;

						case 7:
						mixIndex = mixIndex>>7;
						break;
					}

					//���������ݱ��浽�����ж�Ӧ��λ��
					dataOfBmp[index].rgbRed = pRgb[mixIndex].rgbRed;
					dataOfBmp[index].rgbGreen = pRgb[mixIndex].rgbGreen;
					dataOfBmp[index].rgbBlue = pRgb[mixIndex].rgbBlue;
					dataOfBmp[index].rgbReserved = pRgb[mixIndex].rgbReserved;
					index++;

				}
		}

		if(bitInfoHead.biBitCount==2)
		{
			for(int i=0;i<height;i++)
				for(int j=0;j<width;j++)
				{
					BYTE mixIndex= 0;
					k = i*l_width + j/4;//k:ȡ�ø�������ɫ������ʵ�����������е����
					//j:��ȡ��ǰ���ص���ɫ�ľ���ֵ
					mixIndex = pColorData[k];
					switch(j%4)
					{
						case 0:
						mixIndex = mixIndex<<6;
						mixIndex = mixIndex>>6;
						break;
						case 1:
						mixIndex = mixIndex<<4;
						mixIndex = mixIndex>>6;
						break;
						case 2:
						mixIndex = mixIndex<<2;
						mixIndex = mixIndex>>6;
						break;
						case 3:
						mixIndex = mixIndex>>6;
						break;
					}

					//���������ݱ��浽�����ж�Ӧ��λ��
					dataOfBmp[index].rgbRed = pRgb[mixIndex].rgbRed;
					dataOfBmp[index].rgbGreen = pRgb[mixIndex].rgbGreen;
					dataOfBmp[index].rgbBlue = pRgb[mixIndex].rgbBlue;
					dataOfBmp[index].rgbReserved = pRgb[mixIndex].rgbReserved;
					index++;


				}
		}
		if(bitInfoHead.biBitCount == 4)
		{
			for(int i=0;i<height;i++)
				for(int j=0;j<width;j++)
				{
					BYTE mixIndex= 0;
					k = i*l_width + j/2;
					mixIndex = pColorData[k];
					if(j%2==0)
					{//��
						mixIndex = mixIndex<<4;
						mixIndex = mixIndex>>4;
					}
					else
					{//��
						mixIndex = mixIndex>>4;
					}

					dataOfBmp[index].rgbRed = pRgb[mixIndex].rgbRed;
					dataOfBmp[index].rgbGreen = pRgb[mixIndex].rgbGreen;
					dataOfBmp[index].rgbBlue = pRgb[mixIndex].rgbBlue;
					dataOfBmp[index].rgbReserved = pRgb[mixIndex].rgbReserved;
					index++;

				}

		}
		if(bitInfoHead.biBitCount == 8)
		{
			for(int i=0;i<height;i++)
				for(int j=0;j<width;j++)
				{
					BYTE mixIndex= 0;

					k = i*l_width + j;

					mixIndex = pColorData[k];

					dataOfBmp[index].rgbRed = pRgb[mixIndex].rgbRed;
					dataOfBmp[index].rgbGreen = pRgb[mixIndex].rgbGreen;
					dataOfBmp[index].rgbBlue = pRgb[mixIndex].rgbBlue;
					dataOfBmp[index].rgbReserved = pRgb[mixIndex].rgbReserved;
					index++;



				}
		}
		if(bitInfoHead.biBitCount == 16)
		{
			for(int i=0;i<height;i++)
				for(int j=0;j<width;j++)
				{
					WORD mixIndex= 0;

					k = i*l_width + j*2;
					WORD shortTemp;
					shortTemp = pColorData[k+1];
					shortTemp = shortTemp<<8;

					mixIndex = pColorData[k] + shortTemp;

					dataOfBmp[index].rgbRed = pRgb[mixIndex].rgbRed;
					dataOfBmp[index].rgbGreen = pRgb[mixIndex].rgbGreen;
					dataOfBmp[index].rgbBlue = pRgb[mixIndex].rgbBlue;
					dataOfBmp[index].rgbReserved = pRgb[mixIndex].rgbReserved;
					index++;
				}
		}
	}
	else//λͼΪ24λ���ɫ
	{
		int k;
		int index = 0;
			for(int i=0;i<height;i++)
				for(int j=0;j<width;j++)
				{
					k = i*l_width + j*3;
					dataOfBmp[index].rgbRed = pColorData[k+2];
					dataOfBmp[index].rgbGreen = pColorData[k+1];
					dataOfBmp[index].rgbBlue = pColorData[k];
					index++;
				}
	}


	printf("����������Ϣ:\n");
	for (int i=0; i<width*height; i++)
	{
		if (i%5==0)
		{
			printf("\n");
		}
		// showRgbQuan(&dataOfBmp[i]);
	}

	fclose(pfile);
	if (bitInfoHead.biBitCount<24)
	{
		free(pRgb);
	}
	free(dataOfBmp);
	free(pColorData);
	printf("\n");
	system("pause");
	return 0;
}*/


/* ���֪ʶ */
/*
BMP��һ����Ӳ���豸�޹ص�ͼ���ļ���ʽ��Ҳ���������PC���ϵ�Windowsϵͳ�¼����ı�׼λͼ��ʽ��ʹ�÷�Χ�ܹ㷺��������λӳ��洢��ʽ������ͼ����ȿ�ѡ���⣬�����������κ�ѹ������ˣ�BMP�ļ���ռ�õĿռ�ܴ������ĺô������ܱ��������������ܡ����ɳ�Ϊͨ�ø�ʽ��

BMP�ڹ�ȥ�ǱȽ��ռ���ͼ���ʽ������BMP��Windowλͼ��ͼ����Ҫ������PC������Windowʱ��ǽֽ��BMP�����ṩ����ѹ����ѹ����ʽ��RLE���γ̳��ȱ���ı�д�����ڴ���ǽֽͼ���ļ�ʱ��һ�����õ�ѡ�Window��ʱ�ڲ�����RLEѹ���ļ���ʽ�����ǽֽͼ��ʱҲ�����ʶ����󡣣����ʹ��ʱ����ȹر�RLEѹ�����ܡ�
BMP�ļ����ļ�ͷ��λͼ��Ϣͷ����ɫ��Ϣ��ͼ�������Ĳ�����ɡ�
1�� BMP�ļ�ͷ��BMP�ļ�ͷ���ݽṹ����BMP�ļ������͡��ļ���С��λͼ��ʼλ�õ���Ϣ��
typedef struct tagBITMAPFILEHEADER{
WORD bfType; // λͼ�ļ������ͣ�����ΪBM
DWORD bfSize; // λͼ�ļ��Ĵ�С�����ֽ�Ϊ��λ
WORD bfReserved1; // λͼ�ļ������֣�����Ϊ0
WORD bfReserved2; // λͼ�ļ������֣�����Ϊ0
DWORD bfOffBits; // λͼ���ݵ���ʼλ�ã��������λͼ�ļ�ͷ��ƫ������ʾ�����ֽ�Ϊ��λ
} BITMAPFILEHEADER;
2�� λͼ��Ϣͷ��BMPλͼ��Ϣͷ��������˵��λͼ�ĳߴ����Ϣ��
typedef struct tagBITMAPINFOHEADER{
DWORD biSize; // ���ṹ��ռ���ֽ���
LONGbiWidth; // λͼ�Ŀ�ȣ�������Ϊ��λ
LONGbiHeight; // λͼ�ĸ߶ȣ�������Ϊ��λ
WORD biPlanes; // Ŀ���豸�ļ��𣬱���Ϊ1
WORD biBitCount// ÿ�����������λ����������1(˫ɫ),4(16ɫ)��8(256ɫ)��24(���ɫ)֮һ
DWORD biCompression; // λͼѹ�����ͣ������� 0(��ѹ��),1(BI_RLE8ѹ������)��2(BI_RLE4ѹ������)֮һ
DWORD biSizeImage; // λͼ�Ĵ�С�����ֽ�Ϊ��λ
LONG biXPelsPerMeter; // λͼˮƽ�ֱ��ʣ�ÿ��������
LONG biYPelsPerMeter; // λͼ��ֱ�ֱ��ʣ�ÿ��������
DWORD biClrUsed;// λͼʵ��ʹ�õ���ɫ���е���ɫ��
DWORD biClrImportant;// λͼ��ʾ��������Ҫ����ɫ��
} BITMAPINFOHEADER;
3�� ��ɫ����ɫ������˵��λͼ�е���ɫ���������ɸ����ÿһ��������һ��RGBQUAD���͵Ľṹ������һ����ɫ��
typedef struct tagRGBQUAD {
BYTE rgbBlue;// ��ɫ������(ֵ��ΧΪ0-255)
BYTE rgbGreen; // ��ɫ������(ֵ��ΧΪ0-255)
BYTE rgbRed; // ��ɫ������(ֵ��ΧΪ0-255)
BYTE rgbReserved;// ����������Ϊ0
} RGBQUAD;
��ɫ����RGBQUAD�ṹ���ݵĸ�����biBitCount��ȷ��:
��biBitCount=1,4,8ʱ���ֱ���2,16,256������;
��biBitCount=24ʱ��û����ɫ���
λͼ��Ϣͷ����ɫ�����λͼ��Ϣ��BITMAPINFO�ṹ��������:
typedef struct tagBITMAPINFO {
BITMAPINFOHEADER bmiHeader; // λͼ��Ϣͷ
RGBQUAD bmiColors[1]; // ��ɫ��
} BITMAPINFO;
4�� λͼ���ݣ�λͼ���ݼ�¼��λͼ��ÿһ������ֵ����¼˳������ɨ�������Ǵ�����,ɨ����֮���Ǵ��µ��ϡ�λͼ��һ������ֵ��ռ���ֽ���:
��biBitCount=1ʱ��8������ռ1���ֽ�;
��biBitCount=4ʱ��2������ռ1���ֽ�;
��biBitCount=8ʱ��1������ռ1���ֽ�;
��biBitCount=24ʱ,1������ռ3���ֽ�;
Windows�涨һ��ɨ������ռ���ֽ���������4�ı���(����longΪ��λ),�������0��䣬
һ��ɨ������ռ���ֽ������㷽��:
DataSizePerLine= (biWidth* biBitCount+31)/8; // һ��ɨ������ռ���ֽ���
DataSizePerLine= DataSizePerLine/4*4; // �ֽ���������4�ı���
λͼ���ݵĴ�С(��ѹ�������):
DataSize= DataSizePerLine* biHeight;
����BMP�ļ�����
1�� ���������Hex Workshop �� UltraEdit
2�� ������������ע�����е���ֵ�ڴ洢�϶��ǰ�"��λ�Ÿ�λ����λ�ŵ�λ��ԭ��"����12345678h���ڴ洢���о���7856 3412������ͼ��һ��ͼ16�������ݣ��Դ�Ϊ�����з������ڷ�����Ϊ�˼���������һ���֣������ֽ�Ϊ��λ����424D����һ���֣�Ϊ��ŵ�λ���У�"h"��ʾ��16��������
424D 4690 0000 0000 0000 4600 0000 2800
0000 8000 0000 9000 0000 0100 1000 0300
0000 0090 0000 A00F 0000 A00F 0000 0000
0000 0000 0000 00F8 0000 E007 0000 1F00
0000 0000 0000 02F1 84F1 04F1 84F1 84F1
06F2 84F1 06F2 04F2 86F2 06F2 86F2 86F2
1��ͼ���ļ�ͷ��424Dh='BM'����ʾ��Windows֧�ֵ�BMP��ʽ��
2-3�������ļ���С��4690 0000��Ϊ00009046h=36934��
4-5����������������Ϊ0��
6-7�����ļ���ʼ��λͼ����֮���ƫ������4600 0000��Ϊ00000046h=70��������ļ�ͷ����35��=70�ֽڡ�
8-9��λͼͼ��Ϣͷ���ȡ�
10-11��λͼ��ȣ�������Ϊ��λ��8000 0000��Ϊ00000080h=128��
12-13��λͼ�߶ȣ�������Ϊ��λ��9000 0000��Ϊ00000090h=144��
14��λͼ��λ��������ֵ����1��0100��Ϊ0001h=1��
15��ÿ�����ص�λ������1����ɫ����4��16ɫ����8��256ɫ����16��64Kɫ���߲�ɫ����24��16Mɫ�����ɫ����32��4096Mɫ����ǿ�����ɫ����T408֧�ֵ���16λ��ʽ��1000Ϊ0010h=16��
16-17��ѹ��˵������0����ѹ������1��RLE 8��8λRLEѹ������2��RLE 4��4λRLEѹ������3��Bitfields��λ���ţ���RLE�򵥵�˵�ǲ���������+����ֵ�ķ�ʽ����ѹ����T408���õ���λ���ŷ�ʽ���������ֽڱ�ʾһ�����أ�λ�����Ϊr5b6g5��ͼ��0300 0000Ϊ00000003h=3��
18-19�����ֽ�����ʾ��λͼ���ݵĴ�С������������4�ı�������ֵ�ϵ���λͼ��ȡ�λͼ�߶ȡ�ÿ������λ����0090 0000Ϊ00009000h=80��90��2h=36864��
20-21��������/�ױ�ʾ��ˮƽ�ֱ��ʡ�A00F 0000Ϊ0000 0FA0h=4000��
22-23��������/�ױ�ʾ�Ĵ�ֱ�ֱ��ʡ�A00F 0000Ϊ0000 0FA0h=4000��
2��λͼʹ�õ���ɫ����������Ϊ0�Ļ�����˵��ʹ�����е�ɫ���
26-27����ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0����ʾ����Ҫ��
28-35����ɫ��淶�����ڵ�ɫ���е�ÿ���������������������RGB��ֵ��
1�ֽ�������ɫ����
1�ֽ�������ɫ����
1�ֽ����ں�ɫ����
1�ֽ���������(����Ϊ0)
����24-λ���ɫͼ��Ͳ�ʹ�ò�ɫ����Ϊλͼ�е�RGBֵ�ʹ�����ÿ�����ص���ɫ������16λr5g6b5λ���ɫͼ����Ҫ��ɫ����ǰ���ͼ��������Ľ��Ͳ�̫�Ե��ϣ�Ӧ������Ľ���Ϊ׼��
ͼ�в�ɫ��Ϊ00F8 0000 E007 0000 1F00 0000 0000 0000�����У�
00FB 0000ΪFB00h=1111100000000000�������ƣ����Ǻ�ɫ���������롣
E007 0000Ϊ 07E0h=0000011111100000�������ƣ�������ɫ���������롣
1F00 0000Ϊ001Fh=0000000000011111�������ƣ����Ǻ�ɫ���������롣
0000 0000������Ϊ0��
�����������ֵ����"��"�����ٽ�����λ�����Ϳ��Եõ���ɫ����ֵ���������룬�Ϳ���������ʵ����ÿ������ֵ�������ֽ�16λ�У����Ӹߵ���ȡ5��6��5λ�ֱ����r��g��b����ֵ��ȡ������ֵ���r��g��bֵ�ֱ����8��4��8�Ϳ��Բ���ڸ�����Ϊһ���ֽڣ��ٰ��������ֽڰ�rgb��ϣ�����洢����ͬ��Ҫ���򣩣��Ϳ���ת��Ϊ24λ��׼BMP��ʽ�ˡ�

���⣬��ultraedit�ɲ鿴���������ļ��Ķ����ƴ��룬
void showBmpHead(BITMAPFILEHEADER* pBmpHead)
{
	printf("λͼ�ļ�ͷ:\n");
	printf("�ļ���С:%d\n",pBmpHead->bfSize);
	printf("������:%d\n",pBmpHead->bfReserved1);
	printf("������:%d\n",pBmpHead->bfReserved2);
	printf("ʵ��λͼ���ݵ�ƫ���ֽ���:%d\n",pBmpHead->bfOffBits);

}

void showBmpInforHead(tagBITMAPINFOHEADER* pBmpInforHead)
{
	printf("λͼ��Ϣͷ:\n");
	printf("�ṹ��ĳ���:%d\n",pBmpInforHead->biSize);
	printf("λͼ��:%d\n",pBmpInforHead->biWidth);
	printf("λͼ��:%d\n",pBmpInforHead->biHeight);
	printf("biPlanesƽ����:%d\n",pBmpInforHead->biPlanes);
	printf("biBitCount������ɫλ��:%d\n",pBmpInforHead->biBitCount);
	printf("ѹ����ʽ:%d\n",pBmpInforHead->biCompression);
	printf("biSizeImageʵ��λͼ����ռ�õ��ֽ���:%d\n",pBmpInforHead->biSizeImage);
	printf("X����ֱ���:%d\n",pBmpInforHead->biXPelsPerMeter);
	printf("Y����ֱ���:%d\n",pBmpInforHead->biYPelsPerMeter);
	printf("ʹ�õ���ɫ��:%d\n",pBmpInforHead->biClrUsed);
	printf("��Ҫ��ɫ��:%d\n",pBmpInforHead->biClrImportant);
}

void showRgbQuan(tagRGBQUAD* pRGB)
{
    printf("(%-3d,%-3d,%-3d) ",pRGB->rgbRed,pRGB->rgbGreen,pRGB->rgbBlue);

}
*/