// ConstantDlg2.cpp: 구현 파일
//

#include "pch.h"
#include "ColorImage Processing.h"
#include "afxdialogex.h"
#include "ConstantDlg2.h"


// CConstantDlg2 대화 상자

IMPLEMENT_DYNAMIC(CConstantDlg2, CDialog)

CConstantDlg2::CConstantDlg2(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG2, pParent)
	, m_check1(FALSE)
	, m_edit2(0)
{

}

CConstantDlg2::~CConstantDlg2()
{
}

void CConstantDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_check1);
	DDX_Text(pDX, IDC_EDIT2, m_edit2);
}


BEGIN_MESSAGE_MAP(CConstantDlg2, CDialog)
END_MESSAGE_MAP()


// CConstantDlg2 메시지 처리기
