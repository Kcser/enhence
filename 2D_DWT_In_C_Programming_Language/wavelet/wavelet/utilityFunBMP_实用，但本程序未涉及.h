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

//位图文件头信息结构定义
//其中不包含文件类型信息（由于结构体的内存结构决定，要是加了的话将不能正确读取文件信息）
typedef struct tagBITMAPFILEHEADER {
	DWORD bfSize; //文件大小
	WORD bfReserved1; //保留字，不考虑
	WORD bfReserved2; //保留字，同上
	DWORD bfOffBits; //实际位图数据的偏移字节数，即前三个部分长度之和
} BITMAPFILEHEADER;

//信息头BITMAPINFOHEADER，也是一个结构，其定义如下：
typedef struct tagBITMAPINFOHEADER{
	//public:
	DWORD biSize; //指定此结构体的长度，为40
	LONG biWidth; //位图宽
	LONG biHeight; //位图高
	WORD biPlanes; //平面数，为1
	WORD biBitCount; //采用颜色位数，可以是1，2，4，8，16，24，新的可以是32
	DWORD biCompression; //压缩方式，可以是0，1，2，其中0表示不压缩
	DWORD biSizeImage; //实际位图数据占用的字节数
	LONG biXPelsPerMeter; //X方向分辨率
	LONG biYPelsPerMeter; //Y方向分辨率
	DWORD biClrUsed; //使用的颜色数，如果为0，则表示默认值(2^颜色位数)
	DWORD biClrImportant; //重要颜色数，如果为0，则表示所有颜色都是重要的
} BITMAPINFOHEADER;


//调色板Palette，当然，这里是对那些需要调色板的位图文件而言的。24位和32位是不需要调色板的。
//（似乎是调色板结构体个数等于使用的颜色数。）

typedef struct tagRGBQUAD {
	//public:
	BYTE rgbBlue; //该颜色的蓝色分量
	BYTE rgbGreen; //该颜色的绿色分量
	BYTE rgbRed; //该颜色的红色分量
	BYTE rgbReserved; //保留值
} RGBQUAD;

/* 读取BMP文件相关函数 */
/*int exampleMain(void)
{
    BITMAPFILEHEADER bitHead;
    BITMAPINFOHEADER bitInfoHead;
    FILE* pfile;

    char strFile[50];
    printf("输入bpm文件:\n");
    scanf("%s",strFile);

    pfile = fopen(strFile,"rb");//打开文件

    if(pfile!=NULL)
    {
        printf("成功.\n");
        //读取位图文件头信息
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

        //读取位图信息头信息
        fread(&bitInfoHead,1,sizeof(BITMAPINFOHEADER),pfile);
        // showBmpInforHead(&bitInfoHead);
        printf("\n");
    }
    else
    {
        printf("打开失败!\n");
        system("pause");
        return 0;
    }


    tagRGBQUAD *pRgb ;

	if(bitInfoHead.biBitCount < 24)//有调色板
	{
		//读取调色盘结信息
		long nPlantNum = long(pow(2,double(bitInfoHead.biBitCount))); // Mix color Plant Number;
		pRgb=(tagRGBQUAD *)malloc(nPlantNum*sizeof(tagRGBQUAD));
		memset(pRgb,0,nPlantNum*sizeof(tagRGBQUAD));
		int num = fread(pRgb,4,nPlantNum,pfile);

		printf("Color Plate Number: %d\n",nPlantNum);

		printf("颜色板信息:\n");
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
	//分配内存空间把源图存入内存
	int l_width = WIDTHBYTES(width* bitInfoHead.biBitCount);//计算位图的实际宽度并确保它为32的倍数
	BYTE *pColorData=(BYTE *)malloc(height*l_width);
	memset(pColorData,0,height*l_width);
	long nData = height*l_width;

	//把位图数据信息读到数组里
	fread(pColorData,1,nData,pfile);

	//将位图数据转化为RGB数据
	tagRGBQUAD* dataOfBmp;
	dataOfBmp = (tagRGBQUAD *)malloc(width*height*sizeof(tagRGBQUAD));//用于保存各像素对应的RGB数据
	memset(dataOfBmp,0,width*height*sizeof(tagRGBQUAD));

	if(bitInfoHead.biBitCount<24)//有调色板，即位图为非真彩色
	{
		int k;
		int index = 0;
		if (bitInfoHead.biBitCount == 1)
		{
			for(int i=0;i<height;i++)
				for(int j=0;j<width;j++)
				{
					BYTE mixIndex= 0;
					k = i*l_width + j/8;//k:取得该像素颜色数据在实际数据数组中的序号
					//j:提取当前像素的颜色的具体值
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

					//将像素数据保存到数组中对应的位置
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
					k = i*l_width + j/4;//k:取得该像素颜色数据在实际数据数组中的序号
					//j:提取当前像素的颜色的具体值
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

					//将像素数据保存到数组中对应的位置
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
					{//低
						mixIndex = mixIndex<<4;
						mixIndex = mixIndex>>4;
					}
					else
					{//高
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
	else//位图为24位真彩色
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


	printf("像素数据信息:\n");
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


/* 相关知识 */
/*
BMP是一种与硬件设备无关的图像文件格式，也是我们最常在PC机上的Windows系统下见到的标准位图格式，使用范围很广泛。它采用位映射存储格式，除了图像深度可选以外，不采用其他任何压缩，因此，BMP文件所占用的空间很大。它最大的好处就是能被大多数软件“接受”，可称为通用格式。

BMP在过去是比较普及的图像格式，现在BMP（Window位图）图像主要被用在PC机运行Window时的墙纸。BMP可以提供无损压缩，压缩方式叫RLE（游程长度编码的编写），在创建墙纸图像文件时是一个极好的选项。Window有时在查找以RLE压缩文件方式保存的墙纸图像时也会出现识别错误。，因此使用时最好先关闭RLE压缩功能。
BMP文件由文件头、位图信息头、颜色信息和图形数据四部分组成。
1、 BMP文件头：BMP文件头数据结构含有BMP文件的类型、文件大小和位图起始位置等信息。
typedef struct tagBITMAPFILEHEADER{
WORD bfType; // 位图文件的类型，必须为BM
DWORD bfSize; // 位图文件的大小，以字节为单位
WORD bfReserved1; // 位图文件保留字，必须为0
WORD bfReserved2; // 位图文件保留字，必须为0
DWORD bfOffBits; // 位图数据的起始位置，以相对于位图文件头的偏移量表示，以字节为单位
} BITMAPFILEHEADER;
2、 位图信息头：BMP位图信息头数据用于说明位图的尺寸等信息。
typedef struct tagBITMAPINFOHEADER{
DWORD biSize; // 本结构所占用字节数
LONGbiWidth; // 位图的宽度，以像素为单位
LONGbiHeight; // 位图的高度，以像素为单位
WORD biPlanes; // 目标设备的级别，必须为1
WORD biBitCount// 每个像素所需的位数，必须是1(双色),4(16色)，8(256色)或24(真彩色)之一
DWORD biCompression; // 位图压缩类型，必须是 0(不压缩),1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一
DWORD biSizeImage; // 位图的大小，以字节为单位
LONG biXPelsPerMeter; // 位图水平分辨率，每米像素数
LONG biYPelsPerMeter; // 位图垂直分辨率，每米像素数
DWORD biClrUsed;// 位图实际使用的颜色表中的颜色数
DWORD biClrImportant;// 位图显示过程中重要的颜色数
} BITMAPINFOHEADER;
3、 颜色表：颜色表用于说明位图中的颜色，它有若干个表项，每一个表项是一个RGBQUAD类型的结构，定义一种颜色。
typedef struct tagRGBQUAD {
BYTE rgbBlue;// 蓝色的亮度(值范围为0-255)
BYTE rgbGreen; // 绿色的亮度(值范围为0-255)
BYTE rgbRed; // 红色的亮度(值范围为0-255)
BYTE rgbReserved;// 保留，必须为0
} RGBQUAD;
颜色表中RGBQUAD结构数据的个数有biBitCount来确定:
当biBitCount=1,4,8时，分别有2,16,256个表项;
当biBitCount=24时，没有颜色表项。
位图信息头和颜色表组成位图信息，BITMAPINFO结构定义如下:
typedef struct tagBITMAPINFO {
BITMAPINFOHEADER bmiHeader; // 位图信息头
RGBQUAD bmiColors[1]; // 颜色表
} BITMAPINFO;
4、 位图数据：位图数据记录了位图的每一个像素值，记录顺序是在扫描行内是从左到右,扫描行之间是从下到上。位图的一个像素值所占的字节数:
当biBitCount=1时，8个像素占1个字节;
当biBitCount=4时，2个像素占1个字节;
当biBitCount=8时，1个像素占1个字节;
当biBitCount=24时,1个像素占3个字节;
Windows规定一个扫描行所占的字节数必须是4的倍数(即以long为单位),不足的以0填充，
一个扫描行所占的字节数计算方法:
DataSizePerLine= (biWidth* biBitCount+31)/8; // 一个扫描行所占的字节数
DataSizePerLine= DataSizePerLine/4*4; // 字节数必须是4的倍数
位图数据的大小(不压缩情况下):
DataSize= DataSizePerLine* biHeight;
二、BMP文件分析
1、 工具软件：Hex Workshop 或 UltraEdit
2、 分析：首先请注意所有的数值在存储上都是按"高位放高位、低位放低位的原则"，如12345678h放在存储器中就是7856 3412）。下图是一张图16进制数据，以此为例进行分析。在分析中为了简化叙述，以一个字（两个字节为单位，如424D就是一个字）为序号单位进行，"h"表示是16进制数。
424D 4690 0000 0000 0000 4600 0000 2800
0000 8000 0000 9000 0000 0100 1000 0300
0000 0090 0000 A00F 0000 A00F 0000 0000
0000 0000 0000 00F8 0000 E007 0000 1F00
0000 0000 0000 02F1 84F1 04F1 84F1 84F1
06F2 84F1 06F2 04F2 86F2 06F2 86F2 86F2
1：图像文件头。424Dh='BM'，表示是Windows支持的BMP格式。
2-3：整个文件大小。4690 0000，为00009046h=36934。
4-5：保留，必须设置为0。
6-7：从文件开始到位图数据之间的偏移量。4600 0000，为00000046h=70，上面的文件头就是35字=70字节。
8-9：位图图信息头长度。
10-11：位图宽度，以像素为单位。8000 0000，为00000080h=128。
12-13：位图高度，以像素为单位。9000 0000，为00000090h=144。
14：位图的位面数，该值总是1。0100，为0001h=1。
15：每个像素的位数。有1（单色），4（16色），8（256色），16（64K色，高彩色），24（16M色，真彩色），32（4096M色，增强型真彩色）。T408支持的是16位格式。1000为0010h=16。
16-17：压缩说明：有0（不压缩），1（RLE 8，8位RLE压缩），2（RLE 4，4位RLE压缩），3（Bitfields，位域存放）。RLE简单地说是采用像素数+像素值的方式进行压缩。T408采用的是位域存放方式，用两个字节表示一个像素，位域分配为r5b6g5。图中0300 0000为00000003h=3。
18-19：用字节数表示的位图数据的大小，该数必须是4的倍数，数值上等于位图宽度×位图高度×每个像素位数。0090 0000为00009000h=80×90×2h=36864。
20-21：用象素/米表示的水平分辨率。A00F 0000为0000 0FA0h=4000。
22-23：用象素/米表示的垂直分辨率。A00F 0000为0000 0FA0h=4000。
2：位图使用的颜色索引数。设为0的话，则说明使用所有调色板项。
26-27：对图象显示有重要影响的颜色索引的数目。如果是0，表示都重要。
28-35：彩色板规范。对于调色板中的每个表项，用下述方法来描述RGB的值：
1字节用于蓝色分量
1字节用于绿色分量
1字节用于红色分量
1字节用于填充符(设置为0)
对于24-位真彩色图像就不使用彩色表，因为位图中的RGB值就代表了每个象素的颜色。但是16位r5g6b5位域彩色图像需要彩色表，看前面的图，与上面的解释不太对得上，应以下面的解释为准。
图中彩色板为00F8 0000 E007 0000 1F00 0000 0000 0000，其中：
00FB 0000为FB00h=1111100000000000（二进制），是红色分量的掩码。
E007 0000为 07E0h=0000011111100000（二进制），是绿色分量的掩码。
1F00 0000为001Fh=0000000000011111（二进制），是红色分量的掩码。
0000 0000总设置为0。
将掩码跟像素值进行"与"运算再进行移位操作就可以得到各色分量值。看看掩码，就可以明白事实上在每个像素值的两个字节16位中，按从高到低取5、6、5位分别就是r、g、b分量值。取出分量值后把r、g、b值分别乘以8、4、8就可以补齐第个分量为一个字节，再把这三个字节按rgb组合，放入存储器（同样要反序），就可以转换为24位标准BMP格式了。

另外，用ultraedit可查看几乎所有文件的二进制代码，
void showBmpHead(BITMAPFILEHEADER* pBmpHead)
{
	printf("位图文件头:\n");
	printf("文件大小:%d\n",pBmpHead->bfSize);
	printf("保留字:%d\n",pBmpHead->bfReserved1);
	printf("保留字:%d\n",pBmpHead->bfReserved2);
	printf("实际位图数据的偏移字节数:%d\n",pBmpHead->bfOffBits);

}

void showBmpInforHead(tagBITMAPINFOHEADER* pBmpInforHead)
{
	printf("位图信息头:\n");
	printf("结构体的长度:%d\n",pBmpInforHead->biSize);
	printf("位图宽:%d\n",pBmpInforHead->biWidth);
	printf("位图高:%d\n",pBmpInforHead->biHeight);
	printf("biPlanes平面数:%d\n",pBmpInforHead->biPlanes);
	printf("biBitCount采用颜色位数:%d\n",pBmpInforHead->biBitCount);
	printf("压缩方式:%d\n",pBmpInforHead->biCompression);
	printf("biSizeImage实际位图数据占用的字节数:%d\n",pBmpInforHead->biSizeImage);
	printf("X方向分辨率:%d\n",pBmpInforHead->biXPelsPerMeter);
	printf("Y方向分辨率:%d\n",pBmpInforHead->biYPelsPerMeter);
	printf("使用的颜色数:%d\n",pBmpInforHead->biClrUsed);
	printf("重要颜色数:%d\n",pBmpInforHead->biClrImportant);
}

void showRgbQuan(tagRGBQUAD* pRGB)
{
    printf("(%-3d,%-3d,%-3d) ",pRGB->rgbRed,pRGB->rgbGreen,pRGB->rgbBlue);

}
*/