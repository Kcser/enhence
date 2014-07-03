#include "functions.h"

int Width;
int Height;


uint16_t *graph;
fftw_complex *ap_graph;
BitMapFileHeader file_header;
BitInfoHeader info_header;
uint16_t *read_in(string file_name)
{
	FILE *in_file;
	if((in_file=fopen(file_name.c_str(),"r")) == NULL)
	{
		return NULL;
	}
	short bType;//对齐问题，将其单独列出来
	fread(&bType, sizeof(short), 1, in_file);
	fread(&file_header, sizeof(BitMapFileHeader), 1, in_file);
	fread(&info_header, sizeof(BitInfoHeader), 1, in_file);
	int Offset = file_header.bfbfOffBits;
	Width = info_header.biWidth;
	Height = info_header.biHeight;
	char temp[30];
	fread(temp,sizeof(char),Offset-54,in_file);
	if(graph == NULL)
	{
		graph = (uint16_t *)malloc(sizeof(uint16_t) * Height * Width );
	}
	for(int i = 0; i < Height; i ++)
	{
		for(int j = 0; j < Width; j++)
		{
			fread(&graph[i*Width+j], sizeof(uint16_t), 1, in_file);
		}
	}
	return graph;
}
fftw_complex *pre_process(uint16_t *In_graph)
{
	if(ap_graph == NULL)
	{
		ap_graph = (fftw_complex *)malloc(sizeof(fftw_complex)* Height *Width);
	}
	for(int i=0; i <Height; i++)
	{
		for(int j=0; j < Width; j++)
		{
			if(graph[i*Width+j] > 1017)
			{
				ap_graph[i*Height+j][0]=graph[i*Height+j]-1017;
			    if(ap_graph[i*Width+j][0]>255)
				{
				    ap_graph[i*Width+j][0]=255;
				}
			}else
            {
				ap_graph[i*Width+j][0] = 0;
			}
			ap_graph[i*Width+j][1] = 0;
		}
	}
	return ap_graph;
}