#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

class FFTPro
{
public:
	FFTPro();
	~FFTPro();

    #define PI 3.14159265358979323846	//‘≤÷‹¬ 

	void swap(double &a, double &b);
	void bitrp(double *xreal, double *ximag, int n);
	void FFT(double *xreal, double *ximag, int n);
	void IFFT(double *xreal, double *ximag, int n);
};
