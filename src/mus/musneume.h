/////////////////////////////////////////////////////////////////////////////
// Name:        musneume.h
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Laurent Pugin. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __MUS_NEUME_H__
#define __MUS_NEUME_H__

#ifdef __GNUG__
    #pragma interface "musneume.cpp"    
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include "wx/dynarray.h"

#include "muselement.h"
#include <vector>
#include "neumedef.h"

#define MAX_LENGTH 1

class MusStaff;

// WDR: class declarations

//----------------------------------------------------------------------------
// MusNeume
//
// A neume by nature is a composite class, it can consist of a single note, or
// of many notes. 
//----------------------------------------------------------------------------


//typedef struct n_pitch {
//	int code;
//	int oct;
//	unsigned char val; // this will be the same for the first pitch (or null...)
//}NPitch;
//


class MusNeumePitch
{
public:
	MusNeumePitch();
	MusNeumePitch( int code, int oct, unsigned char val );
	MusNeumePitch( const MusNeumePitch& pitch );
	MusNeumePitch& MusNeumePitch::operator=( const MusNeumePitch& pitch );
	virtual ~MusNeumePitch() {}
	void MusNeumePitch::SetPitch( int code, int oct );
	void MusNeumePitch::SetValue( int value );
	int MusNeumePitch::GetValue( );
	int MusNeumePitch::getPunctumType( );
	
	int MusNeumePitch::Compare(MusNeumePitch *other);
	
	int code;
	int oct;
	unsigned char val;
}; 

class MusNeume: public MusElement
{
public:
    // constructors and destructors
    MusNeume();
	MusNeume( unsigned char _val, unsigned char _code );
	MusNeume( const MusNeume& neume ); // copy contructor
	MusNeume& operator=( const MusNeume& neume ); // copy assignement;
    virtual ~MusNeume();
    
	virtual void MusNeume::Draw( wxDC *dc, MusStaff *staff);
//	void MusNeume::note( wxDC *dc, MusStaff *staff );
	void MusNeume::DrawBox( wxDC *dc, MusStaff *staff );
	void MusNeume::DrawNeume( wxDC *dc, MusStaff *staff );
	void MusNeume::DrawPunctums( wxDC *dc, MusStaff *staff );
	void MusNeume::leg_line( wxDC *dc, int y_n, int y_p, int xn, 
							unsigned int smaller, int pTaille);
	void MusNeume::append( wxDC *dc, MusStaff *staff ); //for creating multi-note neumes
	
	//should have some sort of 'toggle' or 'redraw' method here for switching between styles
	
    // WDR: method declarations for MusNeume
	virtual void MusNeume::SetPitch( int code, int oct, MusStaff *staff = NULL );
	virtual void MusNeume::SetValue( int value, MusStaff *staff = NULL, int vflag = 0 );
	int MusNeume::GetValue();
	
	//helper debug method
	void MusNeume::printNeumeList();
	
	int MusNeume::getYPos(int index, MusStaff *staff);
	
	int MusNeume::GetMaxPitch();
	int MusNeume::GetMinPitch();
	int MusNeume::GetPitchRange();
	int MusNeume::Length();
	
	void MusNeume::SetClosed(bool value);
	
	bool MusNeume::GetNext();
	bool MusNeume::GetPrevious();
	
	bool MusNeume::IsClosed();
	
	//when appending notes, do we discard the object?
	//may need a helper class for composite neumes.
	void MusNeume::InsertPitchAfterSelected();
	void MusNeume::RemoveSelectedPitch();
	void MusNeume::Append();

	//how many pitches are in this neume?
	//int length;
	
	//which note of the group has been selected?
	unsigned int n_selected;
	
	/**the list of actual pitches within the neume
	
	 Pitches for neumes are implemented as follows:
	 The first pitch of the neume group is the same as the 'code' of the entire neume.
	 The rest of the pitches (if any) in the neume group are then drawn relative to the
	 first pitch.	 
	 
	 */
	std::vector<MusNeumePitch*> n_pitches;
	std::vector<MusNeumePitch*>::iterator iter;
	
	int p_max; //the highest pitch in the neume group, relative to base pitch
	int p_min; //lowest pitch, relative to base pitch
	int p_range; // the pitch range, getter acts as setter!
	
	//type of neume, usually to get different type of punctum
	//unsigned char val;
	
	// possible places for ligatures to 'attach'
	// hotCorners[upper/lower][left/right]
	// you cannot have more than one 'vertical attachment' on a single note
		
	bool closed; 
		
	//helper methods
	
	
public:
	// WDR: member variable declarations for MusNeume
	int size;	//how many notes in group
	
private:
    // WDR: handler declarations for MusNeume
	
};


#endif
