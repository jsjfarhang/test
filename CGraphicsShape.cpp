#include "CGraphicsShape.h"

CBitmap BMPImages[NUM_IMAGES];

CGraphicsShape::CGraphicsShape ()
{
	Color = RGB(255, 255, 255);
	WhichShape = 'S';
	CenterX = 100;
	CenterY = 100;
	Size = 40;
	Where = CRect(80, 80, 120, 120);
	Selected = false;
}

CGraphicsShape::CGraphicsShape(COLORREF C, char S, int X, int Y, int Z,
	IMAGE_INDEX I)
{
	Color = C;
	WhichShape = S;
	CenterX = X;
	CenterY = Y;
	Size = Z;
	Where = CRect(CenterX - Z / 2, CenterY - Z / 2, CenterX + Z / 2,
		CenterY + Z / 2);
	Selected = false;
	ImageIdx = I;
}

CRect CGraphicsShape::GetWhere() const
{
	return Where;
}

bool CGraphicsShape::InShape(int x, int y) const
{
	if (WhichShape == 'S')
	{
		if (x > CenterX - (Size / 2))
			if (x < CenterX + (Size / 2))
				if (y > CenterY - (Size / 2))
					if (y < CenterY + (Size / 2))
						return true;
	}
	else if (WhichShape == 'C')
	{
		float a = x - CenterX;
		float b = y - CenterY;
		float c = sqrt((a*a) + (b*b));
		if (c > Size / 2)
			return false;
		else
			return true;
	}
	return false;
}

void CGraphicsShape::SetSelected(bool flag)
{
	Selected = flag;
}

void CGraphicsShape::Resize(int ds)
{
	Size += ds;
}

void CGraphicsShape::Move(int dx, int dy)
{
	Where.left += dx;
	Where.right += dx;
	Where.top += dy;
	Where.bottom += dy;
	CenterX += dx;
	CenterY += dy;
}

void CGraphicsShape::Paint (CPaintDC * dc)
{
	CBrush paintBrush;
	paintBrush.CreateSolidBrush (Color);
	CBrush * pBrushSv = dc->SelectObject (&paintBrush);
	CPen pen, *pPenSv = NULL;
	pen.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
	if (Selected)
		pPenSv = dc->SelectObject(&pen);
	if (WhichShape == 'S')
		dc->Rectangle (Where);
	else if (WhichShape == 'C')
		dc->Ellipse (Where);
	dc->SelectObject (pBrushSv);
	if (Selected)
		dc->SelectObject(pPenSv);
	if (ImageIdx != NONE)
	{
		CDC memDC;
		memDC.CreateCompatibleDC(dc);
		memDC.SelectObject(BMPImages[ImageIdx]);
		dc->TransparentBlt(Where.left + 1, Where.top + 1,
			Where.Width() - 2, Where.Height() - 2,
			&memDC, 0, 0, 80, 80, SRCCOPY);
	}
}
