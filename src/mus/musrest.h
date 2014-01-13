/////////////////////////////////////////////////////////////////////////////
// Name:        musrest.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __MUS_REST_H__
#define __MUS_REST_H__

#include "musdurationinterface.h"
#include "muslayerelement.h"
#include "muspositioninterface.h"

//----------------------------------------------------------------------------
// Rest
//----------------------------------------------------------------------------

/** 
 * This class models the MEI <rest> element. 
 */
class Rest: public MusLayerElement, public MusDurationInterface, public PositionInterface
{
public:
    // constructors and destructors
    Rest();
    virtual ~Rest();
    
    /**
     * Comparison operator. 
     * Check if the MusLayerElement if a Rest and compare attributes
     */
    virtual bool operator==(MusObject& other);
    
    virtual std::string MusClassName( ) { return "Rest"; };
    
private:
    
public:
    /* Indicates the value to print above multi-measure rests */
    int m_multimeasure_dur;

private:
    
};


#endif
