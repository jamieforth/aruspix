//------------------------------------------------------------------------------
// Header generated by wxDesigner from file: rec.wdr
// Do not modify this file, all changes will be lost!
//------------------------------------------------------------------------------

#ifndef __WDR_rec_H__
#define __WDR_rec_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "rec_wdr.h"
#endif

// Include wxWidgets' headers

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/image.h>
#include <wx/statline.h>
#include <wx/spinbutt.h>
#include <wx/spinctrl.h>
#include <wx/splitter.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/notebook.h>
#include <wx/grid.h>
#include <wx/toolbar.h>
#include <wx/tglbtn.h>

// Declare window functions

const int ID4_SPLITTER1 = 15000;
wxSizer *WindowFunc4( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

const int ID4_TOOLPANEL = 15001;
const int ID4_MUSPANEL = 15002;
wxSizer *MusOutputFunc4( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

const int ID4_TREEBOOK = 15003;
const int ID4_LINE2 = 15004;
const int ID4_PREVIEW = 15005;
const int ID4_CB_PREVIEW = 15006;
wxSizer *RecBookFunc4( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

const int ID4_TEXT = 15007;
const int ID4_RISM = 15008;
const int ID4_COMPOSER = 15009;
const int ID4_TITLE = 15010;
const int ID4_PRINTER = 15011;
const int ID4_YEAR = 15012;
const int ID4_LIBRARY = 15013;
const int ID4_BOOK_IMAGES = 15014;
const int ID4_ON_BOOK_IMAGES = 15015;
const int ID4_BOOK_AXFILES = 15016;
const int ID4_ON_BOOK_AXFILES = 15017;
const int ID4_PAGE_BIN_CHOICE = 15018;
const int ID4_RGN_SIZE_SPINCTRL = 15019;
const int ID4_BIN_SELECT_CHECKBOX = 15020;
wxSizer *RecBookDataFunc4( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

// Declare menubar functions

const int ID4_MENU_FILE = 15021;
const int ID4_MENU0 = 15022;
const int ID4_NEW_BOOK = 15023;
const int ID4_OPEN_BOOK = 15024;
const int ID4_CLOSE_BOOK = 15025;
const int ID4_SAVE_BOOK = 15026;
const int ID4_SAVE_AS_BOOK = 15027;
const int ID4_EXPORT_IMAGE = 15028;
const int ID4_EXPORT_AXTYP = 15029;
const int ID4_EXPORT_AXMUS = 15030;
const int ID4_MENU_EDIT = 15031;
const int ID4_MENU = 15032;
const int ID4_PROCESS = 15033;
const int ID4_CANCEL_REC = 15034;
const int ID4_BOOK = 15035;
const int ID4_BOOK_EDIT = 15036;
const int ID4_BOOK_LOAD = 15037;
const int ID4_BOOK_PRE = 15038;
const int ID4_BOOK_REC = 15039;
const int ID4_BOOK_ADAPT = 15040;
const int ID4_BOOK_RESET_ADAPT = 15041;
const int ID4_PAGE = 15042;
const int ID4_ZOOM_OUT = 15043;
const int ID4_ZOOM_IN = 15044;
const int ID4_ADJUST = 15045;
const int ID4_ADJUST_V = 15046;
const int ID4_ADJUST_H = 15047;
const int ID4_TOOLS = 15048;
const int ID4_SHOW_STAFF_BMP = 15049;
const int ID4_INSERT_MODE = 15050;
const int ID4_NOTES = 15051;
const int ID4_KEYS = 15052;
const int ID4_SIGNS = 15053;
const int ID4_SYMBOLES = 15054;
wxMenuBar *MenuBarFunc4();

// Declare toolbar functions

const int ID4_BATCH = 15055;
void ToolBarFunc4( wxToolBar *parent );

// Declare bitmap functions

wxBitmap BitmapsFunc4( size_t index );

wxBitmap BitmapsFuncClassification( size_t index );

#endif

// End of generated file
