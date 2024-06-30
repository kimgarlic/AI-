
// ColorImage ProcessingView.cpp: CColorImageProcessingView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ColorImage Processing.h"
#endif

#include "ColorImage ProcessingDoc.h"
#include "ColorImage ProcessingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CColorImageProcessingView

IMPLEMENT_DYNCREATE(CColorImageProcessingView, CView)

BEGIN_MESSAGE_MAP(CColorImageProcessingView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)

	// ===============================================
	// <화소점 처리>
	// ===============================================
	// i) 동일 이미지
	ON_COMMAND(IDM_EQUAL_IMAGE, &CColorImageProcessingView::OnEqualImage)
	// ii) 반전 이미지
	ON_COMMAND(IDM_REVERSE_IMAGE, &CColorImageProcessingView::OnReverseImage)
	// iii) 명도조절
	ON_COMMAND(IDM_ADD_IMAGE, &CColorImageProcessingView::OnAddImage)
	// iv) 흑백 이미지
	ON_COMMAND(IDM_BLWH128_IMAGE, &CColorImageProcessingView::OnBlwh128Image)		// 128(픽셀값) 기준
	ON_COMMAND(IDM_BLWHAVG_IMAGE, &CColorImageProcessingView::OnBlwhAVGImage)		// 평균값(AVG) 기준
	ON_COMMAND(IDM_BLWHMED_IMAGE, &CColorImageProcessingView::OnBlwhMEDImage)		// 중앙값(MED) 기준
	// v) 포스터라이징(8단계)
	ON_COMMAND(IDM_POSTERIZE_IMAGE, &CColorImageProcessingView::OnPosterizeImage)
	// vi) 감마보정
	ON_COMMAND(IDM_GAMMA_IMAGE, &CColorImageProcessingView::OnGammaImage)
	// ===============================================
	// vii) 파라볼라
	ON_COMMAND(IDM_PARABOLACAP_IMAGE, &CColorImageProcessingView::OnParabolaCAPImage)	// CAP
	ON_COMMAND(IDM_PARABOLACUP_IMAGE, &CColorImageProcessingView::OnParabolaCUPImage)	// CUP
	// ===============================================



	// ===============================================
	// <기하학적 처리>
	// ===============================================
	// i) 화면 축소
	ON_COMMAND(IDM_ZOOMOUTRAW_IMAGE, &CColorImageProcessingView::OnZoomOutRAWImage)	// 일반 축소
	ON_COMMAND(IDM_ZOOMOUTAVG_IMAGE, &CColorImageProcessingView::OnZoomOutAVGImage)	// 평균값(AVG)으로 축소
	ON_COMMAND(IDM_ZOOMOUTMED_IMAGE, &CColorImageProcessingView::OnZoomOutMEDImage)	// 중앙값(MED)으로 축소
	// ===============================================
	// ii) 화면 확대
	ON_COMMAND(IDM_ZOOMIN_IMAGE, &CColorImageProcessingView::OnZoomInImage)
	// ===============================================
	// iii) 화면 이동
	ON_COMMAND(IDM_SHIFT_IMAGE, &CColorImageProcessingView::OnShiftImage)
	// ===============================================
	// iv) 화면 대칭
	ON_COMMAND(IDM_MIRRORUD_IMAGE, &CColorImageProcessingView::OnMirrorUDImage)		// 상하 반전
	ON_COMMAND(IDM_MIRRORLR_IMAGE, &CColorImageProcessingView::OnMirrorLRImage)		// 좌우 반전
	ON_COMMAND(IDM_MIRRORUDLR_IMAGE, &CColorImageProcessingView::OnMirrorUDLRImage)	// 상하좌우 반전
	// ===============================================
	// v) 화면 회전
	ON_COMMAND(IDM_ROTATE_IMAGE, &CColorImageProcessingView::OnRotateImage)
	// ===============================================



	// ===============================================
	// <화소영역 처리>
	// ===============================================
	// i) 엠보싱
	ON_COMMAND(IDM_EMBOSS_IMAGE, &CColorImageProcessingView::OnEmbossImage)
	// ===============================================
	// ii) 블러링
	ON_COMMAND(IDM_BLUR_3BY3, &CColorImageProcessingView::OnBlur3by3)			// 3X3 블러링
	ON_COMMAND(IDM_BLUR_5BY5, &CColorImageProcessingView::OnBlur5by5)			// 5X5 블러링
	// ===============================================
	// iii) 경계선 검출
	ON_COMMAND(IDM_EDGEVERT_IMAGE, &CColorImageProcessingView::OnEdgeVertImage)	// 수직 경계선 검출
	ON_COMMAND(IDM_EDGEHORI_IMAGE, &CColorImageProcessingView::OnEdgeHoriImage)	// 수평 경계선 검출
	ON_COMMAND(IDM_EDGEYUSA_IMAGE, &CColorImageProcessingView::OnEdgeYusaImage)	// 유사 연산자
	ON_COMMAND(IDM_EDGECHA_IMAGE, &CColorImageProcessingView::OnEdgeChaImage)	// 차 연산자
	ON_COMMAND(IDM_EDGELOG_IMAGE, &CColorImageProcessingView::OnEdgeLogImage)	// LOG 연산자
	ON_COMMAND(IDM_EDGEDOG_IMAGE, &CColorImageProcessingView::OnEdgeDogImage)	// DOG 연산자
	// ===============================================
END_MESSAGE_MAP()

// CColorImageProcessingView 생성/소멸

CColorImageProcessingView::CColorImageProcessingView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CColorImageProcessingView::~CColorImageProcessingView()
{
}

BOOL CColorImageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CColorImageProcessingView 그리기

void CColorImageProcessingView::OnDraw(CDC* pDC)
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	unsigned char R, G, B;
	for (int i = 0; i < pDoc->m_inH; i++) {
		for (int k = 0; k < pDoc->m_inW; k++) {
			R = pDoc->m_inImageR[i][k];
			G = pDoc->m_inImageG[i][k];
			B = pDoc->m_inImageB[i][k];
			pDC->SetPixel(k, i, RGB(R, G, B));
		}
	}
	for (int i = 0; i < pDoc->m_outH; i++) {
		for (int k = 0; k < pDoc->m_outW; k++) {
			R = pDoc->m_outImageR[i][k];
			G = pDoc->m_outImageG[i][k];
			B = pDoc->m_outImageB[i][k];
			pDC->SetPixel(k + pDoc->m_inW + 5, i, RGB(R, G, B)); // 출력이미지 show (입력이미지와 겹치지 않도록)
		}
	}
}


// CColorImageProcessingView 인쇄

BOOL CColorImageProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CColorImageProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CColorImageProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CColorImageProcessingView 진단

#ifdef _DEBUG
void CColorImageProcessingView::AssertValid() const
{
	CView::AssertValid();
}

void CColorImageProcessingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CColorImageProcessingDoc* CColorImageProcessingView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CColorImageProcessingDoc)));
	return (CColorImageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG


// CColorImageProcessingView 메시지 처리기
// << 영상 처리 알고리즘 함수 구현 >>

// ===============================================
// <화소점 처리> - i) 동일 이미지
// ===============================================
void CColorImageProcessingView::OnEqualImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEqualImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨.
}


// ===============================================
// <화소점 처리> - ii) 반전 이미지
// ===============================================
void CColorImageProcessingView::OnReverseImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnReverseImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}


// ===============================================
// <화소점 처리> - iii) 명도조절
// ===============================================
void CColorImageProcessingView::OnAddImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnAddImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}


// ===============================================
// <화소점 처리> - iv) 흑백 이미지
// ===============================================
void CColorImageProcessingView::OnBlwh128Image()	// 128(픽셀값) 기준
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnBlwh128Image();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}
void CColorImageProcessingView::OnBlwhAVGImage()	// 평균값(AVG) 기준
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnBlwhAVGImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}
void CColorImageProcessingView::OnBlwhMEDImage()	// 중앙값(MED) 기준
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnBlwhMEDImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}


// ===============================================
// <화소점 처리> - v) 포스터라이징(8단계)
// ===============================================
void CColorImageProcessingView::OnPosterizeImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnPosterizeImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}


// ===============================================
// <화소점 처리> - vi) 감마보정
// ===============================================
void CColorImageProcessingView::OnGammaImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnGammaImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}


// ===============================================
// <화소점 처리> - vii) 파라볼라
// ===============================================
void CColorImageProcessingView::OnParabolaCAPImage()	// CAP
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnParabolaCAPImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}
void CColorImageProcessingView::OnParabolaCUPImage()	// CUP
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnParabolaCUPImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}
// ===============================================



// ===============================================
// <기하학적 처리>
// ===============================================
// i) 화면 축소
void CColorImageProcessingView::OnZoomOutRAWImage()	// 일반 축소
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnZoomOutRAWImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}
void CColorImageProcessingView::OnZoomOutAVGImage()	// 평균값(AVG)으로 축소
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnZoomOutAVGImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}
void CColorImageProcessingView::OnZoomOutMEDImage()	// 중앙값(MED)으로 축소
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnZoomOutMEDImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}


// ===============================================
// <기하학적 처리> - ii) 화면 확대
// ===============================================
void CColorImageProcessingView::OnZoomInImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnZoomInImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}


// ===============================================
// <기하학적 처리> - iii) 화면 이동
// ===============================================
void CColorImageProcessingView::OnShiftImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnShiftImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}


// ===============================================
// <기하학적 처리> - iv) 화면 대칭
// ===============================================
void CColorImageProcessingView::OnMirrorUDImage()	// 상하 대칭
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnMirrorUDImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}
void CColorImageProcessingView::OnMirrorLRImage()	// 좌우 대칭
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnMirrorLRImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}
void CColorImageProcessingView::OnMirrorUDLRImage()	// 상하좌우 대칭
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnMirrorUDLRImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}


// ===============================================
// <기하학적 처리> - v) 화면 회전
// ===============================================
void CColorImageProcessingView::OnRotateImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnRotateImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}


// ===============================================
// <화소영역 처리> - i) 엠보싱
// ===============================================
void CColorImageProcessingView::OnEmbossImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnEmbossImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}


// ===============================================
// <화소영역 처리> - ii) 블러링
// ===============================================
void CColorImageProcessingView::OnBlur3by3()	// 3X3 블러링
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnBlur3by3();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}
void CColorImageProcessingView::OnBlur5by5()	// 5X5 블러링
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnBlur5by5();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}


// ===============================================
// <화소영역 처리> - iii) 경계선 검출
// ===============================================
void CColorImageProcessingView::OnEdgeVertImage()	// 수직 경계선 검출
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnEdgeVertImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}
void CColorImageProcessingView::OnEdgeHoriImage()	// 수평 경계선 검출
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnEdgeHoriImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}
void CColorImageProcessingView::OnEdgeYusaImage()	// 유사 연산자
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnEdgeYusaImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}
void CColorImageProcessingView::OnEdgeChaImage()	// 차 연산자
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnEdgeChaImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}
void CColorImageProcessingView::OnEdgeLogImage()	// LOG 연산자
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnEdgeLogImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}
void CColorImageProcessingView::OnEdgeDogImage()	// DOG 연산자
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnEdgeDogImage();
	Invalidate(TRUE);	// OnDraw() 자동 호출됨
}