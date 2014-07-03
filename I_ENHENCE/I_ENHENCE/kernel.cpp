#include "functions.h"


extern uint16_t *graph;
extern fftw_complex *ap_graph;
extern fftw_complex *af_graph;
extern fftw_complex *four_graph;
struct Four_Trans f_trans;
int main()
{
	read_in("0.bmp");
	pre_process(graph);
	fft(ap_graph);
	enhence_fft(four_graph, f_trans);
	converse_fft(four_graph);
	return 0;
}