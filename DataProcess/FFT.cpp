#include "stdafx.h"
#include "FFT.h"

FFTPro::FFTPro()
{
}


FFTPro::~FFTPro()
{
}

void FFTPro::swap(double &a, double &b)
{
	double t;
	t = a;
	a = b;
	b = t;
}

void FFTPro::bitrp(double *xreal, double *ximag, int n)
{
	// λ��ת�û� Bit-reversal Permutation
	int i, j, a, b, p;

	for (i = 1, p = 0; i < n; i *= 2)
	{
		p++;
	}
	for (i = 0; i < n; i++)
	{
		a = i;
		b = 0;
		for (j = 0; j < p; j++)
		{
			b = (b << 1) + (a & 1);    // b = b * 2 + a % 2;
			a >>= 1;        // a = a / 2;
		}
		if (b > i)
		{
			swap(xreal[i], xreal[b]);
			swap(ximag[i], ximag[b]);
		}
	}
}

void FFTPro::FFT(double *xreal, double *ximag, int n)
{
	// ���ٸ���Ҷ�任�������� x �任���Ա����� x �У�xreal, ximag �ֱ��� x ��ʵ�����鲿
	double *wreal = new double[n / 2];
	double *wimag = new double[n / 2];
	double treal, timag, ureal, uimag, arg;
	int m, k, j, t, index1, index2;

	bitrp(xreal, ximag, n);

	// ���� 1 ��ǰ n / 2 �� n �η����Ĺ���� W'j = wreal [j] + i * wimag [j] , j = 0, 1, ... , n / 2 - 1
	arg = -2 * PI / n;
	treal = cos(arg);
	timag = sin(arg);
	wreal[0] = 1.0;
	wimag[0] = 0.0;
	for (j = 1; j < n / 2; j++)
	{
		wreal[j] = wreal[j - 1] * treal - wimag[j - 1] * timag;
		wimag[j] = wreal[j - 1] * timag + wimag[j - 1] * treal;
	}

	for (m = 2; m <= n; m *= 2)
	{
		for (k = 0; k < n; k += m)
		{
			for (j = 0; j < m / 2; j++)
			{
				index1 = k + j;
				index2 = index1 + m / 2;
				t = n * j / m;    // ��ת���� w ��ʵ���� wreal [] �е��±�Ϊ t
				treal = wreal[t] * xreal[index2] - wimag[t] * ximag[index2];
				timag = wreal[t] * ximag[index2] + wimag[t] * xreal[index2];
				ureal = xreal[index1];
				uimag = ximag[index1];
				xreal[index1] = ureal + treal;
				ximag[index1] = uimag + timag;
				xreal[index2] = ureal - treal;
				ximag[index2] = uimag - timag;
			}
		}
	}
	delete[] wreal;
	delete[] wimag;
}

void FFTPro::IFFT(double *xreal, double *ximag, int n)
{
	// ���ٸ���Ҷ��任
	double *wreal = new double[n / 2];
	double *wimag = new double[n / 2];

	double treal, timag, ureal, uimag, arg;
	int m, k, j, t, index1, index2;

	bitrp(xreal, ximag, n);

	// ���� 1 ��ǰ n / 2 �� n �η��� Wj = wreal [j] + i * wimag [j] , j = 0, 1, ... , n / 2 - 1
	arg = 2 * PI / n;
	treal = cos(arg);
	timag = sin(arg);
	wreal[0] = 1.0;
	wimag[0] = 0.0;
	for (j = 1; j < n / 2; j++)
	{
		wreal[j] = wreal[j - 1] * treal - wimag[j - 1] * timag;
		wimag[j] = wreal[j - 1] * timag + wimag[j - 1] * treal;
	}

	for (m = 2; m <= n; m *= 2)
	{
		for (k = 0; k < n; k += m)
		{
			for (j = 0; j < m / 2; j++)
			{
				index1 = k + j;
				index2 = index1 + m / 2;
				t = n * j / m;    // ��ת���� w ��ʵ���� wreal [] �е��±�Ϊ t
				treal = wreal[t] * xreal[index2] - wimag[t] * ximag[index2];
				timag = wreal[t] * ximag[index2] + wimag[t] * xreal[index2];
				ureal = xreal[index1];
				uimag = ximag[index1];
				xreal[index1] = ureal + treal;
				ximag[index1] = uimag + timag;
				xreal[index2] = ureal - treal;
				ximag[index2] = uimag - timag;
			}
		}
	}
	delete[] wreal;
	delete[] wimag;

	for (j = 0; j < n; j++)
	{
		xreal[j] /= n;
		ximag[j] /= n;
	}
}
