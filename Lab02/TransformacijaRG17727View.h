
// TransformacijaRG17727View.h : interface of the CTransformacijaRG17727View class
//

#pragma once


class CTransformacijaRG17727View : public CView
{
protected: // create from serialization only
	CTransformacijaRG17727View() noexcept;
	DECLARE_DYNCREATE(CTransformacijaRG17727View)

	bool setGrid;
	bool rightMultiply;
	int ugao1;
	int ugao2;
	HENHMETAFILE _metaFileDark;
	HENHMETAFILE _metaFileLight;

// Attributes
public:
	CTransformacijaRG17727Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CTransformacijaRG17727View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in TransformacijaRG17727View.cpp
inline CTransformacijaRG17727Doc* CTransformacijaRG17727View::GetDocument() const
   { return reinterpret_cast<CTransformacijaRG17727Doc*>(m_pDocument); }
#endif

