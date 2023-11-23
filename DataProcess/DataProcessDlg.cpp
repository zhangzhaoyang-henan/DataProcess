
// DataProcessDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataProcess.h"
#include "DataProcessDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDataProcessDlg �Ի���



CDataProcessDlg::CDataProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataProcessDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDataProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDataProcessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_FILE1, &CDataProcessDlg::OnBnClickedFile1)
	ON_BN_CLICKED(IDC_FILE2, &CDataProcessDlg::OnBnClickedFile2)
	ON_BN_CLICKED(IDC_OLD_ALGO, &CDataProcessDlg::OnBnClickedOldAlgo)
	ON_BN_CLICKED(IDC_NEW_ALGO, &CDataProcessDlg::OnBnClickedNewAlgo)
	ON_BN_CLICKED(IDC_FILE3, &CDataProcessDlg::OnBnClickedFile3)
END_MESSAGE_MAP()


// CDataProcessDlg ��Ϣ�������

BOOL CDataProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDataProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDataProcessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDataProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//#include "kernel.cu"
extern int main(int argc, char **argv);

#include "DataPro.h"
#include "DataProNew.h"

CString strFirstFilePath;
CString strSecondFilePath;
CString filetape;

void CDataProcessDlg::OnBnClickedFile1()
{
	// TODO: Add your control notification handler code here   
	// ���ù�����   
	TCHAR szFilter[] = _T("(*.bin)|*.bin|�ı��ļ�(*.txt)|*.txt|�����ļ�(*.*)|*.*||");
	// ������ļ��Ի���   
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);

	// ��ʾ���ļ��Ի���   
	if (IDOK == fileDlg.DoModal())
	{
		// ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����   
		strFirstFilePath = fileDlg.GetPathName();
		SetDlgItemText(IDC_EDIT1, strFirstFilePath);
	}
}

void CDataProcessDlg::OnBnClickedFile2()
{
	// TODO: Add your control notification handler code here   
	// ���ù�����   
	TCHAR szFilter[] = _T("(*.bin)|*.bin|�ı��ļ�(*.txt)|*.txt|�����ļ�(*.*)|*.*||");
	// ������ļ��Ի���   
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);

	// ��ʾ���ļ��Ի���   
	if (IDOK == fileDlg.DoModal())
	{
		// ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����   
		strSecondFilePath = fileDlg.GetPathName();
		SetDlgItemText(IDC_EDIT2, strSecondFilePath);
	}
}

void CDataProcessDlg::OnBnClickedFile3()
{
	// TODO: Add your control notification handler code here   
	// ���ù�����   
	TCHAR szFilter[] = _T("(*.bin)|*.bin|�ı��ļ�(*.txt)|*.txt|�����ļ�(*.*)|*.*||");
	// ������ļ��Ի���   
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);

	// ��ʾ���ļ��Ի���   
	if (IDOK == fileDlg.DoModal())
	{
		// ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����   
		filetape = fileDlg.GetPathName();
		SetDlgItemText(IDC_EDIT3, filetape);
	}
}

void CDataProcessDlg::OnBnClickedOldAlgo()
{
	//����У׼�ļ�
	int numCameraPixels = 2048;
	int numCameraLen = 5000;
	//matlab��1�������˴���һ��������һ�£�������ԡ�
	int StartPix = 59;
	int EndPix = 1949;
	int ColBegin = 99;
	int ColEnd = 149;

	int NoiseBegin1 = 160;
	int NoiseEnd1 = 195;

	int NoiseBegin2 = 210;
	int NoiseEnd2 = 235;

	//test
	strFirstFilePath = "F:\\DATA\\ʹ��ƥ���_01��_02��\\M0008_2022_P0000001\\M0008_2022_P0000001_circle_0.0x3.1_C3_S3.bin";
	strSecondFilePath = "F:\\DATA\\ʹ��ƥ���_01��_02��\\M0008_2022_P0000001\\M0008_2022_P0000001_circle_0.0x3.1_C4_S4.bin";

	UINT16 *data1 = new UINT16[numCameraPixels * numCameraLen];
	CFile cFile1;
	cFile1.Open(strFirstFilePath, CFile::modeRead);
	cFile1.Read(data1, sizeof(UINT16) * numCameraPixels * numCameraLen);
	cFile1.Close();

	UINT16 *data2 = new UINT16[numCameraPixels * numCameraLen];
	CFile cFile2;
	cFile2.Open(strSecondFilePath, CFile::modeRead);
	cFile2.Read(data2, sizeof(UINT16) * numCameraPixels * numCameraLen);
	cFile2.Close();
	
	DataPro *p_DataPro = new DataPro;
	double *phase1 = new double[numCameraPixels * 2];
	double *phase2 = new double[numCameraPixels * 2];
	
	//��������������Ҫ����ת�á�
	UINT16 *DataIn1	= new UINT16[numCameraPixels * numCameraLen];
	UINT16 *DataIn2 = new UINT16[numCameraPixels * numCameraLen];
	for (int i = 0; i < numCameraLen; i++)
	{
		for (int j = 0; j < numCameraPixels; j++)
		{
			DataIn1[i + j * numCameraLen] = data1[i * numCameraPixels + j];
			DataIn2[i + j * numCameraLen] = data2[i * numCameraPixels + j];
		}
	}
	delete[] data1;
	delete[] data2;
 
	p_DataPro->GetPhase(numCameraPixels, numCameraLen, StartPix, EndPix, ColBegin, ColEnd, NoiseBegin1, NoiseEnd1, DataIn1, phase1);
	p_DataPro->GetPhase(numCameraPixels, numCameraLen, StartPix, EndPix, ColBegin, ColEnd, NoiseBegin2, NoiseEnd2, DataIn2, phase2);
	delete[] DataIn1;
	delete[] DataIn2;

	//pixel_nonuniform
	double *pixel_nonuniform = new double[numCameraPixels * 2];
	p_DataPro->GetPixelNonuniform(phase1, phase2, numCameraPixels * 2, pixel_nonuniform);
	delete[] phase1;
	delete[] phase2;

	//����pixel_nonuniform�ļ�����matlab����һ�£�����
	double *pixel_nonuniform_out = new double[numCameraPixels * 2];
	for (int i = 0; i < numCameraPixels * 2; i++)
	{
		pixel_nonuniform_out[numCameraPixels * 2 - 1 - i] = pixel_nonuniform[i];
	}
	delete[] pixel_nonuniform;

	//CFile cFile;
	//cFile.Open("F:\\DATA\\PhaseCalibration_C04_old.txt", CFile::modeWrite | CFile::modeCreate);
	//cFile.Write(pixel_nonuniform_out, numCameraPixels * 2 * sizeof(double));
	//cFile.Close();

	FILE * fp;
	fp = fopen("F:\\DATA\\PhaseCalibration_C04_old.txt", "w");
	if (fp != NULL)
	{
		for (int i = 0; i < numCameraPixels * 2; i++)
		{
			fprintf(fp, "%4.4f ", pixel_nonuniform_out[i]);
		}
		fclose(fp);
	}
	
	delete[] pixel_nonuniform_out;
	delete p_DataPro;

	//��������
	main(1, nullptr);
}

void CDataProcessDlg::OnBnClickedNewAlgo()
{
	//����У׼�ļ�
	//% the number of line scan camera pixels
	int numCameraPixels = 2048;
	//% the number of A - lines to be read
	int numAline = 100;
	//% fft peak windowing half width
	int peakWidth = 50;
	//% dispersion coefficient a2 
	double A2 = -0.0000055;
	//% dispersion coefficient a3
	double A3 = 2.0e-10;

	//test
	strFirstFilePath  = "F:\\DATA\\ʹ��ƥ���_01��_02��\\M0008_2022_P0000001\\M0008_2022_P0000001_circle_0.0x3.1_C3_S3.bin";
	strSecondFilePath = "F:\\DATA\\ʹ��ƥ���_01��_02��\\M0008_2022_P0000001\\M0008_2022_P0000001_circle_0.0x3.1_C4_S4.bin";
	filetape = "F:\\DATA\\ʹ��ƥ���_01��_02��\\M0008_2022_P0000002_circle_3.0x3.1_C7_S7.bin";

	UINT16 *data1 = new UINT16[numCameraPixels * numAline];
	CFile cFile1;
	cFile1.Open(strFirstFilePath, CFile::modeRead);
	cFile1.Read(data1, sizeof(UINT16)* numCameraPixels * numAline);
	cFile1.Close();

	UINT16 *data2 = new UINT16[numCameraPixels * numAline];
	CFile cFile2;
	cFile2.Open(strSecondFilePath, CFile::modeRead);
	cFile2.Read(data2, sizeof(UINT16)* numCameraPixels * numAline);
	cFile2.Close();

	//��������������Ҫ����ת�á�
	UINT16 *DataIn1 = new UINT16[numCameraPixels * numAline];
	UINT16 *DataIn2 = new UINT16[numCameraPixels * numAline];
	for (int i = 0; i < numAline; i++)
	{
		for (int j = 0; j < numCameraPixels; j++)
		{
			DataIn1[i + j * numAline] = data1[i * numCameraPixels + j];
			DataIn2[i + j * numAline] = data2[i * numCameraPixels + j];
		}
	}
	delete[] data1;
	delete[] data2;

	DataProNew *p_DataPro = new DataProNew;
	//data1ZeroPadding = [data1;zeros(numCameraPixels,numAline)];% zeros(2048,100)   ����֮��data1ZeroPadding(4096*100)
	double *data1ZeroPadding = new double[numCameraPixels * 2 * numAline];
	double *data2ZeroPadding = new double[numCameraPixels * 2 * numAline];
	memset(data1ZeroPadding, 0, sizeof(double) * numCameraPixels * 2 * numAline);
	memset(data2ZeroPadding, 0, sizeof(double) * numCameraPixels * 2 * numAline);
	for (int i = 0; i < numCameraPixels * numAline; i++)
	{
		data1ZeroPadding[i] = (double)DataIn1[i];
		data2ZeroPadding[i] = (double)DataIn2[i];
	}
	delete[] DataIn1;
	delete[] DataIn2;

	double *phase1_mean = new double[numCameraPixels];
	double *phase2_mean = new double[numCameraPixels];
 
	//fringebgΪ�������ǰ����
	double *meanOut = new double[numCameraPixels * 2];
	double *fringebg = new double[numCameraPixels * 2];
	p_DataPro->mean(numCameraPixels, numAline, 0, 99, data1ZeroPadding, meanOut);
	//% background calculation  ʹ��10�Ŵ�������ƽ������  smooth(A, span) span Ϊ�ƶ�ƽ���Ĵ����������˴�ȡ5
	p_DataPro->smooth(numCameraPixels * 2, 5, meanOut, fringebg);
	delete[] meanOut;

	p_DataPro->GetPhase(numCameraPixels, numAline, fringebg, data1ZeroPadding, phase1_mean, peakWidth);
	p_DataPro->GetPhase(numCameraPixels, numAline, fringebg, data2ZeroPadding, phase2_mean, peakWidth);
	delete[] data1ZeroPadding;
	delete[] data2ZeroPadding;
	delete[] fringebg;

	//pixel_nonuniform
	double *pixel_nonuniform = new double[numCameraPixels];
	p_DataPro->GetPixelNonuniform(numCameraPixels, phase1_mean, phase2_mean, pixel_nonuniform);
	delete[] phase1_mean;
	delete[] phase2_mean;

	//����pixel_nonuniform�ļ�����matlab����һ�£�����
	double *pixel_nonuniform_out = new double[numCameraPixels];
	for (int i = 0; i < numCameraPixels; i++)
	{
		pixel_nonuniform_out[numCameraPixels - 1 - i] = pixel_nonuniform[i];
	}
	delete[] pixel_nonuniform;
	
	//CFile cFile3;
	//cFile3.Open("F:\\DATA\\PhaseCalibration_new.txt", CFile::modeWrite | CFile::modeCreate);
	//cFile3.Write(pixel_nonuniform_out, numCameraPixels * sizeof(double));
	//cFile3.Close();

	FILE * fp;
	fp = fopen("F:\\DATA\\PhaseCalibration_new.txt", "w");
	if (fp != NULL)
	{
		for (int i = 0; i < numCameraPixels; i++)
		{
			fprintf(fp, "%4.4f ", pixel_nonuniform_out[i]);
		}
		fclose(fp);
	}

	//Tape OCT image ��ȡ���ݣ���ֵ��ɫɢ�����㣬fft
	int TapeDataCol = 1200;
	//CString filetape = "F:\\DATA\\_01_02\\M0008_2022_P0000002_circle_3.0x3.1_C7_S7.bin";
	UINT16 *datatape = new UINT16[numCameraPixels * TapeDataCol];
	CFile cFile4;
	cFile4.Open(filetape, CFile::modeRead);
	cFile4.Read(datatape, sizeof(UINT16)* numCameraPixels * TapeDataCol);
	cFile4.Close();

	//��������������Ҫ����ת�á�
	UINT16 *datatapeIn = new UINT16[numCameraPixels * TapeDataCol];
	for (int i = 0; i < TapeDataCol; i++)
	{
		for (int j = 0; j < numCameraPixels; j++)
		{
			datatapeIn[i + j * TapeDataCol] = datatape[i * numCameraPixels + j];
		}
	}
	delete[] datatape;
	
	UINT8 *scaleTape = new UINT8[numCameraPixels * TapeDataCol];
	p_DataPro->TapeOCT(numCameraPixels, TapeDataCol, datatapeIn, pixel_nonuniform_out, A2, A3, scaleTape);
	delete[] datatapeIn;
	delete[] pixel_nonuniform_out;

	CFile cFile6;
	cFile6.Open("F:\\DATA\\tape303_20220802.raw", CFile::modeWrite | CFile::modeCreate);
	cFile6.Write(scaleTape, numCameraPixels * TapeDataCol * sizeof(UINT8));
	cFile6.Close();
	delete[] scaleTape;

	////scaleTape������Ҫ�л�
	//UINT8 *scaleTapeChange = new UINT8[numCameraPixels * TapeDataCol];
	//for (int i = 0; i < TapeDataCol; i++)
	//{
	//	for (int j = 0; j < numCameraPixels; j++)
	//	{
	//		scaleTapeChange[i + j * TapeDataCol] = scaleTape[i * numCameraPixels + j];
	//	}
	//}
	//delete[] scaleTape;

	//CFile cFile5;
	//cFile5.Open("F:\\DATA\\tape303_20220802.raw", CFile::modeWrite | CFile::modeCreate);
	//cFile5.Write(scaleTapeChange, numCameraPixels * TapeDataCol * sizeof(UINT8));
	//cFile5.Close();
 	//delete[] scaleTapeChange;

	delete p_DataPro;
 }
