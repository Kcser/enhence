#include "parameter.h"


//kernel functions
fftw_complex *pre_process(uint16_t *In_graph);
fftw_complex *fft(fftw_complex *Mid1_graph);
fftw_complex *enhence_fft(fftw_complex *Mid2_graph, struct Four_Trans f_trans);
double *converse_fft(fftw_complex *Mid3_graph);
double	**wavelet(uint8_t **Mid4_graph,struct Wavelet_Trans W_trans);
double	**enhence_wavelet(double **Mid5_graph,struct Wavelet_Trans W_trans);
uint8_t **converse_wavelet(double **Mid6_graph,struct Wavelet_Trans W_trans);

//sub-functions
uint16_t *read_in(string file_name);
struct Wavelet get_wavelet(string name);

//parameters
