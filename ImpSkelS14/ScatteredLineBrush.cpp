#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredLineBrush.h"

#include <cmath>

extern float frand();

ScatteredLineBrush::ScatteredLineBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void ScatteredLineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();

	glPointSize(float(size));

	BrushMove(source, target);
}

void ScatteredLineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	int number = ceil((double)rand()/RAND_MAX * 5);
	//printf("%d",number);

	int x = source.x;
	int y = source.y;
	int size = dlg->getSize();

	for (int i = 0; i < number; i++){
		int deltaX = ((double)rand() / RAND_MAX - 0.5) * size;
		int deltaY = ((double)rand() / RAND_MAX - 0.5) * size;
		double randomLength = (double)rand() / RAND_MAX * size;
		DrawLine(Point(source.x + deltaX,source.y + deltaY),Point(target.x + deltaX,target.y + deltaY),randomLength);
	}


}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

void ScatteredLineBrush::DrawLine(const Point source, const Point target, const double length)
{
	ImpressionistDoc* pDoc = GetDocument();

	double x1, y1, x2, y2;
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
