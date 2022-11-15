
// TransformacijaRG17727View.cpp : implementation of the CTransformacijaRG17727View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TransformacijaRG17727.h"
#endif

#include "TransformacijaRG17727Doc.h"
#include "TransformacijaRG17727View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <corecrt_math_defines.h>


// CTransformacijaRG17727View

IMPLEMENT_DYNCREATE(CTransformacijaRG17727View, CView)

BEGIN_MESSAGE_MAP(CTransformacijaRG17727View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CTransformacijaRG17727View construction/destruction

CTransformacijaRG17727View::CTransformacijaRG17727View() noexcept
{
	setGrid = false;
	rightMultiply = false;
	ugao1 = ugao2 = 0;

	_metaFileDark = GetEnhMetaFile(CString("res\\cactus_part.emf"));
	_metaFileLight = GetEnhMetaFile(CString("res\\cactus_part_light.emf"));

}

CTransformacijaRG17727View::~CTransformacijaRG17727View()
{
}

BOOL CTransformacijaRG17727View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTransformacijaRG17727View drawing

void crtajGrid(CDC* pDC)
{
	CPen gridPen(PS_SOLID, 0, RGB(255, 255, 255));
	CPen* oldPen = pDC->SelectObject(&gridPen);
	for (int i = 0; i < 500; i+=25)
	{
		pDC->MoveTo(i, 0);
		pDC->LineTo(i , 500);
		pDC->MoveTo(0, i);
		pDC->LineTo(500, i);
	}

	pDC->SelectObject(oldPen);
}

void ispisiIme(CDC* pDC)
{
	CFont font;
	CString fontName("ARIAL");
	font.CreateFont(30, 10, -900, 0, 700, 0, 0, 0, 0, 0, 0, 0, 0, fontName);
	int nOldMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF clrOld = pDC->SetTextColor(RGB(255, 255, 0));
	CFont* oldFont = pDC->SelectObject(&font);
	CString text("17727 Dusan Lukic");
	pDC->TextOutW(475, 25, text);

	pDC->SetBkMode(nOldMode);
	pDC->SelectObject(oldFont);
	pDC->SetTextColor(clrOld);
}

void crtajSaksiju(CDC* pDC)
{
	CBrush saksijaBrush(RGB(255, 153, 51));
	CBrush* oldBrush = pDC->SelectObject(&saksijaBrush);

	CPoint points[] = { {200, 450}, {300, 450}, {285, 500}, {215, 500} };

	pDC->Rectangle(180, 430, 322, 450);
	pDC->Polygon(points, 4);

	pDC->SelectObject(oldBrush);
}

double StepeniURadijane(double ugao)
{
	return (ugao * (M_PI / 180));
}

void Rotacija(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM form;
	form.eM11 = cos(StepeniURadijane(angle));
	form.eM12 = sin(StepeniURadijane(angle));
	form.eM21 = -sin(StepeniURadijane(angle));
	form.eM22 = cos(StepeniURadijane(angle));
	form.eDx = 0;
	form.eDy = 0;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&form, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&form, MWT_LEFTMULTIPLY);
}

void Translacija(CDC* pDC, int x, int y, bool rightMultiply)
{
	XFORM form;
	form.eM11 = 1.0;
	form.eM12 = 0.0;
	form.eM21 = 0.0;
	form.eM22 = 1.0;
	form.eDx = x;
	form.eDy = y;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&form, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&form, MWT_LEFTMULTIPLY);
}

void Skaliranje(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM form;
	form.eM11 = sX;
	form.eM12 = 0.0;
	form.eM21 = 0.0;
	form.eM22 = sY;
	form.eDx = 0;
	form.eDy = 0;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&form, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&form, MWT_LEFTMULTIPLY);
}


void CTransformacijaRG17727View::OnDraw(CDC* pDC)
{
	CTransformacijaRG17727Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CBrush brush(RGB(140, 179, 230));
	CBrush* oldBrush = pDC->SelectObject(&brush);
	pDC->Rectangle(0, 0, 500, 500);
	pDC->SelectObject(oldBrush);
	ispisiIme(pDC);

	XFORM start;
	GetWorldTransform(pDC->m_hDC, &start);
	int oldGMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);

	CBrush cactusBrush(RGB(0, 204, 0));
	oldBrush = pDC->SelectObject(&cactusBrush);


	Translacija(pDC, -250, -425, true);
	Rotacija(pDC, ugao1, true);
	Translacija(pDC, 250, 425, true);
	PlayEnhMetaFile(pDC->m_hDC, _metaFileLight, CRect(220, 350, 280, 425));

	PlayEnhMetaFile(pDC->m_hDC, _metaFileDark, CRect(240, 280, 260, 350));


	SetWorldTransform(pDC->m_hDC, &start);
	Translacija(pDC, -250, -425, true);
	Rotacija(pDC, ugao1 - 45, true);
	Translacija(pDC, 250, 425, true);
	PlayEnhMetaFile(pDC->m_hDC, _metaFileDark, CRect(290, 300, 310, 375));

	SetWorldTransform(pDC->m_hDC, &start);
	pDC->Ellipse(CRect(240, 415, 260, 435));
	Translacija(pDC, -250, -425, true);
	Rotacija(pDC, ugao1 + 45, true);
	Translacija(pDC, 250, 425, true);
	PlayEnhMetaFile(pDC->m_hDC, _metaFileDark, CRect(190, 300, 210, 375));
	pDC->Ellipse(CRect(190, 365, 210, 385));

	SetWorldTransform(pDC->m_hDC, &start);
	Translacija(pDC, -250, -425, true);
	Rotacija(pDC, ugao1 + 90, true);
	Translacija(pDC, 250, 425, true);
	pDC->Ellipse(CRect(120, 365, 140, 385));
	pDC->Ellipse(CRect(120, 290, 140, 310));
	PlayEnhMetaFile(pDC->m_hDC, _metaFileDark, CRect(110, 300, 150, 375));

	SetWorldTransform(pDC->m_hDC, &start);
	Translacija(pDC, -250, -425, true);
	Rotacija(pDC, ugao1 + 45, true);
	Translacija(pDC, 250, 425, true);
	PlayEnhMetaFile(pDC->m_hDC, _metaFileDark, CRect(240, 180, 280, 255));

	SetWorldTransform(pDC->m_hDC, &start);
	Translacija(pDC, -250, -425, true);
	Rotacija(pDC, ugao1 + 135, true);
	Translacija(pDC, 250, 425, true);
	PlayEnhMetaFile(pDC->m_hDC, _metaFileDark, CRect(60, 350, 100, 425));


	double xSek = 250 + 300 * cos(StepeniURadijane(ugao1)) - 300 * sin(StepeniURadijane(ugao1)) - 250 * cos(StepeniURadijane(ugao1)) + 425 * sin(StepeniURadijane(ugao1));
	double ySek = 425 + 300 * sin(StepeniURadijane(ugao1)) + 300 * cos(StepeniURadijane(ugao1)) - 250 * sin(StepeniURadijane(ugao1)) - 425 * cos(StepeniURadijane(ugao1));

	SetWorldTransform(pDC->m_hDC, &start);
	Translacija(pDC, -xSek, -ySek, true);
	Rotacija(pDC, ugao1 + ugao2 +180, true);//ovde ide ugao 2
	Translacija(pDC, xSek, ySek, true);
	PlayEnhMetaFile(pDC->m_hDC, _metaFileLight, CRect(xSek - 20, ySek + 75, xSek + 15, ySek-5));

	SetWorldTransform(pDC->m_hDC, &start);
	Translacija(pDC, -250, -425, true);
	Rotacija(pDC, ugao1 + 270, true);
	Translacija(pDC, 250, 425, true);
	PlayEnhMetaFile(pDC->m_hDC, _metaFileDark, CRect(345, 305, 385, 380));

	SetWorldTransform(pDC->m_hDC, &start);
	Translacija(pDC, -250, -425, true);
	Rotacija(pDC, ugao1, true);
	Translacija(pDC, 250, 425, true);
	PlayEnhMetaFile(pDC->m_hDC, _metaFileDark, CRect(180, 230, 220, 305));

	PlayEnhMetaFile(pDC->m_hDC, _metaFileDark, CRect(170, 160, 230, 235));
	pDC->Ellipse(CRect(190, 220, 210, 240));
	pDC->Ellipse(CRect(190, 295, 210, 315));

	SetWorldTransform(pDC->m_hDC, &start);
	pDC->SelectObject(oldBrush);
	crtajSaksiju(pDC);

	if (setGrid) 
	{
		crtajGrid(pDC);
	}


}


// CTransformacijaRG17727View printing

BOOL CTransformacijaRG17727View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTransformacijaRG17727View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTransformacijaRG17727View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CTransformacijaRG17727View diagnostics

#ifdef _DEBUG
void CTransformacijaRG17727View::AssertValid() const
{
	CView::AssertValid();
}

void CTransformacijaRG17727View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTransformacijaRG17727Doc* CTransformacijaRG17727View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTransformacijaRG17727Doc)));
	return (CTransformacijaRG17727Doc*)m_pDocument;
}
#endif //_DEBUG


// CTransformacijaRG17727View message handlers


void CTransformacijaRG17727View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	if (nChar == 71)
	{
		setGrid = !setGrid;
	}

	if (nChar == 65)
	{
		ugao1 -= 6;
	}

	if (nChar == 83)
	{
		ugao1 += 6;
	}

	if (nChar == 81)
	{
		ugao2 -= 6;
	}

	if (nChar == 87)
	{
		ugao2 += 6;
	}

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
