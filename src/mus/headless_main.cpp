//
//  headless_main.cpp
//  aruspix
//
//  Created by Rodolfo Zitellini on 22/06/12.
//  Copyright (c) 2012 com.aruspix.www. All rights reserved.
//

#include <iostream>
#include <string>
#include <sstream>

#include <wx/string.h>
#include "wx/wx.h"

#include "musdoc.h"
#include "musiopae.h"
#include "muslayout.h"
#include "musrc.h"
#include "mussvgdc.h"
#include "musbboxdc.h"
#include "muspage.h"
#include "mussystem.h"
#include "musiomei.h"
#include "musiodarms.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

string m_infile;
string m_svgdir;
string m_outfile;

bool m_pae = false;
bool m_darms = false;
bool m_mei = false;

const char *cmdlineopts = "dmpr:o:h";

// Some handy string split functions
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}


void display_usage() {
    cerr << "Aruspix headless usage:" << endl;
    cerr << "aruspix [-d -p -m] [-o outfile -r resources -h] infile" << endl << endl;

    cerr <<"-d read DARMS file [default if no option is given]" << endl;
    cerr <<"-p read PAE file." << endl;
    cerr <<"-m read MEI file." << endl;
    
    cerr << "Resources default dir: " << MusDoc::GetResourcesPath() << endl;
}

int main(int argc, char** argv) {
    
    int opt;
    
    if (argc < 2) {
        cerr << "Expecting one input file." << endl << endl;
        display_usage();
        exit(1);
    }
    
    opt = getopt( argc, argv, cmdlineopts );
    while( opt != -1 ) {
        switch( opt ) {
            case 'p': m_pae = true; break;
            case 'd': m_darms = true; break;
            case 'm': m_mei = true; break;
                
            case 'r':
                MusDoc::SetResourcesPath(optarg);
                break;
                
            case 'o':
                m_outfile = *new string(optarg);
                break;
                
            case 'h':
                display_usage();
                exit(0);
                break;
                
            default:
                break;
        }
        
        opt = getopt( argc, argv, cmdlineopts );
    }
    
    if (optind <= argc - 1) {
        m_infile = *new string(argv[optind]);
    }
    else {
        cerr << "Incorrect number of options: expecting one input file." << endl << endl;
        display_usage();
        exit(1);
    }
    
    if ((m_pae && m_mei) || (m_pae && m_darms) || (m_darms && m_mei) || (m_pae && m_darms && m_mei)) {
        cerr << "Please select only one output option: -d -m -p." << endl;
        exit(1);
    }
    
    // If no input type is specified, set darms as default
    if (!m_pae && !m_darms && !m_mei)
        m_darms = true;
    
    // create outfile
    if (m_outfile.length() == 0) {
        std::vector<std::string> v = split(m_infile, '/');
        m_outfile = v[v.capacity() - 1] + ".svg";
    }
    
    cerr << "Reading " << m_infile << "..." << endl;
    
    MusDoc *doc =  new MusDoc();

    MusFileInputStream import;
    
    if (m_pae) {
        MusPaeInput mpae( doc, m_infile.c_str() );
        if ( !mpae.ImportFile()) {
            cerr << "Error importing " << m_outfile << "." << endl;
            exit(1);
        }
    } else if (m_darms) {
        MusDarmsInput mdarms( doc, m_infile.c_str() );
        if ( !mdarms.ImportFile()) {
           cerr << "Error importing " << m_outfile << "." << endl;
            exit(1);
        }
    } else if (m_mei){
        MusMeiInput meiinput( doc, m_infile.c_str() );
        if ( !meiinput.ImportFile()) {
            cerr << "Error importing " << m_outfile << "." << endl;
            exit(1);
        }
    } else {
        cerr << "No input type selected! Should never get here, default is DARMS" << endl;
        exit(1); 
    }
    
	MusLayout *layout = new MusLayout( Raw );
	layout->Realize(doc->m_divs[0].m_score);
	doc->AddLayout( layout );
    layout->SpaceMusic();
        
    MusPage *page = &layout->m_pages[0];
    MusSystem *system = &page->m_systems[0];
    
    MusRC rc;
    rc.SetLayout(layout);
    layout->m_leftMargin = 0; // good done here?
    MusSvgDC *svg = new MusSvgDC(m_outfile.c_str(), system->m_contentBB_x2 - system->m_contentBB_x1, (system->m_contentBB_y2 - system->m_contentBB_y1));
    rc.DrawPage(svg, &layout->m_pages[0] , false);
    
    delete svg;
    
    cerr << "Output written to " << m_outfile << endl;
    
    return 0;
}
