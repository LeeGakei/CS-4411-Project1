#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredPointBrush.h"

#include "PointBrush.h"	//to use the function DrawPoint there

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

	//glPointSize((float)size);

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
	for (int i = x - size / 2; i < x + size / 2; i++){
		for (int j = y - size / 2; j < y + size / 2; y++){
			double random = (double)rand() / RAND_MAX;
			if (random > 0.5){
				DrawPoint(source,target);
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
