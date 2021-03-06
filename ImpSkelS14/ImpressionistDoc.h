// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"

class ImpressionistUI;

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *iname);			// called by the UI to load image
	int		saveImage(char *iname);			// called by the UI to save image


	int     clearCanvas();                  // called by the UI to clear the drawing canvas
	void	setBrushType(int type);			// called by the UI to set the brushType
	void    setAngleType(int type);			// called by the UI to set the AngleMethod
	int		getSize();						// get the UI size
	void	setSize(int size);				// set the UI size
	int		getEdgeThreshold();				// get the edge threshold
	char*	getImageName();					// get the current image name
	

// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	
	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;
	unsigned char*	m_ucPainting;

	// Bitmaps for blurred original image
	unsigned char*	m_ucGrayBitmap;


	// The current active brush.
	ImpBrush*			m_pCurrentBrush;	
	// The current angle selection method. Initialized to zero
	int					m_pAngleChoice =0;
	// Size of the brush.
	int m_nSize;	

	ImpressionistUI*	m_pUI;

// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );  

	GLubyte GetGrayPixel(int x, int y);

	void autodraw();
	void GetGradient(Point source, unsigned char &gx, unsigned char &gy);

private:
	char			m_imageName[256];

	//called when brush is chosen, added by Jackie Lee
	void	changeUIState(int type);	
};

extern void MessageBox(char *message);

#endif
