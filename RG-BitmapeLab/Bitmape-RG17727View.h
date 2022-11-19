
// Bitmape-RG17727View.h : interface of the CBitmapeRG17727View class
//

#pragma once


class CBitmapeRG17727View : public CView
{
protected: // create from serialization only
	CBitmapeRG17727View() noexcept;
	DECLARE_DYNCREATE(CBitmapeRG17727View)
	int w, h;

// Attributes
public:
	CBitmapeRG17727Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void UkloniPozadinu(CDC* MskDC, CDC* ImgDC, CString bmp);
	virtual void CBitmapeRG17727View::PromeniRGB(CBitmap* bmp, int bytPP);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CBitmapeRG17727View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Bitmape-RG17727View.cpp
inline CBitmapeRG17727Doc* CBitmapeRG17727View::GetDocument() const
   { return reinterpret_cast<CBitmapeRG17727Doc*>(m_pDocument); }
#endif

