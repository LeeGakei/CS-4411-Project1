//Added by Jackie Lee
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "LineBrush.h"

extern float frand();

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void LineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize((float)size);

	BrushMove(source, target);
}

void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	double x1, y1, x2, y2;
	double length = pDoc->getSize();
	double width = pDoc->m_pUI->getLineWidth();
	double angle = pDoc->m_pUI->getLineAngle();
	x1 = target.x - length / 2;
	x2 = target.x + length / 2;
	y1 = target.y - width / 2;
	y2 = target.y + width / 2;

	//move the coordinate system origin to the target point, make rotation and reset the origin.
	glPushMatrix();
		glTranslatef(target.x, target.y, 0.0);
		glRotatef(angle, 0, 0, 1);
		glTranslatef(-target.x, -target.y, 0.0);
		SetColor(source);
		glRectf(x1, y1, x2, y2);
		glFlush();
	glPopMatrix();
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

//new comment :P