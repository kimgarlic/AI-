#pragma once
#include "afxdialogex.h"


// CConstantDlg2 대화 상자

class CConstantDlg2 : public CDialog
{
	DECLARE_DYNAMIC(CConstantDlg2)

public:
	CConstantDlg2(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CConstantDlg2();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// GrayScale 적용 여부
	BOOL m_check1;
	// 입력 값
	double m_edit2;
};
