#ifndef SCATTERED_LINE_BRUSH_H
#define SCATTERED_LINE_BRUSH_H

#include "ImpBrush.h"

class ScatteredLineBrush : public ImpBrush
{
public:
	ScatteredLineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void DrawLine(const Point source, const Point target, const double length);
	char* BrushName(void);
};

#endif