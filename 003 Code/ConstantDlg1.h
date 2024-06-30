#pragma once
#include "afxdialogex.h"


// CConstantDlg1 대화 상자

class CConstantDlg1 : public CDialog
{
	DECLARE_DYNAMIC(CConstantDlg1)

public:
	CConstantDlg1(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CConstantDlg1();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// GrayScale 적용 여부
	BOOL m_check1;
};
