//------------------------------------------------------------------------------
// Source code generated by wxDesigner from file: wgviewer.wdr
// Do not modify this file, all changes will be lost!
//------------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "wgviewer_wdr.h"
#endif

// For compilers that support precompilation
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// Include private header
#include "wgviewer_wdr.h"

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
#include "mus/mustoolpanel.h"
#include "wgviewer/wgviewer.h"

// Implement window functions

wxSizer *WindowFunc5( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxFlexGridSizer *item0 = new wxFlexGridSizer( 1, 0, 0 );
    item0->AddGrowableCol( 0 );
    item0->AddGrowableRow( 2 );

    wxStaticLine *item1 = new wxStaticLine( parent, ID5_LINE, wxDefaultPosition, wxSize(20,-1), wxLI_HORIZONTAL );
    item0->Add( item1, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxBOTTOM, 5 );

    MusToolPanel *item2 = new MusToolPanel( parent, ID5_TOOLPANEL, wxDefaultPosition, wxSize(10,10), 0 );
    item0->Add( item2, 0, 0, 5 );

    WgvPanel *item3 = new WgvPanel( parent, ID5_MUSPANEL, wxDefaultPosition, wxSize(200,160), wxSUNKEN_BORDER );
    item0->Add( item3, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 0 );

    if (set_sizer)
    {
        parent->SetSizer( item0 );
        if (call_fit)
            item0->SetSizeHints( parent );
    }
    
    return item0;
}

// Implement menubar functions

wxMenuBar *MenuBarFunc5()
{
    wxMenuBar *item0 = new wxMenuBar;
    
    wxMenu* item1 = new wxMenu;
    item1->AppendSeparator();
    item1->Append( ID5_OPEN, _("&Open"), wxT("") );
    item1->Append( ID5_Save, _("&Save"), wxT("") );
    item1->Append( ID5_SAVE_MLF, _("Generate &MLF"), wxT("") );
    item1->Append( ID5_SAVE_MODEL, _("&Generate music model"), wxT("") );
    item1->Append( ID5_OPEN_MLF, _("Load &MLF"), wxT("") );
    item0->Append( item1, _("commun1") );
    
    wxMenu* item2 = new wxMenu;
    item0->Append( item2, _("commun2") );
    
    wxMenu* item3 = new wxMenu;
    item3->Append( ID5_PREVIOUS, _("&Previous page"), _("Go to the previous page") );
    item3->Append( ID5_NEXT, _("&Next page"), _("Go to the next page") );
    item3->Append( ID5_GOTO, _("&Go to page ..."), _("Select a page in the file") );
    item3->AppendSeparator();
    item3->Append( ID5_ZOOMOUT, _("Zoom out\t-"), _("Zoom out the page") );
    item3->Append( ID5_ZOOMIN, _("Zoom in\t+"), _("Zoom in the page") );
    item0->Append( item3, _("&Page") );
    
    wxMenu* item4 = new wxMenu;
    item4->Append( ID5_INSERT_MODE, _("&Insertion"), wxT(""), wxITEM_CHECK );
    item4->Append( ID5_NOTES, _("&Notes and rests"), wxT("") );
    item4->Append( ID5_KEYS, _("&Keys"), wxT("") );
    item4->Append( ID5_SIGNS, _("&Propostions"), wxT("") );
    item4->Append( ID5_SYMBOLES, _("&Symboles"), wxT("") );
    item0->Append( item4, _("&Tools") );
    
    return item0;
}

// Implement toolbar functions

void ToolBarFunc5( wxToolBar *parent )
{
    parent->SetMargins( 2, 2 );
    
    parent->AddTool( ID5_OPEN, wxT(""), BitmapsFunc5( 0 ), wxNullBitmap, wxITEM_NORMAL, wxT("") );
    parent->AddSeparator();
    parent->AddTool( ID5_PREVIOUS, wxT(""), BitmapsFunc5( 1 ), wxNullBitmap, wxITEM_NORMAL, _("Previous page"), _("Go to the previous page") );
    parent->AddTool( ID5_NEXT, wxT(""), BitmapsFunc5( 2 ), wxNullBitmap, wxITEM_NORMAL, _("Next page"), _("Go to next page in the file") );
    parent->AddSeparator();
    parent->AddTool( ID5_ZOOMOUT, wxT(""), BitmapsFunc5( 4 ), wxNullBitmap, wxITEM_NORMAL, _("Zoom out"), _("Zoom out the page") );
    parent->AddTool( ID5_ZOOMIN, wxT(""), BitmapsFunc5( 3 ), wxNullBitmap, wxITEM_NORMAL, _("Zoom in"), _("Zoom in the page") );
    parent->AddSeparator();
    parent->AddTool( ID5_INSERT_MODE, wxT(""), BitmapsFunc5( 5 ), wxNullBitmap, wxITEM_CHECK, wxT("") );
    parent->AddTool( ID5_NOTES, wxT(""), BitmapsFunc5( 6 ), wxNullBitmap, wxITEM_NORMAL, wxT("") );
    parent->AddTool( ID5_KEYS, wxT(""), BitmapsFunc5( 7 ), wxNullBitmap, wxITEM_NORMAL, wxT("") );
    parent->AddTool( ID5_SIGNS, wxT(""), BitmapsFunc5( 9 ), wxNullBitmap, wxITEM_NORMAL, wxT("") );
    parent->AddTool( ID5_SYMBOLES, wxT(""), BitmapsFunc5( 8 ), wxNullBitmap, wxITEM_NORMAL, wxT("") );
    
    parent->Realize();
}

// Implement bitmap functions

wxBitmap BitmapsFunc5( size_t index )
{
    if (index == 0)
    {
        /* XPM */
        static const char *xpm_data[] = {
        /* columns rows colors chars-per-pixel */
        "16 15 5 1",
        "  c None",
        "a c Black",
        "b c #FFFFFF",
        "c c #FFFF00",
        "e c #808000",
        /* pixels */
        "                ",
        "         aaa    ",
        "        a   a a ",
        "             aa ",
        " aaa        aaa ",
        "abcbaaaaaaa     ",
        "acbcbcbcbca     ",
        "abcbcbcbcba     ",
        "acbcbaaaaaaaaaa ",
        "abcbaeeeeeeeeea ",
        "acbaeeeeeeeeea  ",
        "abaeeeeeeeeea   ",
        "aaeeeeeeeeea    ",
        "aaaaaaaaaaa     ",
        "                "
        };
        wxBitmap bitmap( xpm_data );
        return bitmap;
    }
    if (index == 1)
    {
        /* XPM */
        static const char *xpm_data[] = {
        /* columns rows colors chars-per-pixel */
        "16 15 3 1",
        "  c None",
        "a c Black",
        "b c #FFFFFF",
        /* pixels */
        "                ",
        "      aaaaaaa   ",
        "     aabbbbba   ",
        "    ababbbbba   ",
        "   aaaabbbbba   ",
        "   abbbbabbba   ",
        "   abbbaabbba   ",
        "   abbaaaaaba   ",
        "   abaaaaaaba   ",
        "   abbaaaaaba   ",
        "   abbbaabbba   ",
        "   abbbbabbba   ",
        "   abbbbbbbba   ",
        "   aaaaaaaaaa   ",
        "                "
        };
        wxBitmap bitmap( xpm_data );
        return bitmap;
    }
    if (index == 2)
    {
        /* XPM */
        static const char *xpm_data[] = {
        /* columns rows colors chars-per-pixel */
        "16 15 3 1",
        "  c None",
        "a c Black",
        "b c #FFFFFF",
        /* pixels */
        "                ",
        "   aaaaaaa      ",
        "   abbbbbaa     ",
        "   abbbbbaba    ",
        "   abbbbbaaaa   ",
        "   abbbabbbba   ",
        "   abbbaabbba   ",
        "   abaaaaabba   ",
        "   abaaaaaaba   ",
        "   abaaaaabba   ",
        "   abbbaabbba   ",
        "   abbbabbbba   ",
        "   abbbbbbbba   ",
        "   aaaaaaaaaa   ",
        "                "
        };
        wxBitmap bitmap( xpm_data );
        return bitmap;
    }
    if (index == 3)
    {
        /* XPM */
        static const char *xpm_data[] = {
        /* columns rows colors chars-per-pixel */
        "16 15 6 1",
        "  c None",
        "a c Black",
        "b c #FFFFFF",
        "c c #000080",
        "d c #0000FF",
        "e c #C0C0C0",
        /* pixels */
        "    aaaa        ",
        "  aaeebeaa      ",
        " aeebbebbea     ",
        " abeeaabeea     ",
        "abeebaaeeeba    ",
        "aeeaaaaaaeba    ",
        "abeaaaaaaeea    ",
        "aebbeaaebbba    ",
        " abebaabeba     ",
        " aebbebebea     ",
        "  aaeeeeaacd    ",
        "    aaaa  dcdd  ",
        "           dccd ",
        "            dccd",
        "             dcd"
        };
        wxBitmap bitmap( xpm_data );
        return bitmap;
    }
    if (index == 4)
    {
        /* XPM */
        static const char *xpm_data[] = {
        /* columns rows colors chars-per-pixel */
        "16 15 6 1",
        "  c None",
        "a c Black",
        "b c #FFFFFF",
        "c c #000080",
        "d c #0000FF",
        "e c #C0C0C0",
        /* pixels */
        "    aaaa        ",
        "  aaeebeaa      ",
        " aeebbebbea     ",
        " abeeeebeea     ",
        "abeebeeeeeba    ",
        "aeeaaaaaaeba    ",
        "abeaaaaaaeea    ",
        "aebbeeeebbba    ",
        " abebebbeba     ",
        " aebbebebea     ",
        "  aaeeeeaacd    ",
        "    aaaa  dcdd  ",
        "           dccd ",
        "            dccd",
        "             dcd"
        };
        wxBitmap bitmap( xpm_data );
        return bitmap;
    }
    if (index == 5)
    {
        static const unsigned char data[] = 
        {
            192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,
            192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,
            192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,
            192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,
            192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,
            192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,
            192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,
            192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,132,132,132,55,55,55,55,55,
            55,103,103,103,192,192,192,62,62,62,84,84,84,192,192,192,150,150,150,80,80,80,192,192,192,
            167,167,167,74,74,74,55,55,55,78,78,78,157,157,157,156,156,156,44,44,44,21,21,21,139,
            139,139,192,192,192,9,9,9,0,0,0,128,128,128,134,134,134,35,35,35,185,185,185,26,26,
            26,57,57,57,110,110,110,72,72,72,118,118,118,192,192,192,77,77,77,37,37,37,192,192,192,
            192,192,192,9,9,9,46,46,46,16,16,16,115,115,115,35,35,35,185,185,185,18,18,18,36,
            36,36,100,100,100,133,133,133,188,188,188,192,192,192,77,77,77,37,37,37,192,192,192,192,192,
            192,9,9,9,152,152,152,18,18,18,23,23,23,35,35,35,192,192,192,143,143,143,58,58,58,
            26,26,26,0,0,0,86,86,86,180,180,180,66,66,66,32,32,32,175,175,175,192,192,192,9,
            9,9,159,159,159,137,137,137,4,4,4,35,35,35,184,184,184,82,82,82,162,162,162,165,165,
            165,35,35,35,62,62,62,108,108,108,0,0,0,0,0,0,68,68,68,192,192,192,9,9,9,
            159,159,159,192,192,192,57,57,57,35,35,35,184,184,184,43,43,43,0,0,0,0,0,0,45,
            45,45,172,172,172,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,
            192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,
            192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,
            192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,
            192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,
            192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,
            192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,
            192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,
            192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,
            192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192
        };
        wxImage image( 16, 15, (unsigned char*)data, TRUE );
        wxBitmap bitmap( image );
        return bitmap;
    }
    if (index == 6)
    {
        /* XPM */
        static const char *xpm_data[] = {
        /* columns rows colors chars-per-pixel */
        "16 15 2 1",
        "a c Black",
        "b c #FFFFFF",
        /* pixels */
        "bbbbbbbbbbabbbbb",
        "bbbbbbbbbbabbbbb",
        "bbbbbbbbbbabbbbb",
        "bbbbbbbbbbabbbbb",
        "bbbbbbbbbbabbbbb",
        "bbbbbbbbbbabbbbb",
        "bbbbbbbbbbabbbbb",
        "bbbbbbbbbbabbbbb",
        "bbbabbbbbbabbbbb",
        "bbbaaaaaaaabbbbb",
        "bbbaaaaaaaabbbbb",
        "bbbabbbbbbabbbbb",
        "bbbaaaaaaaabbbbb",
        "bbbaaaaaaaabbbbb",
        "bbbabbbbbbabbbbb"
        };
        wxBitmap bitmap( xpm_data );
        return bitmap;
    }
    if (index == 7)
    {
        /* XPM */
        static const char *xpm_data[] = {
        /* columns rows colors chars-per-pixel */
        "16 15 21 1",
        "a c Black",
        "b c #D9D9D9",
        "c c #E1E1E1",
        "d c #FFFFFF",
        "e c #2E2E2E",
        "f c #979797",
        "g c #181818",
        "h c #272727",
        "i c #111111",
        "j c #5C5C5C",
        "k c #737373",
        "l c #1A1A1A",
        "m c #292929",
        "n c #383838",
        "o c #474747",
        "p c #ECECEC",
        "q c #7C7C7C",
        "r c #0C0C0C",
        "s c #B2B2B2",
        "t c #161616",
        "u c #ACACAC",
        /* pixels */
        "dddjactualakdddd",
        "dddjacauaaaadddd",
        "dddjachdidoafddd",
        "dddjacgdddeasddd",
        "dddjacabaaaadddd",
        "dddjacnbamqddddd",
        "dddjacrapddddddd",
        "dddjacaadddddddd",
        "dddjacrapddddddd",
        "dddjacnbamqddddd",
        "dddjacabaaaadddd",
        "dddjacgdddeasddd",
        "dddjachdidoafddd",
        "dddjacauaaaadddd",
        "dddjactualakdddd"
        };
        wxBitmap bitmap( xpm_data );
        return bitmap;
    }
    if (index == 8)
    {
        /* XPM */
        static const char *xpm_data[] = {
        /* columns rows colors chars-per-pixel */
        "16 15 3 1",
        "a c Black",
        "b c #FFFFFF",
        "c c #C0C0C0",
        /* pixels */
        "bbbbbbbbbbbbbbbb",
        "babbbbbbbcacbbbb",
        "babbbbbbbaaabbbb",
        "babbbbbbbcacbbbb",
        "babbbbbbbbbbbbbb",
        "babbbbbbbbbbbbbb",
        "baaabbbbbbbbbbbb",
        "babbabbaabbbbaab",
        "babbabbbaabbaabb",
        "babbabbbbaaaabbb",
        "baaabbbbbbaabbbb",
        "bbbbbbbbbaaaabbb",
        "bbbbbbbbaabbaabb",
        "bbbbbbbaabbbbaab",
        "bbbbbbbbbbbbbbbb"
        };
        wxBitmap bitmap( xpm_data );
        return bitmap;
    }
    if (index == 9)
    {
        static const unsigned char data[] = 
        {
            255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,144,144,144,8,
            8,8,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
            255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,144,144,144,8,8,8,
            255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
            255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,144,144,144,8,8,8,255,255,
            255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
            255,255,255,255,255,255,255,255,255,255,255,255,213,213,213,73,73,73,4,4,4,141,141,141,248,
            248,248,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
            255,255,255,255,244,244,244,102,102,102,19,19,19,21,21,21,1,1,1,32,32,32,30,30,30,
            175,175,175,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
            255,255,184,184,184,24,24,24,140,140,140,135,135,135,8,8,8,228,228,228,54,54,54,66,66,
            66,242,242,242,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
            112,112,112,31,31,31,255,255,255,97,97,97,5,5,5,206,206,206,167,167,167,21,21,21,211,
            211,211,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,112,112,
            112,31,31,31,255,255,255,19,19,19,0,0,0,112,112,112,167,167,167,21,21,21,211,211,211,
            255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,112,112,112,31,
            31,31,255,255,255,106,106,106,5,5,5,217,217,217,167,167,167,21,21,21,211,211,211,255,255,
            255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,193,193,193,26,26,26,
            125,125,125,127,127,127,7,7,7,214,214,214,45,45,45,75,75,75,245,245,245,255,255,255,255,
            255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,244,244,244,113,113,113,24,24,
            24,18,18,18,1,1,1,29,29,29,39,39,39,183,183,183,255,255,255,255,255,255,255,255,255,
            255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,215,215,215,82,
            82,82,4,4,4,157,157,157,250,250,250,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
            255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,144,144,144,
            8,8,8,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
            255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,144,144,144,8,8,
            8,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
            255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,144,144,144,8,8,8,255,
            255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
        };
        wxImage image( 16, 15, (unsigned char*)data, TRUE );
        wxBitmap bitmap( image );
        return bitmap;
    }
    return wxNullBitmap;
}


// End of generated file