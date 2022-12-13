
// OpenGLView.cpp : implementation of the COpenGLView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OpenGL.h"
#endif

#include "OpenGLDoc.h"
#include "OpenGLView.h"
#include <corecrt_math_defines.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenGLView

IMPLEMENT_DYNCREATE(COpenGLView, CView)

BEGIN_MESSAGE_MAP(COpenGLView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// COpenGLView construction/destruction

COpenGLView::COpenGLView() noexcept
{
	// TODO: add construction code here
	alpha = 0.0f;
	beta = 0.0f;
	rotAngl = 0;
}

COpenGLView::~COpenGLView()
{
}

BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// COpenGLView drawing

void COpenGLView::OnDraw(CDC* pDC)
{
	//COpenGLDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	//if (!pDoc)
	//	return;

	COpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	m_glRenderer.DrawScene(pDC);

	// TODO: add draw code for native data here
}


// COpenGLView printing

BOOL COpenGLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COpenGLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COpenGLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// COpenGLView diagnostics

#ifdef _DEBUG
void COpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGLDoc* COpenGLView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLDoc)));
	return (COpenGLDoc*)m_pDocument;
}
#endif //_DEBUG


// COpenGLView message handlers

void COpenGLView::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}

int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}

void COpenGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}

void COpenGLView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}

BOOL COpenGLView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}


void COpenGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	if (nChar == VK_LEFT)
	{
		beta = (beta + 5) * (M_PI / 180);
		m_glRenderer.ChangeAngle(beta, 0);
		Invalidate();
	}
	if (nChar == VK_RIGHT)
	{
		beta = (beta - 5) * (M_PI / 180);
		m_glRenderer.ChangeAngle(beta, 0);
		Invalidate();
	}
	if (nChar == VK_UP)
	{
		alpha = (alpha + 5) * (M_PI / 180);
		m_glRenderer.ChangeAngle(alpha, 1);
		Invalidate();
	}
	if (nChar == VK_DOWN)
	{
		alpha = (alpha - 5) * (M_PI / 180);
		m_glRenderer.ChangeAngle(alpha, 1);
		Invalidate();
	}
	if (nChar == VK_DOWN)
	{
		alpha = (alpha - 5) * (M_PI / 180);
		m_glRenderer.ChangeAngle(alpha, 1);
		Invalidate();
	}
	if (nChar == 'A')
	{
		rotAngl += 10;
		m_glRenderer.SetRot(rotAngl);
		Invalidate();
	}
	

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
