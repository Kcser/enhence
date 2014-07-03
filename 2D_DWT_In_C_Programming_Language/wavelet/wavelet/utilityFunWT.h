#ifndef _utilityFunWT_H
#define _utilityFunWT_H

#include "stdio.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <malloc.h>

#define DATA2D(Matrix,m,n,SizeCol) Matrix[(m)*(SizeCol)+(n)]

#define WAVE_DB3 1
#define WAVE_BIOR3_7 2

#define ExtT_ZPD 1
#define ExtT_SYM 2
#define ExtT_PPD 3

#define CONVT_FULL 1
#define CONVT_SAME 2
#define CONVT_VALID 3

#define DIR_ROW 1
#define DIR_COL 2

#define DYAD_EVEN 1
#define DYAD_ODD 2


/* 小波变换数据描述结构体 */

/* 矩阵描述结构体 */
typedef struct Matrix {
	double * data;			// 指向存放元素的空间
	int height;	// 行数
	int width;	// 列数
} MATRIX;

typedef struct intDataPanel2D {
	int * data;
	int height;
	int width;
} IntDataPanel2D;

/* 小波变换信息描述结构体 */
typedef struct wtInfo {
	int m_nWaveType;	// 小波类型
	int m_nExtType;		// 延拓方式
	int m_nWTLevel;		// 变换级数
	double * m_pC;		// 变换结果，小波系数，[An, Hn, Vn, Dn, H(n-1), V(n-1), D(n-1)..., H1, V1, D1]
	int * m_pS;			// 变换结果描述，一个2*n数组，其中n = m_nWTLevel+2；
} WTInfo;

/* 小波变换结果描述结构体 */
typedef struct WTCoefSet{
	MATRIX * A_LL;		// 低通分量
	MATRIX * H_LH;		// 水平分量
	MATRIX * V_HL;		// 竖直分量
	MATRIX * D_HH;		// 对角分量
} WTCOEFSet;

/* 小波基常量描述结构体 */
typedef struct waveletBase {
	int nFilterLen;
	double * LO_D;
	double * HI_D;
	double * LO_R;
	double * HI_R;
}WaveletBASE;

/* db3小波滤波器 */
double LO_D_db3[6] = {0.0352,   -0.0854,   -0.1350,    0.4599,    0.8069,    0.3327};
double HI_D_db3[6] = {-0.3327,    0.8069,   -0.4599,   -0.1350,    0.0854,    0.0352};
double LO_R_db3[6] = {0.3327,    0.8069,    0.4599,   -0.1350,   -0.0854,    0.0352};
double HI_R_db3[6] = {0.0352,    0.0854,   -0.1350,   -0.4599,    0.8069,   -0.3327};
const WaveletBASE WAVEdb3 = {6, LO_D_db3, HI_D_db3, LO_R_db3, HI_R_db3};

/* bior3.7小波滤波器 */
double LO_D_bior3_7[] = {0.0030, -0.0091, -0.0168, 0.0747, 0.0313, -0.3012, -0.0265, 0.9516, 0.9516, -0.0265, -0.3012, 0.0313, 0.0747, -0.0168, -0.0091, 0.0030};
double HI_D_bior3_7[] = {0,  0,  0,  0,  0,  0, -0.1768, 0.5303, -0.5303, 0.1768,  0,  0,  0,  0,  0,  0};
double LO_R_bior3_7[] = {0,  0,  0,  0,  0,  0, 0.1768, 0.5303, 0.5303, 0.1768,  0,  0,  0,  0,  0,  0};
double HI_R_bior3_7[] = {0.0030, 0.0091, -0.0168, -0.0747, 0.0313, 0.3012, -0.0265, -0.9516, 0.9516, 0.0265, -0.3012, -0.0313, 0.0747, 0.0168, -0.0091, -0.0030};
const WaveletBASE WAVEbior3_7 = {16, LO_D_bior3_7, HI_D_bior3_7, LO_R_bior3_7, HI_R_bior3_7};


/* 函数声明 */
double * SerialReverse(double * pSrcSerial, int nSerialLen);


/* 结构体内存管理相关函数 */
void DDP_FREE(MATRIX * pMATRIX)
{
	if (pMATRIX != NULL) {
		if (pMATRIX->data != NULL)
			free(pMATRIX->data);
		free(pMATRIX);
	}
}

void IDP_FREE(IntDataPanel2D * pIntDataPanel2D)
{
	if (pIntDataPanel2D != NULL) {
		if (pIntDataPanel2D->data != NULL)
			free(pIntDataPanel2D->data);
		free(pIntDataPanel2D);
	}
}

void CS_FREE(WTCOEFSet * pWTCoefSet)
{
	if (pWTCoefSet != NULL) {
		if (pWTCoefSet->A_LL != NULL)
			free(pWTCoefSet->A_LL);
		if (pWTCoefSet->H_LH != NULL)
			free(pWTCoefSet->H_LH);
		if (pWTCoefSet->V_HL != NULL)
			free(pWTCoefSet->V_HL);
		if (pWTCoefSet->D_HH != NULL)
			free(pWTCoefSet->D_HH);
		free(pWTCoefSet);
	}
}

void WB_FREE(WaveletBASE * pWaveletBase)
{
	if (pWaveletBase != NULL) {
		if (pWaveletBase->LO_D != NULL)
			free(pWaveletBase->LO_D);
		if (pWaveletBase->HI_D != NULL)
			free(pWaveletBase->HI_D);
		if (pWaveletBase->LO_R != NULL)
			free(pWaveletBase->LO_R);
		if (pWaveletBase->HI_D != NULL)
			free(pWaveletBase->HI_R);
		free(pWaveletBase);
	}
}

void WI_FREE(WTInfo * pWTInfo)
{
	if (pWTInfo != NULL) {
		if (pWTInfo->m_pC != NULL)
			free(pWTInfo->m_pC);
		if (pWTInfo->m_pS != NULL)
			free(pWTInfo->m_pS);
		free(pWTInfo);
	}
}


/* 小波变换相关函数 */

WaveletBASE * SetWaveletBase(int nWaveType)
{
	WaveletBASE * pWaveletBase = NULL;

	pWaveletBase = (WaveletBASE *)calloc(1, sizeof(WaveletBASE));
	if (!pWaveletBase)
		exit(1);

	switch (nWaveType) {
	case WAVE_DB3:
		pWaveletBase->nFilterLen = WAVEdb3.nFilterLen;
		pWaveletBase->LO_D = SerialReverse(WAVEdb3.LO_D, WAVEdb3.nFilterLen);
		pWaveletBase->HI_D = SerialReverse(WAVEdb3.HI_D, WAVEdb3.nFilterLen);
		pWaveletBase->LO_R = SerialReverse(WAVEdb3.LO_R, WAVEdb3.nFilterLen);
		pWaveletBase->HI_R = SerialReverse(WAVEdb3.HI_R, WAVEdb3.nFilterLen);
		break;
	case WAVE_BIOR3_7:
		pWaveletBase->nFilterLen = WAVEbior3_7.nFilterLen;
		pWaveletBase->LO_D = SerialReverse(WAVEbior3_7.LO_D, WAVEbior3_7.nFilterLen);
		pWaveletBase->HI_D = SerialReverse(WAVEbior3_7.HI_D, WAVEbior3_7.nFilterLen);
		pWaveletBase->LO_R = SerialReverse(WAVEbior3_7.LO_R, WAVEbior3_7.nFilterLen);
		pWaveletBase->HI_R = SerialReverse(WAVEbior3_7.HI_R, WAVEbior3_7.nFilterLen);
		break;
	default:
		pWaveletBase->nFilterLen = WAVEdb3.nFilterLen;
		pWaveletBase->LO_D = SerialReverse(WAVEdb3.LO_D, WAVEdb3.nFilterLen);
		pWaveletBase->HI_D = SerialReverse(WAVEdb3.HI_D, WAVEdb3.nFilterLen);
		pWaveletBase->LO_R = SerialReverse(WAVEdb3.LO_R, WAVEdb3.nFilterLen);
		pWaveletBase->HI_R = SerialReverse(WAVEdb3.HI_R, WAVEdb3.nFilterLen);
	}
	return pWaveletBase;
}


/* 延拓 */
MATRIX * WExtend2D(MATRIX * pSrcData, int nExtType, int nExtSizeRow, int nExtSizeCol)
{
	int i, j;
	MATRIX * pResultData = NULL;

	pResultData = (MATRIX *)calloc(1, sizeof(MATRIX));
	if (!pResultData)
		exit(1);

	pResultData->height = pSrcData->height+nExtSizeRow*2;
	pResultData->width = pSrcData->width+nExtSizeCol*2;
	pResultData->data = (double *) calloc(pResultData->height*pResultData->width, sizeof(double));
	if (!pResultData->data) {
		DDP_FREE(pResultData);
		exit(1);
	}

	for (i = 0; i < pSrcData->height; i++)
		for (j = 0; j < pSrcData->width; j++)
			DATA2D(pResultData->data, i+nExtSizeRow, j+nExtSizeCol, pResultData->width) = DATA2D(pSrcData->data, i, j, pSrcData->width);

	switch (nExtType) {
	case ExtT_ZPD:	// 零延拓
		for (i = 0; i < nExtSizeRow; i++)
			for (j = 0; j < nExtSizeCol; j++) {
				DATA2D(pResultData->data, i, j, pResultData->width) = 0;
				DATA2D(pResultData->data, i, pResultData->width-j-1, pResultData->width) = 0;
				DATA2D(pResultData->data, pResultData->height-i-1, j, pResultData->width) = 0;
				DATA2D(pResultData->data, pResultData->height-i-1, pResultData->width-j-1, pResultData->width) = 0;
			}
		for (i = 0; i < nExtSizeRow; i++)
			for (j = nExtSizeCol; j < pResultData->width-nExtSizeCol; j++) {
				DATA2D(pResultData->data, i, j, pResultData->width) = 0;
				DATA2D(pResultData->data, pResultData->height-1-i, j, pResultData->width) = 0;
			}
		for (i = nExtSizeRow; i < pResultData->height-nExtSizeRow; i++)
			for (j = 0; j < nExtSizeCol; j++) {
				DATA2D(pResultData->data, i, j, pResultData->width) = 0;
				DATA2D(pResultData->data, i, pResultData->width-1-j, pResultData->width) = 0;
			}
		break;
	case ExtT_SYM:	// 对称延拓
		for (i = 0; i < nExtSizeRow; i++)
			for (j = 0; j < nExtSizeCol; j++) {
				DATA2D(pResultData->data, i, j, pResultData->width) = DATA2D(pSrcData->data, nExtSizeRow-i-1, nExtSizeCol-j-1, pSrcData->width);
				DATA2D(pResultData->data, i, pResultData->width-j-1, pResultData->width) = DATA2D(pSrcData->data, nExtSizeRow-i-1, pSrcData->width-(nExtSizeCol-j-1)-1, pSrcData->width);
				DATA2D(pResultData->data, pResultData->height-i-1, j, pResultData->width) = DATA2D(pSrcData->data, pSrcData->height-(nExtSizeRow-i-1)-1, nExtSizeCol-j-1, pSrcData->width);
				DATA2D(pResultData->data, pResultData->height-i-1, pResultData->width-j-1, pResultData->width) = DATA2D(pSrcData->data, pSrcData->height-(nExtSizeRow-i-1)-1, pSrcData->width-(nExtSizeCol-j-1)-1, pSrcData->width);
			}
		for (i = 0; i < nExtSizeRow; i++)
			for (j = nExtSizeCol; j < pResultData->width-nExtSizeCol; j++) {
				DATA2D(pResultData->data, i, j, pResultData->width) = DATA2D(pSrcData->data, nExtSizeRow-i-1, j-nExtSizeCol, pSrcData->width);
				DATA2D(pResultData->data, pResultData->height-1-i, j, pResultData->width) = DATA2D(pSrcData->data, pSrcData->height-(nExtSizeRow-i), j-nExtSizeCol, pSrcData->width);
			}
		for (i = nExtSizeRow; i < pResultData->height-nExtSizeRow; i++)
			for (j = 0; j < nExtSizeCol; j++) {
				DATA2D(pResultData->data, i, j, pResultData->width) = DATA2D(pSrcData->data, i-nExtSizeRow, nExtSizeCol-j-1, pSrcData->width);
				DATA2D(pResultData->data, i, pResultData->width-1-j, pResultData->width) = DATA2D(pSrcData->data, i-nExtSizeRow, pSrcData->width-(nExtSizeCol-j), pSrcData->width);
			}
		break;
	case ExtT_PPD:	// 周期延拓
		for (i = 0; i < nExtSizeRow; i++)
			for (j = 0; j < nExtSizeCol; j++) {
				DATA2D(pResultData->data, i, j, pResultData->width) = DATA2D(pSrcData->data, pSrcData->height-(nExtSizeRow-i-1)-1, pSrcData->width-(nExtSizeCol-j-1)-1, pSrcData->width);
				DATA2D(pResultData->data, i, pResultData->width-j-1, pResultData->width) = DATA2D(pSrcData->data, pSrcData->height-(nExtSizeRow-i-1)-1, nExtSizeCol-j-1, pSrcData->width);
				DATA2D(pResultData->data, pResultData->height-i-1, j, pResultData->width) = DATA2D(pSrcData->data, nExtSizeRow-i-1, pSrcData->width-(nExtSizeCol-j-1)-1, pSrcData->width);
				DATA2D(pResultData->data, pResultData->height-i-1, pResultData->width-j-1, pResultData->width) = DATA2D(pSrcData->data, nExtSizeRow-i-1, nExtSizeCol-j-1, pSrcData->width);
			}
		for (i = 0; i < nExtSizeRow; i++)
			for (j = nExtSizeCol; j < pResultData->width-nExtSizeCol; j++) {
				DATA2D(pResultData->data, i, j, pResultData->width) = DATA2D(pSrcData->data, pSrcData->height-(nExtSizeRow-i), j-nExtSizeCol, pSrcData->width);
				DATA2D(pResultData->data, pResultData->height-1-i, j, pResultData->width) = DATA2D(pSrcData->data, nExtSizeRow-i-1, j-nExtSizeCol, pSrcData->width);
			}
		for (i = nExtSizeRow; i < pResultData->height-nExtSizeRow; i++)
			for (j = 0; j < nExtSizeCol; j++) {
				DATA2D(pResultData->data, i, j, pResultData->width) = DATA2D(pSrcData->data, i-nExtSizeRow, pSrcData->width-(nExtSizeCol-j), pSrcData->width);
				DATA2D(pResultData->data, i, pResultData->width-1-j, pResultData->width) = DATA2D(pSrcData->data, i-nExtSizeRow, nExtSizeCol-j-1, pSrcData->width);
			}
		break;
	default:	// 默认对称延拓
		for (i = 0; i < nExtSizeRow; i++)
			for (j = 0; j < nExtSizeCol; j++) {
				DATA2D(pResultData->data, i, j, pResultData->width) = DATA2D(pSrcData->data, nExtSizeRow-i-1, nExtSizeCol-j-1, pSrcData->width);
				DATA2D(pResultData->data, i, pResultData->width-j-1, pResultData->width) = DATA2D(pSrcData->data, nExtSizeRow-i-1, pSrcData->width-(nExtSizeCol-j-1)-1, pSrcData->width);
				DATA2D(pResultData->data, pResultData->height-i-1, j, pResultData->width) = DATA2D(pSrcData->data, pSrcData->height-(nExtSizeRow-i-1)-1, nExtSizeCol-j-1, pSrcData->width);
				DATA2D(pResultData->data, pResultData->height-i-1, pResultData->width-j-1, pResultData->width) = DATA2D(pSrcData->data, pSrcData->height-(nExtSizeRow-i-1)-1, pSrcData->width-(nExtSizeCol-j-1)-1, pSrcData->width);
			}
		for (i = 0; i < nExtSizeRow; i++)
			for (j = nExtSizeCol; j < pResultData->width-nExtSizeCol; j++) {
				DATA2D(pResultData->data, i, j, pResultData->width) = DATA2D(pSrcData->data, nExtSizeRow-i-1, j-nExtSizeCol, pSrcData->width);
				DATA2D(pResultData->data, pResultData->height-1-i, j, pResultData->width) = DATA2D(pSrcData->data, pSrcData->height-(nExtSizeRow-i), j-nExtSizeCol, pSrcData->width);
			}
		for (i = nExtSizeRow; i < pResultData->height-nExtSizeRow; i++)
			for (j = 0; j < nExtSizeCol; j++) {
				DATA2D(pResultData->data, i, j, pResultData->width) = DATA2D(pSrcData->data, i-nExtSizeRow, nExtSizeCol-j-1, pSrcData->width);
				DATA2D(pResultData->data, i, pResultData->width-1-j, pResultData->width) = DATA2D(pSrcData->data, i-nExtSizeRow, pSrcData->width-(nExtSizeCol-j), pSrcData->width);
			}
	}

	return pResultData;
}


/* 序列倒转 */
double * SerialReverse(double * pSrcSerial, int nSerialLen)
{
	int i;
	double * pResult = NULL;

	pResult = (double *) calloc(nSerialLen, sizeof(double));
	if (!pResult)
		exit(1);

	for (i = 0; i < nSerialLen; i++)
		pResult[i] = pSrcSerial[nSerialLen-i-1];
	return pResult;
}

/* 一维卷积 */
MATRIX * CONV(double * pSerialA, double * pSerialB, int nLengthA, int nLengthB, int nConvType)
{
	int i, j, k;
	int nLengthC;
	int nLenTemp;
	double * pSerialTemp = NULL;
	double * pResultTemp = NULL;
	MATRIX * pResultData = NULL;

	pResultData = (MATRIX *)calloc(1, sizeof(MATRIX));
	if (!pResultData)
		exit(1);

	/* 计算卷积结果的最大长度、延拓序列A的长度 */
	nLengthC = nLengthA+nLengthB-1;
	nLenTemp = nLengthA+nLengthB*2-2;

	pSerialTemp = (double * )calloc(nLenTemp, sizeof(double));
	pResultTemp = (double * )calloc(nLengthC, sizeof(double));
	if (!pResultTemp || !pSerialTemp) {
		if (!pSerialTemp)	free(pSerialTemp);
		if (!pResultTemp)	free(pResultTemp);
		DDP_FREE(pResultData);
		exit(1);
	}

	pResultData->height = 1;
	if (nConvType == CONVT_SAME) 
		pResultData->width = nLengthA;
	else
		pResultData->width = nLengthC;
	pResultData->data = (double * )calloc(pResultData->width, sizeof(double));
	if (!pResultData->data) {
		if (!pSerialTemp)	free(pSerialTemp);
		if (!pResultTemp)	free(pResultTemp);
		DDP_FREE(pResultData);
		exit(1);
	}

	/* 延拓序列A并赋给pSerialTemp */
	for (i = 0; i < nLengthB-2; i++) {
		pSerialTemp[i] = 0;
		pSerialTemp[nLenTemp-i-1] = 0;
	}
	for (i = 0; i < nLengthA; i++) {
		pSerialTemp[i+nLengthB-1] = pSerialA[i];
	}
	
	/* 计算卷积 */
	for (k = 0; k < nLengthC; k++) {
		pResultTemp[k] = 0;
		for (j = 0; j < nLengthB; j++) {
			pResultTemp[k] += pSerialTemp[j+k]*pSerialB[j];
		}
	}

	switch (nConvType) {
	case CONVT_FULL:
		for (i = 0; i < nLengthC; i++)
			pResultData->data[i] = pResultTemp[i];
		break;
	case CONVT_SAME:
		for (i = 0; i < nLengthA; i++)
			pResultData->data[i] = pResultTemp[(nLengthC-nLengthA)/2+i];
		break;
	case CONVT_VALID:
		i = 0;
		while (i < nLengthC && pResultTemp[i] == 0)	i++;
		j = 0;
		while (j < nLengthC && pResultTemp[nLengthC-j-1] == 0)	j++;
		pResultData->width = nLengthC-i-j;
		for (k = 0; k < pResultData->width; k++)
			pResultData->data[k] = pResultTemp[k+i];
		break;
	default:
		for (i = 0; i < nLengthC; i++)
			pResultData->data[i] = pResultTemp[i];
	}

	free(pSerialTemp);
	free(pResultTemp);
	return pResultData;
}


/* 二维卷积 */
MATRIX * CONV2D(MATRIX * pSrcData, double * pFilter, int nFilterLen, int nRowOrCol, int nConvType)
{
	int i, j;
	MATRIX * pResultData = NULL;
	double * pTempSerialA = NULL, * pTempSerialB = NULL;
	MATRIX * pTempSerialC = NULL;

	if (!pSrcData || !pFilter)
		exit(1);

	pResultData = (MATRIX *)calloc(1, sizeof(MATRIX));
	if (!pResultData)
		exit(1);
	
	if (nConvType == CONVT_FULL && nRowOrCol == DIR_ROW) {
		pTempSerialA = (double *) calloc(pSrcData->width, sizeof(double));
		pResultData->height = pSrcData->height;
		pResultData->width = pSrcData->width+nFilterLen-1;
	} else if (nConvType == CONVT_FULL && nRowOrCol == DIR_COL) {
		pTempSerialA = (double *) calloc(pSrcData->height, sizeof(double));
		pResultData->height = pSrcData->height+nFilterLen-1;
		pResultData->width = pSrcData->width;
	} else if (nConvType == CONVT_SAME && nRowOrCol == DIR_ROW) {
		pTempSerialA = (double *) calloc(pSrcData->width, sizeof(double));
		pResultData->height = pSrcData->height;
		pResultData->width = pSrcData->width;
	} else if (nConvType == CONVT_SAME && nRowOrCol == DIR_COL) {
		pTempSerialA = (double *) calloc(pSrcData->height, sizeof(double));
		pResultData->height = pSrcData->height;
		pResultData->width = pSrcData->width;
	}
	pResultData->data = (double *) calloc(pResultData->height*pResultData->width, sizeof(double));	
	if (!pResultData->data || !pTempSerialA) {
		DDP_FREE(pResultData);
		exit(1);
	}
	pTempSerialB = pFilter;

	if (nRowOrCol == DIR_ROW) {
		for (i = 0; i < pResultData->height; i++) {
			for (j = 0; j < pSrcData->width; j++)
				pTempSerialA[j] = DATA2D(pSrcData->data, i, j, pSrcData->width);
			pTempSerialC = CONV(pTempSerialA, pTempSerialB, pSrcData->width, nFilterLen, nConvType);
			for (j = 0; j < pResultData->width; j++)
				DATA2D(pResultData->data, i, j, pResultData->width) = pTempSerialC->data[j];
			DDP_FREE(pTempSerialC);
		}
	} else {
		for (j = 0; j < pResultData->width; j++) {
			for (i = 0; i < pSrcData->height; i++)
				pTempSerialA[i] = DATA2D(pSrcData->data, i, j, pSrcData->width);
			pTempSerialC = CONV(pTempSerialA, pTempSerialB, pSrcData->height, nFilterLen, nConvType);
			for (i = 0; i < pResultData->height; i++)
				DATA2D(pResultData->data, i, j, pResultData->width) = pTempSerialC->data[i];
			DDP_FREE(pTempSerialC);
		}
	}

	free(pTempSerialA);
	return pResultData;
}


/* 下抽样 */
MATRIX * DYADDOWN(MATRIX * pSrcData, int nEvenOrOdd, int nRowOrCol)
{
	int i, j;
	MATRIX * pResultData = NULL;

	pResultData = (MATRIX *)calloc(1, sizeof(MATRIX));
	if (!pResultData)
		exit(1);
	
	if (nEvenOrOdd == DYAD_EVEN && nRowOrCol == DIR_ROW) {
		pResultData->height = pSrcData->height/2;
		pResultData->width = pSrcData->width;
		pResultData->data = (double *) calloc(pResultData->height*pResultData->width, sizeof(double));
		if (!pResultData->data) {
			DDP_FREE(pResultData);
			exit(1);
		}
		for (i = 0; i < pResultData->height; i++)
			for (j = 0; j < pResultData->width; j++) {
				DATA2D(pResultData->data, i, j, pResultData->width) = DATA2D(pSrcData->data, i*2+1, j, pSrcData->width);
			}
	} else if (nEvenOrOdd == DYAD_EVEN && nRowOrCol == DIR_COL) {
		pResultData->height = pSrcData->height;
		pResultData->width = pSrcData->width/2;
		pResultData->data = (double *) calloc(pResultData->height*pResultData->width, sizeof(double));
		if (!pResultData->data) {
			DDP_FREE(pResultData);
			exit(1);
		}
		for (i = 0; i < pResultData->height; i++)
			for (j = 0; j < pResultData->width; j++) {
				DATA2D(pResultData->data, i, j, pResultData->width) = DATA2D(pSrcData->data, i, j*2+1, pSrcData->width);
			}
	} else if (nEvenOrOdd == DYAD_ODD && nRowOrCol == DIR_ROW) {
		pResultData->height = (pSrcData->height+1)/2;
		pResultData->width = pSrcData->width;
		pResultData->data = (double *) calloc(pResultData->height*pResultData->width, sizeof(double));
		if (!pResultData->data) {
			DDP_FREE(pResultData);
			exit(1);
		}
		for (i = 0; i < pResultData->height; i++)
			for (j = 0; j < pResultData->width; j++) {
				DATA2D(pResultData->data, i, j, pResultData->width) = DATA2D(pSrcData->data, i*2, j, pSrcData->width);
			}
	} else if (nEvenOrOdd == DYAD_ODD && nRowOrCol == DIR_COL) {
		pResultData->height = pSrcData->height;
		pResultData->width = (pSrcData->width+1)/2;
		pResultData->data = (double *) calloc(pResultData->height*pResultData->width, sizeof(double));
		if (!pResultData->data) {
			DDP_FREE(pResultData);
			exit(1);
		}
		for (i = 0; i < pResultData->height; i++)
			for (j = 0; j < pResultData->width; j++) {
				DATA2D(pResultData->data, i, j, pResultData->width) = DATA2D(pSrcData->data, i, j*2, pSrcData->width);
			}
	}
	return pResultData;
}


/* 插值 */
MATRIX * DYADUP(MATRIX * pSrcData, int nEvenOrOdd, int nRowOrCol)
{
	int i, j;
	MATRIX * pResultData = NULL;

	pResultData = (MATRIX *)calloc(1, sizeof(MATRIX));
	if (!pResultData)
		exit(1);
	
	if (nEvenOrOdd == DYAD_EVEN && nRowOrCol == DIR_ROW) {
		pResultData->height = pSrcData->height*2;
		pResultData->width = pSrcData->width;
		pResultData->data = (double *) calloc(pResultData->height*pResultData->width, sizeof(double));
		if (!pResultData->data) {
			DDP_FREE(pResultData);
			exit(1);
		}
		for (i = 0; i < pResultData->height; i++)
			for (j = 0; j < pResultData->width; j++) {
				if (i%2 == 0)
					DATA2D(pResultData->data, i, j, pResultData->width) = DATA2D(pSrcData->data, i/2, j, pSrcData->width);
				else
					DATA2D(pResultData->data, i, j, pResultData->width) = 0;
			}
	} else if (nEvenOrOdd == DYAD_EVEN && nRowOrCol == DIR_COL) {
		pResultData->height = pSrcData->height;
		pResultData->width = pSrcData->width*2;
		pResultData->data = (double *) calloc(pResultData->height*pResultData->width, sizeof(double));
		if (!pResultData->data) {
			DDP_FREE(pResultData);
			exit(1);
		}
		for (i = 0; i < pResultData->height; i++)
			for (j = 0; j < pResultData->width; j++) {
				if (j%2 == 0)
					DATA2D(pResultData->data, i, j, pResultData->width) = DATA2D(pSrcData->data, i, j/2, pSrcData->width);
				else
					DATA2D(pResultData->data, i, j, pResultData->width) = 0;
			}
	} else if (nEvenOrOdd == DYAD_ODD && nRowOrCol == DIR_ROW) {
		pResultData->height = pSrcData->height*2;
		pResultData->width = pSrcData->width;
		pResultData->data = (double *) calloc(pResultData->height*pResultData->width, sizeof(double));
		if (!pResultData->data) {
			DDP_FREE(pResultData);
			exit(1);
		}
		for (i = 0; i < pResultData->height; i++)
			for (j = 0; j < pResultData->width; j++) {
				if (i%2 != 0)
					DATA2D(pResultData->data, i, j, pResultData->width) = DATA2D(pSrcData->data, i/2, j, pSrcData->width);
				else
					DATA2D(pResultData->data, i, j, pResultData->width) = 0;
			}
	} else if (nEvenOrOdd == DYAD_ODD && nRowOrCol == DIR_COL) {
		pResultData->height = pSrcData->height;
		pResultData->width = pSrcData->width*2;
		pResultData->data = (double *) calloc(pResultData->height*pResultData->width, sizeof(double));
		if (!pResultData->data) {
			DDP_FREE(pResultData);
			exit(1);
		}
		for (i = 0; i < pResultData->height; i++)
			for (j = 0; j < pResultData->width; j++) {
				if (j%2 != 0)
					DATA2D(pResultData->data, i, j, pResultData->width) = DATA2D(pSrcData->data, i, j/2, pSrcData->width);
				else
					DATA2D(pResultData->data, i, j, pResultData->width) = 0;
			}
	}
	return pResultData;
}


/* 矩阵重定义行列数 */
MATRIX * ReShape(MATRIX * pSrcData, int nNewSizeRow, int nNewSizeCol)
{
	int i, j, k;
	MATRIX * pResultData = NULL;
	
	if (nNewSizeRow*nNewSizeCol != (pSrcData->height)*(pSrcData->width))
		exit(1);

	pResultData = (MATRIX *)calloc(1, sizeof(MATRIX));
	if (!pResultData)
		exit(1);

	pResultData->height = nNewSizeRow;
	pResultData->width = nNewSizeCol;
	pResultData->data = (double *) calloc(pResultData->height*pResultData->width, sizeof(double));
	if (!pResultData->data) {
		DDP_FREE(pResultData);
		exit(1);
	}
	
	k = 0;
	for (i = 0; i < pResultData->height; i++)
		for (j = 0; j < pResultData->width; j++) {
			DATA2D(pResultData->data, i, j, pResultData->width) = pSrcData->data[k];
			k++;
		}

	return pResultData;
}


/* 截取子矩阵 */
MATRIX * GetSubMatrix(MATRIX * pSrcData, int nSubSizeRow, int nSubSizeCol)
{
	int i, j;
	int nOffsetRow, nOffsetCol;
	MATRIX * pResultData = NULL;

	if (!(nSubSizeRow <= pSrcData->height && nSubSizeCol <= pSrcData->width))
		exit(1);

	pResultData = (MATRIX *)calloc(1, sizeof(MATRIX));
	if (!pResultData)
		exit(1);

	pResultData->height = nSubSizeRow;
	pResultData->width = nSubSizeCol;
	pResultData->data = (double *) calloc(pResultData->height*pResultData->width, sizeof(double));
	if (!pResultData->data) {
		DDP_FREE(pResultData);
		exit(1);
	}
	
	nOffsetRow = (pSrcData->height-nSubSizeRow)/2;
	nOffsetCol = (pSrcData->width-nSubSizeCol)/2;
	for (i = 0; i < pResultData->height; i++)
		for (j = 0; j < pResultData->width; j++) {
			DATA2D(pResultData->data, i, j, pResultData->width) = DATA2D(pSrcData->data, nOffsetRow+i, nOffsetCol+j, pSrcData->width);
		}

	return pResultData;
}


/* 拷贝矩阵 */
MATRIX * CopyMatrix(MATRIX * pSrcMatrix)
{
	return(GetSubMatrix(pSrcMatrix, pSrcMatrix->height, pSrcMatrix->width));
}


/* 拼接矩阵 */
MATRIX * SpliceMatrix(MATRIX * pSrcMatrixA, MATRIX * pSrcMatrixB, int nRowOrCol)
{
	int i, j;
	MATRIX * pResultData = NULL;
	
	if (!pSrcMatrixA || !pSrcMatrixB)
		exit(1);

	pResultData = (MATRIX *)calloc(1, sizeof(MATRIX));
	if (!pResultData)
		exit(1);

	if (nRowOrCol == DIR_ROW) {
		if (pSrcMatrixA->height != pSrcMatrixB->height) {
			DDP_FREE(pResultData);
			exit(1);
		}

		pResultData->height = pSrcMatrixA->height;
		pResultData->width = pSrcMatrixA->width+pSrcMatrixB->width;
		pResultData->data = (double *) calloc(pResultData->height*pResultData->width, sizeof(double));
		if (!pResultData->data) {
			DDP_FREE(pResultData);
			exit(1);
		}
	
		for (i = 0; i < pResultData->height; i++)
			for (j = 0; j < pSrcMatrixA->width; j++) {
				DATA2D(pResultData->data, i, j, pResultData->width) = DATA2D(pSrcMatrixA->data, i, j, pSrcMatrixA->width);
			}
		for (i = 0; i < pResultData->height; i++)
			for (j = 0; j < pSrcMatrixB->width; j++) {
				DATA2D(pResultData->data, i, j+pSrcMatrixA->width, pResultData->width) = DATA2D(pSrcMatrixB->data, i, j, pSrcMatrixB->width);
			}

	} else {
		if (pSrcMatrixA->width != pSrcMatrixB->width) {
			DDP_FREE(pResultData);
			exit(1);
		}

		pResultData->height = pSrcMatrixA->height+pSrcMatrixB->height;
		pResultData->width = pSrcMatrixA->width;
		pResultData->data = (double *) calloc(pResultData->height*pResultData->width, sizeof(double));
		if (!pResultData->data) {
			DDP_FREE(pResultData);
			exit(1);
		}
	
		for (i = 0; i < pSrcMatrixA->height; i++)
			for (j = 0; j < pResultData->width; j++) {
				DATA2D(pResultData->data, i, j, pResultData->width) = DATA2D(pSrcMatrixA->data, i, j, pSrcMatrixA->width);
			}
		for (i = 0; i < pSrcMatrixB->height; i++)
			for (j = 0; j < pResultData->width; j++) {
				DATA2D(pResultData->data, i+pSrcMatrixA->height, j, pResultData->width) = DATA2D(pSrcMatrixB->data, i, j, pSrcMatrixB->width);
			}
	}

	return pResultData;
}


/* 矩阵求和 */
MATRIX * SumMatrix(MATRIX * pSrcMatrixA, MATRIX * pSrcMatrixB)
{
	int i, j;
	MATRIX *pResultData = NULL;
	
	if (!pSrcMatrixA || !pSrcMatrixB)
		exit(1);

	if ((pSrcMatrixA->height != pSrcMatrixB->height) ||
		(pSrcMatrixA->width != pSrcMatrixB->width))
		exit(1);

	pResultData = (MATRIX *)calloc(1, sizeof(MATRIX));
	if (!pResultData)
		exit(1);

	pResultData->height = pSrcMatrixA->height;
	pResultData->width = pSrcMatrixA->width;
	
	pResultData->data = (double *) calloc(pResultData->height*pResultData->width, sizeof(double));
	if (!pResultData->data) {
		DDP_FREE(pResultData);
		exit(1);
	}
	
	for (i = 0; i < pResultData->height; i++)
		for (j = 0; j < pResultData->width; j++) {
			DATA2D(pResultData->data, i, j, pResultData->width) = 
				DATA2D(pSrcMatrixA->data, i, j, pSrcMatrixA->width)
				+DATA2D(pSrcMatrixB->data, i, j, pSrcMatrixB->width);
		}

	return pResultData;
}

#endif