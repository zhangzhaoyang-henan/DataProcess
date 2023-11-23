#include "stdafx.h"
#include "DataPro.h"
#include "FFT.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "gsl/gsl_errno.h"
#include "gsl/gsl_spline.h"

using namespace std;

#define pi 3.1415926535

DataPro::DataPro()
{
}


DataPro::~DataPro()
{
}

void DataPro::mean(int numCameraPixels, int numCameraLen, int StartPix, int EndPix, int ColBegin, int ColEnd, UINT16 *DataIn, double *DataOut)
{
	int temp = ColEnd - ColBegin + 1;
	double sum = 0;
	for (int i = StartPix; i <= EndPix; i++)
	{
		for (int j = ColBegin; j <= ColEnd; j++)
		{
			sum += (double)DataIn[i * numCameraLen + j];
		}
		//���ֵ
		DataOut[i - StartPix] = (double) (sum / temp);
		sum = 0;
	}
}

void DataPro::GetPhase(int numCameraPixels, int numCameraLen, int StartPix, int EndPix, int ColBegin, int ColEnd, int NoiseBegin, int NoiseEnd,
	UINT16 *DataIn, double *phase)
{	
	FFTPro *p_FFTPro = new FFTPro;
	//%�����ݽ���ƽ��,���λ��ѡ��100:150��λ����ƽ�� Ҳ������һ�������Ĳ�û�й涨
	//matlab��1�������˴���һ��������һ�£�������ԡ� 100:150 ��ColBegin��ColEnd���ƣ��������ⲿ�Ѿ�д���ˡ�
	int tempSize = (EndPix - StartPix + 1);
	double *MeanDataOut = new double[tempSize];
	mean(numCameraPixels, numCameraLen, StartPix, EndPix, ColBegin, ColEnd, DataIn, MeanDataOut);

	//FFT
	//FFT��ҪΪ2�Ĵ��ݵ�����,MeanDataOutΪʵ�����鲿Ϊ0
	double *FFTDataReal = new double[numCameraPixels];
	double *FFTDataImag = new double[numCameraPixels];
	memset(FFTDataReal, 0, sizeof(double)* numCameraPixels);
	memset(FFTDataImag, 0, sizeof(double)* numCameraPixels);
	memcpy(FFTDataReal, MeanDataOut, sizeof(double)* tempSize);
	delete[] MeanDataOut;
	p_FFTPro->FFT(FFTDataReal, FFTDataImag, numCameraPixels);

	//ѡȡ��󣬽�����λ�õ����ݱ�Ϊ0 ȥ����
	memset(FFTDataReal, 0, sizeof(double)* NoiseBegin);
	memset(FFTDataImag, 0, sizeof(double)* NoiseBegin);
	memset(FFTDataReal + (NoiseEnd - 1), 0, sizeof(double)* (numCameraPixels - (NoiseEnd - 1)));
	memset(FFTDataImag + (NoiseEnd - 1), 0, sizeof(double)* (numCameraPixels - (NoiseEnd - 1)));

	//IFFT��Ҫ��2048������4096
	double *IFFTDataReal = new double[numCameraPixels * 2];
	double *IFFTDataImag = new double[numCameraPixels * 2];
	memset(IFFTDataReal, 0, sizeof(double)* numCameraPixels * 2);
	memset(IFFTDataImag, 0, sizeof(double)* numCameraPixels * 2);
	memcpy(IFFTDataReal, FFTDataReal, sizeof(double)* numCameraPixels);
	memcpy(IFFTDataImag, FFTDataImag, sizeof(double)* numCameraPixels);
	delete[] FFTDataReal;
	delete[] FFTDataImag;

	//IFFT
	p_FFTPro->IFFT(IFFTDataReal, IFFTDataImag, numCameraPixels * 2);

	//������λ��
	double *AngleOut = new double[numCameraPixels * 2];
	Angle(IFFTDataReal, IFFTDataImag, numCameraPixels * 2, AngleOut);
	delete[] IFFTDataReal;
	delete[] IFFTDataImag;

	//umwrap
	Unwrap(AngleOut, numCameraPixels * 2, phase);
	delete[] AngleOut;

	delete p_FFTPro;
}

void DataPro::GetPixelNonuniform(double *phase1, double *phase2, int DataLen, double *pixel_nonuniform)
{
	//phase = phase1 - phase2����ֵ������Ҫ���phase��������
	double *phase = new double[DataLen];
	double max = phase2[0] - phase1[0];
	double min = phase2[0] - phase1[0];
	for (int i = 0; i < DataLen; i++)
	{
		phase[i] = phase2[i] - phase1[i];
		if (phase[i] > max)
		{
			max = phase[i];
		}
		if (phase[i] < min)
		{
			min = phase[i];
		}
	}

	//�ҵ��յ�
	int tempNode = 0;
	for (int i = 0; i < DataLen - 1; i++)
	{
		if (phase[i + 1] > phase[i])
		{
			continue;
		}
		tempNode = i;
		break;
	}

	//�����������ְ�y = kx,��Ϊ��������
	double startNode = phase[tempNode];
	double k = (max - startNode) / (DataLen - tempNode);
	for (int i = tempNode, j = 0; i < DataLen; i++, j++)
	{
		phase[i] = startNode + j * k;
	}
	//��ֹk���������һ��ֵΪ���ֵ
	if (phase[DataLen - 1] < max)
	{
		phase[DataLen - 1] = max;
	}

	//�����Լ�飬�ǵ������˳�
	for (int i = 0; i < DataLen - 1; i++)
	{
		if (phase[i + 1] > phase[i])
		{
			continue;
		}
		return;
	}

	//��ֵ
	double *y = new double[DataLen];
	double *xi = new double[DataLen];
	double temp = (max - min) / DataLen;
	for (int i = 0; i < DataLen; i++)
	{
		y[i] = i + 1;
		xi[i] = min + i * temp;
	}

	gsl_interp_accel *acc = gsl_interp_accel_alloc();
	gsl_spline *spline = gsl_spline_alloc(gsl_interp_cspline, DataLen);

	gsl_spline_init(spline, phase, y, DataLen);

	for (int i = 0; i < DataLen; i++)
	{
		pixel_nonuniform[i] = gsl_spline_eval(spline, xi[i], acc);
	}

	gsl_spline_free(spline);
	gsl_interp_accel_free(acc);


	for (int i = 0; i < DataLen; i++)
	{
		if (pixel_nonuniform[i] >= DataLen - 1)
		{
			pixel_nonuniform[i] = DataLen - 1;
		}
		if (pixel_nonuniform[i] < 1)
		{
			pixel_nonuniform[i] = 1;
		}
	}

	delete[] xi;
	delete[] y;
	delete[] phase;
}

void DataPro::Angle(double * DataReal, double * DataImag, int DataLen, double *DataOut)
{
	for (int i = 0; i < DataLen; i++)
	{
		// atan2(double y,double x)
		DataOut[i] = atan2(DataImag[i], DataReal[i]);
	}
}

void DataPro::Unwrap(double * data_in, int DataLen, double *data_out)
{
	int cen = 0;
	data_out[0] = data_in[0];
	if (((data_in[1]>data_in[0]) && ((data_in[1] - data_in[0])<pi)) || ((data_in[1] - data_in[0])<-pi))
	{
		cen = 1;//+
	}
	else
	{
		cen = 2;//-
	}

	for (int i = 1; i < DataLen; i++)
	{
		if (abs(data_in[i] - data_out[i - 1]) < pi)
		{
			data_out[i] = data_in[i];
			//printf("%f ", data_out[i]);
			//cout << endl;
		}
		else
		{
			if (cen == 1)//+
			{
				data_out[i] = data_in[i] + 2 * pi;
			loop1:
				if (abs(data_out[i] - data_out[i - 1]) > pi)
				{
					data_out[i] = data_out[i] + 2 * pi;
					goto loop1;
				}
				//printf("%f ", data_out[i]);
				//cout << endl;
			}
			else if (cen == 2)//-
			{
				data_out[i] = data_in[i] - 2 * pi;
			loop2:
				if (abs(data_out[i] - data_out[i - 1]) > pi)
				{
					data_out[i] = data_out[i] - 2 * pi;
					goto loop2;
				}
				//printf("%f ", data_out[i]);
				//cout << endl;
			}
		}
	}
}
