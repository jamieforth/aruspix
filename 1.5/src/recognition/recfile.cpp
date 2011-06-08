/////////////////////////////////////////////////////////////////////////////
// Name:        recfile.cpp
// Author:      Laurent Pugin
// Created:     2004
// Copyright (c) Laurent Pugin. All rights reserved.   
/////////////////////////////////////////////////////////////////////////////

#ifdef AX_RECOGNITION

#if defined(__GNUG__) && ! defined(__APPLE__)
    #pragma implementation "recfile.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/file.h"
#include "wx/filename.h"
#include "wx/txtstrm.h"

#include "recfile.h"
#include "rec.h"
#include "recmodels.h"
#include "im/impage.h"
#include "im/imstaff.h"

#include "app/axapp.h"
#include "app/axprocess.h"

#include "mus/muspage.h"
#include "mus/musstaff.h"
#include "mus/musfile.h"
#include "mus/musiobin.h"
#include "mus/musiomlf.h"

//#include "ml/mldecoder.h"


// WDR: class implementations

//----------------------------------------------------------------------------
// RecFile
//----------------------------------------------------------------------------

RecFile::RecFile( wxString name, RecEnv *env )
	: AxFile( name, AX_FILE_DEFAULT, AX_FILE_RECOGNITION )
{
	m_envPtr = env;

	m_musFilePtr = NULL;
	m_imPagePtr = NULL;
	
	m_isPreprocessed = false;
	m_isRecognized = false;
}



RecFile::~RecFile()
{
	if ( m_musFilePtr )
		delete m_musFilePtr;
	if ( m_imPagePtr )
		delete m_imPagePtr;
}


void RecFile::UpgradeTo_1_4_0()
{
	if ( AxFile::FormatVersion(m_vmaj, m_vmin, m_vrev) >= AxFile::FormatVersion(1, 4, 0) )
		return; // when do not need to upgrade the file
		
	// From version 1.4.0, they are no staff segments anymore
	// Files with staff multiple segments cannot be upgraded
	// Margin were fixed in the music editor
	// To upgrade, we neww to remap the xml file

	
	if ( !wxFileExists( m_basename + "page.wwg") )
		return; // nothing has to be done for files that were only preprocessed
		
	bool failed = false;
		
	// map the xml file
	TiXmlElement *root = NULL;
    TiXmlNode *node1 = NULL;
	TiXmlNode *node2 = NULL;
    TiXmlElement *elem1 = NULL;
	TiXmlElement *elem2 = NULL;
	
	node1 = m_xml_root->FirstChild( "impage" );
	if ( !node1 ) 
		failed = true;
	else
		root = node1->ToElement();
    if ( !root )
		failed = true;

    // staves
    for( node1 = root->FirstChild( "staff" ); node1; node1 = node1->NextSibling( "staff" ) )
    {
		if ( failed )
			break;
			
		int x1 = 0;
		int x2 = 0;
        elem1 = node1->ToElement();
        if (!elem1)
			continue;
			
		int i = 0;
		for( node2 = elem1->FirstChild( "staffsegment" ); node2; node2 = node2->NextSibling( "staffsegment" ) )
		{
			if ( i > 0 ) // Upgrade cannot handle file with multiple segments
			{
				failed = true;
				break;
			}
			i++;
			elem2 = node2->ToElement();
			if (!elem2) 
				continue;
			elem2->Attribute("x1", &x1 );
			elem2->Attribute("x2", &x2 );
			elem2->Attribute("order", &m_rec_lm_order );
			elem2->Attribute("scaling", &m_rec_lm_scaling );
		}
		elem1->SetAttribute("x1", x1 );
		elem1->SetAttribute("x2", x2 );
    }
	
	if ( failed )
	{
		wxLogError(_("This file was generated with a previous version of Aruspix and the recognition results cannot be loaded"));
		this->CancelRecognition( false );
	}
	else
		wxLogDebug("File successfully upgraded to 1.4.0");
}

void RecFile::UpgradeTo_1_5_0()
{
	if ( AxFile::FormatVersion(m_vmaj, m_vmin, m_vrev) >= AxFile::FormatVersion(1, 5, 0) )
		return; // when do not need to upgrade the file
		
	// From version 1.5.0, we don't use wwg anymore, but aruspix binaries
    // We need to reload the page from the mlf
	
	if ( !wxFileExists( m_basename + "page.wwg") )
		return; // nothing has to be done for files that were only preprocessed
    // backup rec.mlf
	if ( !wxCopyFile( m_basename + "rec.mlf", m_basename + "rec.old.mlf") )  
        return;
    // we need to read page.mlf 
	if ( !wxCopyFile( m_basename + "page.mlf", m_basename + "rec.mlf", true) )  
        return;
        
    wxArrayPtrVoid params;
    
    if ( !this->RealizeFromMLF( params, NULL ) )
        return;
	if ( !wxCopyFile( m_basename + "rec.old.mlf", m_basename + "rec.mlf", true) )  
        return;
        
    // output the new binary file
    MusBinOutput bin_output( m_musFilePtr, m_musFilePtr->m_fname );
    bin_output.ExportFile();
        
    wxRemoveFile( m_basename + "rec.old.mlf" );        
    wxRemoveFile( m_basename + "page.wwg" );
}

void RecFile::NewContent( )
{
	wxASSERT_MSG( !m_imPagePtr, "ImPage should be NULL" );
	wxASSERT_MSG( !m_musFilePtr, "MusFile should be NULL" );

	// new MusFile
    m_musFilePtr = new MusFile();
    m_musFilePtr->m_fname = m_basename + "page.bin";  
        
	// new ImPage and Load
    m_imPagePtr = new ImPage( m_basename, &m_isModified );

	m_pre_image_binarization_method = ImOperator::s_pre_image_binarization_method;
	m_pre_page_binarization_method =  ImPage::s_pre_page_binarization_method;
	m_pre_page_binarization_method_size = ImPage::s_pre_page_binarization_method_size;
	
	m_imPagePtr->m_pre_image_binarization_methodPtr = &m_pre_image_binarization_method;
	m_imPagePtr->m_pre_page_binarization_methodPtr = &m_pre_page_binarization_method;
	m_imPagePtr->m_pre_page_binarization_method_sizePtr = &m_pre_page_binarization_method_size;
	
	m_isPreprocessed = false;
	m_isRecognized = false;
}


void RecFile::OpenContent( )
{
	this->NewContent();
	
	TiXmlElement *root = NULL;
    TiXmlNode *node = NULL;
	
	UpgradeTo_1_4_0();
	
	// start
    bool failed = false;
	
	failed = !m_imPagePtr->Load( m_xml_root );
	if ( failed )
		return;
	else
		m_isPreprocessed = true;
        
    UpgradeTo_1_5_0();
	
    // binarization variables	
    node = m_xml_root->FirstChild( "binarization" );
    if ( node )
    {
        root = node->ToElement();
        if ( !root ) return;
            
        if ( root->Attribute( "pre_image_binarization_method" ) )
            RecFile::m_pre_image_binarization_method = atoi( root->Attribute( "pre_image_binarization_method" ) );
        if ( root->Attribute( "pre_page_binarization_method" ) )
            RecFile::m_pre_page_binarization_method = atoi( root->Attribute( "pre_page_binarization_method" ) );
        if ( root->Attribute( "pre_page_binarization_method_size" ) )
            RecFile::m_pre_page_binarization_method_size = atoi( root->Attribute( "pre_page_binarization_method_size" ) );
    }
		
	if ( wxFileExists( m_basename + "page.bin") )
	{
		MusBinInput *bin_input = new MusBinInput( m_musFilePtr, m_musFilePtr->m_fname );
		failed = !bin_input->ImportFile();
		delete bin_input;
		if ( failed )
			return;
		else
			m_isRecognized = true;	
	}
	
	return;       
	// Load XML file. Don't know what's necessary yet...
	/*
    TiXmlDocument dom( m_xmlfile.c_str() );
    bool ok = dom.LoadFile();

    if ( !ok )
    {
        wxLogWarning(_("Unable to load file '%s'"), m_xmlfile.c_str() );
        return;
    }

    TiXmlNode *node = NULL;
    TiXmlElement *elem = NULL;

    TiXmlElement *root = dom.RootElement();
	if ( !root )
    {
        wxLogError( _("Unknown error" ) );
        return;
    }

	// models
	node = root->FirstChild("models");
    if ( node && (elem = node->ToElement()) && elem)
	{
		m_rec_typ_model = elem->Attribute("typographic_model");
		m_rec_mus_model = elem->Attribute("music_model");
	}

	// decoder
	node = root->FirstChild("decoder");
    if ( node && (elem = node->ToElement()) && elem)
	{
		m_rec_wrdtrns = elem->Attribute("wrdtrns");
		int val;
		elem->Attribute("lm_delayed", &val );
		m_rec_delayed = (val > 0) ? true : false;
		elem->Attribute("order", &m_rec_lm_order );
		elem->Attribute("scaling", &m_rec_lm_scaling );
	}
	*/
}

void RecFile::SaveContent( )
{
	wxASSERT_MSG( m_imPagePtr, "ImPage should not be NULL" );
	wxASSERT_MSG( m_musFilePtr, "MusFile should not be NULL" );
	wxASSERT( m_xml_root );
		
	if ( !m_isPreprocessed )
		return;
	else {
		m_imPagePtr->Save( m_xml_root ); // save in the RecFile directory		
		
		// binarization variables
		TiXmlElement binarization( "binarization" );
		binarization.SetAttribute( "pre_image_binarization_method", RecFile::m_pre_image_binarization_method );
		binarization.SetAttribute( "pre_page_binarization_method", RecFile::m_pre_page_binarization_method );
		binarization.SetAttribute( "pre_page_binarization_method_size", RecFile::m_pre_page_binarization_method_size );
		m_xml_root->InsertEndChild( binarization );
	}

	if ( !m_isRecognized )
		return;
	else
	{
		
		// save
		MusBinOutput *bin_output = new MusBinOutput( m_musFilePtr, m_musFilePtr->m_fname );
		bin_output->ExportFile();
		delete bin_output;
		
		MusMLFOutput *mlfoutput = new MusMLFOutput( m_musFilePtr, m_basename + "page.mlf", NULL );
		mlfoutput->m_pagePosition = true;
		mlfoutput->WritePage( &m_musFilePtr->m_pages[0] , "staff", m_imPagePtr );
		delete mlfoutput;
	
		TiXmlElement root( "recpage" );
    
		// models
		TiXmlElement models( "models" );
		models.SetAttribute( "typographic_model",  m_rec_typ_model.c_str() );
		models.SetAttribute( "music_model",  m_rec_mus_model.c_str() );	
		root.InsertEndChild( models );
		
		// decoder
		TiXmlElement decoder( "decoder" );
		decoder.SetAttribute( "wrdtrns",  m_rec_wrdtrns.c_str() );
		decoder.SetAttribute( "lm_delayed",  m_rec_delayed );
		decoder.SetAttribute( "order",  m_rec_lm_order );
		decoder.SetDoubleAttribute( "scaling",  m_rec_lm_scaling );
		root.InsertEndChild( decoder );
			
		m_xml_root->InsertEndChild( root );
	}
}

void RecFile::CloseContent( )
{
	// nouveau fichier ?
    if ( m_musFilePtr )
    {
        delete m_musFilePtr;
        m_musFilePtr = NULL;
    }

	// old ImPage
    if ( m_imPagePtr )
    {
        delete m_imPagePtr;
        m_imPagePtr = NULL;
    }
	
	m_isPreprocessed = false;
	m_isRecognized = false;
}

void RecFile::SetBinarization( int image_binarization_method, int page_binarization_method, int page_binarization_size ){
	this->m_pre_image_binarization_method = image_binarization_method;
	this->m_pre_page_binarization_method = page_binarization_method;
	this->m_pre_page_binarization_method_size = page_binarization_size;
}

// static
bool RecFile::IsRecognized( wxString filename )
{
	return AxFile::ContainsFile( filename, "page.bin"  ) || AxFile::ContainsFile( filename, "page.wwg"  ) ; // page.wwg for files before 1.5
}

void RecFile::GetImage1( AxImage *image )
{
	wxASSERT_MSG( image, "AxImage cannot be NULL" );
	wxASSERT_MSG( m_imPagePtr, "imPagePtr cannot be NULL" );
	wxASSERT_MSG( m_imPagePtr->m_img0, "Img0 cannot be NULL" );
	
	// for backwards compatibility, return m_img0 if m_img1 doesn't exists
	if ( m_imPagePtr->m_img1 )
		SetImImage( m_imPagePtr->m_img1, image );
	else
		SetImImage( m_imPagePtr->m_img0, image );
}

void RecFile::GetImage0( AxImage *image )
{
	wxASSERT_MSG( image, "AxImage cannot be NULL" );
	wxASSERT_MSG( m_imPagePtr, "imPagePtr cannot be NULL" );
	wxASSERT_MSG( m_imPagePtr->m_img0, "Img0 cannot be NULL" );
	
	SetImImage( m_imPagePtr->m_img0, image );
}

void RecFile::SetImage( wxString filename )
{

}


void RecFile::SetImage( AxImage *image )
{

}

bool RecFile::CancelRecognition( bool ask_user )
{
	if ( ask_user )
	{	
		wxString msg = wxString::Format(_("This will erase previous recognition results. Do you want to continue ?") );
		int res = wxMessageBox( msg, wxGetApp().GetAppName() , wxYES_NO | wxICON_QUESTION );
		if ( res != wxYES )
			return false;
	}

	wxArrayString names;
	wxDir::GetAllFiles( m_basename, &names , "staff*", wxDIR_FILES );
	for( int i = names.GetCount() - 1; i >=0; i-- )
		wxRemoveFile( names[i] );
	wxRemoveFile( m_basename + "rec.mlf" );
	wxRemoveFile( m_basename + "rec.xml" );
	wxRemoveFile( m_basename + "mfc.input" );
	wxRemoveFile( m_basename + "page.bin" );
	wxRemoveFile( m_basename + "page.mlf" );
	wxRemoveFile( m_basename + "staves.tif" );
	
	m_isRecognized = false;
	m_isModified = true;
	return true;
	
}

void RecFile::WriteNoPitchMLF(  )
{
	if ( !m_isRecognized )
		return;

	MusMLFOutputNoPitch *mlfoutputwp = new MusMLFOutputNoPitch( m_musFilePtr, m_basename + "pagewp.mlf", NULL );
	mlfoutputwp->m_pagePosition = true;
	//mlfoutputwp->CreateSubFile();
	mlfoutputwp->WritePage( &m_musFilePtr->m_pages[0] , "" );//wxString("staff") );
	delete mlfoutputwp;
	
	wxFileInputStream input( m_basename + "pagewp.mlf" );
	if ( !input.Ok() )
		return;
		
	wxTextInputStream text( input );
	wxFile output;

	while( !input.Eof() )
	{
		wxString str = text.ReadLine();
		if ( str.IsEmpty() || ( str[0]=='#' ) )
			continue; // skip line
		else if ( str[0]=='"' )
		{
			wxASSERT( !output.IsOpened() );
			wxString out = str.AfterLast('_').BeforeLast('.');
			output.Open( m_basename + "staff_" + out + ".nplab", wxFile::write );
		}
		else if ( str[0]=='.' )
		{
			wxASSERT( output.IsOpened() );
			output.Close();
		}
		else // write content
		{
			wxASSERT( output.IsOpened() );
			output.Write( str + "\n" );
		}
	}
}

// functors

bool RecFile::Preprocess( wxArrayPtrVoid params, AxProgressDlg *dlg )
{
	wxASSERT_MSG( dlg, "AxProgressDlg cannot be NULL" );
	
    // params 0: wxString: output_dir
	wxString image_file = *(wxString*)params[0];
	
	m_imPagePtr->SetProgressDlg( dlg );

    wxString ext, shortname;
    wxFileName::SplitPath( image_file, NULL, &shortname, &ext );

    dlg->SetMaxJobBar( 17 );
    dlg->SetJob( shortname );
    wxYield();
		
    bool failed = false;
		
    if ( RecEnv::s_check && !failed ) 
        failed = !m_imPagePtr->Check( image_file, 2500, 1200 ); // 2 operations max
	
	//// TEST
	//m_error = m_imPagePtr->GetError();
	//this->m_isPreprocessed = true;
	//return true;
	
    if ( RecEnv::s_deskew && !failed ) 
        failed = !m_imPagePtr->Deskew( 10.0 ); // 2 operations max
    //op.m_inputfile = output + "/deskew." + shortname + ".tif";
        
    if ( RecEnv::s_staves_position && !failed ) 
        failed = !m_imPagePtr->FindStaves( 3, 50 );  // 4 operations max
    //op.m_inputfile = output + "/resize." + shortname + ".tif";

    if ( RecEnv::s_binarize_and_clean  && !failed ) 
        failed = !m_imPagePtr->BinarizeAndClean( );  // 3 operations max

    if ( RecEnv::s_find_borders && !failed ) 
        failed = !m_imPagePtr->FindBorders( );  // 1 operation max
    //op.m_inputfile = output + "/border." + shortname + ".tif";
        
    if ( RecEnv::s_find_ornate_letters && !failed ) 
        failed = !m_imPagePtr->FindOrnateLetters( ); // 1 operation max

    if ( RecEnv::s_find_text_in_staves && !failed ) 
        failed = !m_imPagePtr->FindTextInStaves( ); // 1 operation max

    if ( RecEnv::s_find_text && !failed ) 
        failed = !m_imPagePtr->FindText( ); // 1 operation max
		
	if (!failed)
		this->m_isPreprocessed = true;
			
	m_error = m_imPagePtr->GetError();
	
	return !failed;
}

bool RecFile::Recognize( wxArrayPtrVoid params, AxProgressDlg *dlg )
{
	wxASSERT_MSG( dlg, "AxProgressDlg cannot be NULL" );
	
    // params 0: RecTypModel: typModelPtr
	// params 1: RecMusModel: musModelPtr
	// params 2: bool: rec_delayed
	// params 3: int: rec_lm_order
	// params 4: double: rec_lm_scaling
	// params 5: wxString: rec_wrdtrns
	
	//if (this->m_isRecognized)
	//	return true;
	
	wxArrayPtrVoid mfc_params;
	wxString file = "";
	mfc_params.Add( &file );
	
	m_imPagePtr->SetProgressDlg( dlg );
	
    dlg->SetMaxJobBar( 5 );
    dlg->SetJob( m_shortname );
    wxYield();	
	
    bool failed = false;
	
    if ( !failed ) 
        failed = !m_imPagePtr->ExtractStaves( ); // 1 operation
	
    if ( RecEnv::s_save_images && !failed )
        failed = !m_imPagePtr->SaveStaffImages( );

    if ( !failed )
        failed = !this->GenerateMFC( mfc_params, dlg ); // 2 operations
	
	// just pre-classification
	/*
	wxLogMessage("Modified method RecFile::Recognize Pre-classification");
	if ( RecEnv::s_binarize_and_clean  && !failed ) 
        failed = !m_imPagePtr->BinarizeAndClean( );

    if ( RecEnv::s_find_borders && !failed ) 
        failed = !m_imPagePtr->FindBorders( );
    //op.m_inputfile = output + "/border." + shortname + ".tif";
        
    if ( RecEnv::s_find_ornate_letters && !failed ) 
        failed = !m_imPagePtr->FindOrnateLetters( );

    if ( RecEnv::s_find_text_in_staves && !failed ) 
        failed = !m_imPagePtr->FindTextInStaves( );

    if ( RecEnv::s_find_text && !failed ) 
        failed = !m_imPagePtr->FindText( );
	
	wxArrayString names;
	wxDir::GetAllFiles( m_basename, &names , "staff*", wxDIR_FILES );
	for( int i = names.GetCount() - 1; i >=0; i-- )
		wxRemoveFile( names[i] );
	wxRemoveFile( m_basename + "rec.mlf" );
	wxRemoveFile( m_basename + "rec.xml" );
	wxRemoveFile( m_basename + "mfc.input" );
	wxRemoveFile( m_basename + "staves.tif" );
		
    if ( !failed ) 
        failed = !m_imPagePtr->StaffSegments( ); // 1 operation
	
    if ( RecEnv::s_save_images && !failed )
        failed = !m_imPagePtr->SaveSegmentsImages( );

    if ( !failed )
        failed = !this->GenerateMFC( mfc_params, dlg ); // 2 operations
		
	return !failed;
	*/
    
	if ( !failed )
        failed = !this->Decode( params, dlg );
 
    if ( !failed )
		failed = !this->RealizeFromMLF( params, dlg );
	
	if (!failed)
		this->m_isRecognized = true;
	
	if (!failed)	
		this->Modify();

	
	return !failed;
}


bool RecFile::Decode( wxArrayPtrVoid params, AxProgressDlg *dlg )
{
	wxASSERT_MSG( m_imPagePtr, "ImPage cannot be NULL" );
	wxASSERT_MSG( dlg, "AxProgressDlg cannot be NULL" );
	
    // params 0: RecTypModel: typModelPtr
	// params 1: RecMusModel: musModelPtr
	// params 2: bool: rec_delayed
	// params 3: int: rec_lm_order
	// params 4: double: rec_lm_scaling
	// params 5: wxString: rec_wrdtrns
	
	RecTypModel *typModelPtr = (RecTypModel*)params[0];
	RecMusModel *musModelPtr = (RecMusModel*)params[1];
	bool rec_delayed =  *(bool*)params[2];
	int rec_lm_order =  *(int*)params[3];
	if ( rec_lm_order > MUS_NGRAM_ORDER )
		rec_lm_order = MUS_NGRAM_ORDER;
	double rec_lm_scaling =  *(double*)params[4];
	wxString rec_wrdtrns =  *(wxString*)params[5];
	
	wxString input = m_basename + "mfc.input";
	input.Replace( "\\/", "/" );
	wxString rec_models = typModelPtr->m_basename + "hmm";
	wxString rec_dict = typModelPtr->m_basename + "dic";
	wxString rec_lm = musModelPtr->m_basename + "ngram.gram";
	wxString rec_output = m_basename + "rec.mlf";
	// cannot be changed from interface
	double rec_phone_pen = RecEnv::s_rec_phone_pen;
	double rec_int_prune = RecEnv::s_rec_int_prune;
	double rec_word_pen = RecEnv::s_rec_word_pen;
	
	/*
	wxString log = wxGetApp().m_logDir + "/decoder.log";
	
	MlDecoder *decoder = new MlDecoder( input, rec_models, rec_dict );

	decoder->log_fname = log;
	decoder->am_models_fname = rec_models;
	decoder->am_sil_phone = "{s}";
	decoder->am_phone_del_pen = rec_phone_pen;

	decoder->lex_dict_fname = rec_dict;

	if ( rec_lm_order && !rec_lm.IsEmpty() )
	{
		decoder->lm_fname = musModelPtr->m_basename + "ngram.gram";
		decoder->lm_ngram_order = rec_lm_order;
		decoder->lm_scaling_factor = rec_lm_scaling;
	}
	
	if ( rec_int_prune != 0.0 )
		decoder->dec_int_prune_window = rec_int_prune;
		
	if ( rec_word_pen != 0.0 )
		decoder->dec_word_entr_pen = rec_word_pen;

	if ( rec_delayed )
		decoder->dec_delayed_lm = true;
	
	if ( !rec_output.IsEmpty() )
		decoder->output_fname = rec_output;

	if ( !rec_wrdtrns.IsEmpty() )
		decoder->wrdtrns_fname = rec_wrdtrns;

	decoder->Create();
	decoder->Run();
	
	//wxMilliSleep( 10000 );
	
	while  ( decoder->IsAlive()  )
	{
		wxMilliSleep( 200 );
		if( !dlg->IncTimerOperation( ) )
		{
				//process->m_deleteOnTerminate = true;
				//process->m_canceled = true;
				//wxKill( pid, wxSIGKILL );
				decoder->Delete(); 
				return this->Terminate( ERR_CANCELED );
		}
	}
	*/
	
#ifdef __WXMSW__
	#if defined(_DEBUG)
		wxString cmd = "Decoder.exe";
	#else
		wxString cmd = "Decoder.exe";
	#endif   
#elif __WXGTK__
	#if defined(__DEBUG__)
		wxString cmd = "decoderd";
	#else
		wxString cmd = "decoder";
	#endif   
#elif __WXMAC__
	#ifdef __AXDEBUG__
		wxString cmd = "decoderd";
	#else
		wxString cmd = "decoder";
	#endif   
#endif

	wxString args = " ";
	
	wxString log = wxGetApp().m_logDir + "/decoder.log";
	
	args << " -am_models_fname " << "\"" << rec_models.c_str() << "\"";
	args << " -am_sil_phone \"{s}\" ";
	args << " -am_phone_del_pen " << rec_phone_pen;

	args << " -lex_dict_fname " << "\"" << rec_dict.c_str() << "\"";

	if ( rec_lm_order && !rec_lm.IsEmpty() )
	{
		args << " -lm_fname " << "\"" << rec_lm << "\"";
		args << " -lm_ngram_order " << rec_lm_order;
		args << " -lm_scaling_factor " << rec_lm_scaling;
	}
	
	if ( rec_int_prune != 0.0 )
		args << " -dec_int_prune_window " << rec_int_prune;
		
	if ( rec_word_pen != 0.0 )
		args << " -dec_word_entr_pen " << rec_word_pen;

	if ( rec_delayed )
		args << " -dec_delayed_lm";

	args << " -input_fname " << "\"" << input.c_str() << "\"";
	
	if ( !rec_output.IsEmpty() )
		args << " -output_fname " << "\"" << rec_output.c_str() << "\"";

	if ( !rec_wrdtrns.IsEmpty() )
		args << " -wrdtrns_fname " << "\"" << rec_wrdtrns.c_str() << "\"";

	wxLogDebug(args);

	if (!dlg->SetOperation( _("Recognition...") ) )
		return this->Terminate( ERR_CANCELED );

	dlg->StartTimerOperation( TIMER_DECODING, m_imPagePtr->GetStaffCount() );
	int pid;
	AxProcess *process = new AxProcess( cmd, args, NULL );
	process->SetLog( log );
	if ( process->Start() )
	{
		pid = process->GetPid();
		while  ( !process->HasEnded() )
		{
			if( !dlg->IncTimerOperation( ) )
			{
				wxKill( pid, wxSIGKILL );
				wxYield();
				delete process; 
				return this->Terminate( ERR_CANCELED );
			}
			wxMilliSleep( 200 );
		}
		
	}
	wxYield(); // flush termination event before deleting the process.
	if ( process->m_status != 0 )
	{
		delete process;
		return this->Terminate( ERR_UNKNOWN );
	}
	dlg->EndTimerOperation( TIMER_DECODING );	
	delete process;



	/*Torch::DiskXFile::setBigEndianMode() ;

	wxString input = wxGetApp().m_workingDir + "/" + imPage->GetShortName() + ".input";
	input.Replace( "\\/", "/" );

	Torch::LexiconInfo lex_info( m_rec_models.c_str() , "{s}" , "" , m_rec_dict.c_str() , 
                          "" , "" , "" ) ;

    Torch::PhoneModels phone_models ( lex_info.phone_info , (char*) m_rec_models.c_str(),
							   true , m_rec_phone_pen , 
                               false , "" , "" , 
                               9 , "" , false , 
                               (real)0.005 , (real)0.005 ) ;

    Torch::LinearLexicon lexicon( &lex_info , &phone_models ) ;

    Torch::LanguageModel *lang_model;
    if ( m_rec_lm_order <= 0 )
        lang_model = NULL ;
    else
    {
        lang_model = new Torch::LanguageModel( m_rec_lm_order , lex_info.vocabulary , 
			(char*)m_rec_lm.c_str() , m_rec_ls_scaling ) ;
    }

	real end_prune = m_rec_end_prune;
	if ( end_prune == 0 )
		end_prune = LOG_ZERO;
    Torch::BeamSearchDecoder bs_decoder( &lexicon , lang_model , m_rec_word_pen ,
			LOG_ZERO, end_prune ,
			m_rec_delayed , false ) ;


    Torch::DecoderBatchTest batch_tester( (char*)input.c_str() , Torch::DST_FEATS_HTK , (char*)m_rec_wrdtrns.c_str()  , &bs_decoder , 
                                   true , true , (char*)m_rec_output.c_str() , false , 10.0 ) ;
    	
	batch_tester.run() ;

    if ( lang_model != NULL )
        delete lang_model ; 
    return(0) ;*/

	return true;
}

bool RecFile::RealizeFromMLF( wxArrayPtrVoid params, AxProgressDlg *dlg )
{
    wxASSERT_MSG( m_imPagePtr , "Page cannot be NULL");
    wxASSERT_MSG( m_musFilePtr , "MusFile cannot be NULL");
	//wxASSERT_MSG( dlg, "AxProgressDlg cannot be NULL" );
	
	if (dlg && !dlg->SetOperation( _("Load results...") ) )
		return this->Terminate( ERR_CANCELED );

    MusPage *musPage = new MusPage();

	// deprecated, now always replace first page
    /*if ( m_musFilePtr->m_fheader.nbpage == 0) // premiere page
    {
        m_musFilePtr->m_fheader.param.pageFormatHor = m_imPagePtr->m_size.GetWidth() / 10;
        m_musFilePtr->m_fheader.param.pageFormatVer = m_imPagePtr->m_size.GetHeight() / 10;
    }
    else 
    {   
        if ( m_musFilePtr->m_fheader.param.pageFormatHor < m_imPagePtr->m_size.GetWidth() / 10 )
            m_musFilePtr->m_fheader.param.pageFormatHor = m_imPagePtr->m_size.GetWidth() / 10;
        if ( m_musFilePtr->m_fheader.param.pageFormatVer < m_imPagePtr->m_size.GetHeight() / 10 )
            m_musFilePtr->m_fheader.param.pageFormatVer = m_imPagePtr->m_size.GetHeight() / 10;
    }*/
	m_musFilePtr->m_pages.Clear();
	m_musFilePtr->m_fheader.param.pageFormatHor = m_imPagePtr->m_size.GetWidth() / 10;
	m_musFilePtr->m_fheader.param.pageFormatVer = m_imPagePtr->m_size.GetHeight() / 10;

    int x1 = 5, x2 = 195;
    m_imPagePtr->CalcLeftRight( &x1, &x2 ); 
	x1 = 0; // force it, indentation will be calculated staff by staff
    m_musFilePtr->m_fheader.param.MargeGAUCHEIMPAIRE = x1 / 10;
    m_musFilePtr->m_fheader.param.MargeGAUCHEPAIRE = x1 / 10;
    musPage->lrg_lign = (x2 - x1) / 10;

    int nb = (int)m_imPagePtr->m_staves.GetCount();
    int previous = 0;
    ImStaff *imStaff;   
    for (int i = 0; i < nb; i++)
    {
        imStaff = &m_imPagePtr->m_staves[i];
        MusStaff *musStaff = new MusStaff();
        musStaff->no = nb;
        musStaff->indent = imStaff->CalcIndentation( x1 );
        musStaff->ecart = (m_imPagePtr->ToViewY( imStaff->m_y ) -  previous ) / musPage->defin;
            //imStaff->CalcEcart( previous ) / musPage->defin;
        musStaff->vertBarre = DEB_FIN;
        //musStaff->brace = DEB_FIN;
        previous += musStaff->ecart * musPage->defin;
        musPage->m_staves.Add( musStaff );
    }   
    m_musFilePtr->m_pages.Add( musPage );
    m_musFilePtr->CheckIntegrity();
	
	///// FAKE JUST FOR COMPILATION
	wxString m_rec_output = m_basename + "rec.mlf";
	
    MusMLFInput *mlfinput = new MusMLFInput( m_musFilePtr, m_rec_output );
    mlfinput->ReadPage( musPage, true, m_imPagePtr );
    delete mlfinput;
	
	// save ????
    // MusBinOutput *bin_output = new MusBinOutput( m_musFilePtr, m_musFilePtr->m_fname );
    //bin_output->ExportFile();
    //delete bin_output;

    return true;
}

bool RecFile::GenerateMFC( wxArrayPtrVoid params, AxProgressDlg *dlg )
{
	// params 0: wxString: output_dir
	
	wxString output_dir = *(wxString*)params[0];
	
    wxASSERT_MSG( m_imPagePtr , "Page cannot be NULL");
	wxASSERT_MSG( dlg, "AxProgressDlg cannot be NULL" );
	
	m_imPagePtr->SetProgressDlg( dlg );

	// start
    bool failed = false;

	// if (m_imPagePtr->m_staff_height == 0) non, le faire de toute facon
	// la position verticale des portee n'a pas encore
	// ete calculee - un peu merdique de le faire ici ...
	//{
	if ( !failed )
		failed = !m_imPagePtr->StaffCurvatures();

	//if ( !failed ) 
	// attention, supprime les valeurs de reconnaissance dans le fichier xml
	// normalement, elles n'existe pas puisque m_staff_height n'a pas ete calcule
	//	failed = !m_imPagePtr->Save( m_basename + "img0.xml" );
	//}

    if ( !failed )
        failed = !m_imPagePtr->GenerateMFC( output_dir );

	//if ( !failed )
    //    failed = !m_imPagePtr->GenerateLyricMFC( output_dir );
	
	m_error = m_imPagePtr->GetError();

	return !failed;
}

// WDR: handler implementations for RecFile

#endif //AX_RECOGNITION
