#include "fftw\fftw3.h"
int main1()
{
    fftw_complex *in, *out;
	fftw_complex *in1;
    fftw_plan p,q;
    int N= 8;
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	in1 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    for( int i=0; i < N; i++)
           {
        in[i][0] = 1.0;
        in[i][1] = 0.0;
        printf("%6.2f ",in[i][0]);
        }
    printf("\n");  // in 是输入的数据
    p=fftw_plan_dft_1d(N,in,out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p); /* repeat as needed*/
	q=fftw_plan_dft_1d(N,out,in1,FFTW_BACKWARD, FFTW_ESTIMATE);
    for(int j = 0;j < N;j++)
          {
        printf("%6.2f ",out[j][0]);
          }
    printf("\n");
    fftw_destroy_plan(p);
    fftw_free(in); 
    fftw_free(out);
    return 0;
}