//Added by Jackie Lee
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "CircleBrush.h"

#include <math.h>


extern float frand();

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void CircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize((float)size);

	BrushMove(source, target);
}

void CircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	glBegin(GL_TRIANGLE_FAN);
		SetColor(source);
		glVertex2d(target.x,target.y);
		int sections = 200;
		float size = pDoc->getSize() / 2.0;
		for (int count = 0; count <= 2 * sections; count++){
			glVertex2f(target.x + size * cos(count*M_PI / sections), target.y + size * sin(count*M_PI / sections));
		}

		//reference : http://blog.sina.com.cn/s/blog_858820890100xyop.html
	glEnd();
}

void CircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
