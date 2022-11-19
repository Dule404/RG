
// Bitmape-RG17727View.cpp : implementation of the CBitmapeRG17727View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Bitmape-RG17727.h"
#endif

#include "Bitmape-RG17727Doc.h"
#include "Bitmape-RG17727View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <corecrt_math_defines.h>
#include <string>
#include "DImage.h"


// CBitmapeRG17727View

IMPLEMENT_DYNCREATE(CBitmapeRG17727View, CView)

BEGIN_MESSAGE_MAP(CBitmapeRG17727View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CBitmapeRG17727View construction/destruction

CBitmapeRG17727View::CBitmapeRG17727View() noexcept
{
	// TODO: add construction code here
	w = h = 0;
}

CBitmapeRG17727View::~CBitmapeRG17727View()
{
}

BOOL CBitmapeRG17727View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CBitmapeRG17727View drawing
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

void Ogledalo(CDC* pDC, bool mx, bool my, bool rightMultiply)
{
	XFORM form;
	if(mx)
		form.eM11 = 1;
	else
		form.eM11 = -1;
	form.eM12 = 0.0;
	form.eM21 = 0.0;
	if (my)
		form.eM22 = 1;
	else
		form.eM22 = -1;
	form.eDx = 0;
	form.eDy = 0;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&form, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&form, MWT_LEFTMULTIPLY);
}

void crtajGrid(CDC* pDC)
{
	CPen penR(PS_SOLID, 2, RGB(255, 0, 0));
	CPen penG(PS_SOLID, 2, RGB(0, 255, 0));
	CPen* oldPen = pDC->SelectObject(&penR);
	pDC->MoveTo(0, 0);
	pDC->LineTo(1000, 0);
	pDC->SelectObject(&penG);
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, 1000);

	CPen pen(PS_SOLID, 2, RGB(166, 164, 161));
	pDC->SelectObject(&pen);

	for (int i = 25; i < 1000; i += 25) 
	{
		pDC->MoveTo(i, 0);
		pDC->LineTo(i, 1000);
		pDC->MoveTo(0, i);
		pDC->LineTo(1000, i);
	}

	pDC->SelectObject(oldPen);
}

void CBitmapeRG17727View::UkloniPozadinu(CDC* MskDC, CDC* ImgDC, CString bmp)
{
	DImage Image;
	Image.Load(bmp);

	w = Image.Width();
	h = Image.Height();

	CBitmap* bmpImage = new CBitmap();
	bmpImage->CreateBitmap(Image.Width(), Image.Height(), 1, 32, NULL);
	BITMAP bitmapInfo;
	bmpImage->GetBitmap(&bitmapInfo);
	bmpImage->SetBitmapBits( Image.Width()*Image.Height()*(bitmapInfo.bmBitsPixel / 8) ,Image.GetDIBBits());
	
	int curBPP = bitmapInfo.bmBitsPixel / 8;
	

	CBitmap bmpMask;
	// Pravljenje monohromatske bitmape iste veli?ine 
	bmpMask.CreateBitmap(w, h, 1, 1, NULL);
	BITMAP bm;
	bmpMask.GetBitmap(&bm);
	int curBPPMsk = bm.bmBitsPixel / 8;

	//ucitavanje bpma u dc
	CDC* SrcDC = new CDC();
	SrcDC->CreateCompatibleDC(MskDC);
	CDC* DstDC = new CDC();
	DstDC->CreateCompatibleDC(MskDC);

	CBitmap* pOldSrcBmp = SrcDC->SelectObject(bmpImage);
	CBitmap* pOldDstBmp = DstDC->SelectObject(&bmpMask);


	//popunjavanje monohromatske bpm
	COLORREF clrTopLeft = SrcDC->GetPixel(3, 0);
	COLORREF clrSaveBk = SrcDC->SetBkColor(clrTopLeft);
	DstDC->BitBlt(0, 0, w, h, SrcDC, 0, 0, SRCCOPY);
	PromeniRGB(bmpImage, curBPP);
	//markiranje pozadine u izvornoj bpm
	COLORREF clrSaveDstText = SrcDC->SetTextColor(RGB(255, 255, 255));
	SrcDC->SetBkColor(RGB(0, 0, 0));
	SrcDC->BitBlt(0, 0, w, h, DstDC, 0, 0, SRCAND);
	//MskDC->BitBlt(0, 0, w, h, SrcDC, 0, 0, SRCCOPY);

	//oslobadjanje resursa
	DstDC->SetTextColor(clrSaveDstText);
	SrcDC->SetBkColor(clrSaveBk);
	SrcDC->SelectObject(pOldSrcBmp);
	DstDC->SelectObject(pOldDstBmp);

	//PromeniRGB(&bmpMask, curBPPMsk);

	MskDC->SelectObject(&bmpMask);
	ImgDC->SelectObject(bmpImage);
}

void CBitmapeRG17727View::PromeniRGB(CBitmap* bmp, int bytPP)
{
	DWORD dwSize = h * w * bytPP;
	unsigned char* pData = new unsigned char[dwSize];
	bmp->GetBitmapBits(dwSize, pData);
	
	int rgb;
	int r, g, b;
	for (int i = 2; i < dwSize; i+=3)
	{
		r = pData[i - 2];
		g = pData[i - 1];
		b = pData[i];

		rgb = 64 + (r + g + b) / 3;
		if (rgb > 255)
			rgb = 255;

		pData[i] = rgb;
		pData[i - 1] = rgb;
		pData[i - 2] = rgb;
	}

	bmp->SetBitmapBits(dwSize, pData);
}

void CBitmapeRG17727View::OnDraw(CDC* pDC)
{
	CBitmapeRG17727Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	XFORM start;
	GetWorldTransform(pDC->m_hDC, &start);
	int oldGMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);

	CDC* ImgDC = new CDC();
	ImgDC->CreateCompatibleDC(pDC);
	CDC* MskDC = new CDC();
	MskDC->CreateCompatibleDC(pDC);

	CString putanja1("res\\puzzle1.dib");
	CString putanja2("res\\puzzle2.dib");
	CString putanja3("res\\puzzle3.dib");
	CString putanja4("res\\puzzle4.dib");
	CString putanja5("res\\puzzle5.dib");
	CString putanja6("res\\puzzle6.dib");
	CString putanja7("res\\puzzle7.dib");
	CString putanja8("res\\puzzle8.dib");
	CString putanja9("res\\puzzle9.dib");

	crtajGrid(pDC);

	UkloniPozadinu(MskDC, ImgDC, putanja1);
	Translacija(pDC, 25, 100, 0);
	Rotacija(pDC, -25, 0);
	Ogledalo(pDC, 1, 0, 0);
	Translacija(pDC, -25, -200, 0);

	pDC->BitBlt(0, 0, w, h, MskDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, w, h, ImgDC, 0, 0, SRCPAINT);

	SetWorldTransform(pDC->m_hDC, &start);

	UkloniPozadinu(ImgDC, MskDC, putanja2);
	Translacija(pDC, -370, -272, 1);
	Rotacija(pDC, 56, 1);
	Ogledalo(pDC, 1, 0, 1);
	Translacija(pDC, 222, 38, 1);

	pDC->BitBlt(225, 30, w, h, ImgDC, 0, 0, SRCAND);
	pDC->BitBlt(225, 30, w, h, MskDC, 0, 0, SRCPAINT);

	SetWorldTransform(pDC->m_hDC, &start);

	UkloniPozadinu(ImgDC, MskDC, putanja3);
	Translacija(pDC, 450, 111, 0);
	Rotacija(pDC, 13, 0);
	Translacija(pDC, -547, -165, 0);

	pDC->BitBlt(425, 30, w, h, ImgDC, 0, 0, SRCAND);
	pDC->BitBlt(425, 30, w, h, MskDC, 0, 0, SRCPAINT);

	SetWorldTransform(pDC->m_hDC, &start);

	UkloniPozadinu(ImgDC, MskDC, putanja4);
	Translacija(pDC, -175, -275, 1);
	Rotacija(pDC, -73, 1);
	Ogledalo(pDC, 1, 0, 1);
	Translacija(pDC, 70, 345, 1);

	pDC->BitBlt(0, 230, w, h, ImgDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 230, w, h, MskDC, 0, 0, SRCPAINT);

	SetWorldTransform(pDC->m_hDC, &start);

	UkloniPozadinu(ImgDC, MskDC, putanja5);
	Translacija(pDC, -98, -455, 1);
	Rotacija(pDC, -122, 1);
	Ogledalo(pDC, 1, 0, 1);
	Translacija(pDC, 220, 190, 1);

	pDC->BitBlt(0, 430, w, h, ImgDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 430, w, h, MskDC, 0, 0, SRCPAINT);

	SetWorldTransform(pDC->m_hDC, &start);

	UkloniPozadinu(ImgDC, MskDC, putanja6);
	Translacija(pDC, -500, -245, 1);
	Rotacija(pDC, -107, 1);
	Ogledalo(pDC, 1, 0, 1);
	Translacija(pDC, 385, 200, 1);

	pDC->BitBlt(425, 200, w, h, ImgDC, 0, 0, SRCAND);
	pDC->BitBlt(425, 200, w, h, MskDC, 0, 0, SRCPAINT);

	SetWorldTransform(pDC->m_hDC, &start);

	UkloniPozadinu(ImgDC, MskDC, putanja7);
	Translacija(pDC, -170, -450, 1);
	Rotacija(pDC, 20, 1);
	Ogledalo(pDC, 0, 1, 1);
	Translacija(pDC, 70, 365, 1);

	pDC->BitBlt(0, 400, w, h, ImgDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 400, w, h, MskDC, 0, 0, SRCPAINT);

	SetWorldTransform(pDC->m_hDC, &start);

	UkloniPozadinu(ImgDC, MskDC, putanja8);
	Translacija(pDC, -425, -575, 1);
	Rotacija(pDC, -67, 1);
	Ogledalo(pDC, 1, 0, 1);
	Translacija(pDC, 375, 506, 1);

	pDC->BitBlt(200, 400, w, h, ImgDC, 0, 0, SRCAND);
	pDC->BitBlt(200, 400, w, h, MskDC, 0, 0, SRCPAINT);

	SetWorldTransform(pDC->m_hDC, &start);

	UkloniPozadinu(ImgDC, MskDC, putanja9);
	Translacija(pDC, -430, -580, 1);
	Rotacija(pDC, -20, 1);
	Ogledalo(pDC, 1, 0, 1);
	Translacija(pDC, 380, 360, 1);

	pDC->BitBlt(400, 400, w, h, ImgDC, 0, 0, SRCAND);
	pDC->BitBlt(400, 400, w, h, MskDC, 0, 0, SRCPAINT);
}


// CBitmapeRG17727View printing

BOOL CBitmapeRG17727View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBitmapeRG17727View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBitmapeRG17727View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CBitmapeRG17727View diagnostics

#ifdef _DEBUG
void CBitmapeRG17727View::AssertValid() const
{
	CView::AssertValid();
}

void CBitmapeRG17727View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBitmapeRG17727Doc* CBitmapeRG17727View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBitmapeRG17727Doc)));
	return (CBitmapeRG17727Doc*)m_pDocument;
}
#endif //_DEBUG


// CBitmapeRG17727View message handlers
