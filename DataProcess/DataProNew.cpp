#include "stdafx.h"
#include "DataProNew.h"
#include "FFT.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <complex>
#include "gsl/gsl_errno.h"
#include "gsl/gsl_spline.h"

using namespace std;

#define pi 3.1415926535

DataProNew::DataProNew()
{
}


DataProNew::~DataProNew()
{
}

void DataProNew::mean(int numCameraPixels, int numAline, int ColBegin, int ColEnd, double *DataIn, double *DataOut)
{
	int temp = ColEnd - ColBegin + 1;
	double sum = 0;
	for (int i = 0; i < numCameraPixels * 2; i++)
	{
		for (int j = ColBegin; j <= ColEnd; j++)
		{
			sum += DataIn[i * numAline + j];
		}
		//求均值
		DataOut[i] = (double)(sum / temp);
		sum = 0;
	}
}

void DataProNew::mean_matrix(int numCameraPixels, int numAline, double *DataIn, double *DataOut)
{
	double sum = 0;
	for (int i = 0; i < numCameraPixels; i++)
	{
		for (int j = 0; j < numAline; j++)
		{
			sum += DataIn[i * numAline + j];
		}
		//求均值
		DataOut[i] = (double)(sum / numAline);
		sum = 0;
	}
}

void DataProNew::smooth(int Total, int span, double *DataIn, double *DataOut)
{
	int b = (span - 1) / 2;
	if (Total > span)
	{
		for (int i = 0; i < Total; i++)
		{

			if (i < b)
			{
				DataOut[i] = 0;
				for (int j = -i; j < i + 1; j++)
				{
					DataOut[i] += DataIn[i + j];
				}
				DataOut[i] = DataOut[i] / (2 * i + 1);
			}
 			else  if ( ( (i > b) || (i = b) ) & ( (Total - i) > b ) )
			{
				DataOut[i] = 0;
				for (int j = -b; j < b + 1; j++)
				{
					DataOut[i] += DataIn[i + j];
				}
				DataOut[i] = DataOut[i] / span;
			}
			else
			{
				DataOut[i] = 0;
				int i1 = (Total - 1) - i;
				for (int j = -i1; j < i1 + 1; j++)
				{
					DataOut[i] += DataIn[i + j];
				}
				DataOut[i] = DataOut[i] / (2 * i1 + 1);
			}
		}
	}
}

void DataProNew::Angle(double * DataReal, double * DataImag, int DataLen, double *DataOut)
{
	for (int i = 0; i < DataLen; i++)
	{
		// atan2(double y,double x)
		DataOut[i] = atan2(DataImag[i], DataReal[i]);
	}
}

void DataProNew::Unwrap(double * data_in, int DataLen, double *data_out)
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

void DataProNew::GetPhase(int numCameraPixels, int numAline, double *fringebg, double *DataIn, double *DataOut, int peakWidth)
{
	FFTPro *p_FFTPro = new FFTPro;
	//% 将(4096*100)的行数据进行平均,这个位置选择1:10的位置做平均 也可以试一试其他的并没有规定
	double *fringe = new double[numCameraPixels * 2];
	mean(numCameraPixels, numAline, 0, 9, DataIn, fringe);

	//FFT
	double *FFTDataReal = new double[numCameraPixels * 2 * numAline];
	double *FFTDataImag = new double[numCameraPixels * 2 * numAline];

	for (int i = 0; i < numAline; i++)
	{	
		double *FFTSingeReal = new double[numCameraPixels * 2];
		double *FFTSingeImag = new double[numCameraPixels * 2];
		for (int j = 0; j < numCameraPixels * 2; j++)
		{
			double test = DataIn[i + j * numAline];
			FFTSingeReal[j] = DataIn[i + j * numAline] - fringebg[j];
			FFTSingeImag[j] = 0;
		}
		p_FFTPro->FFT(FFTSingeReal, FFTSingeImag, numCameraPixels * 2);

		for (int j = 0; j < numCameraPixels * 2; j++)
		{
			FFTDataReal[i + j * numAline] = FFTSingeReal[j];
			FFTDataImag[i + j * numAline] = FFTSingeImag[j];
		}
		delete[] FFTSingeReal;
		delete[] FFTSingeImag;
	}

	//%% Peak Windowing  fringe=mean(dataZeroPadding),FTfringe为4096 complex double。
	double *FTfringeReal = new double[numCameraPixels * 2];
	memcpy(FTfringeReal, fringe, sizeof(double)* numCameraPixels * 2);
	delete[] fringe;
	double *FTfringeImag = new double[numCameraPixels * 2];
	memset(FTfringeImag, 0, sizeof(double)* numCameraPixels * 2);
	p_FFTPro->FFT(FTfringeReal, FTfringeImag, numCameraPixels * 2);
 
	//FTfringe1(1:50) = 0;  %没有规定必须前50为0，可以选择不同的值，这里的目的是为了将前边的噪音比较高的位置截掉
	memset(FTfringeReal, 0, 50 * sizeof(double));
	memset(FTfringeImag, 0, 50 * sizeof(double));

	//[value1,index1] = max(abs(FTfringe1(1:numCameraPixels)));  %取出峰值最高的 index为最高点的位置
	int index = 0;
	double tempMax = 0, absValue = 0, SquareValue = 0;
	for (int i = 0; i < numCameraPixels; i++)
	{
		double SquareValue = FTfringeReal[i] * FTfringeReal[i] + FTfringeImag[i] * FTfringeImag[i];
		double absValue = sqrt(SquareValue);
		if ( absValue > tempMax)
		{
			tempMax = absValue;
			index = i;
		}
	}
	delete[] FTfringeReal;
	delete[] FTfringeImag;

	//选择信号的宽度为peakWidth * 2   FTdata到这里还是一个4096 * 100的 complex double的数组
	for (int i = 0; i < numAline; i++)
	{
		for (int j = 0; j < index + 1 - peakWidth; j++)
		{
			FFTDataReal[i + j * numAline] = 0;
			FFTDataImag[i + j * numAline] = 0;
		}
 		for (int j = index + peakWidth; j < numCameraPixels * 2; j++)
		{
			FFTDataReal[i + j * numAline] = 0;
			FFTDataImag[i + j * numAline] = 0;
		}
	}

	//IFFT
	double *BPFdataReal = new double[numCameraPixels * 2 * numAline];
	double *BPFdataImag = new double[numCameraPixels * 2 * numAline];
	for (int i = 0; i < numAline; i++)
	{
		double *BPFSingeReal = new double[numCameraPixels * 2];
		double *BPFSingeImag = new double[numCameraPixels * 2];
		for (int j = 0; j < numCameraPixels * 2; j++)
		{
			BPFSingeReal[j] = FFTDataReal[i + j * numAline];
			BPFSingeImag[j] = FFTDataImag[i + j * numAline];
		}
		p_FFTPro->IFFT(BPFSingeReal, BPFSingeImag, numCameraPixels * 2);
		for (int j = 0; j < numCameraPixels * 2; j++)
		{
			BPFdataReal[i + j * numAline] = BPFSingeReal[j];
			BPFdataImag[i + j * numAline] = BPFSingeImag[j];
		}
		delete[] BPFSingeReal;
		delete[] BPFSingeImag;
	}
	delete[] FFTDataReal;
	delete[] FFTDataImag;

	//计算相位角,umwrap;
	//unwrap(angle(BPFdata1(1:numCameraPixels,n)));	只计算4096*100数据的前2048行。UnwrapOut为2048*100的数组。
	double *UnwrapOut = new double[numCameraPixels * numAline];
	for (int i = 0; i < numAline; i++)
	{	
		double *UnwrapSingleIn = new double[numCameraPixels];
		double *UnwrapSingleOut = new double[numCameraPixels];
		for (int j = 0; j < numCameraPixels; j++)
		{
			// atan2(double y,double x)
			UnwrapSingleIn[j] = atan2(BPFdataImag[i + j * numAline], BPFdataReal[i + j * numAline]);
		}

		Unwrap(UnwrapSingleIn, numCameraPixels, UnwrapSingleOut);
		delete[] UnwrapSingleIn;

		for (int j = 0; j < numCameraPixels; j++)
		{
			UnwrapOut[i + j * numAline] = UnwrapSingleOut[j];
		}
		delete[] UnwrapSingleOut;	
	}
	delete[] BPFdataReal;
	delete[] BPFdataImag;

	mean_matrix(numCameraPixels, numAline, UnwrapOut, DataOut);
	delete[] UnwrapOut;

	delete p_FFTPro;
}

void DataProNew::GetPixelNonuniform(int DataLen, double *phase1, double *phase2, double *pixel_nonuniform)
{
	//phase = phase1 - phase2。插值函数需要入参phase单调递增
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

	//找到拐点
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

	//将不单调部分按y = kx,变为单调函数
	double startNode = phase[tempNode];
	double k = (max - startNode) / (DataLen - tempNode);
	for (int i = tempNode, j = 0; i < DataLen; i++, j++)
	{
		phase[i] = startNode + j * k;
	}
	//防止k出现误差。最后一个值为最大值
	if (phase[DataLen - 1] < max)
	{
		phase[DataLen - 1] = max;
	}

	//单调性检查，非单调则退出
	for (int i = 0; i < DataLen - 1; i++)
	{
		if (phase[i + 1] > phase[i])
		{
			continue;
		}
		return;
	}

	//插值
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

void DataProNew::TapeOCT(int numCameraPixels, int TapeDataCol, UINT16 *datatape, double *pixel_nonuniform, double A2, double A3, UINT8 *scaleTape)
{
	//datatapei(:,n) = interp1(1:numCameraPixels, datatape(:,n), pixel_nonuniform, 'spline'); %interp1(样本点，样本值，查询点，插值方法)
	//datatap为2048*1200 double；
	double *datatape_real = new double[numCameraPixels * TapeDataCol];
	double *datatape_imag = new double[numCameraPixels * TapeDataCol];
	double *datatape_single = new double[numCameraPixels];
	double *x = new double[numCameraPixels];
	double *y = new double[numCameraPixels];
	for (int i = 0; i < numCameraPixels; i++)
	{
		x[i] = i + 1;
	}
 	for (int i = 0; i < TapeDataCol; i++)
	{
		for (int j = 0; j < numCameraPixels; j++)
		{
			y[j] = (double)datatape[i + j * TapeDataCol];
		}

		//插值
		gsl_interp_accel *acc = gsl_interp_accel_alloc();
		gsl_spline *spline = gsl_spline_alloc(gsl_interp_cspline, numCameraPixels);
		gsl_spline_init(spline, x, y, numCameraPixels);
		memset(datatape_single, 0, sizeof(double));
		for (int j = 0; j < numCameraPixels; j++)
		{
			datatape_single[j] = gsl_spline_eval(spline, pixel_nonuniform[j], acc);
		}
		gsl_spline_free(spline);
		gsl_interp_accel_free(acc);

		for (int j = 1; j <= numCameraPixels; j++)
		{
			//exp(-i*A2*(m - 1024)*(m - 1024) - i*A3*(m - 1024)*(m - 1024)*(m - 1024))
			double tempImag = -1 * A2 * (j - 1024)*(j - 1024) - 1 * A3 * (j - 1024) * (j - 1024) * (j - 1024);
			const std::complex<double> tempComplex(0.0, tempImag);
			const std::complex<double> tempComplexOut = std::exp(tempComplex);
			datatape_real[i + (j - 1) * TapeDataCol] = tempComplexOut.real() * datatape_single[j - 1];
			datatape_imag[i + (j - 1) * TapeDataCol] = tempComplexOut.imag() * datatape_single[j - 1];
		}
	}
	delete[] x;
	delete[] y;
	delete[] datatape_single;

	//2048*1200补零2048行获取4096*1200的tapeZeroPadding
	double *tapeZeroPadding_real = new double[numCameraPixels * 2 * TapeDataCol];
	double *tapeZeroPadding_imag = new double[numCameraPixels * 2 * TapeDataCol];
	memset(tapeZeroPadding_real, 0, sizeof(double)* numCameraPixels * 2 * TapeDataCol);
	memcpy(tapeZeroPadding_real, datatape_real, sizeof(double)* numCameraPixels * TapeDataCol);
	memset(tapeZeroPadding_imag, 0, sizeof(double)* numCameraPixels * 2 * TapeDataCol);
	memcpy(tapeZeroPadding_imag, datatape_imag, sizeof(double)* numCameraPixels * TapeDataCol);
	delete[] datatape_real;
	delete[] datatape_imag;

	//FTtape(:,n) = fft(tapeZeroPadding(:,n)); %4096*1200
	double *FTtape_real = new double[numCameraPixels * 2 * TapeDataCol];
	double *FTtape_imag = new double[numCameraPixels * 2 * TapeDataCol];
	FFTPro *p_FFTPro = new FFTPro;
	for (int i = 0; i < TapeDataCol; i++)
	{
		double *FFTSingeReal = new double[numCameraPixels * 2];
		double *FFTSingeImag = new double[numCameraPixels * 2];
		for (int j = 0; j < numCameraPixels * 2; j++)
		{
			FFTSingeReal[j] = tapeZeroPadding_real[i + j * TapeDataCol];
			FFTSingeImag[j] = tapeZeroPadding_imag[i + j * TapeDataCol];
		}

		p_FFTPro->FFT(FFTSingeReal, FFTSingeImag, numCameraPixels * 2);


		for (int j = 0; j < numCameraPixels * 2; j++)
		{
			FTtape_real[i + j * TapeDataCol] = FFTSingeReal[j];
			FTtape_imag[i + j * TapeDataCol] = FFTSingeImag[j];
		}
		delete[] FFTSingeReal;
		delete[] FFTSingeImag;
	}
	delete p_FFTPro;
	delete[] tapeZeroPadding_real;
	delete[] tapeZeroPadding_imag;

	//logTape = log(abs(FTtape(1:2048, : ))); %选择1:2048行 log频谱对数变换
	//此处logTape为2048*1200 double
	double *logTape = new double[numCameraPixels * TapeDataCol];
	for (int i = 0; i < TapeDataCol; i++)
	{
		for (int j = 0; j < numCameraPixels; j++)
		{
			double temp = FTtape_real[i + j * TapeDataCol] * FTtape_real[i + j * TapeDataCol]
				+ FTtape_imag[i + j * TapeDataCol] * FTtape_imag[i + j * TapeDataCol];
			double abs = sqrt(temp);
			logTape[i + j * TapeDataCol] = log(abs);
		}
	}
	delete[] FTtape_real;
	delete[] FTtape_imag;

	//maxTape = max(max(logTape));  %先取一行最大再去所有行中最大
	double maxTape = logTape[0];
	double minTape = logTape[0];
	for (int i = 0; i < TapeDataCol; i++)
	{
		for (int j = 0; j < numCameraPixels; j++)
		{
			if (maxTape < logTape[i + j * TapeDataCol])
			{
				maxTape = logTape[i + j * TapeDataCol];
			}
			if (minTape > logTape[i + j * TapeDataCol])
			{
				minTape = logTape[i + j * TapeDataCol];
			}
		}
	}

	//scaleTape = uint8(255*(logTape-minTape)/(maxTape-minTape));
	for (int i = 0; i < TapeDataCol; i++)
	{
		for (int j = 0; j < numCameraPixels; j++)
		{
			scaleTape[i + j * TapeDataCol] = (UINT8)(255 * (logTape[i + j * TapeDataCol] - minTape) / (maxTape - minTape));
		}
	}
	delete[] logTape;
}