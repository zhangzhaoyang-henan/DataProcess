#pragma once
class DataProNew
{
public:
	DataProNew();
	~DataProNew();

	void mean(int numCameraPixels, int numAline, int ColBegin, int ColEnd, double *DataIn, double *DataOut);
	void mean_matrix(int numCameraPixels, int numAline, double *DataIn, double *DataOut);
	void smooth(int Total, int span, double *DataIn, double *DataOut);
	void Angle(double * DataReal, double * DataImag, int DataLen, double *DataOut);
	void Unwrap(double * data_in, int DataLen, double *data_out);
	void GetPhase(int numCameraPixels, int numAline, double *fringebg, double *DataIn, double *DataOut, int peakWidth);
	void GetPixelNonuniform(int DataLen, double *phase1, double *phase2, double *pixel_nonuniform);
	void TapeOCT(int numCameraPixels, int TapeDataCol, UINT16 *datatape, double *pixel_nonuniform, double A2, double A3, UINT8 *scaleTape);
};

