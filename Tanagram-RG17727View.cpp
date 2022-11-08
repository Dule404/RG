
// Tanagram-RG17727View.cpp : implementation of the CTanagramRG17727View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Tanagram-RG17727.h"
#endif

#include "Tanagram-RG17727Doc.h"
#include "Tanagram-RG17727View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <corecrt_math_defines.h>


// CTanagramRG17727View

IMPLEMENT_DYNCREATE(CTanagramRG17727View, CView)

BEGIN_MESSAGE_MAP(CTanagramRG17727View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CTanagramRG17727View construction/destruction

CTanagramRG17727View::CTanagramRG17727View() noexcept
{
	// TODO: add construction code here

}

CTanagramRG17727View::~CTanagramRG17727View()
{
}

BOOL CTanagramRG17727View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle) 
{
	double x, y, alfa;
	CPoint* points = new CPoint[n];

	for (int i = 0; i < n; i++)
	{
		alfa = 2 * M_PI * ((double)i / n);
		x = cx + r * cos(alfa + rotAngle);
		y = cy + r * sin(alfa + rotAngle);

		points[i] = CPoint(x, y);
	}
	pDC->Polygon(points, n);
}

void DrawTriangle(CDC* pDC, int cx, int cy, int r, float rotAngle)
{
	double x, y, alfa;
	CPoint* points = new CPoint[3];
	int i = 0, j = 0, n=4;
	while (i < n)
	{
		if (i != 2)
		{
			alfa = 2 * M_PI * ((double)i / n);
			x = cx + r * cos(alfa + rotAngle);
			y = cy + r * sin(alfa + rotAngle);

			points[j] = CPoint(x, y);
			j++;
		}
		i++;
	}
	pDC->Polygon(points, 3);
}

void DrawParallelogram(CDC* pDC, int x1, int y1, int x2, int y2)
{
	int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	DWORD dw = GetLastError();

	XFORM transformation, oldXform;
	BOOL b = GetWorldTransform(pDC->m_hDC, &oldXform);

	transformation.eM11 = (FLOAT)1.0;
	transformation.eM12 = (FLOAT)0.0;
	transformation.eM21 = (FLOAT)-1.0;
	transformation.eM22 = (FLOAT)1.0;
	transformation.eDx = (FLOAT)0.0;
	transformation.eDy = (FLOAT)0.0;
	
	b = SetWorldTransform(pDC->m_hDC, &transformation);
	dw = GetLastError();

	pDC->Rectangle(x1, y1, x2, y2);

	b = SetWorldTransform(pDC->m_hDC, &oldXform);
	SetGraphicsMode(pDC->m_hDC, prevMode);
}

void DrawGrid(CDC* pDC)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, RGB(202, 203, 207));
	CPen* oldPen = pDC->SelectObject(&pen);

	for (int i = 0; i < 501; i += 25) 
	{
		pDC->MoveTo(i, 0);
		pDC->LineTo(i, 500);
	}

	pDC->MoveTo(0, 0);
	for (int i = 0; i < 501; i += 25)
	{
		pDC->MoveTo(0, i);
		pDC->LineTo(500, i);
	}

	pDC->SelectObject(oldPen);
	pen.DeleteObject();
}



// CTanagramRG17727View drawing

void CTanagramRG17727View::OnDraw(CDC* pDC)
{
	CTanagramRG17727Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	//drawing background
	CBrush greyBrush;
	greyBrush.CreateSolidBrush(RGB(187, 191, 188));
	CBrush* oldBrush = pDC->SelectObject(&greyBrush);
	pDC->Rectangle(0, 0, 500, 500);
	pDC->SelectObject(oldBrush);
	greyBrush.DeleteObject();
	
	//changing pen
	CPen pen;
	pen.CreatePen(PS_SOLID, 4, RGB(0, 255, 255));
	CPen* oldPen = pDC->SelectObject(&pen);

	//drawing pink triangle
	CBrush pinkBrush;
	pinkBrush.CreateSolidBrush(RGB(245, 188, 237));
	oldBrush = pDC->SelectObject(&pinkBrush);
	DrawTriangle(pDC, 100, 100, 50, 0);
	DrawRegularPolygon(pDC, 120, 100, 13, 5, 0);
	pDC->SelectObject(oldBrush);
	pinkBrush.DeleteObject();

	//drawing yellow
	CBrush yellowBrush;
	yellowBrush.CreateSolidBrush(RGB(255, 255, 0));
	oldBrush = pDC->SelectObject(&yellowBrush);
	DrawTriangle(pDC, 200, 100, 50, 3.14);
	DrawRegularPolygon(pDC, 180, 100, 13, 6, 0);
	pDC->SelectObject(oldBrush);
	yellowBrush.DeleteObject();

	//drawing purple
	CBrush purpleBrush;
	purpleBrush.CreateSolidBrush(RGB(153, 45, 186));
	oldBrush = pDC->SelectObject(&purpleBrush);
	DrawRegularPolygon(pDC, 150, 150, 50, 4, 0);
	pDC->SelectObject(oldBrush);
	purpleBrush.DeleteObject();

	//drawing brown
	CBrush brownBrush;
	brownBrush.CreateSolidBrush(RGB(217, 144, 72));
	oldBrush = pDC->SelectObject(&brownBrush);
	DrawTriangle(pDC, 150, 313, 113, 0);
	DrawRegularPolygon(pDC, 190, 313, 20, 4, 0);
	pDC->SelectObject(oldBrush);
	brownBrush.DeleteObject();
	
	//drawing green
	CBrush greenBrush;
	greenBrush.CreateSolidBrush(RGB(40, 181, 59));
	oldBrush = pDC->SelectObject(&greenBrush);
	DrawTriangle(pDC, 150, 275, 75, 3.14);
	DrawRegularPolygon(pDC, 120, 275, 15, 8, 0);
	pDC->SelectObject(oldBrush);
	greenBrush.DeleteObject();

	//drawing hatch
	CBrush hatchBrush;
	hatchBrush.CreateHatchBrush(HS_VERTICAL, RGB(78, 98, 212));
	oldBrush = pDC->SelectObject(&hatchBrush);
	DrawTriangle(pDC, 188, 388, 107, 0.785);
	DrawRegularPolygon(pDC, 213, 425, 25, 7, 0);
	pDC->SelectObject(oldBrush);
	hatchBrush.DeleteObject();

	//drawing red
	CBrush redBrush;
	redBrush.CreateSolidBrush(RGB(255, 0, 0));
	oldBrush = pDC->SelectObject(&redBrush);
	DrawParallelogram(pDC, 725, 407, 820, 463);
	pDC->SelectObject(oldBrush);
	redBrush.DeleteObject();

	pDC->SelectObject(oldPen);
	pen.DeleteObject();

	if(setGrid)
	{
		DrawGrid(pDC);
	}

}


// CTanagramRG17727View printing

BOOL CTanagramRG17727View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTanagramRG17727View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTanagramRG17727View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CTanagramRG17727View diagnostics

#ifdef _DEBUG
void CTanagramRG17727View::AssertValid() const
{
	CView::AssertValid();
}

void CTanagramRG17727View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTanagramRG17727Doc* CTanagramRG17727View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTanagramRG17727Doc)));
	return (CTanagramRG17727Doc*)m_pDocument;
}
#endif //_DEBUG


// CTanagramRG17727View message handlers


void CTanagramRG17727View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 71)
	{
		setGrid = !setGrid;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
