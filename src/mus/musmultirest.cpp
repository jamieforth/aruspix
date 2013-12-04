//
//  musmultirest.cpp
//  aruspix
//
//  Created by Rodolfo Zitellini on 02/12/13.
//  Copyright (c) 2013 com.aruspix.www. All rights reserved.
//


#include "musmultirest.h"

//----------------------------------------------------------------------------
// MusClef
//----------------------------------------------------------------------------

MusMultiRest::MusMultiRest():
MusLayerElement("multirest-")
{
    m_number = 0;
}

MusMultiRest::MusMultiRest(int number):
    MusLayerElement("multirest-")
{
    m_number = number;
}


MusMultiRest::~MusMultiRest()
{
}


bool MusMultiRest::operator==( MusObject& other )
{
    MusMultiRest *otherM = dynamic_cast<MusMultiRest*>( &other );
    if ( !otherM ) {
        return false;
    }
    if ( this->m_number != otherM->m_number ) {
        return false;
    }
    return true;
}
