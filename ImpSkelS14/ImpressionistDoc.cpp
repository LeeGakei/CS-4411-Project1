// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "ImpBrush.h"

// Include individual brush headers here.
#include "PointBrush.h"
#include "LineBrush.h"
#include "CircleBrush.h"
#include "scatteredpointbrush.h"
#include "scatteredlinebrush.h"
#include "scatteredcirclebrush.h"



#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucBitmap		= NULL;
	m_ucPainting	= NULL;
	m_ucGrayBitmap = NULL;


	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES]				
		= new LineBrush( this, "Lines" );
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]				
		= new CircleBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]	
		= new ScatteredPointBrush(this, "Scattered Points");
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]		
		= new ScatteredLineBrush(this, "Scattered Lines");
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]	
		= new ScatteredCircleBrush(this, "Scattered Circles");

	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];

}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
	changeUIState(BRUSH_POINTS);
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
	changeUIState(type);	//the type will be BRUSH_POINTS by default ,added by Jackie Lee
}

//likewise for angle choice
void ImpressionistDoc::setAngleType(int type)
{
	m_pAngleChoice = type;
	if (type == SLIDER)		m_pUI->m_LineAngleSlider->activate();
	else					m_pUI->m_LineAngleSlider->deactivate();
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

int ImpressionistDoc::getEdgeThreshold()
{
	return m_pUI->getEdgeThreshold();
}


//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=readBMP(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	// release old storage
	if ( m_ucBitmap ) delete [] m_ucBitmap;
	if ( m_ucPainting ) delete [] m_ucPainting;
	if (m_ucGrayBitmap) delete[] m_ucGrayBitmap;

	m_ucBitmap		= data;

	// allocate space for draw view
	m_ucPainting	= new unsigned char [width*height*3];
	memset(m_ucPainting, 0, width*height*3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();

	//generate the blurred image
	m_ucGrayBitmap = new unsigned char[m_nPaintWidth * m_nPaintHeight];
	unsigned char *nonFilteredOne = new unsigned char[m_nPaintWidth * m_nPaintHeight];
	for (int i = 0; i < m_nPaintWidth; i++){
		for (int j = 0; j < m_nPaintHeight; j++){
			GLubyte color[3];
			memcpy(color, GetOriginalPixel(Point(i, j)), 3);
			nonFilteredOne[m_nPaintWidth * j + i] = GLubyte(0.299 * color[0] + 0.587 * color[1] + 0.144 * color[2]);
			m_ucGrayBitmap[m_nPaintWidth * j + i] = GLubyte(0.299 * color[0] + 0.587 * color[1] + 0.144 * color[2]);
		}
	}

	//Gaussian filter
	for (int i = 1; i < m_nPaintWidth - 1; i++){
		for (int j = 1; j < m_nPaintHeight - 1; j++){
			m_ucGrayBitmap[m_nPaintWidth * j + i] = GLubyte(1.0 / 16 * nonFilteredOne[m_nPaintWidth * (j - 1) + i - 1] +
				2.0 / 16 * nonFilteredOne[m_nPaintWidth * (j - 1) + i] + 
				1.0 / 16 * nonFilteredOne[m_nPaintWidth * (j - 1) + i + 1] + 
				2.0 / 16 * nonFilteredOne[m_nPaintWidth * (j) + i - 1] + 
				4.0 / 16 * nonFilteredOne[m_nPaintWidth * (j) + i] + 
				2.0 / 16 * nonFilteredOne[m_nPaintWidth * (j) + i + 1] + 
				1.0 / 16 * nonFilteredOne[m_nPaintWidth * (j + 1) + i - 1] + 
				2.0 / 16 * nonFilteredOne[m_nPaintWidth * (j + 1) + i] + 
				1.0 / 16 * nonFilteredOne[m_nPaintWidth * (j + 1) + i + 1]);

		}
	}

	return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) 
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}

GLubyte ImpressionistDoc::GetGrayPixel(int x, int y){
	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return *(m_ucGrayBitmap + (y*m_nWidth + x));
}

//-------------------------------------------------
// Automatically draws the picture
//-------------------------------------------------
void ImpressionistDoc::autodraw()
{
	
	int size = m_pUI->getSize();			//lock a central size
	if (size < 3)	size = 3;
	int width = m_pUI->getLineWidth(); //likewise
	if (width < 3) width = 3;

	//get dimensions of picture
	int sideEdge = m_nPaintWidth;
	int bottomEdge = m_nPaintHeight;

	for (int i = 0; i < sideEdge; i += 5)
	{
		for (int j = 0; j < bottomEdge; j += 5)
		{
			Point current(i, j);
			//slight randomization
			int factor = (rand() % 5) - 2;
			m_pUI->setSize(size + factor);		//may be two more or two less
			m_pUI->setWidth(width + factor);
			m_pCurrentBrush->BrushBegin(current, current);
		}
	}
	m_pUI->setSize(size); m_pUI->setWidth(width);
	m_pUI->m_paintView->SaveCurrentContent();
	m_pUI->m_paintView->RestoreContent();
}

void ImpressionistDoc::GetGradient(Point source, unsigned char &gx, unsigned char &gy){
	unsigned char local[3][3];
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			local[i][j] = GetGrayPixel(source.x + i - 1, source.y + j - 1);
			//printf("%d ", local[i][j]);
		}
	}

	//matrix computing
	gx = unsigned char(1 * local[0][0] + (2) * local[0][1] + (1) * local[0][2] +
		(-1) * local[2][0] + (-2) * local[2][1] + (-1) * local[2][2]);

	gy = unsigned char(1 * local[0][0] + (2) * local[1][0] + (1) * local[2][0] +
		(-1) * local[0][2] + (-2) * local[1][2] + (-1) * local[2][2]);
}//----------------------------------------------------------------
// enable or disable some parts of UI when a specified type is chosen
//----------------------------------------------------------------
void ImpressionistDoc::changeUIState(int type){
	switch (type){
	case BRUSH_POINTS:
	case BRUSH_CIRCLES:
	case BRUSH_SCATTERED_POINTS:
	case BRUSH_SCATTERED_CIRCLES:
		m_pUI->m_AngleTypeChoice->deactivate();
		m_pUI->m_LineWidthSlider->deactivate();
		m_pUI->m_LineAngleSlider->deactivate();
		break;
	case BRUSH_LINES:
	case BRUSH_SCATTERED_LINES:
		m_pUI->m_AngleTypeChoice->activate();
		m_pUI->m_LineWidthSlider->activate();
		if (m_pAngleChoice == SLIDER)		//else it should remain disabled
			m_pUI->m_LineAngleSlider->activate();
		break;
	default:
		printf("error in ImpressionistDoc.ccp changeUIState : Invalid type!");
	}
}

