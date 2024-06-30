
// ColorImage ProcessingDoc.h: CColorImageProcessingDoc 클래스의 인터페이스
//


#pragma once


class CColorImageProcessingDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CColorImageProcessingDoc() noexcept;
	DECLARE_DYNCREATE(CColorImageProcessingDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CColorImageProcessingDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	// 입력이미지 관련
	unsigned char** m_inImageR = NULL;
	unsigned char** m_inImageG = NULL;
	unsigned char** m_inImageB = NULL;	// 동적메모리(RGB채널 구분)
	int m_inH = 0, m_inW = 0;	// 크기(높이, 폭)

	// 출력이미지 관련
	unsigned char** m_outImageR = NULL;
	unsigned char** m_outImageG = NULL;
	unsigned char** m_outImageB = NULL;	// 동적메모리(RGB채널 구분)
	int m_outH = 0, m_outW = 0;	// 크기(높이, 폭)

	// << 재정의 함수 >>
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);	// 파일 열기
	virtual void OnCloseDocument();	// 끝내기

	// << 사용자 정의 함수(1) : 동적 메모리 관리 구현 >>
	unsigned char** OnMalloc2D(int h, int w);	// 동적메모리 할당(unsigned char**형)
	void OnFree2D(unsigned char** memory, int h);	// 동적메모리 해제(unsigned char**형)
	double** OnMalloc2D_double(int h, int w);	// 동적메모리 할당(double**형)
	void OnFree2D_double(double** memory, int h);	// 동적메모리 해제(double**형)

	// << 사용자 정의 함수(2) : 영상처리 알고리즘 구현 >>
	// ===============================================
	// <화소점 처리>
	// ===============================================
	// i) 동일 이미지
	void OnEqualImage();
	// ===============================================
	// ii) 반전 이미지
	void OnReverseImage();
	// ===============================================
	// iii) 명도조절
	void OnAddImage();
	// ===============================================
	// iv) 흑백 이미지
	void OnBlwh128Image();		// 128(픽셀값) 기준
	void OnBlwhAVGImage();		// 평균값(AVG) 기준
	void OnBlwhMEDImage();		// 중앙값(MED) 기준
	// ===============================================
	// v) 포스터라이징(8단계)
	void OnPosterizeImage();
	// ===============================================
	// vi) 감마보정 이미지
	void OnGammaImage();
	// ===============================================
	// vii) 파라볼라 이미지
	void OnParabolaCAPImage();	// CAP
	void OnParabolaCUPImage();	// CUP
	// ===============================================



	// ===============================================
	// <기하학적 처리>
	// ===============================================
	// i) 화면 축소
	void OnZoomOutRAWImage();	// 일반 축소
	void OnZoomOutAVGImage();	// 평균값(AVG)으로 축소
	void OnZoomOutMEDImage();	// 중앙값(MED)으로 축소
	// ===============================================
	// ii) 화면 확대
	void OnZoomInImage();
	// ===============================================
	// iii) 화면 이동
	void OnShiftImage();
	// ===============================================
	// iv) 화면 대칭
	void OnMirrorUDImage();		// 상하 대칭
	void OnMirrorLRImage();		// 좌우 대칭
	void OnMirrorUDLRImage();	// 상하좌우 대칭
	// ===============================================
	// v) 화면 회전
	void OnRotateImage();
	// ===============================================



	// ===============================================
	// <화소영역 처리>
	// ===============================================
	// i) 엠보싱
	void OnEmbossImage();
	// ===============================================
	// ii) 블러링
	void OnBlur3by3();			// 3X3 블러링
	void OnBlur5by5();			// 5X5 블러링
	// ===============================================
	// iii) 경계선 검출
	void OnEdgeVertImage();		// 수직 경계선 검출
	void OnEdgeHoriImage();		// 수평 경계선 검출
	void OnEdgeYusaImage();		// 유사 연산자
	void OnEdgeChaImage();		// 차 연산자
	void OnEdgeLogImage();		// LOG 연산자
	void OnEdgeDogImage();		// DOG 연산
	// ===============================================
};
