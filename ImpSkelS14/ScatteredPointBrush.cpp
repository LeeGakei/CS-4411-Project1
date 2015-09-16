#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredPointBrush.h"

extern float frand();

ScatteredPointBrush::ScatteredPointBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void ScatteredPointBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	//int size = pDoc->getSize();

	glPointSize(1.0f);

	BrushMove(source, target);
}

void ScatteredPointBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	int x = source.x;
	int y = source.y;
	int size = pDoc->getSize();
	for (int i = - size / 2; i < size / 2; i++){
		for (int j = - size / 2; j < size / 2; j++){
			double random = (double)rand() / RAND_MAX;
			if (random > 0.75){
				DrawPoint(Point(source.x + i, source.y + j), Point(target.x + i, target.y + j));
			}
		}
	}


}

void ScatteredPointBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

void ScatteredPointBrush::DrawPoint(const Point source, const Point target)
{
	glBegin(GL_POINTS);
	SetColor(source);
	glVertex2d(target.x, target.y);
	glEnd();
}
