//
// originalview.cpp
//
// The code maintaining the original view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "originalview.h"

#include <cmath>

#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#endif

OriginalView::OriginalView(int			x, 
						   int			y, 
						   int			w, 
						   int			h, 
						   const char*	l)
							: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;

	edgeBit = NULL;	//if this is not null, the originalView will prefer to show this one other than original image
}

void OriginalView::draw()
{
	if(!valid())
	{
		glClearColor(0.7f, 0.7f, 0.7f, 0.0);
		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		// Tell openGL to read from the front buffer when capturing
		// out paint strokes 
		glReadBuffer( GL_FRONT );
		ortho();

	}

	glClear( GL_COLOR_BUFFER_BIT );

	if (edgeBit){
		int drawWidth, drawHeight;
		drawWidth = min(m_nWindowWidth, m_pDoc->m_nWidth);
		drawHeight = min(m_nWindowHeight, m_pDoc->m_nHeight);
		glRasterPos2i(0, m_nWindowHeight - drawHeight);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, m_pDoc->m_nWidth);
		glDrawBuffer(GL_BACK);
		glDrawPixels(drawWidth, drawHeight, GL_RGB, GL_UNSIGNED_BYTE, edgeBit);
		printf("%d\n", drawWidth);
		printf("%d\n", drawHeight);
		printf("I am really showing!");
	}
	else if ( m_pDoc->m_ucBitmap ) 
	{
		// note that both OpenGL pixel storage and the Windows BMP format
		// store pixels left-to-right, BOTTOM-to-TOP!!  thus all the fiddling
		// around with startrow.

		m_nWindowWidth=w();
		m_nWindowHeight=h();

		int drawWidth, drawHeight;
		GLvoid* bitstart;

		// we are not using a scrollable window, so ignore it
		Point scrollpos;	// = GetScrollPosition();
		scrollpos.x=scrollpos.y=0;

		drawWidth	= min( m_nWindowWidth, m_pDoc->m_nWidth );
		drawHeight	= min( m_nWindowHeight, m_pDoc->m_nHeight );

		int	startrow	= m_pDoc->m_nHeight - (scrollpos.y + drawHeight);
		if ( startrow < 0 ) 
			startrow = 0;


		bitstart = m_pDoc->m_ucBitmap + 3 * ((m_pDoc->m_nWidth * startrow) + scrollpos.x);

		// just copy image to GLwindow conceptually
		glRasterPos2i( 0, m_nWindowHeight - drawHeight );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nWidth );
		glDrawBuffer( GL_BACK );
		glDrawPixels( drawWidth, drawHeight, GL_RGB, GL_UNSIGNED_BYTE, bitstart );

	}
			
	glFlush();
}

void OriginalView::refresh()
{
	redraw();
}

void OriginalView::resizeWindow(int	width, 
								int	height)
{
	resize(x(), y(), width, height);
}


// called by UI when "do it" button is pressed!
void OriginalView::showEdge(){
	edgeBit = new GLbyte[m_pDoc->m_nWidth * m_pDoc->m_nHeight * 3];
	int edgeThreshold = m_pDoc->getEdgeThreshold();
	printf("%d\n", m_pDoc->m_nWidth);
	printf("%d\n", m_pDoc->m_nHeight);

	for (int i = 0; i < m_pDoc->m_nWidth; i++){
		for (int j = 0; j < m_pDoc->m_nHeight; j++){
			unsigned char gx, gy;
			m_pDoc->GetGradient(Point(i,j),gx,gy);
			unsigned char g = sqrt(1.0 * gx * gx + 1.0 * gy * gy);
			if (g > edgeThreshold){
				edgeBit[3 * (m_pDoc->m_nWidth * j + i)] = 255;
				edgeBit[3 * (m_pDoc->m_nWidth * j + i) + 1] = 255;
				edgeBit[3 * (m_pDoc->m_nWidth * j + i) + 2] = 255;
			}
			else{
				edgeBit[3 * (m_pDoc->m_nWidth * j + i)] = 0;
				edgeBit[3 * (m_pDoc->m_nWidth * j + i) + 1] = 0;
				edgeBit[3 * (m_pDoc->m_nWidth * j + i) + 2] = 0;
			}
		}
	}

	printf("I am drawing!");
	redraw();
}


