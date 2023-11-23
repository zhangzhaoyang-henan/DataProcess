#pragma once
class DataPro
{
public:
	DataPro();
	~DataPro();

	void mean(int numCameraPixels, int numCameraLen, int StartPix, int EndPix, int ColBegin, int ColEnd, UINT16 *DataIn, double *DataOut);
	void GetPhase(int numCameraPixels, int numCameraLen, int StartPix, int EndPix, int ColBegin, int ColEnd, int NoiseBegin,
		int NoiseEnd, UINT16 *DataIn, double *phase);
	void GetPixelNonuniform(double *phase1, double *phase2, int DataLen, double *pixel_nonuniform);
	void Angle(double * DataReal, double * DataImag, int DataLen, double *DataOut);
	void Unwrap(double * data_in, int DataLen, double *data_out);
};

