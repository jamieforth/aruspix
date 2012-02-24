/////////////////////////////////////////////////////////////////////////////
// Name:        layout.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __MUS_LAYOUT_H__
#define __MUS_LAYOUT_H__

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "musobject.h"
#include "musdc.h"

class MusScore;
//class MusPage;

enum LayoutType {
    Raw = 0,
    Normal,
    Facsimile
};


//----------------------------------------------------------------------------
// MusLayout
//----------------------------------------------------------------------------

/**
 * This class represents a laid-out score.
 * A MusLayout is contained in a MusDoc.
 * It contains MusPage objects.
*/
class MusLayout: public MusObject
{
public:
    // constructors and destructors
    MusLayout( LayoutType type );
    virtual ~MusLayout();
	
	void AddPage( MusPage *page );
    
    void Clear();
    
    /** The parent MusDoc setter */
    void SetDoc( MusDoc *doc );
	/** Realize the layout */
	void Realize( MusScore *score );
	
	/*
     Calcul la taille de la page = calculFormatPapier()
     */
	void PaperSize( );
    /**
     Initialise les donnees de visualisation par page
     */
	virtual void UpdatePageValues();
    /**
     Initialise les donnees de polices (music and lyrics)
     */
	virtual void UpdateFontValues();
    
	int GetPageCount() const { return (int)m_pages.GetCount(); };
    
    // moulinette
    virtual void Process(MusLayoutFunctor *functor, wxArrayPtrVoid params );
    // functors
    
private:
    // method calculating the font size
    int CalcMusicFontSize( );
    int CalcNeumesFontSize( );
    
public:
    /** The MusPage objects of the layout */
    ArrayOfMusPages m_pages;
    
    MusDoc *m_doc;
    
    MusEnv m_env;
    
    // Previously in MusRC
    /** Editor step (10 by default) */
    int m_step1;
    /** Editor medium step (3 * m_step1) */
    int m_step2;
    /** Editor large step (6 * m_step1) */
    int m_step3;
    /** Half a the space between to staff lines for normal and small staff (10 and 8 by default) */
    int m_halfInterl[2];
    /** Space between to staff lines for normal and small staff (20 and 16 by default) */
    int m_interl[2];
    /** Height of a five line staff for normal and small staff (80 and 64 by default) */
    int m_staffSize[2];
    /** Height of an octave for normal and small staff (70 and 56 by default) */
    int m_octaveSize[2];
    /** Font height (100 par defaut) */
    int m_fontHeight;
    /** Font height with ascent for normal and small staff and normal and grace size */
	int m_fontHeightAscent[2][2];
    /** Normal and small staff ration (16 / 20 by default) */
    int m_smallStaffRatio[2];
    /** Normal and grace size (3 / 4 by default) */
    int m_graceRatio[2];
    /** Height of a beam for normal and small staff (10 and 6 by default) */
    int m_beamWidth[2];
    /** Height of a beam spacing (white) for normal and small staff (10 and 6 by default) */
    int m_beamWhiteWidth[2];
    /** Current font height with ascent for normal and small staff and normal and grace size */
    int m_fontSize[2][2];
    /** Note radius for normal and small staff and normal and grace size */
    int m_noteRadius[2][2];
    /** Ledger line length for normal and small staff and normal and grace size */
    int m_ledgerLine[2][3];
    /** Brevis width for normal and small staff */
    int m_brevisWidth[2];
    /** Accident width for normal and small staff and normal and grace size */
    int m_accidWidth[2][2];
    /** A vertical unit corresponding to the 1/4 of an interline */
    float m_verticalUnit1[2];
    /** A vertical unit corresponding to the 1/8 of an interline */
    float m_verticalUnit2[2];
    /** Spacing between barlines */
    int m_barlineSpacing;
    /** Current Leipzig font for normal and small staff and normal and grace size */
    MusFontInfo m_activeFonts[2][2];				
    /** Leipzig font */
    MusFontInfo m_ftLeipzig;
    /** Current FestaDiesA font for normal and small staff and normal and grace size */
	MusFontInfo m_activeChantFonts[2][2];
	/** FestaDiesA font for neume notation */
	MusFontInfo m_ftFestaDiesA;
    /** Current lyric font for normal and small staff and normal and grace size */
    MusFontInfo m_activeLyricFonts[2];
    /** Lyric font by default */
    MusFontInfo m_ftLyrics;
	
	float m_beamMinSlope, m_beamMaxSlope;
	int m_pageWidth, m_pageHeight;
	int m_leftMargin;
	
    //int mesureNum, mesureDen;
	//float MesVal;
    
    /** indique si la definition de page poue laquelle fontes actuelles est a jour */
    int m_charDefin;

    
private:
    LayoutType m_type;
    
};


#endif
