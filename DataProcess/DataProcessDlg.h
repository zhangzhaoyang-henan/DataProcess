
// DataProcessDlg.h : ͷ�ļ�
//

#pragma once


// CDataProcessDlg �Ի���
class CDataProcessDlg : public CDialogEx
{
// ����
public:
	CDataProcessDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DATAPROCESS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFile1();
	afx_msg void OnBnClickedFile2();
	afx_msg void OnBnClickedOldAlgo();
	afx_msg void OnBnClickedNewAlgo();
	afx_msg void OnBnClickedFile3();
};
