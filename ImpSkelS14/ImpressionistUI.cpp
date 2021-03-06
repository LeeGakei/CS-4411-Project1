//
// impressionistUI.h
//
// The user interface part for the program.
//


#include <FL/fl_ask.h>

#include <math.h>
#include <iostream>

#include "impressionistUI.h"
#include "impressionistDoc.h"

using namespace std;

/*
//------------------------------ Widget Examples -------------------------------------------------
Here is some example code for all of the widgets that you may need to add to the 
project.  You can copy and paste these into your code and then change them to 
make them look how you want.  Descriptions for all of the widgets here can be found 
in links on the fltk help session page.

//---------Window/Dialog and Menubar-----------------------------------
	
	//----To install a window--------------------------
	Fl_Window* myWindow = new Fl_Window(600, 300, "MyWindow");
		myWindow->user_data((void*)(this));	// record self to be used by static callback functions
		
		// install menu bar
		myMenubar = new Fl_Menu_Bar(0, 0, 600, 25);
		Fl_Menu_Item ImpressionistUI::myMenuItems[] = {
			{ "&File",		0, 0, 0, FL_SUBMENU },
				{ "&Load...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load },
				{ "&Save...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save }.
				{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
				{ 0 },
			{ "&Edit",		0, 0, 0, FL_SUBMENU },
				{ "&Copy",FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_copy, (void *)COPY },
				{ "&Cut",	FL_ALT + 'x', (Fl_Callback *)ImpressionistUI::cb_cut, (void *)CUT },
				{ "&Paste",	FL_ALT + 'v', (Fl_Callback *)ImpressionistUI::cb_paste, (void *)PASTE },
				{ 0 },
			{ "&Help",		0, 0, 0, FL_SUBMENU },
				{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
				{ 0 },
			{ 0 }
		};
		myMenubar->menu(myMenuItems);
    m_mainWindow->end();

	//----The window callback--------------------------
	// One of the callbacks
	void ImpressionistUI::cb_load(Fl_Menu_* o, void* v) 
	{	
		ImpressionistDoc *pDoc=whoami(o)->getDocument();

		char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
		if (newfile != NULL) {
			pDoc->loadImage(newfile);
		}
	}


//------------Slider---------------------------------------

	//----To install a slider--------------------------
	Fl_Value_Slider * mySlider = new Fl_Value_Slider(10, 80, 300, 20, "My Value");
	mySlider->user_data((void*)(this));	// record self to be used by static callback functions
	mySlider->type(FL_HOR_NICE_SLIDER);
    mySlider->labelfont(FL_COURIER);
    mySlider->labelsize(12);
	mySlider->minimum(1);
	mySlider->maximum(40);
	mySlider->step(1);
	mySlider->value(m_nMyValue);
	mySlider->align(FL_ALIGN_RIGHT);
	mySlider->callback(cb_MyValueSlides);

	//----The slider callback--------------------------
	void ImpressionistUI::cb_MyValueSlides(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyValue=int( ((Fl_Slider *)o)->value() ) ;
	}
	

//------------Choice---------------------------------------
	
	//----To install a choice--------------------------
	Fl_Choice * myChoice = new Fl_Choice(50,10,150,25,"&myChoiceLabel");
	myChoice->user_data((void*)(this));	 // record self to be used by static callback functions
	Fl_Menu_Item ImpressionistUI::myChoiceMenu[3+1] = {
	  {"one",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)ONE},
	  {"two",FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)TWO},
	  {"three",FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)THREE},
	  {0}
	};
	myChoice->menu(myChoiceMenu);
	myChoice->callback(cb_myChoice);
	
	//-----The choice callback-------------------------
	void ImpressionistUI::cb_myChoice(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
		ImpressionistDoc* pDoc=pUI->getDocument();

		int type=(int)v;

		pDoc->setMyType(type);
	}


//------------Button---------------------------------------

	//---To install a button---------------------------
	Fl_Button* myButton = new Fl_Button(330,220,50,20,"&myButtonLabel");
	myButton->user_data((void*)(this));   // record self to be used by static callback functions
	myButton->callback(cb_myButton);

	//---The button callback---------------------------
	void ImpressionistUI::cb_myButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI*)(o->user_data()));
		ImpressionistDoc* pDoc = pUI->getDocument();
		pDoc->startPainting();
	}


//---------Light Button------------------------------------
	
	//---To install a light button---------------------
	Fl_Light_Button* myLightButton = new Fl_Light_Button(240,10,150,25,"&myLightButtonLabel");
	myLightButton->user_data((void*)(this));   // record self to be used by static callback functions
	myLightButton->callback(cb_myLightButton);

	//---The light button callback---------------------
	void ImpressionistUI::cb_myLightButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI *pUI=((ImpressionistUI*)(o->user_data()));

		if (pUI->myBool==TRUE) pUI->myBool=FALSE;
		else pUI->myBool=TRUE;
	}

//----------Int Input--------------------------------------

    //---To install an int input-----------------------
	Fl_Int_Input* myInput = new Fl_Int_Input(200, 50, 5, 5, "&My Input");
	myInput->user_data((void*)(this));   // record self to be used by static callback functions
	myInput->callback(cb_myInput);

	//---The int input callback------------------------
	void ImpressionistUI::cb_myInput(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyInputValue=int( ((Fl_Int_Input *)o)->value() );
	}

//------------------------------------------------------------------------------------------------
*/

//------------------------------------- Help Functions --------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI* ImpressionistUI::whoami(Fl_Menu_* o)	
{
	return ( (ImpressionistUI*)(o->parent()->user_data()) );
}


//--------------------------------- Callback Functions --------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
	if (newfile != NULL) {
		pDoc->loadImage(newfile);
	}
}


//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Save File?", "*.bmp", "save.bmp" );
	if (newfile != NULL) {
		pDoc->saveImage(newfile);
	}
}

//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_brushDialog->show();
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc=whoami(o)->getDocument();

	pDoc->clearCanvas();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_mainWindow->hide();
	whoami(o)->m_brushDialog->hide();

}



//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("Impressionist FLTK version for CS341, Spring 2002");
}

//------- UI should keep track of the current for all the controls for answering the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush 
// choice.  
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc=pUI->getDocument();

	int type=(int)v;


	pDoc->setBrushType(type);
}

//likewise for angle selection
void ImpressionistUI::cb_angleChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	int type = (int)v;


	pDoc->setAngleType(type);
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->clearCanvas();
}

//show the edge in the original view
void ImpressionistUI::cb_draw_edge_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();
	pDoc->m_pUI->m_origView->showEdge();
	//printf("show the edge now!");
}


//-----------------------------------------------------------
// Updates the brush size, angle, etc to use from the value of
// the slider
// Called by the UI when the slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeSlides(Fl_Widget* o, void* v) //change size
{
	((ImpressionistUI*)(o->user_data()))->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
}

void ImpressionistUI::cb_lineWidthSlides(Fl_Widget* o, void* v) //change width (LineBrush)
{
	((ImpressionistUI*)(o->user_data()))->m_lineWidth = int(((Fl_Slider *)o)->value());
	//printf("line Width Changing");
}

void ImpressionistUI::cb_lineAngleSlides(Fl_Widget* o, void* v) //change angle (LineBrush)
{
	((ImpressionistUI*)(o->user_data()))->m_lineAngle = int(((Fl_Slider *)o)->value());
	//printf("line Angle Changing");
	//printf("%d", ((ImpressionistUI*)(o->user_data()))->m_lineAngle);
}

void ImpressionistUI::cb_alphaSlides(Fl_Widget* o, void* v) //change alpha, also enables alpha blending ~JT
{
	((ImpressionistUI*)(o->user_data()))->m_nAlpha = double(((Fl_Slider *)o)->value());
	//printf("Alpha Changing");
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//printf("%e", double(((Fl_Slider *)o)->value()));
	//cout << ((ImpressionistUI*)(o->user_data()))->m_nAlpha;
}

void ImpressionistUI::cb_auto(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();
	pDoc->autodraw();
}

void ImpressionistUI::cb_edgeThresholdSlides(Fl_Widget* o, void* v) //change alpha, also enables alpha blending ~JT
{
	((ImpressionistUI*)(o->user_data()))->m_edgeThreshold = int(((Fl_Slider *)o)->value());
	printf("%d", int(((Fl_Slider *)o)->value()));
}

//---------------------------------- per instance functions --------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
ImpressionistDoc* ImpressionistUI::getDocument()
{
	return m_pDoc;
}

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show() {
	m_mainWindow->show();
	m_paintView->show();
	m_origView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to 
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h) {
	m_paintView->size(w,h);
	m_origView->size(w,h);
}

//------------------------------------------------ 
// Set the ImpressionistDoc used by the UI to 
// communicate with the brushes 
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc* doc)
{
	m_pDoc = doc;

	m_origView->m_pDoc = doc;
	m_paintView->m_pDoc = doc;
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getSize()
{
	return m_nSize;
}

int ImpressionistUI::getLineWidth()
{
	return m_lineWidth;
}

int ImpressionistUI::getLineAngle()
{
	return m_lineAngle;
}

double ImpressionistUI::getAlpha()
{
	return m_nAlpha;
}

int	ImpressionistUI::getEdgeThreshold(){
	return m_edgeThreshold;
}



//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize( int size )
{
	m_nSize=size;

	if (size<=40) 
		m_BrushSizeSlider->value(m_nSize);
}

//-------------------------------------------------
// Set width for LineBrush
//-=-----------------------------------------------
void ImpressionistUI::setWidth(int width)
{
	m_lineWidth = width;

	if (width <= 40)
		m_LineWidthSlider->value(m_lineWidth);

}

//-------------------------------------------------
// Set angle for LineBrush
//-------------------------------------------------
void ImpressionistUI::setAngle(int angle)
{
	m_lineAngle = angle;
}


// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Image...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load_image },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image },
		{ "&Brushes...",	FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes }, 
		{ "&Clear Canvas", FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_clear_canvas, 0, FL_MENU_DIVIDER },
		
		{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
		{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
		{ 0 },

	{ 0 }
};

// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE+1] = {
  {"Points",			FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_POINTS},
  {"Lines",				FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_LINES},
  {"Circles",			FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_CIRCLES},
  {"Scattered Points",	FL_ALT+'q', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_POINTS},
  {"Scattered Lines",	FL_ALT+'m', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_LINES},
  {"Scattered Circles",	FL_ALT+'d', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_CIRCLES},
  {0}
};

//Angle choice menu definition
Fl_Menu_Item ImpressionistUI::angleTypeMenu[NUM_ANGLE_TYPE+1] = {
	{ "Sliders", FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_angleChoice, (void *)SLIDER },
	{ "Right click", FL_ALT + 'r', (Fl_Callback *)ImpressionistUI::cb_angleChoice, (void *)RIGHTCLICK },
	{ "Brush direction", FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_angleChoice, (void *)MOVE },
	{ "Gradient", FL_ALT + 'g', (Fl_Callback *)ImpressionistUI::cb_angleChoice, (void *)GRADIENT },
	{0}
};


//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI() {
	// Create the main window
	m_mainWindow = new Fl_Window(600, 300, "Impressionist");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
		m_menubar->menu(menuitems);

		// Create a group that will hold two sub windows inside the main
		// window
		Fl_Group* group = new Fl_Group(0, 25, 600, 275);

			// install paint view window
			m_paintView = new PaintView(300, 25, 300, 275, "This is the paint view");//0jon
			m_paintView->box(FL_DOWN_FRAME);

			// install original view window
			m_origView = new OriginalView(0, 25, 300, 275, "This is the orig view");//300jon
			m_origView->box(FL_DOWN_FRAME);
			m_origView->deactivate();

		group->end();
		Fl_Group::current()->resizable(group);
    m_mainWindow->end();

	// init values

	m_nSize = 10;
	m_lineWidth = 1;
	m_lineAngle = 0;
	m_nAlpha = 1.00;

	// brush dialog definition
	m_brushDialog = new Fl_Window(410, 325, "Brush Dialog");
		// Add a brush type choice to the dialog
		m_BrushTypeChoice = new Fl_Choice(50,10,150,25,"&Brush");
		m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushTypeChoice->menu(brushTypeMenu);
		m_BrushTypeChoice->callback(cb_brushChoice);

		m_ClearCanvasButton = new Fl_Button(240,10,150,25,"&Clear Canvas");
		m_ClearCanvasButton->user_data((void*)(this));
		m_ClearCanvasButton->callback(cb_clear_canvas_button);

		//LineBrush Angle selection button
		m_AngleTypeChoice = new Fl_Choice(50, 50, 150, 25, "&AngleMethod");
		m_AngleTypeChoice->user_data((void*)(this));
		m_AngleTypeChoice->menu(angleTypeMenu);

		// Add brush size slider to the dialog 
		m_BrushSizeSlider = new Fl_Value_Slider(10, 80, 300, 20, "Size");
		m_BrushSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
        m_BrushSizeSlider->labelfont(FL_COURIER);
        m_BrushSizeSlider->labelsize(12);
		m_BrushSizeSlider->minimum(1);
		m_BrushSizeSlider->maximum(40);
		m_BrushSizeSlider->step(1);
		m_BrushSizeSlider->value(m_nSize);
		m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
		m_BrushSizeSlider->callback(cb_sizeSlides);

		{
			// Add Line Width slider to the dialog 
			m_LineWidthSlider = new Fl_Value_Slider(10, 110, 300, 20, "Line Width");
			m_LineWidthSlider->user_data((void*)(this));	// record self to be used by static callback functions
			m_LineWidthSlider->type(FL_HOR_NICE_SLIDER);
			m_LineWidthSlider->labelfont(FL_COURIER);
			m_LineWidthSlider->labelsize(12);
			m_LineWidthSlider->minimum(1);
			m_LineWidthSlider->maximum(40);
			m_LineWidthSlider->step(1);
			m_LineWidthSlider->value(m_lineWidth);
			m_LineWidthSlider->align(FL_ALIGN_RIGHT);
			m_LineWidthSlider->callback(cb_lineWidthSlides);

			// Add Line Angle slider to the dialog 
			m_LineAngleSlider = new Fl_Value_Slider(10, 140, 300, 20, "Line Angle");
			m_LineAngleSlider->user_data((void*)(this));	// record self to be used by static callback functions
			m_LineAngleSlider->type(FL_HOR_NICE_SLIDER);
			m_LineAngleSlider->labelfont(FL_COURIER);
			m_LineAngleSlider->labelsize(12);
			m_LineAngleSlider->minimum(1);
			m_LineAngleSlider->maximum(359);
			m_LineAngleSlider->step(1);
			m_LineAngleSlider->value(m_lineAngle);
			m_LineAngleSlider->align(FL_ALIGN_RIGHT);
			m_LineAngleSlider->callback(cb_lineAngleSlides);

			// Add Brush Alpha slider to the dialog 
			m_BrushAlphaSlider = new Fl_Value_Slider(10, 170, 300, 20, "Brush Alpha");
			m_BrushAlphaSlider->user_data((void*)(this));	// record self to be used by static callback functions
			m_BrushAlphaSlider->type(FL_HOR_NICE_SLIDER);
			m_BrushAlphaSlider->labelfont(FL_COURIER);
			m_BrushAlphaSlider->labelsize(12);
			m_BrushAlphaSlider->minimum(0);
			m_BrushAlphaSlider->maximum(1);
			m_BrushAlphaSlider->step(0.01);
			m_BrushAlphaSlider->value(m_nAlpha);
			m_BrushAlphaSlider->align(FL_ALIGN_RIGHT);
			m_BrushAlphaSlider->callback(cb_alphaSlides);

			// Add Brush edge threshold slider to the dialog 
			m_BrushAlphaSlider = new Fl_Value_Slider(10, 200, 200, 20, "Edge Threshold");
			m_BrushAlphaSlider->user_data((void*)(this));	// record self to be used by static callback functions
			m_BrushAlphaSlider->type(FL_HOR_NICE_SLIDER);
			m_BrushAlphaSlider->labelfont(FL_COURIER);
			m_BrushAlphaSlider->labelsize(12);
			m_BrushAlphaSlider->minimum(0);
			m_BrushAlphaSlider->maximum(500);
			m_BrushAlphaSlider->step(1);
			m_BrushAlphaSlider->value(m_nAlpha);
			m_BrushAlphaSlider->align(FL_ALIGN_RIGHT);
			m_BrushAlphaSlider->callback(cb_edgeThresholdSlides);

			//Add Show edge button to the dialog
			m_ShowEdgeButton = new Fl_Button(320, 200, 70, 20, "Do it");
			m_ShowEdgeButton->user_data((void*)(this));
			m_ShowEdgeButton->callback(cb_draw_edge_button);
		}

		//autodraw button //didn't work
		/*Fl_Button* autodraw = new Fl_Button(330, 220, 50, 20, "Paint Canvas");
		autodraw->user_data((void*)(this));   // record self to be used by static callback functions
		autodraw->callback(cb_auto);*/

    m_brushDialog->end();	

}
