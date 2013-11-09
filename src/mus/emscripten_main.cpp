//
//  emscripten_main.cpp
//  aruspix
//
//  Created by Rodolfo Zitellini on 05/11/13.
//  Copyright (c) 2013 com.aruspix.www. All rights reserved.
//

#include <sstream>
#include <string>

#include "mus.h"
#include "muscontroller.h"

using namespace std;


extern "C" {
    const char * convertMusic(const char * c_notation, const char * c_options) {
        
        string notation(c_notation);
        string json_options(c_options);
        string out_str;
        MusController controller;
        
        if (!controller.ParseOptions( json_options )) {
            Mus::LogError( "Could not load JSON options." );
            return NULL;
        }
        
        Mus::SetResourcesPath("/data");
        
        // default to mei if unset.
        controller.LoadString( notation );

        // in the future we will be able to render to mei too
        out_str = controller.RenderToSvg();
        return out_str.c_str();
    }
}