#include"functions.h"

fftw_complex *four_graph;
fftw_complex *af_graph;
double *daf_graph;
extern fftw_complex *ap_graph;
extern int Height;
extern int Width;
fftw_complex *fft(fftw_complex *In_graph)
{
	if(four_graph == NULL)
	{
		four_graph = (fftw_complex *)malloc(sizeof(fftw_complex)*Height*Width);

	}
	fftw_plan forward;
	forward = fftw_plan_dft_2d(Height, Width, In_graph, four_graph, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(forward);
	return four_graph;
}

fftw_complex *enhence_fft(fftw_complex *Mid2_graph, struct Four_Trans f_trans)
{
	for(int i = 0; i < Height; i++)
	{
		for(int j = 0; j < Width; j++)
		{
			if((Height*Height + Width*Width) <= f_trans.Threshold)
			{
				Mid2_graph[Width*i+j][0] *= f_trans.L_enhence;
				Mid2_graph[Width*i+j][1] *= f_trans.L_enhence;
			}
			else
			{
				Mid2_graph[Width*i+j][0] *= f_trans.H_enhence;
				Mid2_graph[Width*i+j][1] *= f_trans.H_enhence;
			}
		}
	}
	return Mid2_graph;
}

double *converse_fft(fftw_complex *Mid3_graph)
{
	if(af_graph == NULL)
	{
		af_graph = (fftw_complex *)malloc(sizeof(fftw_complex) * Height * Width);
	}
	fftw_plan back;
	back = fftw_plan_dft_2d(Height, Width, Mid3_graph, af_graph, FFTW_BACKWARD, FFTW_ESTIMATE);
	fftw_execute(back);
	if(daf_graph == NULL)
	{
		daf_graph = (double *)malloc(sizeof(double) * Height * Width );
	}
	for(int i=0; i < Height*Width; i++)
	{
		daf_graph[i] = af_graph[i][0];
	}
	return daf_graph;
}