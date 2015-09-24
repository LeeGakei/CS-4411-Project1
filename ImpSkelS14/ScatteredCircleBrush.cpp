#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredCircleBrush.h"

#include <cmath>

extern float frand();

ScatteredCircleBrush::ScatteredCircleBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void ScatteredCircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();

	glPointSize(float(size));

	BrushMove(source, target);
}

void ScatteredCircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	int number = ceil((double)rand() / RAND_MAX * 4);
	//printf("%d", number);

	int x = source.x;
	int y = source.y;
	int size = dlg->getSize();

	for (int i = 0; i < number; i++){
		int deltaX = ((double)rand() / RAND_MAX - 0.5) * size;
		int deltaY = ((double)rand() / RAND_MAX - 0.5) * size;
		double randomLength = (double)rand() / RAND_MAX * size;
		DrawCircle(Point(source.x + deltaX, source.y + deltaY), Point(target.x + deltaX, target.y + deltaY));
	}


}

void ScatteredCircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

void ScatteredCircleBrush::DrawCircle(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();

	glBegin(GL_TRIANGLE_FAN);
		SetColor(source);
		glVertex2d(target.x, target.y);
		int sections = 200;
		float size = pDoc->getSize() / 2.0;
		for (int count = 0; count <= 2 * sections; count++){
			glVertex2f(target.x + size * cos(count*M_PI / sections), target.y + size * sin(count*M_PI / sections));
		}

		//reference : http://blog.sina.com.cn/s/blog_858820890100xyop.html
	glEnd();
}
