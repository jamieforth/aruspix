// Copyright (C) 2005 Laurent Pugin (laurent.pugin@lettres.unige.ch)
//
// All rights reserved.

#include "Vocabulary.h"
#include "CmdLine.h"


#include "LMNGramBuilder.h"

#ifdef WIN32
	#include <io.h>
#else
	#include <unistd.h>
#endif

using namespace Torch ;

// Lexicon Parameters
char *lex_sent_start_word=NULL ;
char *lex_sent_end_word=NULL ;
char *lex_sil_word=NULL ;

#define SP_START "SP_START"
#define SP_END "SP_END"
#define SP_WORD "SP"

// Language Model Parameters
int lm_ngram_order=0 ;
char *input_wrdtrns_fname=NULL ;
char *input_dict_fname=NULL ;
char *output_fname=NULL ;
char *data_fname=NULL ;
char *log_fname=NULL ;
char *reload_data_fname=NULL ;

char *end_fname=NULL ; // file to notify the end of the process

bool dec_verbose=false ;


void processCmdLine( CmdLine *cmd , int argc , char *argv[] )
{
    // Lexicon Parameters
    cmd->addText("\nGrammar Options:") ;
    cmd->addSCmdArg( "-input_wrdtrns_fname" , &input_wrdtrns_fname ,
                        "the file containing word-level reference transcriptions" ) ;
    cmd->addSCmdArg( "-input_dict_fname" , &input_dict_fname ,
                        "the dictionary file" ) ;
    cmd->addICmdArg( "-lm_ngram_order" , &lm_ngram_order ,
                        "the ngram order" ) ;
	cmd->addSCmdArg( "-output_fname" , &output_fname ,
                        "the output file" ) ;

    // START and END words
	cmd->addSCmdOption( "-lex_sent_start_word" , &lex_sent_start_word , SP_START ,
                        "the name of the dictionary word that will start every sentence" ) ;
    cmd->addSCmdOption( "-lex_sent_end_word" , &lex_sent_end_word , SP_END ,
                        "the name of the dictionary word that will end every sentence" ) ;
					
	// reload file
    cmd->addSCmdOption( "-reload_data_fname" , &reload_data_fname , "" ,
                        "the name of the file containing data that have to be reloaded" ) ;
    
	// output
	cmd->addSCmdOption( "-data_fname" , &data_fname , "" ,
                        "the data output file" ) ;
	cmd->addSCmdOption( "-log_fname" , &log_fname , "",
                        "the log output file, standard output if none" ) ;
	cmd->addSCmdOption( "-end_fname" , &end_fname , "", 
						"File used to notify the end of the process. Used to avoid a bug in Mac 10.5 that cannot be fixed" ) ;

    cmd->read( argc , argv ) ;
        
    // Basic parameter checks
    if ( lm_ngram_order < 1 )
        error("invalid ngram order\n") ;
}


int main( int argc , char *argv[] )
{
    CmdLine cmd ;
    DiskXFile::setBigEndianMode() ;

    processCmdLine( &cmd , argc , argv ) ;
	
	// log file
	int saved_stdout = 0;
	//FILE *out_fd = NULL;
	if ( strlen( log_fname ) )
	{
		// redirect stdout if output file is supplied
		saved_stdout = dup( fileno(stdout) );
		FILE *out_fd = freopen( log_fname, "a", stdout );
		if ( out_fd == NULL )
		{
			dup2(saved_stdout, fileno(stdout) );
			close(saved_stdout);
			warning("Opening log file failed") ;
		}
	}

    Vocabulary vocabulary ( input_dict_fname , lex_sent_start_word , 
                                            lex_sent_end_word , lex_sil_word ) ;

	LMNGramBuilder ngram( lm_ngram_order, &vocabulary );
	
	if ( strlen( reload_data_fname ) )
		ngram.reloadFile( reload_data_fname );
		
	ngram.loadFile( input_wrdtrns_fname, output_fname, data_fname);
	
	if ( strlen( end_fname ) )
	{
		FILE *out_fd = fopen( end_fname, "w"  );
		fclose( out_fd );
	}
 
    return(0) ;
}

