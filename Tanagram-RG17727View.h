
// Tanagram-RG17727View.h : interface of the CTanagramRG17727View class
//

#pragma once


class CTanagramRG17727View : public CView
{
protected: // create from serialization only
	CTanagramRG17727View() noexcept;
	DECLARE_DYNCREATE(CTanagramRG17727View)

// Attributes
public:
	CTanagramRG17727Doc* GetDocument() const;
	bool setGrid;

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
	virtual ~CTanagramRG17727View();
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

#ifndef _DEBUG  // debug version in Tanagram-RG17727View.cpp
inline CTanagramRG17727Doc* CTanagramRG17727View::GetDocument() const
   { return reinterpret_cast<CTanagramRG17727Doc*>(m_pDocument); }
#endif

