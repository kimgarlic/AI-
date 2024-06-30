
// ColorImage ProcessingView.h: CColorImageProcessingView 클래스의 인터페이스
//

#pragma once


class CColorImageProcessingView : public CView
{
protected: // serialization에서만 만들어집니다.
	CColorImageProcessingView() noexcept;
	DECLARE_DYNCREATE(CColorImageProcessingView)

// 특성입니다.
public:
	CColorImageProcessingDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CColorImageProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:

	// ===============================================
	// <화소점 처리>
	// ===============================================
	// i) 동일 이미지
	afx_msg void OnEqualImage();
	// ===============================================
	// ii) 반전 이미지
	afx_msg void OnReverseImage();
	// ===============================================
	// iii) 명도조절
	afx_msg void OnAddImage();
	// ===============================================
	// iv) 흑백 이미지
	afx_msg void OnBlwh128Image();		// 128(픽셀값) 기준
	afx_msg void OnBlwhAVGImage();		// 평균값(AVG) 기준
	afx_msg void OnBlwhMEDImage();		// 중앙값(MED) 기준
	// ===============================================
	// v) 포스터라이징(8단계)
	afx_msg void OnPosterizeImage();
	// ===============================================
	// vi) 감마보정
	afx_msg void OnGammaImage();
	// ===============================================
	// vii) 파라볼라
	afx_msg void OnParabolaCAPImage();	// CAP
	afx_msg void OnParabolaCUPImage();	// CUP
	// ===============================================



	// ===============================================
	// <기하학적 처리>
	// ===============================================
	// i) 화면 축소
	afx_msg void OnZoomOutRAWImage();	// 일반 축소
	afx_msg void OnZoomOutAVGImage();	// 평균값(AVG)으로 축소
	afx_msg void OnZoomOutMEDImage();	// 중앙값(MED)으로 축소
	// ===============================================
	// ii) 화면 확대
	afx_msg void OnZoomInImage();
	// ===============================================
	// iii) 화면 이동
	afx_msg void OnShiftImage();
	// ===============================================
	// iv) 화면 대칭
	afx_msg void OnMirrorUDImage();		// 상하 반전
	afx_msg void OnMirrorLRImage();		// 좌우 반전
	afx_msg void OnMirrorUDLRImage();	// 상하좌우 반전
	// ===============================================
	// v) 화면 회전
	afx_msg void OnRotateImage();
	// ===============================================



	// ===============================================
	// <화소영역 처리>
	// ===============================================
	// i) 엠보싱
	afx_msg void OnEmbossImage();
	// ===============================================
	// ii) 블러링
	afx_msg void OnBlur3by3();			// 3X3 블러링
	afx_msg void OnBlur5by5();			// 5X5 블러링
	// ===============================================
	// iii) 경계선 검출
	afx_msg void OnEdgeVertImage();		// 수직 경계선 검출
	afx_msg void OnEdgeHoriImage();		// 수평 경계선 검출
	afx_msg void OnEdgeYusaImage();		// 유사 연산자
	afx_msg void OnEdgeChaImage();		// 차 연산자
	afx_msg void OnEdgeLogImage();		// LOG 연산자
	afx_msg void OnEdgeDogImage();		// DOG 연산자
	// ===============================================
};

#ifndef _DEBUG  // ColorImage ProcessingView.cpp의 디버그 버전
inline CColorImageProcessingDoc* CColorImageProcessingView::GetDocument() const
   { return reinterpret_cast<CColorImageProcessingDoc*>(m_pDocument); }
#endif

