
// ColorImage ProcessingDoc.cpp: CColorImageProcessingDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ColorImage Processing.h"
#endif

#include "ColorImage ProcessingDoc.h"

#include <propkey.h>
#include <cmath>

#include "ConstantDlg1.h"
#include "ConstantDlg2.h"
#include "ConstantDlg3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CColorImageProcessingDoc

IMPLEMENT_DYNCREATE(CColorImageProcessingDoc, CDocument)

BEGIN_MESSAGE_MAP(CColorImageProcessingDoc, CDocument)
END_MESSAGE_MAP()


// CColorImageProcessingDoc 생성/소멸

CColorImageProcessingDoc::CColorImageProcessingDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CColorImageProcessingDoc::~CColorImageProcessingDoc()
{
}

BOOL CColorImageProcessingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CColorImageProcessingDoc serialization

void CColorImageProcessingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CColorImageProcessingDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CColorImageProcessingDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CColorImageProcessingDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CColorImageProcessingDoc 진단

#ifdef _DEBUG
void CColorImageProcessingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CColorImageProcessingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// 함수 재정의
// i) "파일 열기" 클릭 시 동작
BOOL CColorImageProcessingDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	// 1) 기존의 메모리를 해제 & 관련 변수의 초기화
	// i) 입력이미지 관련
	OnFree2D(m_inImageR, m_inH); OnFree2D(m_inImageG, m_inH); OnFree2D(m_inImageB, m_inH);
	m_inImageR = m_inImageG = m_inImageB = NULL; m_inH = m_inW = 0;
	// ii) 출력이미지 관련
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 선택한 파일 열기
	CImage image;
	image.Load(lpszPathName);

	// 3) 입력이미지 관련 변수 결정
	// i) 파일의 크기 계산 -> 입력이미지의 크기를 결정
	m_inH = image.GetHeight();
	m_inW = image.GetWidth();
	// ii) 메모리할당
	m_inImageR = OnMalloc2D(m_inH, m_inW); m_inImageG = OnMalloc2D(m_inH, m_inW); m_inImageB = OnMalloc2D(m_inH, m_inW);

	// 4) 파일(CImage 객체) -> 메모리로 로딩
	COLORREF px;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			px = image.GetPixel(k, i);		// px에 R,G,B값 모두 들어 있음.
			m_inImageR[i][k] = GetRValue(px);
			m_inImageG[i][k] = GetGValue(px);
			m_inImageB[i][k] = GetBValue(px);
		}
	}

	return TRUE;
}

// ii) "끝내기" 클릭 시 동작
void CColorImageProcessingDoc::OnCloseDocument()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	OnFree2D(m_inImageR, m_inH); OnFree2D(m_inImageG, m_inH); OnFree2D(m_inImageB, m_inH);
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	AfxMessageBox(L"프로그램 끝내기");	// 메시지박스 출력

	CDocument::OnCloseDocument();
}

// 동적메모리 할당(unsigned char**형)
// cf) h, w: 새로 생성하는 동적메모리의 높이와 폭
unsigned char** CColorImageProcessingDoc::OnMalloc2D(int h, int w)
{
	// TODO: 여기에 구현 코드 추가.

	unsigned char** retMemory;
	retMemory = (unsigned char**)malloc(sizeof(unsigned char*) * h);
	for (int i = 0; i < h; i++)
		retMemory[i] = (unsigned char*)malloc(sizeof(unsigned char) * w);
	return retMemory;
}



// 동적메모리 해제(unsigned char**형)
// cf) memory, h: 해제할 동적메모리, 높이 (h: 완벽한 메모리 해제를 위해 받음.)
void CColorImageProcessingDoc::OnFree2D(unsigned char** memory, int h)
{
	// TODO: 여기에 구현 코드 추가.

	if (memory == NULL) return;
	for (int i = 0; i < h; i++)
		free(memory[i]);
	free(memory);
}


// 동적메모리 할당(double**형)
double** CColorImageProcessingDoc::OnMalloc2D_double(int h, int w)
{
	// TODO: 여기에 구현 코드 추가.
	
	double** retMemory;
	retMemory = (double**)malloc(sizeof(double*) * h);
	for (int i = 0; i < h; i++)
		retMemory[i] = (double*)malloc(sizeof(double) * w);
	return retMemory;
}


// 동적메모리 해제(double**형)
void CColorImageProcessingDoc::OnFree2D_double(double** memory, int h)
{
	// TODO: 여기에 구현 코드 추가.

	if (memory == NULL) return;
	for (int i = 0; i < h; i++)
		free(memory[i]);
	free(memory);
}


// << 영상 처리 알고리즘 함수 구현 >>

// ===============================================
// <화소점 처리> - i) 동일 이미지
// ===============================================
void CColorImageProcessingDoc::OnEqualImage()
{
	// TODO: 여기에 구현 코드 추가.
	
	// 0) Dialog에서 정보 가져오기
	CConstantDlg1 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력 이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 3) 영상 처리 알고리즘 (동일 이미지)

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				m_outImageR[i][k] = m_inImageR[i][k];
				m_outImageG[i][k] = m_inImageG[i][k];
				m_outImageB[i][k] = m_inImageB[i][k];
			}
		}
	}
}


// ===============================================
// <화소점 처리> - ii) 반전 이미지
// ===============================================
void CColorImageProcessingDoc::OnReverseImage()
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg1 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력 이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 3) 영상 처리 알고리즘 (동일 이미지)

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = 255 - (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				m_outImageR[i][k] = 255 - m_inImageR[i][k];
				m_outImageG[i][k] = 255 - m_inImageG[i][k];
				m_outImageB[i][k] = 255 - m_inImageB[i][k];
			}
		}
	}
}


// ===============================================
// <화소점 처리> - iii) 명도조절
// ===============================================
void CColorImageProcessingDoc::OnAddImage()
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg2 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;
	int value = (int)dlg.m_edit2;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// +) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 3) 영상 처리 알고리즘 (동일 이미지)
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = m_inImageR[i][k];
				tmpImageG[i][k] = m_inImageG[i][k];
				tmpImageB[i][k] = m_inImageB[i][k];
			}

			// R
			if (tmpImageR[i][k] + value > 255)	m_outImageR[i][k] = 255;
			else if (tmpImageR[i][k] + value < 0)	m_outImageR[i][k] = 0;
			else m_outImageR[i][k] = tmpImageR[i][k] + value;
			// G
			if (tmpImageG[i][k] + value > 255)	m_outImageG[i][k] = 255;
			else if (tmpImageG[i][k] + value < 0)	m_outImageG[i][k] = 0;
			else m_outImageG[i][k] = tmpImageG[i][k] + value;
			// B
			if (tmpImageB[i][k] + value > 255)	m_outImageB[i][k] = 255;
			else if (tmpImageB[i][k] + value < 0)	m_outImageB[i][k] = 0;
			else m_outImageB[i][k] = tmpImageB[i][k] + value;
		}
	}

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}


// ===============================================
// <화소점 처리> - iv) 흑백 이미지
// ===============================================
void CColorImageProcessingDoc::OnBlwh128Image()		// 128(픽셀값) 기준
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg1 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// +) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 3) 영상 처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = m_inImageR[i][k];
				tmpImageG[i][k] = m_inImageG[i][k];
				tmpImageB[i][k] = m_inImageB[i][k];
			}

			// R
			if (tmpImageR[i][k] < 128)	m_outImageR[i][k] = 0;
			else m_outImageR[i][k] = 255;
			// G
			if (tmpImageG[i][k] < 128)	m_outImageG[i][k] = 0;
			else m_outImageG[i][k] = 255;
			// B
			if (tmpImageB[i][k] < 128)	m_outImageB[i][k] = 0;
			else m_outImageB[i][k] = 255;
		}
	}

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}
void CColorImageProcessingDoc::OnBlwhAVGImage()		// 평균값(AVG) 기준
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg1 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// +) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 3) 영상 처리 알고리즘
	double base[3];
	double hap[3] = { 0.0, 0.0, 0.0 };
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = m_inImageR[i][k];
				tmpImageG[i][k] = m_inImageG[i][k];
				tmpImageB[i][k] = m_inImageB[i][k];
			}

			hap[0] += tmpImageR[i][k];
			hap[1] += tmpImageG[i][k];
			hap[2] += tmpImageB[i][k];
		}
	}

	for (int z = 0; z < 3; z++)
		base[z] = hap[z] / (m_outH * m_outW);

	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			// R
			if (tmpImageR[i][k] < base[0])	m_outImageR[i][k] = 0;
			else m_outImageR[i][k] = 255;
			// G
			if (tmpImageG[i][k] < base[1])	m_outImageG[i][k] = 0;
			else m_outImageG[i][k] = 255;
			// B
			if (tmpImageB[i][k] < base[2])	m_outImageB[i][k] = 0;
			else m_outImageB[i][k] = 255;
		}
	}

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}
void CColorImageProcessingDoc::OnBlwhMEDImage()		// 중앙값(MED) 기준
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg1 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// +) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 3) 영상 처리 알고리즘
	double base[3];
	int pixelhist[3][256] = { 0 };
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = m_inImageR[i][k];
				tmpImageG[i][k] = m_inImageG[i][k];
				tmpImageB[i][k] = m_inImageB[i][k];
			}

			pixelhist[0][tmpImageR[i][k]]++;
			pixelhist[1][tmpImageG[i][k]]++;
			pixelhist[2][tmpImageB[i][k]]++;
		}
	}

	int index[3] = { 0, 0, 0 };
	int down[3] = { -1, -1, -1 }, up[3] = { -1, -1, -1 };
	int whole = m_outH * m_outW;

	for (int z = 0; z < 3; z++) {
		for (int i = 0; i < 256; i++) {
			for (int k = 0; k < pixelhist[z][i]; k++) {
				index[z] += 1;
				if (whole % 2 == 0) {
					if (index[z] == whole / 2) down[z] = i;
					else if (index[z] = whole / 2 + 1) up[z] = i;
				}
				else
					if (index[z] > whole / 2) down[z] = up[z] = i;
				if (down[z] != -1 && up[z] != -1)	break;
			}
			if (down[z] != -1 && up[z] != -1) break;
		}
		base[z] = (down[z] + up[z]) / 2.0;
	}


	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			// R
			if (tmpImageR[i][k] < base[0])	m_outImageR[i][k] = 0;
			else m_outImageR[i][k] = 255;
			// G
			if (tmpImageG[i][k] < base[1])	m_outImageG[i][k] = 0;
			else m_outImageG[i][k] = 255;
			// B
			if (tmpImageB[i][k] < base[2])	m_outImageB[i][k] = 0;
			else m_outImageB[i][k] = 255;
		}
	}

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}


// ===============================================
// <화소점 처리> - v) 포스터라이징(8단계)
// ===============================================
void CColorImageProcessingDoc::OnPosterizeImage()
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg1 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// +) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 3) 영상 처리 알고리즘 (동일 이미지)
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = m_inImageR[i][k];
				tmpImageG[i][k] = m_inImageG[i][k];
				tmpImageB[i][k] = m_inImageB[i][k];
			}

			// R
			if (tmpImageR[i][k] < 35)	m_outImageR[i][k] = 0;
			else if (tmpImageR[i][k] < 70) m_outImageR[i][k] = 35;
			else if (tmpImageR[i][k] < 105) m_outImageR[i][k] = 70;
			else if (tmpImageR[i][k] < 140) m_outImageR[i][k] = 105;
			else if (tmpImageR[i][k] < 175) m_outImageR[i][k] = 140;
			else if (tmpImageR[i][k] < 210) m_outImageR[i][k] = 175;
			else if (tmpImageR[i][k] < 245) m_outImageR[i][k] = 210;
			else m_outImageR[i][k] = 255;
			// G
			if (tmpImageG[i][k] < 35)	m_outImageG[i][k] = 0;
			else if (tmpImageG[i][k] < 70) m_outImageG[i][k] = 35;
			else if (tmpImageG[i][k] < 105) m_outImageG[i][k] = 70;
			else if (tmpImageG[i][k] < 140) m_outImageG[i][k] = 105;
			else if (tmpImageG[i][k] < 175) m_outImageG[i][k] = 140;
			else if (tmpImageG[i][k] < 210) m_outImageG[i][k] = 175;
			else if (tmpImageG[i][k] < 245) m_outImageG[i][k] = 210;
			else m_outImageG[i][k] = 255;
			// B
			if (tmpImageB[i][k] < 35)	m_outImageB[i][k] = 0;
			else if (tmpImageB[i][k] < 70) m_outImageB[i][k] = 35;
			else if (tmpImageB[i][k] < 105) m_outImageB[i][k] = 70;
			else if (tmpImageB[i][k] < 140) m_outImageB[i][k] = 105;
			else if (tmpImageB[i][k] < 175) m_outImageB[i][k] = 140;
			else if (tmpImageB[i][k] < 210) m_outImageB[i][k] = 175;
			else if (tmpImageB[i][k] < 245) m_outImageB[i][k] = 210;
			else m_outImageB[i][k] = 255;
		}
	}

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}


// ===============================================
// <화소점 처리> - vi) 감마보정
// ===============================================
void CColorImageProcessingDoc::OnGammaImage()
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg2 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;
	double gamma = dlg.m_edit2;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 3) 영상 처리 알고리즘 (동일 이미지)
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = (int)((unsigned char)avg * (1 / gamma));
			}
			else {	// GrayScale 미적용
				m_outImageR[i][k] = (int)(m_inImageR[i][k] * (1 / gamma));
				m_outImageG[i][k] = (int)(m_inImageG[i][k] * (1 / gamma));
				m_outImageB[i][k] = (int)(m_inImageB[i][k] * (1 / gamma));
			}
		}
	}
}


// ===============================================
// <화소점 처리> - vii) 파라볼라
// ===============================================
void CColorImageProcessingDoc::OnParabolaCAPImage()		// CAP
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg1 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// +) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 3) 영상 처리 알고리즘 (동일 이미지)
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (int)(255 * pow(((unsigned char)avg / 127.0 - 1), 2));

			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = (int)(255 * pow((m_inImageR[i][k] / 127.0 - 1), 2));
				tmpImageG[i][k] = (int)(255 * pow((m_inImageG[i][k] / 127.0 - 1), 2));
				tmpImageB[i][k] = (int)(255 * pow((m_inImageB[i][k] / 127.0 - 1), 2));
			}

			// R
			if (tmpImageR[i][k] > 255)	m_outImageR[i][k] = 255;
			else if (tmpImageR[i][k] < 0)	m_outImageR[i][k] = 0;
			else m_outImageR[i][k] = tmpImageR[i][k];
			// G
			if (tmpImageG[i][k] > 255)	m_outImageG[i][k] = 255;
			else if (tmpImageG[i][k] < 0)	m_outImageG[i][k] = 0;
			else m_outImageG[i][k] = tmpImageG[i][k];
			// B
			if (tmpImageB[i][k] > 255)	m_outImageB[i][k] = 255;
			else if (tmpImageB[i][k] < 0)	m_outImageB[i][k] = 0;
			else m_outImageB[i][k] = tmpImageB[i][k];
		}
	}

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}
void CColorImageProcessingDoc::OnParabolaCUPImage()	// CUP
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg1 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// +) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 3) 영상 처리 알고리즘 (동일 이미지)
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (int)(-255 * pow(((unsigned char)avg / 127.0 - 1), 2) + 255);
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = (int)(-255 * pow((m_inImageR[i][k] / 127.0 - 1), 2) + 255);
				tmpImageG[i][k] = (int)(-255 * pow((m_inImageG[i][k] / 127.0 - 1), 2) + 255);
				tmpImageB[i][k] = (int)(-255 * pow((m_inImageB[i][k] / 127.0 - 1), 2) + 255);
			}

			// R
			if (tmpImageR[i][k] > 255)	m_outImageR[i][k] = 255;
			else if (tmpImageR[i][k] < 0)	m_outImageR[i][k] = 0;
			else m_outImageR[i][k] = tmpImageR[i][k];
			// G
			if (tmpImageG[i][k] > 255)	m_outImageG[i][k] = 255;
			else if (tmpImageG[i][k] < 0)	m_outImageG[i][k] = 0;
			else m_outImageG[i][k] = tmpImageG[i][k];
			// B
			if (tmpImageB[i][k] > 255)	m_outImageB[i][k] = 255;
			else if (tmpImageB[i][k] < 0)	m_outImageB[i][k] = 0;
			else m_outImageB[i][k] = tmpImageB[i][k];
		}
	}

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}


// ===============================================
// <기하학적 처리> - i) 화면 축소
// ===============================================
void CColorImageProcessingDoc::OnZoomOutRAWImage()	// 일반 축소 
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg2 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;
	int scale = (int)dlg.m_edit2;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH / scale;
	m_outW = m_inW / scale;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// +) temp이미지 세팅
	unsigned char** tmp1ImageR = OnMalloc2D(m_inH, m_inW);
	unsigned char** tmp1ImageG = OnMalloc2D(m_inH, m_inW);
	unsigned char** tmp1ImageB = OnMalloc2D(m_inH, m_inW);
	double** tmp2ImageR = OnMalloc2D_double(m_outH, m_outW);
	double** tmp2ImageG = OnMalloc2D_double(m_outH, m_outW);
	double** tmp2ImageB = OnMalloc2D_double(m_outH, m_outW);

	// 3) 영상 처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmp1ImageR[i][k] = tmp1ImageG[i][k] = tmp1ImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmp1ImageR[i][k] = m_inImageR[i][k];
				tmp1ImageG[i][k] = m_inImageG[i][k];
				tmp1ImageB[i][k] = m_inImageB[i][k];
			}

			// R
			tmp2ImageR[i / scale][k / scale] = tmp1ImageR[i][k];
			// G
			tmp2ImageG[i / scale][k / scale] = tmp1ImageG[i][k];
			// B
			tmp2ImageB[i / scale][k / scale] = tmp1ImageB[i][k];
		}
	}

	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			// R
			m_outImageR[i][k] = (unsigned char)tmp2ImageR[i][k];
			// G
			m_outImageG[i][k] = (unsigned char)tmp2ImageG[i][k];
			// B
			m_outImageB[i][k] = (unsigned char)tmp2ImageB[i][k];
		}
	}

	// +) temp이미지 해제
	OnFree2D(tmp1ImageR, m_inH); OnFree2D(tmp1ImageG, m_inH); OnFree2D(tmp1ImageB, m_inH);
	OnFree2D_double(tmp2ImageR, m_outH); OnFree2D_double(tmp2ImageG, m_outH); OnFree2D_double(tmp2ImageB, m_outH);

}
void CColorImageProcessingDoc::OnZoomOutAVGImage()	// 평균값(AVG)으로 축소
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg2 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;
	int scale = (int)dlg.m_edit2;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH / scale;
	m_outW = m_inW / scale;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// +) temp이미지 세팅
	unsigned char** tmp1ImageR = OnMalloc2D(m_inH, m_inW);
	unsigned char** tmp1ImageG = OnMalloc2D(m_inH, m_inW);
	unsigned char** tmp1ImageB = OnMalloc2D(m_inH, m_inW);
	double** tmp2ImageR = OnMalloc2D_double(m_outH, m_outW);
	double** tmp2ImageG = OnMalloc2D_double(m_outH, m_outW);
	double** tmp2ImageB = OnMalloc2D_double(m_outH, m_outW);

	// 3) 영상 처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmp1ImageR[i][k] = tmp1ImageG[i][k] = tmp1ImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmp1ImageR[i][k] = m_inImageR[i][k];
				tmp1ImageG[i][k] = m_inImageG[i][k];
				tmp1ImageB[i][k] = m_inImageB[i][k];
			}
		}
	}

	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			tmp2ImageR[i][k] = 0.0;
			tmp2ImageG[i][k] = 0.0;
			tmp2ImageB[i][k] = 0.0;
		}
	}
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (i / scale >= m_outH || k / scale >= m_outW) continue;
			tmp2ImageR[i / scale][k / scale] += tmp1ImageR[i][k];
			tmp2ImageG[i / scale][k / scale] += tmp1ImageG[i][k];
			tmp2ImageB[i / scale][k / scale] += tmp1ImageB[i][k];
		}
	}
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			tmp2ImageR[i][k] /= pow(scale, 2);
			tmp2ImageG[i][k] /= pow(scale, 2);
			tmp2ImageB[i][k] /= pow(scale, 2);
		}
	}

	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			// R
			m_outImageR[i][k] = (unsigned char)tmp2ImageR[i][k];
			// G
			m_outImageG[i][k] = (unsigned char)tmp2ImageG[i][k];
			// B
			m_outImageB[i][k] = (unsigned char)tmp2ImageB[i][k];
		}
	}

	// +) temp이미지 해제
	OnFree2D(tmp1ImageR, m_inH); OnFree2D(tmp1ImageG, m_inH); OnFree2D(tmp1ImageB, m_inH);
	OnFree2D_double(tmp2ImageR, m_outH); OnFree2D_double(tmp2ImageG, m_outH); OnFree2D_double(tmp2ImageB, m_outH);
}
void CColorImageProcessingDoc::OnZoomOutMEDImage()	// 중앙값(MED)으로 축소
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg2 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;
	int scale = (int)dlg.m_edit2;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH / scale;
	m_outW = m_inW / scale;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// +) temp이미지 세팅
	unsigned char** tmp1ImageR = OnMalloc2D(m_inH, m_inW);
	unsigned char** tmp1ImageG = OnMalloc2D(m_inH, m_inW);
	unsigned char** tmp1ImageB = OnMalloc2D(m_inH, m_inW);
	double** tmp2ImageR = OnMalloc2D_double(m_outH, m_outW);
	double** tmp2ImageG = OnMalloc2D_double(m_outH, m_outW);
	double** tmp2ImageB = OnMalloc2D_double(m_outH, m_outW);

	// 3) 영상 처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmp1ImageR[i][k] = tmp1ImageG[i][k] = tmp1ImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmp1ImageR[i][k] = m_inImageR[i][k];
				tmp1ImageG[i][k] = m_inImageG[i][k];
				tmp1ImageB[i][k] = m_inImageB[i][k];
			}
		}
	}

	int flatsize = (int)pow(scale, 2);
	int down, up;
	double* flatsliceImageR = (double*)malloc(sizeof(double) * flatsize);
	double* flatsliceImageG = (double*)malloc(sizeof(double) * flatsize);
	double* flatsliceImageB = (double*)malloc(sizeof(double) * flatsize);

	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			// 1단계 : scale*scale 크기를 flat화
			for (int h = 0; h < scale; h++) {
				for (int w = 0; w < scale; w++) {
					flatsliceImageR[h * scale + w] = tmp1ImageR[i * scale + h][k * scale + w];
					flatsliceImageG[h * scale + w] = tmp1ImageG[i * scale + h][k * scale + w];
					flatsliceImageB[h * scale + w] = tmp1ImageB[i * scale + h][k * scale + w];
				}
			}
			// 2단계 : 오름차순으로 정리
			for (int x = 0; x < flatsize - 1; x++) {
				for (int y = x + 1; y < flatsize; y++) {
					if (flatsliceImageR[x] > flatsliceImageR[y]) {
						double temp = flatsliceImageR[x];
						flatsliceImageR[x] = flatsliceImageR[y];
						flatsliceImageR[y] = temp;
					}
					if (flatsliceImageG[x] > flatsliceImageG[y]) {
						double temp = flatsliceImageG[x];
						flatsliceImageG[x] = flatsliceImageG[y];
						flatsliceImageG[y] = temp;
					}
					if (flatsliceImageB[x] > flatsliceImageB[y]) {
						double temp = flatsliceImageB[x];
						flatsliceImageB[x] = flatsliceImageB[y];
						flatsliceImageB[y] = temp;
					}
				}
			}
			// 3단계 : 중앙값 찾기
			if (flatsize % 2 == 0) {
				down = (flatsize / 2) - 1; up = flatsize / 2;
			}
			else if (flatsize % 2 != 0) {
				down = up = (flatsize - 1) / 2;
			}
			tmp2ImageR[i][k] = (flatsliceImageR[down] + flatsliceImageR[up]) / 2.0;
			tmp2ImageG[i][k] = (flatsliceImageG[down] + flatsliceImageG[up]) / 2.0;
			tmp2ImageB[i][k] = (flatsliceImageB[down] + flatsliceImageB[up]) / 2.0;
		}
	}
	free(flatsliceImageR); free(flatsliceImageG); free(flatsliceImageB);

	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			// R
			m_outImageR[i][k] = (unsigned char)tmp2ImageR[i][k];
			// G
			m_outImageG[i][k] = (unsigned char)tmp2ImageG[i][k];
			// B
			m_outImageB[i][k] = (unsigned char)tmp2ImageB[i][k];
		}
	}

	// +) temp이미지 해제
	OnFree2D(tmp1ImageR, m_inH); OnFree2D(tmp1ImageG, m_inH); OnFree2D(tmp1ImageB, m_inH);
	OnFree2D_double(tmp2ImageR, m_outH); OnFree2D_double(tmp2ImageG, m_outH); OnFree2D_double(tmp2ImageB, m_outH);
}


// ===============================================
// <기하학적 처리> - ii) 화면 확대
// ===============================================
void CColorImageProcessingDoc::OnZoomInImage()
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg2 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;
	int scale = (int)dlg.m_edit2;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH * scale;
	m_outW = m_inW * scale;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// +) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_inH, m_inW);
	unsigned char** tmpImageG = OnMalloc2D(m_inH, m_inW);
	unsigned char** tmpImageB = OnMalloc2D(m_inH, m_inW);

	// 3) 영상 처리 알고리즘 (동일 이미지)
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = m_inImageR[i][k];
				tmpImageG[i][k] = m_inImageG[i][k];
				tmpImageB[i][k] = m_inImageB[i][k];
			}
		}
	}

	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			// R
			m_outImageR[i][k] = tmpImageR[i / scale][k / scale];
			// G
			m_outImageG[i][k] = tmpImageG[i / scale][k / scale];
			// B
			m_outImageB[i][k] = tmpImageB[i / scale][k / scale];
		}
	}

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_inH); OnFree2D(tmpImageG, m_inH); OnFree2D(tmpImageB, m_inH);
}


// ===============================================
// <기하학적 처리> - iii) 화면 이동
// ===============================================
void CColorImageProcessingDoc::OnShiftImage()
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg3 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;
	int shiftLR = (int)(dlg.m_edit2_right - dlg.m_edit2_left);
	int shiftUD = (int)(dlg.m_edit2_down - dlg.m_edit2_up);

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// +) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 3) 영상 처리 알고리즘
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = (unsigned char)0.0;
			m_outImageG[i][k] = (unsigned char)0.0;
			m_outImageB[i][k] = (unsigned char)0.0;
		}
	}

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = m_inImageR[i][k];
				tmpImageG[i][k] = m_inImageG[i][k];
				tmpImageB[i][k] = m_inImageB[i][k];
			}

			int new_i = i + shiftUD;
			int new_k = k + shiftLR;

			if ((0 <= new_i && new_i < m_outH) && (0 <= new_k && new_k < m_outW)) {
				m_outImageR[new_i][new_k] = tmpImageR[i][k];
				m_outImageG[new_i][new_k] = tmpImageG[i][k];
				m_outImageB[new_i][new_k] = tmpImageB[i][k];
			}
		}
	}

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}


// ===============================================
// <기하학적 처리> - iv) 화면 대칭
// ===============================================
void CColorImageProcessingDoc::OnMirrorUDImage()	// 상하 대칭
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg1 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// +) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 3) 영상 처리 알고리즘 (동일 이미지)
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = m_inImageR[i][k];
				tmpImageG[i][k] = m_inImageG[i][k];
				tmpImageB[i][k] = m_inImageB[i][k];
			}
		}
	}

	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[(m_outH - 1) - i][k] = tmpImageR[i][k];
			m_outImageG[(m_outH - 1) - i][k] = tmpImageG[i][k];
			m_outImageB[(m_outH - 1) - i][k] = tmpImageB[i][k];

		}
	}

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}
void CColorImageProcessingDoc::OnMirrorLRImage()	// 좌우 대칭
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg1 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// +) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 3) 영상 처리 알고리즘 (동일 이미지)
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = m_inImageR[i][k];
				tmpImageG[i][k] = m_inImageG[i][k];
				tmpImageB[i][k] = m_inImageB[i][k];
			}
		}
	}

	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
				m_outImageR[i][(m_outW - 1) - k] = tmpImageR[i][k];
				m_outImageG[i][(m_outW - 1) - k] = tmpImageG[i][k];
				m_outImageB[i][(m_outW - 1) - k] = tmpImageB[i][k];
		}
	}

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}
void CColorImageProcessingDoc::OnMirrorUDLRImage()	// 상하좌우 대칭
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg1 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// +) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 3) 영상 처리 알고리즘 (동일 이미지)
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = m_inImageR[i][k];
				tmpImageG[i][k] = m_inImageG[i][k];
				tmpImageB[i][k] = m_inImageB[i][k];
			}
		}
	}

	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[(m_outH - 1) - i][(m_outW - 1) - k] = tmpImageR[i][k];
			m_outImageG[(m_outH - 1) - i][(m_outW - 1) - k] = tmpImageG[i][k];
			m_outImageB[(m_outH - 1) - i][(m_outW - 1) - k] = tmpImageB[i][k];
		}
	}

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}


// ===============================================
// <기하학적 처리> - v) 화면 회전
// ===============================================
#define M_PI 3.14159265358979323846
void CColorImageProcessingDoc::OnRotateImage()
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg2 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;
	double angle = dlg.m_edit2 * M_PI / 180.0;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// +) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 3) 영상 처리 알고리즘 (동일 이미지)
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = m_inImageR[i][k];
				tmpImageG[i][k] = m_inImageG[i][k];
				tmpImageB[i][k] = m_inImageB[i][k];
			}
		}
	}

	// 원본 이미지의 중심 좌표
	int cx = m_inW / 2, cy = m_inH / 2;
	// 출력 이미지의 중심 좌표 (원본 이미지와 동일 크기)
	int ncx = m_outW / 2; int ncy = m_outH / 2;

	// 각 픽셀을 회전 변환하여 새로운 위치에 배치
	for (int i = 0; i < m_outH; ++i) {
		for (int j = 0; j < m_outW; ++j) {
			// 현재 위치를 중심을 기준으로 한 좌표로 변환
			int x = j - ncx, y = i - ncy;

			// 역변환 (출력 이미지에서 원본 이미지로의 좌표 변환)
			int origX = static_cast<int>(x * cos(angle) + y * sin(angle) + cx);
			int origY = static_cast<int>(-x * sin(angle) + y * cos(angle) + cy);

			// 원본 이미지 좌표가 유효한지 확인
			if (origX >= 0 && origX < m_inW && origY >= 0 && origY < m_inH) {
				m_outImageR[i][j] = tmpImageR[origY][origX];
				m_outImageG[i][j] = tmpImageG[origY][origX];
				m_outImageB[i][j] = tmpImageB[origY][origX];
			}
			else {
				m_outImageR[i][j] = m_outImageG[i][j] = m_outImageB[i][j] = 0;	// 범위를 벗어난 경우, 빈 공간 처리 (검정색 배경)
			}
		}
	}

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}


// ===============================================
// <화소영역 처리> - i) 엠보싱
// ===============================================
void CColorImageProcessingDoc::OnEmbossImage()
{
	// TODO: 여기에 구현 코드 추가.
	
	// 0) Dialog에서 정보 가져오기
	CConstantDlg1 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// +) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 3) 영상 처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = m_inImageR[i][k];
				tmpImageG[i][k] = m_inImageG[i][k];
				tmpImageB[i][k] = m_inImageB[i][k];
			}
		}
	}

	// ** 마스크 정의 **
	double mask[3][3] = { {-1.0, 0.0, 0.0},
					  {0.0, 0.0, 0.0},
					  {0.0, 0.0, 1.0} };	// 엠보싱 마스크
	// ** 임시 입력, 출력 메모리 확보 **
	double** tmpInImageR = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMalloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = OnMalloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = OnMalloc2D_double(m_outH, m_outW);
	// ** 임시 입력 메모리 초기화 (128로) **
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}
	// ** 입력 메모리 --> 임시 입력 메모리의 가운데 쏙 넣기 **
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}
	// ** 회선 연산 : 마스크로 쭈욱~ 긁어가면서 계산하기 **
	double S[3]; // 마스크로 3x3을 씌워서 곱한 후 합계낸 값.
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3)과 한점을 중심으로 한 3x3을 곱하기
			S[0] = S[1] = S[2] = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					S[0] += tmpInImageR[i + m][k + n] * mask[m][n];
					S[1] += tmpInImageG[i + m][k + n] * mask[m][n];
					S[2] += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = S[0];
			tmpOutImageG[i][k] = S[1];
			tmpOutImageB[i][k] = S[2];
		}
	}
	// ** 후처리 : 마스크의 합계가 0일 경우, 127 정도를 더하기 **
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127.0;
			tmpOutImageG[i][k] += 127.0;
			tmpOutImageB[i][k] += 127.0;
		}
	}
	// ** 임시 출력 이미지 --> 출력 이미지 **
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			// R
			if (tmpOutImageR[i][k] > 255.0) m_outImageR[i][k] = 255;
			else if (tmpOutImageR[i][k] < 0) m_outImageR[i][k] = 0;
			else m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			// G
			if (tmpOutImageG[i][k] > 255.0) m_outImageG[i][k] = 255;
			else if (tmpOutImageG[i][k] < 0) m_outImageG[i][k] = 0;
			else m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			// B
			if (tmpOutImageB[i][k] > 255.0) m_outImageB[i][k] = 255;
			else if (tmpOutImageB[i][k] < 0) m_outImageB[i][k] = 0;
			else m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
		}
	}
	// ** 임시 입력, 출력 메모리 해제 **
	OnFree2D_double(tmpInImageR, m_inH + 2); OnFree2D_double(tmpInImageG, m_inH + 2); OnFree2D_double(tmpInImageB, m_inH + 2);
	OnFree2D_double(tmpOutImageR, m_outH); OnFree2D_double(tmpOutImageG, m_outH); OnFree2D_double(tmpOutImageB, m_outH);

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}


// ===============================================
// <화소영역 처리> - ii) 블러링
// ===============================================
void CColorImageProcessingDoc::OnBlur3by3()	// 3X3 블러링
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg1 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// +) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 3) 영상 처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = m_inImageR[i][k];
				tmpImageG[i][k] = m_inImageG[i][k];
				tmpImageB[i][k] = m_inImageB[i][k];
			}
		}
	}

	// ** 마스크 정의 **
	double** mask = OnMalloc2D_double(3, 3);
	for (int i = 0; i < 3; i++) {
		for (int k = 0; k < 3; k++)
			mask[i][k] = 1.0 / pow(3, 2);
	}

	// ** 임시 입력, 출력 메모리 확보 **
	double** tmpInImageR = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMalloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = OnMalloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = OnMalloc2D_double(m_outH, m_outW);
	// ** 임시 입력 메모리 초기화 (128로) **
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}
	// ** 입력 메모리 --> 임시 입력 메모리의 가운데 쏙 넣기 **
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = tmpImageR[i][k];
			tmpInImageG[i + 1][k + 1] = tmpImageG[i][k];
			tmpInImageB[i + 1][k + 1] = tmpImageB[i][k];
		}
	}
	// ** 회선 연산 : 마스크로 쭈욱~ 긁어가면서 계산하기 **
	double S[3];
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3)과 한점을 중심으로 한 3x3을 곱하기
			S[0] = S[1] = S[2] = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					S[0] += tmpInImageR[i + m][k + n] * mask[m][n];
					S[1] += tmpInImageG[i + m][k + n] * mask[m][n];
					S[2] += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = S[0]; tmpOutImageG[i][k] = S[1]; tmpOutImageB[i][k] = S[2];
		}
	}
	// ** 임시 출력 이미지 --> 출력 이미지 **
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			// R
			if (tmpOutImageR[i][k] > 255.0) m_outImageR[i][k] = 255;
			else if (tmpOutImageR[i][k] < 0) m_outImageR[i][k] = 0;
			else m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			// G
			if (tmpOutImageG[i][k] > 255.0) m_outImageG[i][k] = 255;
			else if (tmpOutImageG[i][k] < 0) m_outImageG[i][k] = 0;
			else m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			// B
			if (tmpOutImageB[i][k] > 255.0) m_outImageB[i][k] = 255;
			else if (tmpOutImageB[i][k] < 0) m_outImageB[i][k] = 0;
			else m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
		}
	}
	// ** 임시 입력, 출력 메모리 해제 **
	OnFree2D_double(tmpInImageR, m_inH + 2); OnFree2D_double(tmpInImageG, m_inH + 2); OnFree2D_double(tmpInImageB, m_inH + 2);
	OnFree2D_double(tmpOutImageR, m_outH); OnFree2D_double(tmpOutImageG, m_outH); OnFree2D_double(tmpOutImageB, m_outH);

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}
void CColorImageProcessingDoc::OnBlur5by5()	// 5X5 블러링
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg1 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// +) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 3) 영상 처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = m_inImageR[i][k];
				tmpImageG[i][k] = m_inImageG[i][k];
				tmpImageB[i][k] = m_inImageB[i][k];
			}
		}
	}

	// ** 마스크 정의 **
	double** mask = OnMalloc2D_double(5, 5);
	for (int i = 0; i < 5; i++) {
		for (int k = 0; k < 5; k++)
			mask[i][k] = 1.0 / pow(5, 2);
	}

	// ** 임시 입력, 출력 메모리 확보 **
	double** tmpInImageR = OnMalloc2D_double(m_inH + 4, m_inW + 4);
	double** tmpInImageG = OnMalloc2D_double(m_inH + 4, m_inW + 4);
	double** tmpInImageB = OnMalloc2D_double(m_inH + 4, m_inW + 4);
	double** tmpOutImageR = OnMalloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = OnMalloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = OnMalloc2D_double(m_outH, m_outW);
	// ** 임시 입력 메모리 초기화 (128로) **
	for (int i = 0; i < m_inH + 4; i++) {
		for (int k = 0; k < m_inW + 4; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}
	// ** 입력 메모리 --> 임시 입력 메모리의 가운데 쏙 넣기 **
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 2][k + 2] = tmpImageR[i][k];
			tmpInImageG[i + 2][k + 2] = tmpImageG[i][k];
			tmpInImageB[i + 2][k + 2] = tmpImageB[i][k];
		}
	}
	// ** 회선 연산 : 마스크로 쭈욱~ 긁어가면서 계산하기 **
	double S[3];
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3)과 한점을 중심으로 한 3x3을 곱하기
			S[0] = S[1] = S[2] = 0.0;
			for (int m = 0; m < 5; m++) {
				for (int n = 0; n < 5; n++) {
					S[0] += tmpInImageR[i + m][k + n] * mask[m][n];
					S[1] += tmpInImageG[i + m][k + n] * mask[m][n];
					S[2] += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = S[0]; tmpOutImageG[i][k] = S[1]; tmpOutImageB[i][k] = S[2];
		}
	}
	// ** 임시 출력 이미지 --> 출력 이미지 **
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			// R
			if (tmpOutImageR[i][k] > 255.0) m_outImageR[i][k] = 255;
			else if (tmpOutImageR[i][k] < 0) m_outImageR[i][k] = 0;
			else m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			// G
			if (tmpOutImageG[i][k] > 255.0) m_outImageG[i][k] = 255;
			else if (tmpOutImageG[i][k] < 0) m_outImageG[i][k] = 0;
			else m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			// B
			if (tmpOutImageB[i][k] > 255.0) m_outImageB[i][k] = 255;
			else if (tmpOutImageB[i][k] < 0) m_outImageB[i][k] = 0;
			else m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
		}
	}
	// ** 임시 입력, 출력 메모리 해제 **
	OnFree2D_double(tmpInImageR, m_inH + 4); OnFree2D_double(tmpInImageG, m_inH + 4); OnFree2D_double(tmpInImageB, m_inH + 4);
	OnFree2D_double(tmpOutImageR, m_outH); OnFree2D_double(tmpOutImageG, m_outH); OnFree2D_double(tmpOutImageB, m_outH);

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}


// ===============================================
// <화소영역 처리> - iii) 경계선 검출
// ===============================================
void CColorImageProcessingDoc::OnEdgeVertImage()		// 수직 경계선 검출
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg1 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 3) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 4) 영상 처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = m_inImageR[i][k];
				tmpImageG[i][k] = m_inImageG[i][k];
				tmpImageB[i][k] = m_inImageB[i][k];
			}
		}
	}
	////// 4-1) 마스크 정의
	double mask[3][3] = { {-1.0, 0.0, 1.0},
						  {-2.0, 0.0, 2.0},
						  {-1.0, 0.0, 1.0} };	// 수직 에지 검출 마스크

	////// 4-2) 임시 입력, 출력 메모리 확보
	double** tmpInImageR = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMalloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = OnMalloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = OnMalloc2D_double(m_outH, m_outW);

	////// 4-3) 임시 입력 메모리 초기화 (128로) ==> 필요시 평균값 넣기.
	// cf) 패딩값인 128으로 미리 다 채워놓고.
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	////// 4-4) 입력 메모리 --> 임시 입력 메모리의 가운데 쏙 넣기.
	// cf) 패딩이 아닌 영역은 입력 메모리로 채우기 (=입력 메모리 주위로 128이 패딩되어 있음.)
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = tmpImageR[i][k];
			tmpInImageG[i + 1][k + 1] = tmpImageG[i][k];
			tmpInImageB[i + 1][k + 1] = tmpImageB[i][k];
		}
	}

	////// 4-5) 회선 연산 : 마스크로 쭈욱~ 긁어가면서 계산하기.
	double S[3]; // 마스크로 3x3을 씌워서 곱한 후 합계낸 값.
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3)과 한점을 중심으로 한 3x3을 곱하기
			S[0] = S[1] = S[2] = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					S[0] += tmpInImageR[i + m][k + n] * mask[m][n];
					S[1] += tmpInImageG[i + m][k + n] * mask[m][n];
					S[2] += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = S[0];
			tmpOutImageG[i][k] = S[1];
			tmpOutImageB[i][k] = S[2];
		}
	}

	////// 4-7) 임시 출력 이미지 --> 출력 이미지
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			// R
			if (tmpOutImageR[i][k] > 255.0) m_outImageR[i][k] = 255;
			else if (tmpOutImageR[i][k] < 0) m_outImageR[i][k] = 0;
			else m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			// G
			if (tmpOutImageG[i][k] > 255.0) m_outImageG[i][k] = 255;
			else if (tmpOutImageG[i][k] < 0) m_outImageG[i][k] = 0;
			else m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			// B
			if (tmpOutImageB[i][k] > 255.0) m_outImageB[i][k] = 255;
			else if (tmpOutImageB[i][k] < 0) m_outImageB[i][k] = 0;
			else m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
		}
	}

	////// 4-8) 임시 입력, 출력 메모리 해제
	OnFree2D_double(tmpInImageR, m_inH + 2); OnFree2D_double(tmpInImageG, m_inH + 2); OnFree2D_double(tmpInImageB, m_inH + 2);
	OnFree2D_double(tmpOutImageR, m_outH); OnFree2D_double(tmpOutImageG, m_outH); OnFree2D_double(tmpOutImageB, m_outH);

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}
void CColorImageProcessingDoc::OnEdgeHoriImage()	// 수평 경계선 검출
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg1 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 3) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 4) 영상 처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = m_inImageR[i][k];
				tmpImageG[i][k] = m_inImageG[i][k];
				tmpImageB[i][k] = m_inImageB[i][k];
			}
		}
	}
	////// 4-1) 마스크 정의
	double mask[3][3] = { {-1.0, -2.0, -1.0},
						  { 0.0,  0.0,  0.0},
						  { 1.0,  2.0,  1.0} };	// 수평 에지 검출 마스크

	////// 4-2) 임시 입력, 출력 메모리 확보
	double** tmpInImageR = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMalloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = OnMalloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = OnMalloc2D_double(m_outH, m_outW);

	////// 4-3) 임시 입력 메모리 초기화 (128로) ==> 필요시 평균값 넣기.
	// cf) 패딩값인 128으로 미리 다 채워놓고.
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	////// 4-4) 입력 메모리 --> 임시 입력 메모리의 가운데 쏙 넣기.
	// cf) 패딩이 아닌 영역은 입력 메모리로 채우기 (=입력 메모리 주위로 128이 패딩되어 있음.)
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = tmpImageR[i][k];
			tmpInImageG[i + 1][k + 1] = tmpImageG[i][k];
			tmpInImageB[i + 1][k + 1] = tmpImageB[i][k];
		}
	}

	////// 4-5) 회선 연산 : 마스크로 쭈욱~ 긁어가면서 계산하기.
	double S[3]; // 마스크로 3x3을 씌워서 곱한 후 합계낸 값.
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크(3x3)과 한점을 중심으로 한 3x3을 곱하기
			S[0] = S[1] = S[2] = 0.0;
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					S[0] += tmpInImageR[i + m][k + n] * mask[m][n];
					S[1] += tmpInImageG[i + m][k + n] * mask[m][n];
					S[2] += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = S[0];
			tmpOutImageG[i][k] = S[1];
			tmpOutImageB[i][k] = S[2];
		}
	}

	////// 4-7) 임시 출력 이미지 --> 출력 이미지
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			// R
			if (tmpOutImageR[i][k] > 255.0) m_outImageR[i][k] = 255;
			else if (tmpOutImageR[i][k] < 0) m_outImageR[i][k] = 0;
			else m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			// G
			if (tmpOutImageG[i][k] > 255.0) m_outImageG[i][k] = 255;
			else if (tmpOutImageG[i][k] < 0) m_outImageG[i][k] = 0;
			else m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			// B
			if (tmpOutImageB[i][k] > 255.0) m_outImageB[i][k] = 255;
			else if (tmpOutImageB[i][k] < 0) m_outImageB[i][k] = 0;
			else m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
		}
	}

	////// 4-8) 임시 입력, 출력 메모리 해제
	OnFree2D_double(tmpInImageR, m_inH + 2); OnFree2D_double(tmpInImageG, m_inH + 2); OnFree2D_double(tmpInImageB, m_inH + 2);
	OnFree2D_double(tmpOutImageR, m_outH); OnFree2D_double(tmpOutImageG, m_outH); OnFree2D_double(tmpOutImageB, m_outH);

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}
void CColorImageProcessingDoc::OnEdgeYusaImage()	// 유사 연산자
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg1 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 3) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 4) 영상 처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = m_inImageR[i][k];
				tmpImageG[i][k] = m_inImageG[i][k];
				tmpImageB[i][k] = m_inImageB[i][k];
			}
		}
	}

	////// 4-2) 임시 입력, 출력 메모리 확보
	double** tmpInImageR = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMalloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = OnMalloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = OnMalloc2D_double(m_outH, m_outW);

	////// 4-3) 임시 입력 메모리 초기화 (128로) ==> 필요시 평균값 넣기.
	// cf) 패딩값인 128으로 미리 다 채워놓고.
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	////// 4-4) 입력 메모리 --> 임시 입력 메모리의 가운데 쏙 넣기.
	// cf) 패딩이 아닌 영역은 입력 메모리로 채우기 (=입력 메모리 주위로 128이 패딩되어 있음.)
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = tmpImageR[i][k];
			tmpInImageG[i + 1][k + 1] = tmpImageG[i][k];
			tmpInImageB[i + 1][k + 1] = tmpImageB[i][k];
		}
	}

	////// 4-5) 회선 연산 : 마스크로 쭈욱~ 긁어가면서 계산하기.

	double diff[3];
	for (int i = 1; i < m_inH + 1; i++) {
		for (int k = 1; k < m_inW + 1; k++) {
			double maxDiff[3] = { 0.0, 0.0, 0.0 };
			for (int m = -1; m <= 1; m++) {
				for (int n = -1; n <= 1; n++) {
					diff[0] = fabs(tmpInImageR[i][k] - tmpInImageR[i + m][k + n]);
					if (diff[0] > maxDiff[0])	maxDiff[0] = diff[0];
					diff[1] = fabs(tmpInImageG[i][k] - tmpInImageG[i + m][k + n]);
					if (diff[1] > maxDiff[1])	maxDiff[1] = diff[1];
					diff[2] = fabs(tmpInImageB[i][k] - tmpInImageB[i + m][k + n]);
					if (diff[2] > maxDiff[2])	maxDiff[2] = diff[2];
				}
			}
			tmpOutImageR[i - 1][k - 1] = maxDiff[0];
			tmpOutImageG[i - 1][k - 1] = maxDiff[1];
			tmpOutImageB[i - 1][k - 1] = maxDiff[2];
		}
	}

	////// 4-7) 임시 출력 이미지 --> 출력 이미지
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			// R
			if (tmpOutImageR[i][k] > 255.0) m_outImageR[i][k] = 255;
			else if (tmpOutImageR[i][k] < 0) m_outImageR[i][k] = 0;
			else m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			// G
			if (tmpOutImageG[i][k] > 255.0) m_outImageG[i][k] = 255;
			else if (tmpOutImageG[i][k] < 0) m_outImageG[i][k] = 0;
			else m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			// B
			if (tmpOutImageB[i][k] > 255.0) m_outImageB[i][k] = 255;
			else if (tmpOutImageB[i][k] < 0) m_outImageB[i][k] = 0;
			else m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
		}
	}

	////// 4-8) 임시 입력, 출력 메모리 해제
	OnFree2D_double(tmpInImageR, m_inH + 2); OnFree2D_double(tmpInImageG, m_inH + 2); OnFree2D_double(tmpInImageB, m_inH + 2);
	OnFree2D_double(tmpOutImageR, m_outH); OnFree2D_double(tmpOutImageG, m_outH); OnFree2D_double(tmpOutImageB, m_outH);

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}
void CColorImageProcessingDoc::OnEdgeChaImage()	// 차 연산자
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg1 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 3) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 4) 영상 처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = m_inImageR[i][k];
				tmpImageG[i][k] = m_inImageG[i][k];
				tmpImageB[i][k] = m_inImageB[i][k];
			}
		}
	}

	////// 4-2) 임시 입력, 출력 메모리 확보
	double** tmpInImageR = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	double** tmpInImageG = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	double** tmpInImageB = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	double** tmpOutImageR = OnMalloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = OnMalloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = OnMalloc2D_double(m_outH, m_outW);

	////// 4-3) 임시 입력 메모리 초기화 (128로) ==> 필요시 평균값 넣기.
	// cf) 패딩값인 128으로 미리 다 채워놓고.
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	////// 4-4) 입력 메모리 --> 임시 입력 메모리의 가운데 쏙 넣기.
	// cf) 패딩이 아닌 영역은 입력 메모리로 채우기 (=입력 메모리 주위로 128이 패딩되어 있음.)
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = tmpImageR[i][k];
			tmpInImageG[i + 1][k + 1] = tmpImageG[i][k];
			tmpInImageB[i + 1][k + 1] = tmpImageB[i][k];
		}
	}

	////// 4-5) 회선 연산 : 마스크로 쭈욱~ 긁어가면서 계산하기.

	double diff[3][4] = { 0.0 };
	for (int i = 1; i < m_inH + 1; i++) {
		for (int k = 1; k < m_inW + 1; k++) {
			double maxDiff[3] = { 0.0 };

			diff[0][0] = fabs(tmpInImageR[i - 1][k - 1] - tmpInImageR[i + 1][k + 1]);
			diff[0][1] = fabs(tmpInImageR[i - 1][k] - tmpInImageR[i + 1][k]);
			diff[0][2] = fabs(tmpInImageR[i - 1][k + 1] - tmpInImageR[i + 1][k - 1]);
			diff[0][3] = fabs(tmpInImageR[i][k - 1] - tmpInImageR[i][k + 1]);

			diff[1][0] = fabs(tmpInImageG[i - 1][k - 1] - tmpInImageG[i + 1][k + 1]);
			diff[1][1] = fabs(tmpInImageG[i - 1][k] - tmpInImageG[i + 1][k]);
			diff[1][2] = fabs(tmpInImageG[i - 1][k + 1] - tmpInImageG[i + 1][k - 1]);
			diff[1][3] = fabs(tmpInImageG[i][k - 1] - tmpInImageG[i][k + 1]);

			diff[2][0] = fabs(tmpInImageB[i - 1][k - 1] - tmpInImageB[i + 1][k + 1]);
			diff[2][1] = fabs(tmpInImageB[i - 1][k] - tmpInImageB[i + 1][k]);
			diff[2][2] = fabs(tmpInImageB[i - 1][k + 1] - tmpInImageB[i + 1][k - 1]);
			diff[2][3] = fabs(tmpInImageB[i][k - 1] - tmpInImageB[i][k + 1]);

			for (int z = 0; z < 4; z++) {
				if (maxDiff[0] < diff[0][z])	maxDiff[0] = diff[0][z];
				if (maxDiff[1] < diff[1][z])	maxDiff[1] = diff[1][z];
				if (maxDiff[2] < diff[2][z])	maxDiff[2] = diff[2][z];
			}
				
			tmpOutImageR[i - 1][k - 1] = maxDiff[0];
			tmpOutImageG[i - 1][k - 1] = maxDiff[1];
			tmpOutImageB[i - 1][k - 1] = maxDiff[2];
		}
	}

	////// 4-7) 임시 출력 이미지 --> 출력 이미지
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			// R
			if (tmpOutImageR[i][k] > 255.0) m_outImageR[i][k] = 255;
			else if (tmpOutImageR[i][k] < 0) m_outImageR[i][k] = 0;
			else m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			// G
			if (tmpOutImageG[i][k] > 255.0) m_outImageG[i][k] = 255;
			else if (tmpOutImageG[i][k] < 0) m_outImageG[i][k] = 0;
			else m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			// B
			if (tmpOutImageB[i][k] > 255.0) m_outImageB[i][k] = 255;
			else if (tmpOutImageB[i][k] < 0) m_outImageB[i][k] = 0;
			else m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
		}
	}

	////// 4-8) 임시 입력, 출력 메모리 해제
	OnFree2D_double(tmpInImageR, m_inH + 2); OnFree2D_double(tmpInImageG, m_inH + 2); OnFree2D_double(tmpInImageB, m_inH + 2);
	OnFree2D_double(tmpOutImageR, m_outH); OnFree2D_double(tmpOutImageG, m_outH); OnFree2D_double(tmpOutImageB, m_outH);

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}
void CColorImageProcessingDoc::OnEdgeLogImage()	// LOG 연산자
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg1 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 3) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 4) 영상 처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = m_inImageR[i][k];
				tmpImageG[i][k] = m_inImageG[i][k];
				tmpImageB[i][k] = m_inImageB[i][k];
			}
		}
	}
	////// 4-1) 마스크 정의
	double mask[5][5] = { {0.0, 0.0, -1.0, 0.0, 0.0},
						  {0.0, -1.0, -2.0, -1.0, 0.0},
						  {-1.0, -2.0, 16.0, -2.0, -1.0},
						  {0.0, -1.0, -2.0, -1.0, 0.0},
						  {0.0, 0.0, -1.0, 0.0, 0.0} };		// LoG 마스크

	////// 4-2) 임시 입력, 출력 메모리 확보
	double** tmpInImageR = OnMalloc2D_double(m_inH + 4, m_inW + 4);
	double** tmpInImageG = OnMalloc2D_double(m_inH + 4, m_inW + 4);
	double** tmpInImageB = OnMalloc2D_double(m_inH + 4, m_inW + 4);
	double** tmpOutImageR = OnMalloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = OnMalloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = OnMalloc2D_double(m_outH, m_outW);

	////// 4-3) 임시 입력 메모리 초기화
	for (int i = 0; i < m_inH + 4; i++) {
		for (int k = 0; k < m_inW + 4; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 0.0;
		}
	}

	////// 4-4) 입력 메모리 --> 임시 입력 메모리의 가운데 쏙 넣기.
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 2][k + 2] = tmpImageR[i][k];
			tmpInImageG[i + 2][k + 2] = tmpImageG[i][k];
			tmpInImageB[i + 2][k + 2] = tmpImageB[i][k];
		}
	}

	////// 4-5) 회선 연산 : 마스크로 쭈욱~ 긁어가면서 계산하기.
	double S[3];
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			S[0] = S[1] = S[2] = 0.0;
			for (int m = 0; m < 5; m++) {
				for (int n = 0; n < 5; n++) {
					S[0] += tmpInImageR[i + m][k + n] * mask[m][n];
					S[1] += tmpInImageG[i + m][k + n] * mask[m][n];
					S[2] += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = S[0];	tmpOutImageG[i][k] = S[1]; tmpOutImageB[i][k] = S[2];
		}
	}

	////// 4-7) 임시 출력 이미지 --> 출력 이미지
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			// R
			if (tmpOutImageR[i][k] > 255.0) m_outImageR[i][k] = 255;
			else if (tmpOutImageR[i][k] < 0) m_outImageR[i][k] = 0;
			else m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			// G
			if (tmpOutImageG[i][k] > 255.0) m_outImageG[i][k] = 255;
			else if (tmpOutImageG[i][k] < 0) m_outImageG[i][k] = 0;
			else m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			// B
			if (tmpOutImageB[i][k] > 255.0) m_outImageB[i][k] = 255;
			else if (tmpOutImageB[i][k] < 0) m_outImageB[i][k] = 0;
			else m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
		}
	}

	////// 4-8) 임시 입력, 출력 메모리 해제
	OnFree2D_double(tmpInImageR, m_inH + 4); OnFree2D_double(tmpInImageG, m_inH + 4); OnFree2D_double(tmpInImageB, m_inH + 4);
	OnFree2D_double(tmpOutImageR, m_outH); OnFree2D_double(tmpOutImageG, m_outH); OnFree2D_double(tmpOutImageB, m_outH);

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}
void CColorImageProcessingDoc::OnEdgeDogImage()	// DOG 연산자
{
	// TODO: 여기에 구현 코드 추가.

	// 0) Dialog에서 정보 가져오기
	CConstantDlg1 dlg;
	if (dlg.DoModal() != IDOK) return;
	bool isGrayScale = dlg.m_check1;

	// 1) 기존 출력메모리 해제.
	OnFree2D(m_outImageR, m_outH); OnFree2D(m_outImageG, m_outH); OnFree2D(m_outImageB, m_outH);
	m_outImageR = m_outImageG = m_outImageB = NULL; m_outH = m_outW = 0;

	// 2) 출력이미지 세팅
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 3) temp이미지 세팅
	unsigned char** tmpImageR = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageG = OnMalloc2D(m_outH, m_outW);
	unsigned char** tmpImageB = OnMalloc2D(m_outH, m_outW);

	// 4) 영상 처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (isGrayScale) {	// GrayScale 적용
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				tmpImageR[i][k] = tmpImageG[i][k] = tmpImageB[i][k] = (unsigned char)avg;
			}
			else {	// GrayScale 미적용
				tmpImageR[i][k] = m_inImageR[i][k];
				tmpImageG[i][k] = m_inImageG[i][k];
				tmpImageB[i][k] = m_inImageB[i][k];
			}
		}
	}
	////// 4-1) 마스크 정의
	double mask[7][7] = { {0.0, 0.0, -1.0, -1.0, -1.0, 0.0, 0.0},
						  {0.0, -2.0, -3.0, -3.0, -3.0, -2.0, 0.0},
						  {-1.0, -3.0, 5.0, 5.0, 5.0, -3.0, -1.0},
						  {-1.0, -3.0, 5.0, 16.0, 5.0, -3.0, -1.0},
						  {-1.0, -3.0, 5.0, 5.0, 5.0, -3.0, -1.0},
						  {0.0, -2.0, -3.0, -3.0, -3.0, -2.0, 0.0},
						  {0.0, 0.0, -1.0, -1.0, -1.0, 0.0, 0.0} };		// DoG 마스크

	////// 4-2) 임시 입력, 출력 메모리 확보

	double** tmpInImageR = OnMalloc2D_double(m_inH + 6, m_inW + 6);
	double** tmpInImageG = OnMalloc2D_double(m_inH + 6, m_inW + 6);
	double** tmpInImageB = OnMalloc2D_double(m_inH + 6, m_inW + 6);
	double** tmpOutImageR = OnMalloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = OnMalloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = OnMalloc2D_double(m_outH, m_outW);

	////// 4-3) 임시 입력 메모리 초기화
	for (int i = 0; i < m_inH + 6; i++) {
		for (int k = 0; k < m_inW + 6; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 0.0;
		}
	}

	////// 4-4) 입력 메모리 --> 임시 입력 메모리의 가운데 쏙 넣기.
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 3][k + 3] = tmpImageR[i][k];
			tmpInImageG[i + 3][k + 3] = tmpImageG[i][k];
			tmpInImageB[i + 3][k + 3] = tmpImageB[i][k];
		}
	}

	////// 4-5) 회선 연산 : 마스크로 쭈욱~ 긁어가면서 계산하기.
	double S[3];
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			S[0] = S[1] = S[2] = 0.0;
			for (int m = 0; m < 7; m++) {
				for (int n = 0; n < 7; n++) {
					S[0] += tmpInImageR[i + m][k + n] * mask[m][n];
					S[1] += tmpInImageG[i + m][k + n] * mask[m][n];
					S[2] += tmpInImageB[i + m][k + n] * mask[m][n];
				}
			}
			tmpOutImageR[i][k] = S[0]; tmpOutImageG[i][k] = S[1]; tmpOutImageB[i][k] = S[2];
		}
	}

	////// 4-7) 임시 출력 이미지 --> 출력 이미지
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			// R
			if (tmpOutImageR[i][k] > 255.0) m_outImageR[i][k] = 255;
			else if (tmpOutImageR[i][k] < 0) m_outImageR[i][k] = 0;
			else m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			// G
			if (tmpOutImageG[i][k] > 255.0) m_outImageG[i][k] = 255;
			else if (tmpOutImageG[i][k] < 0) m_outImageG[i][k] = 0;
			else m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			// R
			if (tmpOutImageB[i][k] > 255.0) m_outImageB[i][k] = 255;
			else if (tmpOutImageB[i][k] < 0) m_outImageB[i][k] = 0;
			else m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
		}
	}

	////// 4-8) 임시 입력, 출력 메모리 해제
	OnFree2D_double(tmpInImageR, m_inH + 6); OnFree2D_double(tmpInImageG, m_inH + 6); OnFree2D_double(tmpInImageB, m_inH + 6);
	OnFree2D_double(tmpOutImageR, m_outH); OnFree2D_double(tmpOutImageG, m_outH); OnFree2D_double(tmpOutImageB, m_outH);

	// +) temp이미지 해제
	OnFree2D(tmpImageR, m_outH); OnFree2D(tmpImageG, m_outH); OnFree2D(tmpImageB, m_outH);
}