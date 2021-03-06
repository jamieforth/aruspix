/////////////////////////////////////////////////////////////////////////////
// Name:        musiopae.cpp
// Author:      Rodolfo Zitellini // this is the important stuff!
// Created:     2012
// Copyright (c) Laurent Pugin. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <sstream>

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wx.h"

#include "wx/filename.h"
#include "wx/txtstrm.h"

#include "musiopae.h"

#include "musbarline.h"
#include "musclef.h"
#include "musmensur.h"
#include "musneume.h"
#include "musnote.h"
#include "musrest.h"
#include "mussymbol.h"
#include "muskeysig.h"

//#include "app/axapp.h"

#include <vector>
#include <regex.h>

using std::vector;

#define BEAM_INITIAL    0x01
#define BEAM_MEDIAL     0x02
#define BEAM_TERMINAL      0x04

// User interface variables:
int       debugQ = 0;                // used with --debug option
int       stdoutQ = 0;
char      outdir[1024] = {0};        // used with -d option
char      extension[1024] = {0};     // used with -e option
char      hum2abc[1024] = {0};       // used with -a option
int       quietQ  = 0;               // used with -q option
int       quiet2Q = 0;               // used with -Q option

// Global variables:
char data_line[10001] = {0};
#define MAX_DATA_LEN 1024 // One line of the pae file whould not be this long!
char data_key[MAX_DATA_LEN]; 
char data_value[MAX_DATA_LEN]; //ditto as above


//////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// MusPaeInput
//----------------------------------------------------------------------------

MusPaeInput::MusPaeInput( MusDoc *doc, wxString filename ) :
// This is pretty bad. We open a bad fileoinputstream as we don't use it
MusFileInputStream( doc, -1 )
{
    m_filename = filename;
	m_staff = NULL;
	m_layer = NULL;
    m_current_tie = NULL;
    m_current_tuplet =  NULL;
    
    m_current_beam = NULL;
}

MusPaeInput::~MusPaeInput()
{
}

//////////////////////////////////////////////////////////////////////////

bool MusPaeInput::ImportFile()
{
    
    std::ifstream infile;
    infile.open(m_filename);
    
    convertPlainAndEasyToKern(infile, std::cout);
    
    return true;
}


//////////////////////////////
//
// convertPlainAndEasyToKern --
//

void MusPaeInput::convertPlainAndEasyToKern(std::istream &infile, std::ostream &out) {
    // buffers
    char c_clef[1024] = {0};
    char c_key[1024] = {0};
    char c_keysig[1024] = {0};
    char c_timesig[1024] = {0};
    char c_alttimesig[1024] = {0};
    char incipit[10001] = {0};
    bool in_beam = false;
    
    wxString s_key;
    MeasureObject current_measure;
    NoteObject current_note;
    NoteObject *prev_note;
    
    //Array<int> current_key; // not the measure one, which will be altered by temporary alterations
    //current_key.setSize(7);
    //current_key.setAll(0);
    
    vector<MeasureObject> staff;
    
    // read values
    while (!infile.eof()) {
        infile.getline(data_line, 10000, '\n');
        if (infile.eof()) {
            //std::cerr << "Truncated file or ending tag missing" << std::endl;
            //exit(1);
        }
        getAtRecordKeyValue(data_key, data_value, data_line);
        if (strcmp(data_key,"end")==0) {   
            break;
        } else if (strcmp(data_key,"clef")==0) { 
            strcpy( c_clef, data_value );
        } else if (strcmp(data_key,"key")==0) { 
            strcpy( c_key, data_value );
        } else if (strcmp(data_key,"keysig")==0) { 
            strcpy( c_keysig, data_value );
        } else if (strcmp(data_key,"timesig")==0) { 
            strcpy( c_timesig, data_value );
        } else if (strcmp(data_key,"alttimesig")==0) { 
            strcpy( c_alttimesig, data_value );
        } else if (strcmp(data_key,"data")==0) { 
            strcpy( incipit, data_value );
        } else if (strncmp(data_line,"!!", 2) == 0) { 
            out << data_line << "\n";
        }
    }
    
    if (strlen(c_clef)) {
        MusClef *c = new MusClef;
        getClefInfo(c_clef, c );    // do we need to put a default clef?
        current_measure.clef = c;
    }

    if (strlen(c_keysig)) {
        getKeyInfo( c_keysig, &current_measure);
    }
    if (strlen(c_timesig)) {
        getTimeInfo( c_timesig, &current_measure);
    }   
    
    // read the incipit string
    int length = strlen(incipit);
    int i = 0;
	while(i < length) {
        // eat the input...
        
		if (incipit[i] == ' ') {
            // just skip
            i++;
        }
        
        // octaves
        if ((incipit[i] == '\'') || (incipit[i] == ',')) {
            i += getOctave( incipit, &current_note.octave, i );
        }
        
        // rhythmic values
        else if (isdigit(incipit[i]) != 0) {
            i += getDurations( incipit, &current_measure, i );
        }
        
        //accidentals (1 = n; 2 = x; 3 = xx; 4 = b; 5 = bb)    
        else if (incipit[i] == 'n' || incipit[i] == 'x' || incipit[i] == 'b') {
            i += getAccidental( incipit, &current_note.accidental, i );
        }
        
        //
        // beaming starts
		else if (incipit[i] == '{') {
			//current_note.beam = 1;
            current_note.beam |= BEAM_INITIAL;
            in_beam = true;
        }
        
        // beaming ends
		else if (incipit[i] == '}') {
			//current_note.beam = 0; // should not have to be done, but just in case
            prev_note->beam |= BEAM_TERMINAL;
            current_note.beam = 0;
            in_beam = false;
		}
        // // ax2.3 LP
		
        // slurs are read when adding the note
		else if (incipit[i] == '+') {
        }
        
		// beginning tuplets & fermatas
		else if (incipit[i] == '(') {
            i += getTupletFermata( incipit, &current_measure, &current_note, i );
		}
        
        // end of tuplets
		else if ((incipit[i] == ';') || (incipit[i] == ')')) {
            i += getTupletFermataEnd( incipit, &current_note, i );
		}
        
		// trills are read when adding the note
		else if (incipit[i] == 't') {
        }
        
		//grace notes
		else if ((incipit[i] == 'g') || (incipit[i] == 'q')) {
			i += getGraceNote( incipit, &current_note, i );
		}
		
        
        // end of appogiatura
		else if (incipit[i] == 'r') {
			current_note.appoggiatura = 0; // should not have to be done, but just in case
        }
        
        //note and rest
        // getNote also creates a new note object
        else if (((incipit[i]-'A'>=0) && (incipit[i]-'A'<7)) || (incipit[i]=='-')) {
            prev_note = &current_note; // save the old note for beaming, etc
            i += getNote( incipit, &current_note, &current_measure, i );
        }
        
  		// whole rest
		else if (incipit[i] == '=') {
            i += getWholeRest( incipit, &current_measure.wholerest, i );		
		} 
        
		// abbreviation
        else if (incipit[i] == '!') {
            i += getAbbreviation( incipit, &current_measure, i );
        }
        
        // measure repetition
        else if ((incipit[i] == 'i') && staff.size() > 0) {
            MeasureObject last_measure = staff[staff.size() - 1];
            current_measure.notes = last_measure.notes;
            // time was copied explicitally, but now not anymore
            //current_measure.time = last_measure.time;
            
            // if old measure does not end with a tie
            // force the first note of the newly copied measure to be without tie
            // this is to prevent copying tie closes which are invalid
            if (last_measure.notes[last_measure.notes.capacity() - 1].tie == 0)
                current_measure.notes[0].tie = 0;
        }
        
        //barline
        else if ((incipit[i] == ':') || (incipit[i] == '/')) {
            i += getBarline(incipit, &current_measure.barline, i);
            current_measure.abbreviation_offset = 0; // just in case...
            staff.push_back( current_measure );
            current_measure.reset();
        }
        
		//clef change
		else if ((incipit[i] == '%') && (i+1 < length)) {
            MusClef *c = new MusClef;
            i += getClefInfo(incipit, c, i + 1);
            current_note.clef = c;
        }
        
		//time signature change
		else if ((incipit[i] == '@') && (i+1 < length)) {
            i += getTimeInfo( incipit, &current_measure, i + 1);
        } 
        
  		//key signature change
		else if ((incipit[i] == '$') && (i+1 < length)) {
            i += getKeyInfo( incipit, &current_measure, i + 1);
		} 
        
        //
        if (in_beam && (current_note.beam & BEAM_INITIAL) == 0 && (current_note.beam & BEAM_TERMINAL) == 0) {
            current_note.beam |= BEAM_MEDIAL;
        }
        // // ax2.3 LP
            
        i++;
    }
    
    // we need to add the last measure if it has no barline at the end
    if (current_measure.notes.size() != 0) {
        //current_measure.barline = "=-";
        staff.push_back( current_measure );
        current_measure.notes.clear();
    }
    
    m_staff = new MusStaff( 1 );
    m_layer = new MusLayer( 1 );
    m_staff->AddLayer(m_layer);
    
    vector<MeasureObject>::iterator it;
    for ( it = staff.begin() ; it < staff.end(); it++ ) {
        MeasureObject obj = *it;
        printMeasure( out, &obj );
    }
    
    m_doc->Reset( Raw );
    MusPage *page = new MusPage();
    MusSystem *system = new MusSystem();
    system->AddStaff( m_staff );
    page->AddSystem( system );
    m_doc->AddPage( page );
}



//////////////////////////////
//
// getOctave --
//
#define BASE_OCT 4
int MusPaeInput::getOctave (const char* incipit, unsigned char *octave, int index ) {
    
    int i = index;
    int length = strlen(incipit);
    if (incipit[i] == '\'') {
        *octave = BASE_OCT;
        while ((i+1 < length) && (incipit[i+1] == '\'')) {
            (*octave)++;
            i++;
        }
    } else if (incipit[i] == ',') {
        //negative octave
        *octave = BASE_OCT - 1;
        while ((i+1 < length) && (incipit[i+1] == ',')) {
            (*octave)--;
            i++;
        }
    }
    
    return i - index;
}



//////////////////////////////
//
// getDuration --
//

int MusPaeInput::getDuration(const char* incipit, int *duration, int *dot, int index ) {
    
    int i = index;
    int length = strlen(incipit);
    
    switch (incipit[i]) {
        case '0': *duration = DUR_LG; break;
        case '1': *duration = DUR_1; break;
        case '2': *duration = DUR_2; break;
        case '3': *duration = DUR_32; break;
        case '4': *duration = DUR_4; break;
        case '5': *duration = DUR_64; break;
        case '6': *duration = DUR_16; break;
        case '7': *duration = DUR_128; break;
        case '8': *duration = DUR_8; break;
        case '9': *duration = DUR_BR; break;
    }
     
    
    *dot=0;
    if ((i+1 < length) && (incipit[i+1] == '.')) {
        // one dot
        (*dot)++;
        i++;
    }
    if ((i+1 < length) && (incipit[i+1] == '.')) {
        // two dots
        (*dot)++;
        i++;
    }
    if ((*dot == 1) && (*duration == 7)) {
        // neumatic notation
        *duration = 1.0;
        *dot = 0;
        std::cout << "Warning: found a note in neumatic notation (7.), using quarter note instead" << std::endl;				
    }
    
    return i - index;
}



//////////////////////////////
//
// getDurations --
//

int MusPaeInput::getDurations(const char* incipit, MeasureObject* measure, int index ) {
    
    int i = index;
    int length = strlen(incipit);
    
    measure->durations_offset = 0;
    measure->durations.clear();
    measure->dots.clear();
    
    //int j = 0;
    do {
        int dur, dot;
        //measure->dots.setSize(j+1);
        i += getDuration(incipit, &dur, &dot, i );
        measure->durations.push_back(dur);
        measure->dots.push_back(dot);
        //j++;
        if ((i+1 < length) && isdigit(incipit[i+1])) {
            i++;
        } else {
            break;
        }
    } while ( 1 );
    
    return i - index;
}



//////////////////////////////
//
// getAccidental --
//

int MusPaeInput::getAccidental(const char* incipit, unsigned char *accident, int index ) {
    
    int i = index;
    int length = strlen(incipit);
    
    if (incipit[i] == 'n') {
        *accident = ACCID_NATURAL;
    } else if (incipit[i] == 'x') {
        *accident = ACCID_SHARP;
        if ((i+1 < length) && (incipit[i+1] == 'x')) {
            *accident = ACCID_DOUBLE_SHARP;
            i++;
        }
    } else if (incipit[i] == 'b') {
        *accident = ACCID_FLAT;
        if ((i+1 < length) && (incipit[i+1] == 'b')) {
            *accident = ACCID_DOUBLE_FLAT;
            i++;
        }
    }
    return i - index;
}



//////////////////////////////
//
// getTupletOrFermata --
//

int MusPaeInput::getTupletFermata(const char* incipit, MeasureObject *measure, NoteObject *note, int index ) {
    
    int i = index;
    int length = strlen(incipit);
    
    // detect if it is a fermata or a tuplet
    regex_t re;
    regcomp(&re, "^([^)]*[ABCDEFG-][^)]*[ABCDEFG-][^)]*)", REG_EXTENDED);
    int is_tuplet = regexec(&re, incipit + i, 0, NULL, 0);
    regfree(&re);
    
    if (is_tuplet == 0) {
        int t = i;
        int t2 = 0;
        int tuplet_val = 0;
        char *buf;
        
        // Triplets are in the form (4ABC)
        // index points to the '(', so we look back
        // if the resut is a number or dot, it means we have the long format
        // i.e. 4(6ABC;5) or 4.(6ABC;5)
        if ((index != 0) && (isdigit(incipit[index - 1]) || incipit[index - 1] == '.')) {
            
            // create the buffer so we can convert the tuplet nr to int
            buf = (char*)malloc(length + 1); // allocate it with space for 0x00
            memset(buf, 0x00, length + 1); // wipe it up
            
            // move until we find the ;
            while ((t < length) && (incipit[t] != ';')) {
                
                // we should not find the parens before the ';' !
                // FIXME find a graceful way to exit signaling this to user
                if (incipit[t] == ')') {
                    printf("You have a ) before the ; in a tuplet!\n");
                    return i - index;
                }
                
                t++;
            }
            
            //t + 1 should point to the number
            t++; // move one char to the number
            while (( (t + t2) < length) && (incipit[t + t2] != ')')) {
                
                // If we have extraneous chars, exit here
                if (!isdigit(incipit[t + t2])) {
                    printf("You have a non-number in a tuplet number\n");
                    return i - index;
                }
                
                // copy the number char-by-char
                buf[t2] = incipit[t + t2];
                t2++;
            }
            
            tuplet_val = atoi(buf);
            free(buf); // dispose of the buffer
        
        } else { // it is a triplet
            // don't care to parse all the stuff
            tuplet_val = 3;
        }
        
        // durations where already parsed, get them from measure
        note->tuplet_duration = measure->durations[measure->durations_offset];
        // same for dots
        note->tuplet_dots = measure->dots[measure->durations_offset];
        // this is the first note, the notal number of notes = tuplet_val
        note->tuplet_notes = tuplet_val;
        // but also the note counter
        note->tuplet_note =  tuplet_val;
        
    } else {
        if ( note->tuplet_notes > 0 ) {
            std::cout << "Warning: fermata within a tuplet. Won't be handled correctly" << std::endl;
        }
        note->fermata = true;
    }
    
    return i - index;
    
}



//////////////////////////////
//
// getTupletFermataEnd --
//
// this can be deleted in the future?
int MusPaeInput::getTupletFermataEnd(const char* incipit, NoteObject *note, int index ) {
    
    int i = index;
    //int length = strlen(incipit);
        
    // TODO currently fermatas inside tuplets won't be handled correctly
    note->fermata = false;
        
    return i - index;
}



//////////////////////////////
//
// getGraceNote --
//

int MusPaeInput::getGraceNote(const char* incipit, NoteObject *note, int index ) {
    
    int i = index;
    int length = strlen(incipit);
    
    //acciaccatura
    if (incipit[i] == 'g') {
        note->acciaccatura = true;
    }
    
    // appoggiatura
    else if (incipit[i] == 'q') {
        note->appoggiatura = 1;
        if ((i+1 < length) && (incipit[i+1] == 'q')) {
            note->appoggiatura_multiple = true;
            i++;
            int r = i;
            while ((r < length) && (incipit[r] != 'r')) {
                if ((incipit[r]-'A'>=0) && (incipit[r]-'A'<7)) {
                    note->appoggiatura++;
                }
                r++;
            }
        }
    }
    return i - index;
}


//////////////////////////////
//
// getPitch --
//

int MusPaeInput::getPitch( char c_note ) {
    int pitch;
    
    switch (c_note) {
        case 'A':
            pitch = PITCH_A;
            break;
        case 'B': 
            pitch = PITCH_B;
            break;
        case 'C': 
            pitch = PITCH_C;
            break;
        case 'D': 
            pitch = PITCH_D;
            break;
        case 'E': 
            pitch = PITCH_E;
            break;
        case 'F': 
            pitch = PITCH_F;
            break;
        case 'G': 
            pitch = PITCH_G;
            break;
        case '-': pitch = 255; break;
        default:
            break;
    }
    return pitch;
}


//////////////////////////////
//
// getTimeInfo -- read the key signature.
//

int MusPaeInput::getTimeInfo( const char* incipit, MeasureObject *measure, int index) {
    
    int i = index;
    int length = strlen(incipit);
    MusMensur *meter = new MusMensur;
    
    if (!isdigit(incipit[i]) && (incipit[i] != 'c') && (incipit[i] != 'o'))
        return 0;
    
    // find the end of time signature end
    i++; // the time signature length is a least 1
    while (i < length) {
        if (!isdigit(incipit[i]) && (incipit[i] != '/') && (incipit[i] != '.')) {
            break;
        }
        i++;
    }
        
    // use a substring for the time signature 
    char timesig_str[1024];
    memset( timesig_str, 0, 1024 );
    // strncpy not always put the \0 in the end!
    strncpy( timesig_str, incipit + index, i - index); 
    
    std::ostringstream sout;
    regex_t re;
    
    // check if format X/X or one digit only
    regcomp(&re, "^[0-9]*/[0-9]*$", REG_EXTENDED);
    int is_standard = regexec(&re, timesig_str, 0, NULL, 0);
    regfree(&re);
    regcomp(&re, "^[0-9]*$", REG_EXTENDED);
    int is_one_number = regexec(&re, timesig_str, 0, NULL, 0);
    regfree(&re);
    
    if ( is_standard == 0) {
        char buf_str[1024];
        strcpy(buf_str, timesig_str);
        int beats = atoi(strtok(buf_str, "/"));
        int note_value = atoi(strtok(NULL, "/")); 
        //timeinfo[0] = (double)beats; timeinfo[1] = 4.0/(double)note_value;
        //sout << "*M" << beats << "/" << note_value;
        meter->m_num = beats;
        meter->m_numBase = note_value;
    } else if ( is_one_number == 0) {
        int beats = atoi(timesig_str);
        meter->m_num = beats;
        meter->m_numBase = 1;
        //timeinfo[0] = (double)beats; timeinfo[1] = 4.0/1.0;
        //sout << "*M" << beats << "/1\n*met(" << beats << ")";
        //std::cout << output << std::endl;
    } else if (strcmp(timesig_str, "c") == 0) {
        // C
        ////imeinfo[0] = 4.0; timeinfo[1] = 4.0/4.0;
        meter->m_meterSymb = METER_SYMB_COMMON;
    } else if (strcmp(timesig_str, "c/") == 0) {
        // C|
        //timeinfo[0] = 2.0; timeinfo[1] = 4.0/2.0;
        meter->m_meterSymb = METER_SYMB_CUT;
    } else if (strcmp(timesig_str, "c3") == 0) {
        // C3
        //timeinfo[0] = 3.0; timeinfo[1] = 4.0/1.0;
        meter->m_meterSymb = METER_SYMB_3;
    } else if (strcmp(timesig_str, "c3/2") == 0) {
        // C3/2
        //timeinfo[0] = 3.0; timeinfo[1] = 4.0/2.0;
        meter->m_meterSymb = METER_SYMB_3_CUT; // ??
    } else {
        //timeinfo[0] = 4.0; timeinfo[1] = 4.0/4.0;
        //sout << "*M4/4\n!! unknown time signature";
        std::cout << "Warning: unknown time signature: " << timesig_str << std::endl;
        
    }

    measure->time = meter;
    
    return i - index;
}



//////////////////////////////
//
// getClefInfo -- read the key signature.
//

int MusPaeInput::getClefInfo( const char *incipit, MusClef *mclef, int index ) {
    
    // a clef is maximum 3 character length
    // go through the 3 character and retrieve the letter (clef) and the line
    // mensural clef (with + in between) currently ignored
    // clef with octava correct?
    int length = strlen(incipit);
    int i = 0;
    char clef = 'G';
    char line = '2';
    while ((index < length) && (i < 3)) {
        if (i == 0) {
            clef = incipit[index];
        } else if (i == 2) {
            line = incipit[index];
        }
        i++;
        index++;
    }

    
    if (clef == 'C') {
        switch (line) {
            case '1': mclef->m_clefId = UT1; break;
            case '2': mclef->m_clefId = UT2; break;
            case '3': mclef->m_clefId = UT3; break;
            case '4': mclef->m_clefId = UT4; break;
        }
    } else if (clef == 'G') {
        switch (line) {
            case '1': mclef->m_clefId = SOL1; break;
            case '2': mclef->m_clefId = SOL2; break;
        }
    } else if (clef == 'F') {
        switch (line) {
            case '3': mclef->m_clefId = FA3; break;
            case '4': mclef->m_clefId = FA4; break;
            case '5': mclef->m_clefId = FA5; break;
        }
    } else {
        // what the...
        printf("Clef is ??\n");
    }
    
    //measure->clef = mclef;
        
    return i;
}



//////////////////////////////
//
// getWholeRest -- read the getWholeRest.
//

int MusPaeInput::getWholeRest( const char *incipit, int *wholerest, int index ) {
    
    int length = strlen(incipit);
    int i = index;
    
    *wholerest = 1;
    if ((i+1 < length) && isdigit(incipit[i+1])) {
        sscanf(&(incipit[i+1]), "%d", wholerest);
        char buf[10];
        memset( buf, 0, 10 );
        sprintf( buf, "%d", *wholerest );
        i += strlen( buf );
    }
    return i - index;
}



//////////////////////////////
//
// getBarline -- read the barline.
//

int MusPaeInput::getBarline( const char *incipit, wxString *output, int index ) {
    
    regex_t re;
    regcomp(&re, "^://:", REG_EXTENDED);
    int is_rep_db_rep = regexec(&re, incipit + index, 0, NULL, 0);
    regfree(&re);
    regcomp(&re, "^://", REG_EXTENDED);
    int is_rep_db = regexec(&re, incipit + index, 0, NULL, 0);
    regfree(&re);
    regcomp(&re, "^//:", REG_EXTENDED);
    int is_db_rep = regexec(&re, incipit + index, 0, NULL, 0);
    regfree(&re);
    regcomp(&re, "^//", REG_EXTENDED);
    int is_db = regexec(&re, incipit + index, 0, NULL, 0);
    regfree(&re);
    
    int i = 0; // number of characters
    if (is_rep_db_rep == 0) {
        *output = "=:||:";
        i = 3;
    } else if (is_rep_db == 0) {
        *output = "=:|!";
        i = 2;
    } else if (is_db_rep == 0) {
        *output = "=!|:";
        i = 2;
    } else if (is_db == 0) {
        *output = "=||";
        i = 1;
    } else {
        *output = "=";
        i = 0;
    }
    return i;
}



//////////////////////////////
//
// getAbbreviation -- read abbreviation
//

int MusPaeInput::getAbbreviation(const char* incipit, MeasureObject *measure, int index ) {
    
    int length = strlen(incipit);
    int i = index;
    int j;
    
    if (measure->abbreviation_offset == -1) { // start
        measure->abbreviation_offset = measure->notes.size();
    } else { //
        int abbreviation_stop = measure->notes.size();
        while ((i+1 < length) && (incipit[i+1]=='f')) {
            i++;
            for(j=measure->abbreviation_offset; j<abbreviation_stop; j++) {
                measure->notes.push_back( measure->notes[j] );
            }
        }
        measure->abbreviation_offset = -1;   
    }
    
    return i - index;
}



//////////////////////////////
//
// getKeyInfo -- read the key signature.
//

int MusPaeInput::getKeyInfo(const char *incipit, MeasureObject *measure, int index ) {
            
    // at the key information line, extract data
    int length = strlen(incipit);
    int i = index;
    bool end_of_keysig = false;
    while ((i < length) && (!end_of_keysig)) {
        switch (incipit[i]) {
            case 'b': measure->key_alteration = ACCID_FLAT; break;
            case 'x': measure->key_alteration = ACCID_SHARP; break;
            //case '[': paren =  1; break;
            //case ']': paren =  1; break;
            case 'F': measure->key.push_back(PITCH_F); break;
            case 'C': measure->key.push_back(PITCH_C); break;
            case 'G': measure->key.push_back(PITCH_G); break;
            case 'D': measure->key.push_back(PITCH_D); break;
            case 'A': measure->key.push_back(PITCH_A); break;
            case 'E': measure->key.push_back(PITCH_E); break;
            case 'B': measure->key.push_back(PITCH_B); break;
            default:
                end_of_keysig = true;
                //if (debugQ) {
                //  std::cout << "Warning: unknown character: " << keysig_str[i] << " in key signature " << keysig_str
                //       << std::endl;
                //exit(1);
                //}
                break;
        }
        if (!end_of_keysig)
            i++;
    }
    
    return i - index;
}



//////////////////////////////
//
// getNote --
//

int MusPaeInput::getNote( const char* incipit, NoteObject *note, MeasureObject *measure, int index ) {
    
    regex_t re;
    int oct, tie;
    int i = index;
    bool acc;
    int app;
    int tuplet_num;
    
    if (note->acciaccatura) {
        // acciaccaturas are always eights regardless
        // and have no dots
        note->duration = DUR_8;
    } else {
        note->duration = measure->durations[measure->durations_offset];
        note->dots = measure->dots[measure->durations_offset];
    }
    note->pitch = getPitch( incipit[i] );
    
    // lookout, hack. If it is a rest (255 val) then create the rest object.
    // it will be added instead of the note
    if (note->pitch == 255)
        note->rest = true;
    
    // beaming
    // detect if it is a fermata or a tuplet
    if (note->beam > 0) {
        regcomp(&re, "^[^}/]*[ABCDEFG-].*", REG_EXTENDED);
        int is_not_last_note = regexec(&re, incipit + i + 1, 0, NULL, 0);
        regfree(&re);
        //std::cout << "regexp " << is_not_last_note << std::endl;
        if ( is_not_last_note != 0 ) {
            //note->beam = -1; // close the beam
            /*
            note->beam |= BEAM_TERMINAL;
            */ // ax2.3
        }
    }
    
    // trills
    regcomp(&re, "^[^ABCDEFG]*t", REG_EXTENDED);
    int has_trill = regexec(&re, incipit + i + 1, 0, NULL, 0);
    regfree(&re);
    if ( has_trill == 0 ) {
        note->trill = true;
    }
    
    // tie
    regcomp(&re, "^[^ABCDEFG]*\\+", REG_EXTENDED);
    int has_tie = regexec(&re, incipit + i + 1, 0, NULL, 0);
    regfree(&re);
    //std::cout << "regexp " << has_tie << std::endl;
    if ( has_tie == 0) {
        if (note->tie == 0)
            note->tie = 1; // reset 1 for the first note, >1 for next ones is incremented under
    }
    
    tie = note->tie;
    // last note in tie
    if (has_tie != 0 && tie > 0)
        tie = 0;
    
    oct = note->octave;
    measure->notes.push_back( *note );
    
    acc = note->acciaccatura;
    app = note->appoggiatura;
    tuplet_num = note->tuplet_note;
    
    // Reset note to defaults
    note->clear();
    
    // write back the values we need to save

    note->octave = oct; // save octave
    
    // tuplets. Decrease the current number is we are in a tuplet
    // i.e. tuplet_num > 0
    // al the other values just need to be in the first note
    if (tuplet_num > 0)
        note->tuplet_note = --tuplet_num;
    
    // If prevous note has tie, increment tie count
    if (tie > 0)
        note->tie = ++tie;
        
    // reset values
    // beam
    //if (note->beam > 0) {
    //    note->beam++;
    //} else if (note->beam == -1) {
    //    note->beam = 0;
    //}
    // tie
    //if (note->tie == 1) {
    //    note->tie++;
    //} else if (note->tie == 2) {
    //    note->tie = 0;
    //}
    
    // grace notes
    note->acciaccatura = false;
    if (app > 0) {
        //std::cout << note->appoggiatura << std::endl; 
        note->appoggiatura = --app;
        note->appoggiatura_multiple = false;
    }
    // durations
    if (measure->durations.size() > 0) {
        measure->durations_offset++;
        if (measure->durations_offset >= measure->durations.size()) {
            measure->durations_offset = 0;
        }
    }
    
    note->fermata = false; // only one note per fermata;
    note->trill = false;
    
    return i - index;
}



//////////////////////////////
//
// printMeasure --
//

void MusPaeInput::printMeasure(std::ostream& out, MeasureObject *measure ) {
    bool tupletized = false, beamed = false;
    MusBeam *appog_beam = NULL;
    
//    m_staff = new MusStaff( 1 );
//    m_layer = new MusLayer( 1 );
    
    
    if ( measure->clef != NULL ) {
        m_layer->AddElement(measure->clef);
    }
    
    if ( measure->key.size() > 0 ) {
        MusKeySig *key = new MusKeySig(measure->key.size(), measure->key_alteration);
        m_layer->AddElement(key);
    }
    
    if ( measure->time != NULL ) {
        m_layer->AddElement(measure->time);
    }
    
    if ( measure->wholerest > 0 ) {     
        MusRest *r = new MusRest();
        r->m_dur = VALSilSpec; //
        r->m_multimeasure_dur = measure->wholerest;
        m_layer->AddElement(r);
    }
    
    for (unsigned int i=0; i<measure->notes.size(); i++) {
        NoteObject *note = &measure->notes[i];
        
        // A note can have a chef change before it
        if ( note->clef != NULL ) {
            m_layer->AddElement(note->clef);
        }
                    
        if (note->rest) {
            MusRest *r = new MusRest();
            r->m_dur = note->duration;
            r->m_pname = REST_AUTO;
            r->m_dots = note->dots;
            
            // draw a fermata?
            if (note->fermata)
                r->m_fermata = true;
            
            m_layer->AddElement(r); // create a rest
        } else {
            MusNote *n = new MusNote();
            n->m_pname = note->pitch;
            n->m_oct = note->octave;
            n->m_dur = note->duration;
            n->m_dots = note->dots;
            n->m_accid = note->accidental;
            
            // handle appoggiatura. if appoggiatura_multiple is set, it is the first
            // in this case make a beam too
            // note->appoggiatura == 1 is a SINGLE note
            // note->appoggiatura == 2..n is multiple notes
            // so if note->appoggiatura == 2 we now it is the last in a beamed appogg
            if (note->appoggiatura > 0) {
                n->m_cueSize = true;
                
                if (note->appoggiatura_multiple && note->appoggiatura > 1) {
                    appog_beam = new MusBeam;
                    m_layer->AddElement(appog_beam);
                }
                
                // if the beam was not created, this is a single note appoggiatura
                if (appog_beam)
                    appog_beam->AddElement(n);
                
                // this is the last note in appoggiatura beam, set the beam to null
                if (note->appoggiatura == 2) // last one in a beam is 2
                    appog_beam = NULL;
            }
            
            // Acciaccaturas are similar but do not get beamed (do they)
            // this case is simpler. NOTE a note can not be acciacctura AND appoggiatura
            if (note->acciaccatura) {
                n->m_cueSize = true;
                n->m_acciaccatura = true;
            }
            
            // do beaming, EXCEPT for grace notes
            // which are unbeamed in the beam / have their own beam
            // m_current_beam is global so we can have cross-measure beams
            if (!note->appoggiatura && !note->acciaccatura) {
                
                beamed = true;
                
                if (note->beam & BEAM_INITIAL) {
                    m_current_beam = new MusBeam;
                    if (note->tuplet_note < 1)
                        m_current_beam->AddElement(n);
                    
                    if (m_current_tuplet)
                        m_current_tuplet->AddElement(m_current_beam);
                    else
                        m_layer->AddElement(m_current_beam);
                    
                } else if (note->beam & BEAM_MEDIAL) {
                    if (m_current_beam && note->tuplet_note < 1)
                        m_current_beam->AddElement(n);
                    
                } else if (note->beam & BEAM_TERMINAL) {
                    m_current_beam = NULL;
                }
                // // ax2.3 LP
            }
            
            // draw a fermata?
            // this is identical in rest, maybe find a wiser solution?
            if (note->fermata)
                n->m_fermata = true;
            
            // draw a trill?
            if (note->trill == true) {
                n->m_embellishment = EMB_TRILL;
            }
            
            // FOR THE MOMENT WORKS ONLY WITH TWO NOTES
            // FIXME
            // if tie is 1, we are the first note in a tie.
            // Create it and add it to the layer
            if (note->tie == 1) {
                MusTie *tie = new MusTie;
                tie->m_first = n;
                m_layer->AddElement(tie);
                m_current_tie = tie;
            } else if (note->tie == 2) {
                if (m_current_tie)
                    m_current_tie->m_second = n;
                // If a measure is duplicated, it is possible
                // that the first note still bears a tie
                // so we make this ref null and avoid duplicate ties
                // I added a chech when copying measure
                // but nevertheless we check two times
                m_current_tie = NULL;
            }
            
            // we have a tuplet, the tuplet_note is > 0
            // which means we are counting a tuplet
            if (note->tuplet_note > 0) {
                
                tupletized = true;
                
                if (note->tuplet_notes == note->tuplet_note) { // first elem in tuplet
                    m_current_tuplet = new MusTuplet;
                    if (m_current_beam)
                        m_current_beam->AddElement(m_current_tuplet);
                    else
                        m_layer->AddElement(m_current_tuplet);
                }
                
                m_current_tuplet->AddElement(n); // place it into the array
                
                // the last note counts always '1'
                // insert the tuplet elem
                // and reset the tuplet counter
                if (note->tuplet_note == 1) {
                    m_current_tuplet = NULL;
                }
            }
            
            if (!beamed && !tupletized)
                m_layer->AddElement(n); // create a note
            
        } // note or rest

    }
    // Set barline
    // FIXME use flags for proper barline identification
    if ( measure->barline.length() ) {
        MusBarline *bline = new MusBarline;
        if (measure->barline == "=")
            bline->m_barlineType = BARLINE_SINGLE;
        else 
            bline->m_barlineType = BARLINE_DBL;
        
        m_layer->AddElement(bline);
    }
    
    //m_staff->AddLayer(m_layer);
}



//////////////////////////////
//
// getAtRecordKeyValue --
//   Formats: @key: value
//            @key:value
//            @key :value
//            @ key :value
//            @ key : value
//   only one per line
//

void MusPaeInput::getAtRecordKeyValue(char *key, char* value,
                                      const char* input) {
    
#define SKIPSPACE while((index<length)&&isspace(input[index])){index++;}
    
    char MARKER    = '@';
    char SEPARATOR = ':';
    char EMPTY     = '\0';
    
    int length = strlen(input);
    int count = 0;
    
    // zero out strings
    memset(key, EMPTY, MAX_DATA_LEN);
    memset(value, EMPTY, MAX_DATA_LEN);

    
    if (length == 0)
        return;
    
    char ch;
    int index = 0;
    
    // find starting @ symbol (ignoring any starting space)
    SKIPSPACE
    if (input[index] != MARKER) {
        // invalid record format since it does not start with @
        return;
    }
    index++;
    SKIPSPACE
    
    // start storing the key value:
    while ((index < length) && (input[index] != SEPARATOR)) {
        if (isspace(input[index])) {
            continue;
            index++;
        }
        ch = input[index]; 
        
        // Should never happen
        if (count >= MAX_DATA_LEN)
            return;
        
        key[count] = ch;
        count++;
        index++;
    }
    // check to see if valid format: (:) must be the current character
    if (input[index] != SEPARATOR) {
        key[0] = EMPTY;
        return;
    }
    index++;
    SKIPSPACE
    
    // Also should never happen
    if (strlen(&input[index]) > MAX_DATA_LEN)
        return;
    strcpy(value, &input[index]);
    
    // Thruncate string to first space
    for (int i = strlen(value) - 2; i > 0; i--) {
        if (isspace(value[i])) {
            value[i] = EMPTY;
            continue;
        }
        break;
    }
}

// md5sum: 00d5e9dedeb47c815390eac97f8c9f42 pae2kern.cpp [20050403]

