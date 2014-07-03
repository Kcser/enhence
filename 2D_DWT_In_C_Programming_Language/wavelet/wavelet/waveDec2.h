#include "utilityFunWT.h"
#include "malloc.h"

WTCOEFSet * SubWaveDec2(MATRIX * pSrcData, WaveletBASE * pWaveBase, int nExtType);

WTInfo * waveDec2(MATRIX * pSrcData, int nWTLevel, int nWaveType, int nExtType)
{
	int i, j, k;
	int nCoefLen;
	WTInfo * pDecResult = NULL;

	MATRIX pTempCoefPanel;
	WTCOEFSet * * pWTCoefSet = NULL;
	WaveletBASE * pWaveletBase = NULL;

	/* Ԥ����Ž���Ľṹ�� */
	if (!(pDecResult = (WTInfo *)calloc(1, sizeof(WTInfo))))
		exit(1);
	pDecResult->m_nExtType = nExtType;
	pDecResult->m_nWaveType = nWaveType;
	pDecResult->m_nWTLevel = nWTLevel;
	pDecResult->m_pC = NULL;
	pDecResult->m_pS = NULL;

	/* ����ռ䣬���ڴ��ָ��С��ϵ����ָ�� */
	pWTCoefSet = (WTCOEFSet * *)calloc(nWTLevel, sizeof(WTCOEFSet *));
	if (!pWTCoefSet) {
		WI_FREE(pDecResult);
		exit(1);
	}

	/* ����nWaveType��ȡС�������˲���ϵ����Ϊ�˽�Ͼ�����㣬���ȶԸ�ϵ�����˵��� */
	if (!(pWaveletBase = SetWaveletBase(nWaveType))) {
		WI_FREE(pDecResult);
		exit(1);
	}


	pTempCoefPanel.height = pSrcData->height;
	pTempCoefPanel.width = pSrcData->width;
	pTempCoefPanel.data = pSrcData->data;
	for (i = 0; i < nWTLevel; i++) {
		pWTCoefSet[i] = SubWaveDec2(&pTempCoefPanel, pWaveletBase, nExtType);
		pTempCoefPanel.height = pWTCoefSet[i]->A_LL->height;
		pTempCoefPanel.width = pWTCoefSet[i]->A_LL->width;
		pTempCoefPanel.data = pWTCoefSet[i]->A_LL->data;
	}

	/* ͳ��С��ϵ�����ܳ��� */
	nCoefLen = 0;
	for (i = 0; i < nWTLevel; i++) {
		nCoefLen += pWTCoefSet[i]->H_LH->height*pWTCoefSet[i]->H_LH->width;
		nCoefLen += pWTCoefSet[i]->V_HL->height*pWTCoefSet[i]->V_HL->width;
		nCoefLen += pWTCoefSet[i]->D_HH->height*pWTCoefSet[i]->D_HH->width;
		if (i == nWTLevel-1)
			nCoefLen += pWTCoefSet[i]->A_LL->height*pWTCoefSet[i]->A_LL->width;
	}

	/* �ռ�С���ֽ�ϵ�� */
	pDecResult->m_pC = (double *)calloc(nCoefLen, sizeof(double));
	pDecResult->m_pS = (int *)calloc((nWTLevel+2)*2, sizeof(int));
	if (pDecResult->m_pC == NULL || pDecResult->m_pS == NULL) {
		WB_FREE(pWaveletBase);
		WI_FREE(pDecResult);
		exit(1);
	}

	/* ��¼��n���ĵ�Ƶ�����Լ���������ˮƽ����ֱ���ԽǷ��� */
	k = 0;
	for (i = nWTLevel-1; i >= 0; i--) {
		if (i == nWTLevel-1) {
			for (j = 0; j < pWTCoefSet[i]->A_LL->height*pWTCoefSet[i]->A_LL->width; j++) {
				pDecResult->m_pC[k] = pWTCoefSet[i]->A_LL->data[j];
				k++;
			}
			DATA2D(pDecResult->m_pS, 0, 0, 2) = pWTCoefSet[nWTLevel-1]->A_LL->height;
			DATA2D(pDecResult->m_pS, 0, 1, 2) = pWTCoefSet[nWTLevel-1]->A_LL->width;
		}
		for (j = 0; j < pWTCoefSet[i]->H_LH->height*pWTCoefSet[i]->H_LH->width; j++) {
			pDecResult->m_pC[k] = pWTCoefSet[i]->H_LH->data[j];
			k++;
		}
		for (j = 0; j < pWTCoefSet[i]->V_HL->height*pWTCoefSet[i]->V_HL->width; j++) {
			pDecResult->m_pC[k] = pWTCoefSet[i]->V_HL->data[j];
			k++;
		}
		for (j = 0; j < pWTCoefSet[i]->D_HH->height*pWTCoefSet[i]->D_HH->width; j++) {
			pDecResult->m_pC[k] = pWTCoefSet[i]->D_HH->data[j];
			k++;
		}
		DATA2D(pDecResult->m_pS, nWTLevel-i, 0, 2) = pWTCoefSet[i]->A_LL->height;
		DATA2D(pDecResult->m_pS, nWTLevel-i, 1, 2) = pWTCoefSet[i]->A_LL->width;
	}

	DATA2D(pDecResult->m_pS, nWTLevel+1, 0, 2) = pSrcData->height;
	DATA2D(pDecResult->m_pS, nWTLevel+1, 1, 2) = pSrcData->width;

	WB_FREE(pWaveletBase);
	return pDecResult;
}


WTCOEFSet * SubWaveDec2(MATRIX * pSrcData, WaveletBASE * pWaveBase, int nExtType)
{
	WTCOEFSet * pWTResult = NULL;
	MATRIX * pTempDataA_LL = NULL, * pTempDataH_LH = NULL, * pTempDataV_HL = NULL, * pTempDataD_HH = NULL;
	MATRIX * pTempDataA_LL2 = NULL, * pTempDataH_LH2 = NULL, * pTempDataV_HL2 = NULL, * pTempDataD_HH2 = NULL;
	MATRIX * pTempDataExt = NULL, * pTempDataA_H = NULL, * pTempDataV_D = NULL;
	MATRIX * pTempDataA_H2 = NULL, * pTempDataV_D2 = NULL;

	if (!(pWTResult = (WTCOEFSet*)calloc(1, sizeof(WTCOEFSet))))
		exit(1);

	/* ����nExtType��ԭʼ���ݽ������� */
	pTempDataExt = WExtend2D(pSrcData, nExtType, pWaveBase->nFilterLen-1, pWaveBase->nFilterLen-1);

	/* �з�������ͨ�˲� �в��� */
	pTempDataA_H = CONV2D(pTempDataExt, pWaveBase->LO_D, pWaveBase->nFilterLen, DIR_ROW, CONVT_FULL);
	pTempDataA_H2 = DYADDOWN(pTempDataA_H, DYAD_EVEN, DIR_COL);
	DDP_FREE(pTempDataA_H);

	/* �з�������ͨ/��ͨ�˲����в������õ���Ƶ������ˮƽ���� */
	pTempDataA_LL = CONV2D(pTempDataA_H2, pWaveBase->LO_D, pWaveBase->nFilterLen, DIR_COL, CONVT_FULL);
	pTempDataA_LL2 = DYADDOWN(pTempDataA_LL, DYAD_EVEN, DIR_ROW);
	DDP_FREE(pTempDataA_LL);

	pTempDataH_LH = CONV2D(pTempDataA_H2, pWaveBase->HI_D, pWaveBase->nFilterLen, DIR_COL, CONVT_FULL);
	pTempDataH_LH2 = DYADDOWN(pTempDataH_LH, DYAD_EVEN, DIR_ROW);
	DDP_FREE(pTempDataH_LH);

	/* �з�������ͨ�˲� �в��� */
	pTempDataV_D = CONV2D(pTempDataExt, pWaveBase->HI_D, pWaveBase->nFilterLen, DIR_ROW, CONVT_FULL);
	pTempDataV_D2 = DYADDOWN(pTempDataV_D, DYAD_EVEN, DIR_COL);
	DDP_FREE(pTempDataV_D);

	/* �з�������ͨ/��ͨ�˲����в������õ���ֱ�����ͶԽǷ��� */
	pTempDataV_HL = CONV2D(pTempDataV_D2, pWaveBase->LO_D, pWaveBase->nFilterLen, DIR_COL, CONVT_FULL);
	pTempDataV_HL2 = DYADDOWN(pTempDataV_HL, DYAD_EVEN, DIR_ROW);
	DDP_FREE(pTempDataV_HL);

	pTempDataD_HH = CONV2D(pTempDataV_D2, pWaveBase->HI_D, pWaveBase->nFilterLen, DIR_COL, CONVT_FULL);
	pTempDataD_HH2 = DYADDOWN(pTempDataD_HH, DYAD_EVEN, DIR_ROW);
	DDP_FREE(pTempDataD_HH);

	pWTResult->A_LL = pTempDataA_LL2;
	pWTResult->H_LH = pTempDataH_LH2;
	pWTResult->V_HL = pTempDataV_HL2;
	pWTResult->D_HH = pTempDataD_HH2;

	DDP_FREE(pTempDataExt);	DDP_FREE(pTempDataA_H2);	DDP_FREE(pTempDataV_D2);
	return pWTResult;
}


WTInfo * waveDec2Int(IntDataPanel2D * pSrcData, int nWTLevel, int nWaveType, int nExtType)
{
	int i, j;
	MATRIX * pTempSrcData = NULL;
	WTInfo * pWTResult = NULL;

	if (!pSrcData)
		exit(1);

	if (!(pTempSrcData = (MATRIX *)calloc(1, sizeof(MATRIX))))
		exit(1);

	pTempSrcData->height = pSrcData->height;
	pTempSrcData->width = pSrcData->width;
	pTempSrcData->data = (double *) calloc(pTempSrcData->height*pTempSrcData->width, sizeof(double));
	if (!pTempSrcData->data) {
		DDP_FREE(pTempSrcData);
		exit(1);
	}

	for (i = 0; i < pTempSrcData->height; i++)
		for (j = 0; j < pTempSrcData->width; j++)
			DATA2D(pTempSrcData->data, i, j, pTempSrcData->width) = (double)DATA2D(pSrcData->data, i, j, pSrcData->width);

	pWTResult = waveDec2(pTempSrcData, nWTLevel, nWaveType, nExtType);

	DDP_FREE(pTempSrcData);
	return pWTResult;
}
