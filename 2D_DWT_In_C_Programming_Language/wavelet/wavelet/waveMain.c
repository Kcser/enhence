#include "stdio.h"
#include "stdlib.h"
#include "waveDec2.h"
#include "waveRec2.h"


int main(int argc, char * argv[])
{
	int i, j, k;
	IntDataPanel2D ImageDataSrc;
	WTInfo * pImageDataDec = NULL;
	IntDataPanel2D * pImageDataRec = NULL;
	double * pTracePointer = NULL;
	int nImageRow, nImageCol;
	FILE * fpImageDataFile = NULL;

//	int nCLen;
///	int nTest;
//	double dbTest;

	/* 读取数据 */
	if ((fpImageDataFile = fopen("LENA.txt", "r")) == NULL) {
		return 1;
	}
	
	fscanf(fpImageDataFile, "%d", &nImageRow);
	fscanf(fpImageDataFile, "%d", &nImageCol);
	
	ImageDataSrc.height = nImageRow;
	ImageDataSrc.width = nImageCol;
	ImageDataSrc.data = (int *)calloc(nImageRow*nImageCol, sizeof(int));
	if (ImageDataSrc.data == NULL)
		return 1;

	for (i = 0; i < nImageRow*nImageCol; i++)
		fscanf(fpImageDataFile, "%d", &(ImageDataSrc.data[i]));
	fclose(fpImageDataFile);
	
	/* 小波变换 */
	pImageDataDec = waveDec2Int(&ImageDataSrc, 3, WAVE_DB3, ExtT_SYM);
	
	pTracePointer = pImageDataDec->m_pC;
	printf("\n\n------------%d--A-------\n", 0);
	for (i = 0; i < DATA2D(pImageDataDec->m_pS, 0, 0, 2); i++) {
		for (j = 0; j < DATA2D(pImageDataDec->m_pS, 0, 1, 2); j++) {
			printf("%10f\t", *pTracePointer);
			pTracePointer++;
		}
		printf("\n");
	}
	for (k = 1; k < pImageDataDec->m_nWTLevel; k++) {
		printf("\n\n------------%d--H-------\n", k-1);
		for (i = 0; i < DATA2D(pImageDataDec->m_pS, k, 0, 2); i++) {
			for (j = 0; j < DATA2D(pImageDataDec->m_pS, k, 1, 2); j++) {
				printf("%10f\t", *pTracePointer);
				pTracePointer++;
			}
			printf("\n");
		}
		printf("\n\n------------%d--V-------\n", k-1);
		for (i = 0; i < DATA2D(pImageDataDec->m_pS, k, 0, 2); i++) {
			for (j = 0; j < DATA2D(pImageDataDec->m_pS, k, 1, 2); j++) {
				printf("%10f\t", *pTracePointer);
				pTracePointer++;
			}
			printf("\n");
		}
		printf("\n\n------------%d--D-------\n", k-1);
		for (i = 0; i < DATA2D(pImageDataDec->m_pS, k, 0, 2); i++) {
			for (j = 0; j < DATA2D(pImageDataDec->m_pS, k, 1, 2); j++) {
				printf("%10f\t", *pTracePointer);
				pTracePointer++;
			}
			printf("\n");
		}
	}
	//nCLen = DATA2D(pImageDataDec->m_pS, 0, 0, 2)*DATA2D(pImageDataDec->m_pS, 0, 1, 2);;
	//for (i = 1; i < pImageDataDec->m_nWTLevel+1; i++) {
	//	j = DATA2D(pImageDataDec->m_pS, i, 0, 2);
	//	j = DATA2D(pImageDataDec->m_pS, i, 1, 2);
	//	nCLen = DATA2D(pImageDataDec->m_pS, i, 0, 2)*DATA2D(pImageDataDec->m_pS, i, 1, 2)*3;
	//}
	//for (i = 0; i < nCLen; i++) {
	//	// dbTest = pImageDataDec->m_pC[i];
	//	printf("%10f", pImageDataDec->m_pC[i]);
	//}

	/* 小波逆变换 */
	printf("\n--------Inverse Result--------\n");
	pImageDataRec = waveRec2Int(pImageDataDec);
	for (i = 0; i < pImageDataRec->height; i++) {
		for (j = 0; j < pImageDataRec->width; j++)
			// nTest = DATA2D(pImageDataRec->data, i, j, pImageDataRec->width);
			printf("%5d", DATA2D(pImageDataRec->data, i, j, pImageDataRec->width));
		printf("\n");
	}

	WI_FREE(pImageDataDec);
	IDP_FREE(pImageDataRec);
	return 0;
}