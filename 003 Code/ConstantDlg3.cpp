// ConstantDlg3.cpp: 구현 파일
//

#include "pch.h"
#include "ColorImage Processing.h"
#include "afxdialogex.h"
#include "ConstantDlg3.h"


// CConstantDlg3 대화 상자

IMPLEMENT_DYNAMIC(CConstantDlg3, CDialog)

CConstantDlg3::CConstantDlg3(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG3, pParent)
	, m_check1(FALSE)
	, m_edit2_up(0)
	, m_edit2_right(0)
	, m_edit2_down(0)
	, m_edit2_left(0)
{

}

CConstantDlg3::~CConstantDlg3()
{
}

void CConstantDlg3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_check1);
	DDX_Text(pDX, IDC_EDIT2_UP, m_edit2_up);
	DDX_Text(pDX, IDC_EDIT2_RIGHT, m_edit2_right);
	DDX_Text(pDX, IDC_EDIT2_DOWN, m_edit2_down);
	DDX_Text(pDX, IDC_EDIT2_LEFT, m_edit2_left);
}


BEGIN_MESSAGE_MAP(CConstantDlg3, CDialog)
END_MESSAGE_MAP()


// CConstantDlg3 메시지 처리기
