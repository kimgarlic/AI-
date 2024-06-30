// ConstantDlg1.cpp: 구현 파일
//

#include "pch.h"
#include "ColorImage Processing.h"
#include "afxdialogex.h"
#include "ConstantDlg1.h"


// CConstantDlg1 대화 상자

IMPLEMENT_DYNAMIC(CConstantDlg1, CDialog)

CConstantDlg1::CConstantDlg1(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
	, m_check1(FALSE)
{

}

CConstantDlg1::~CConstantDlg1()
{
}

void CConstantDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_check1);
}


BEGIN_MESSAGE_MAP(CConstantDlg1, CDialog)
END_MESSAGE_MAP()


// CConstantDlg1 메시지 처리기
