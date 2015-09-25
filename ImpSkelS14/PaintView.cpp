//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintview.h"
#include "ImpBrush.h"
#include "Math.h" //for tangent ~JT


#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6
#define MOUSE_MOVE			7
#define radtodeg			57.29577951

#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;
static Point	coord;
static int initx, inity; //for right click angle setting

PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l)
						: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;

}

void PaintView::setStrokeAngleByGradient(Point source){
	//printf("computing begin!");
	unsigned char local[3][3];
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			local[i][j] = m_pDoc->GetGrayPixel(source.x + i - 1, source.y + j - 1);
			//printf("%d ", local[i][j]);
		}
	}

	//matrix computing
	unsigned char gx = unsigned char(1 * local[0][0] + (2) * local[0][1] + (1) * local[0][2] +
									-1 * local[2][0] + (-2) * local[2][1] + (-1) * local[2][2]);

	unsigned char gy = unsigned char(1 * local[0][0] + (2) * local[1][0] + (1) * local[2][0] +
									-1 * local[0][2] + (-2) * local[1][2] + (-1) * local[2][2]);

	printf("gx:%d gy:%d\t",gx,gy);
	int angle = (int)atan2f(gy,gx) / M_PI * 180;
	m_pDoc->m_pUI->setAngle(angle);
}


void PaintView::draw()
{
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA

	if(!valid())
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		glClear( GL_COLOR_BUFFER_BIT );
	}

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;

	if ( m_pDoc->m_ucPainting && !isAnEvent) 
	{
		RestoreContent();
	}

	if ( m_pDoc->m_ucPainting && isAnEvent) 
	{

		// Clear it after processing.
		isAnEvent	= 0;	

		Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		Point target( coord.x, m_nWindowHeight - coord.y );
		

		// This is the event handler
		switch (eventToDo) 
		{
		case LEFT_MOUSE_DOWN:
			m_pDoc->m_pCurrentBrush->BrushBegin( source, target );
			initx = target.x;
			inity = target.y;
			break;
		case LEFT_MOUSE_DRAG:
			m_pDoc->m_pCurrentBrush->BrushMove( source, target );
			//only in brush direction mode
			if (m_pDoc->m_pAngleChoice == MOVE){
				//set angle equal to line from previous point
				m_pDoc->m_pUI->setAngle((int)radtodeg*atan(((double)(target.y - inity)) / (target.x - initx)));
				initx = target.x;
				inity = target.y;
			}
			else if (m_pDoc->m_pAngleChoice == GRADIENT){
				setStrokeAngleByGradient(source);
			}

			break;
		case LEFT_MOUSE_UP:
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );

			SaveCurrentContent();
			RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
			initx = target.x;
			inity = target.y;
			if (m_pDoc->m_pAngleChoice == RIGHTCLICK) break; //right mouse button calls autodraw outside RIGHTCLICK mode
			m_pDoc->autodraw();
			break;
		case RIGHT_MOUSE_DRAG:

			break;
		case RIGHT_MOUSE_UP:
			//only in rightclick mode
			if (m_pDoc->m_pAngleChoice != RIGHTCLICK) break;

			if (target.x == initx) break; //no division by zero error
			//set angle equal to line from where you right clicked		
			m_pDoc->m_pUI->setAngle((int) radtodeg*atan(((double)(target.y - inity)) / (target.x - initx)));
			break;
		/*case MOUSE_MOVE: //didn't work
			//show red cursor on original
			glBegin(GL_POINTS);
			glColor3f(1.0, 0, 0);
			glVertex2d(target.x, target.y);
			glEnd();
			break;*/
		default:
			printf("Unknown event!!\n");		
			break;
		}
	}

	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA

}


int PaintView::handle(int event)
{
	switch(event)
	{
	case FL_ENTER:
	    redraw();
		break;
	case FL_PUSH:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DOWN;
		else
			eventToDo=LEFT_MOUSE_DOWN;
		isAnEvent=1;
		redraw();
		break;
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DRAG;
		else
			eventToDo=LEFT_MOUSE_DRAG;
		isAnEvent=1;
		redraw();
		break;
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		redraw();
		break;
	case FL_MOVE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		break;
	default:
		return 0;
		break;

	}



	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent()
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(GL_FRONT);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );
}


void PaintView::RestoreContent()
{
	glDrawBuffer(GL_BACK);

	glClearColor(0.0f,1.0f,0.0f,1.0f);
	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);

//	glDrawBuffer(GL_FRONT);
}
