//------------------------------------------------------------------------------
// Source code generated by wxDesigner from file: sup.wdr
// Do not modify this file, all changes will be lost!
//------------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "sup_wdr.h"
#endif

// For compilers that support precompilation
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// Include private header
#include "sup_wdr.h"

#include <wx/intl.h>

// Euro sign hack of the year
#if wxUSE_UNICODE
    #define __WDR_EURO__ wxT("\u20ac")
#else
    #if defined(__WXMAC__)
        #define __WDR_EURO__ wxT("\xdb")
    #elif defined(__WXMSW__)
        #define __WDR_EURO__ wxT("\x80")
    #else
        #define __WDR_EURO__ wxT("\xa4")
    #endif
#endif

// Custom source
#include "superimposition/supbookctrl.h"

// Implement window functions

wxSizer *WindowFunc2( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxFlexGridSizer *item0 = new wxFlexGridSizer( 1, 0, 0 );
    item0->AddGrowableCol( 0 );
    item0->AddGrowableRow( 0 );

    wxSplitterWindow *item1 = new wxSplitterWindow( parent, ID2_SPLITTER1, wxDefaultPosition, wxDefaultSize, 0 );
    item0->Add( item1, 0, wxGROW|wxALL, 0 );

    if (set_sizer)
    {
        parent->SetSizer( item0 );
        if (call_fit)
            item0->SetSizeHints( parent );
    }
    
    return item0;
}

wxSizer *SupBookFunc2( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxFlexGridSizer *item0 = new wxFlexGridSizer( 1, 0, 0 );
    item0->AddGrowableCol( 0 );
    item0->AddGrowableRow( 0 );

    SupBookCtrl *item1 = new SupBookCtrl( parent, ID2_TREEBOOK, wxDefaultPosition, wxSize(120,160), wxTR_HAS_BUTTONS|wxTR_LINES_AT_ROOT|wxSUNKEN_BORDER );
    item0->Add( item1, 0, wxGROW|wxALL, 5 );

    wxStaticLine *item2 = new wxStaticLine( parent, ID2_LINE2, wxDefaultPosition, wxSize(20,-1), wxLI_HORIZONTAL );
    item0->Add( item2, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticBitmap *item3 = new wxStaticBitmap( parent, ID2_PREVIEW, BitmapsFunc2( 0 ), wxDefaultPosition, wxSize(200,200) );
    item0->Add( item3, 0, wxALIGN_CENTER|wxALL, 5 );

    wxCheckBox *item4 = new wxCheckBox( parent, ID2_CB_PREVIEW, _("Preview"), wxDefaultPosition, wxDefaultSize, 0 );
    item0->Add( item4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    if (set_sizer)
    {
        parent->SetSizer( item0 );
        if (call_fit)
            item0->SetSizeHints( parent );
    }
    
    return item0;
}

wxSizer *SupBookDataFunc2( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxFlexGridSizer *item0 = new wxFlexGridSizer( 1, 0, 0 );
    item0->AddGrowableCol( 0 );
    item0->AddGrowableCol( 1 );
    item0->AddGrowableRow( 0 );

    wxStaticBox *item2 = new wxStaticBox( parent, -1, _("Book information") );
    wxStaticBoxSizer *item1 = new wxStaticBoxSizer( item2, wxVERTICAL );

    wxFlexGridSizer *item3 = new wxFlexGridSizer( 2, 0, 0 );

    wxStaticText *item4 = new wxStaticText( parent, ID2_TEXT, _("RISM"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item4, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item5 = new wxTextCtrl( parent, ID2_RISM, wxT(""), wxDefaultPosition, wxSize(300,-1), 0 );
    item3->Add( item5, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticText *item6 = new wxStaticText( parent, ID2_TEXT, _("Composer"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item6, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item7 = new wxTextCtrl( parent, ID2_COMPOSER, wxT(""), wxDefaultPosition, wxSize(300,-1), 0 );
    item3->Add( item7, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticText *item8 = new wxStaticText( parent, ID2_TEXT, _("Title"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item8, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item9 = new wxTextCtrl( parent, ID2_TITLE, wxT(""), wxDefaultPosition, wxSize(300,-1), 0 );
    item3->Add( item9, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticText *item10 = new wxStaticText( parent, ID2_TEXT, _("Printer"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item10, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item11 = new wxTextCtrl( parent, ID2_PRINTER, wxT(""), wxDefaultPosition, wxSize(300,-1), 0 );
    item3->Add( item11, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticText *item12 = new wxStaticText( parent, ID2_TEXT, _("Year"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item12, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item13 = new wxTextCtrl( parent, ID2_YEAR, wxT(""), wxDefaultPosition, wxSize(300,-1), 0 );
    item3->Add( item13, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticText *item14 = new wxStaticText( parent, ID2_TEXT, _("Library 1"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item14, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item15 = new wxTextCtrl( parent, ID2_LIBRARY1, wxT(""), wxDefaultPosition, wxSize(300,-1), 0 );
    item3->Add( item15, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticText *item16 = new wxStaticText( parent, ID2_TEXT, _("Library 2"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item16, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item17 = new wxTextCtrl( parent, ID2_LIBRARY2, wxT(""), wxDefaultPosition, wxSize(300,-1), 0 );
    item3->Add( item17, 0, wxALIGN_CENTER|wxALL, 5 );

    item1->Add( item3, 0, wxALIGN_CENTER, 5 );

    item0->Add( item1, 0, wxGROW|wxALL, 5 );

    wxStaticBox *item19 = new wxStaticBox( parent, -1, _("File directories") );
    wxStaticBoxSizer *item18 = new wxStaticBoxSizer( item19, wxVERTICAL );

    wxStaticText *item20 = new wxStaticText( parent, ID2_TEXT, _("Images 1"), wxDefaultPosition, wxDefaultSize, 0 );
    item18->Add( item20, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );

    wxFlexGridSizer *item21 = new wxFlexGridSizer( 2, 0, 0 );

    wxBoxSizer *item22 = new wxBoxSizer( wxHORIZONTAL );

    wxTextCtrl *item23 = new wxTextCtrl( parent, ID2_BOOK_IMAGES1, wxT(""), wxDefaultPosition, wxSize(300,-1), 0 );
    item22->Add( item23, 0, wxALIGN_CENTER|wxALL, 5 );

    wxButton *item24 = new wxButton( parent, ID2_ON_BOOK_IMAGES1, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
    item22->Add( item24, 0, wxALIGN_CENTER|wxALL, 5 );

    item21->Add( item22, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0 );

    item18->Add( item21, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0 );

    wxStaticText *item25 = new wxStaticText( parent, ID2_TEXT, _("Images 2"), wxDefaultPosition, wxDefaultSize, 0 );
    item18->Add( item25, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );

    wxFlexGridSizer *item26 = new wxFlexGridSizer( 2, 0, 0 );

    wxBoxSizer *item27 = new wxBoxSizer( wxHORIZONTAL );

    wxTextCtrl *item28 = new wxTextCtrl( parent, ID2_BOOK_IMAGES2, wxT(""), wxDefaultPosition, wxSize(300,-1), 0 );
    item27->Add( item28, 0, wxALIGN_CENTER|wxALL, 5 );

    wxButton *item29 = new wxButton( parent, ID2_ON_BOOK_IMAGES2, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
    item27->Add( item29, 0, wxALIGN_CENTER|wxALL, 5 );

    item26->Add( item27, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0 );

    item18->Add( item26, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0 );

    wxStaticText *item30 = new wxStaticText( parent, ID2_TEXT, _("Aruspix files"), wxDefaultPosition, wxDefaultSize, 0 );
    item18->Add( item30, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );

    wxFlexGridSizer *item31 = new wxFlexGridSizer( 2, 0, 0 );

    wxBoxSizer *item32 = new wxBoxSizer( wxHORIZONTAL );

    wxTextCtrl *item33 = new wxTextCtrl( parent, ID2_BOOK_AXFILES, wxT(""), wxDefaultPosition, wxSize(300,-1), 0 );
    item32->Add( item33, 0, wxALIGN_CENTER|wxALL, 5 );

    wxButton *item34 = new wxButton( parent, ID2_ON_BOOK_AXFILES, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
    item32->Add( item34, 0, wxALIGN_CENTER|wxALL, 5 );

    item31->Add( item32, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0 );

    item18->Add( item31, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0 );

    item0->Add( item18, 0, wxGROW|wxALL, 5 );

    wxBoxSizer *item35 = new wxBoxSizer( wxHORIZONTAL );

    wxButton *item36 = new wxButton( parent, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    item35->Add( item36, 0, wxALIGN_CENTER|wxALL, 5 );

    wxButton *item37 = new wxButton( parent, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    item35->Add( item37, 0, wxALIGN_CENTER|wxALL, 5 );

    item0->Add( item35, 0, wxALIGN_CENTER|wxALL, 5 );

    if (set_sizer)
    {
        parent->SetSizer( item0 );
        if (call_fit)
            item0->SetSizeHints( parent );
    }
    
    return item0;
}

wxSizer *SupAdjustDlgFunc2( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    wxFlexGridSizer *item1 = new wxFlexGridSizer( 2, 0, 0 );

    wxStaticText *item2 = new wxStaticText( parent, ID_TEXT2, _("Source 1 brightness"), wxDefaultPosition, wxDefaultSize, 0 );
    item1->Add( item2, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxSlider *item3 = new wxSlider( parent, ID2_SRC1_B, 0, -5, 5, wxDefaultPosition, wxSize(150,-1), wxSL_AUTOTICKS );
    item1->Add( item3, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticText *item4 = new wxStaticText( parent, ID_TEXT2, _("Source 1 contrast"), wxDefaultPosition, wxDefaultSize, 0 );
    item1->Add( item4, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxSlider *item5 = new wxSlider( parent, ID2_SRC1_C, 0, -5, 5, wxDefaultPosition, wxSize(150,-1), wxSL_AUTOTICKS );
    item1->Add( item5, 0, wxALIGN_CENTER|wxALL, 5 );

    item0->Add( item1, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticLine *item6 = new wxStaticLine( parent, ID_LINE2, wxDefaultPosition, wxSize(20,-1), wxLI_HORIZONTAL );
    item0->Add( item6, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxFlexGridSizer *item7 = new wxFlexGridSizer( 2, 0, 0 );

    wxStaticText *item8 = new wxStaticText( parent, ID_TEXT2, _("Source 2 brightness"), wxDefaultPosition, wxDefaultSize, 0 );
    item7->Add( item8, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxSlider *item9 = new wxSlider( parent, ID2_SRC2_B, 0, -5, 5, wxDefaultPosition, wxSize(150,-1), wxSL_AUTOTICKS );
    item7->Add( item9, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticText *item10 = new wxStaticText( parent, ID_TEXT2, _("Source 2 contrast"), wxDefaultPosition, wxDefaultSize, 0 );
    item7->Add( item10, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxSlider *item11 = new wxSlider( parent, ID2_SRC2_C, 0, -5, 5, wxDefaultPosition, wxSize(150,-1), wxSL_AUTOTICKS );
    item7->Add( item11, 0, wxALIGN_CENTER|wxALL, 5 );

    item0->Add( item7, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticLine *item12 = new wxStaticLine( parent, ID_LINE2, wxDefaultPosition, wxSize(20,-1), wxLI_HORIZONTAL );
    item0->Add( item12, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxBoxSizer *item13 = new wxBoxSizer( wxHORIZONTAL );

    wxButton *item14 = new wxButton( parent, ID2_RESET_ADJ, _("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
    item13->Add( item14, 0, wxALIGN_CENTER|wxALL, 5 );

    wxButton *item15 = new wxButton( parent, wxID_OK, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
    item15->SetDefault();
    item13->Add( item15, 0, wxALIGN_CENTER|wxALL, 5 );

    wxButton *item16 = new wxButton( parent, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    item13->Add( item16, 0, wxALIGN_CENTER|wxALL, 5 );

    item0->Add( item13, 0, wxALIGN_CENTER|wxLEFT|wxRIGHT|wxBOTTOM, 5 );

    if (set_sizer)
    {
        parent->SetSizer( item0 );
        if (call_fit)
            item0->SetSizeHints( parent );
    }
    
    return item0;
}

// Implement menubar functions

wxMenuBar *MenuBarFunc2()
{
    wxMenuBar *item0 = new wxMenuBar;
    
    wxMenu* item1 = new wxMenu;
    item1->AppendSeparator();
    item1->Append( ID2_NEW_BOOK, _("N&ew book"), _("Create a new book") );
    item1->Append( ID2_OPEN_BOOK, _("Open &book"), _("Open a book") );
    item1->Append( ID2_CLOSE_BOOK, _("C&lose book"), _("Close the book") );
    item1->AppendSeparator();
    item1->Append( ID2_SAVE_BOOK, _("Sa&ve book"), _("Save the book") );
    item1->Append( ID2_SAVE_AS_BOOK, _("Save boo&k as"), _("Save the book under a new name") );
    item1->AppendSeparator();
    item1->Append( ID2_EXPORT_IMAGE, _("Export as &image"), _("Save superimposed result  into an image file") );
    item0->Append( item1, _("commun1") );
    
    wxMenu* item2 = new wxMenu;
    item2->AppendSeparator();
    item2->Append( ID2_MANUAL_POINTS, _("Set points manually\tAlt-Ctrl-P"), _("Set the superimposition points manually"), wxITEM_CHECK );
    item2->Append( ID2_CANCEL_SUP, _("Cancel superimposition\tAlt-Ctrl-Z"), _("Undo the superimposition of the sources") );
    item2->Append( ID2_PROCESS, _("&Run"), _("Process current page") );
    item2->Append( ID2_ADJUST_DLG, _("Adjust brightness and contrast"), _("Open the dialog for adjusting brightness and contrast") );
    item0->Append( item2, _("commun2") );
    
    wxMenu* item3 = new wxMenu;
    item3->Append( ID2_BOOK_EDIT, _("Edit"), _("Edit the book informations") );
    item3->Append( ID2_BOOK_LOAD, _("Reload files"), _("Reload images and Aruspix files into the book") );
    item3->AppendSeparator();
    item3->Append( ID2_BOOK_SUP, _("Batch superimposition"), _("Superimpose active pages of the book") );
    item0->Append( item3, _("&Book") );
    
    wxMenu* item4 = new wxMenu;
    item4->Append( ID2_ZOOM_OUT, _("Zoom out\t-"), _("Zoom out") );
    item4->Append( ID2_ZOOM_IN, _("Zoom in\t+"), _("Zoom in") );
    item4->AppendSeparator();
    item4->Append( ID2_ZOOM_OUT1, _("Zoom out source 1"), _("Zoom out source 1") );
    item4->Append( ID2_ZOOM_IN1, _("Zoom in source 1"), _("Zoom in source 1") );
    item4->Append( ID2_ZOOM_OUT2, _("Zoom out source 2"), _("Zoom out source 2") );
    item4->Append( ID2_ZOOM_IN2, _("Zoom in source 2"), _("Zoom in source 2") );
    item0->Append( item4, _("&Page") );
    
    return item0;
}

// Implement toolbar functions

// Implement bitmap functions

wxBitmap BitmapsFunc2( size_t index )
{
    if (index == 0)
    {
        /* XPM */
        static const char *xpm_data[] = {
        /* columns rows colors chars-per-pixel */
        "16 15 1 1",
        "  c None",
        /* pixels */
        "                ",
        "                ",
        "                ",
        "                ",
        "                ",
        "                ",
        "                ",
        "                ",
        "                ",
        "                ",
        "                ",
        "                ",
        "                ",
        "                ",
        "                "
        };
        wxBitmap bitmap( xpm_data );
        return bitmap;
    }
    return wxNullBitmap;
}


// End of generated file
