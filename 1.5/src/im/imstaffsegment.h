/////////////////////////////////////////////////////////////////////////////
// Name:        imstaffsegment.h
// Author:      Laurent Pugin
// Created:     2004
// Copyright (c) Laurent Pugin. All rights reserved.   
/////////////////////////////////////////////////////////////////////////////

#ifndef __imstaffsegment_H__
#define __imstaffsegment_H__

#if defined(__GNUG__) && ! defined(__APPLE__)
    #pragma interface "imstaffsegment.h"
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "im/imoperator.h"


// WDR: class declarations

//----------------------------------------------------------------------------
// ImStaffSegment
//----------------------------------------------------------------------------

class ImStaffSegment: public ImOperator
{
public:
    // constructors and destructors
	ImStaffSegment( );
    virtual ~ImStaffSegment();
    
    // WDR: method declarations for ImStaffSegment
    // Processing
    bool AnalyzeSegment();

	int m_x1;
	int m_x2;
    float m_compactness;
    
private:
    // WDR: member variable declarations for ImStaffSegment
    

};

#endif // __imstaffsegment_H__