/////////////////////////////////////////////////////////////////////////////
// Name:        cmpmlf.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.   
/////////////////////////////////////////////////////////////////////////////

#ifdef AX_RECOGNITION
		#ifdef AX_COMPARISON

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#include "wx/filename.h"
#include "wx/textfile.h"
#include "wx/tokenzr.h"
#include "wx/txtstrm.h"

#include "cmpmlf.h"

#include "im/impage.h"
#include "im/imstaff.h"

#include "musstaff.h"
//#include "musnote.h"
//#include "mussymbol.h"

//----------------------------------------------------------------------------
// CmpMLFSymb
//----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(CmpMLFSymb, MusMLFSymbol)

CmpMLFSymb::CmpMLFSymb( ) :
	MusMLFSymbol()
{
}

void CmpMLFSymb::SetValue( char type, wxString subtype, int position, int value, char pitch, int oct, int flag)
{
	if ( flag & NOTE_STEM )
		flag -= NOTE_STEM;
	MusMLFSymbol::SetValue( type, subtype, position, value, pitch, oct, flag );
}


wxString CmpMLFSymb::GetLabel( )
{
	if ( !m_cache.IsEmpty() )
		return m_cache;
		
	if ( m_type == 0 )
	{
		wxLogWarning( _("Uninitialized symbol") );
		return "";
	}

	wxString label, label_width;
	label << m_type;

	if ((m_type == TYPE_NOTE) || (m_type == TYPE_REST))
	{	
		label << "_" << m_value; 
		if (m_type == TYPE_NOTE) // ignore pitch and oct with rest for comparison 	
			label << "_" << m_pitch << "_" << m_oct;
		if ( m_flag )
			label << "_" << m_flag;
	}
	else if (m_type == TYPE_KEY)
	{
		label << "_" << m_subtype << m_value;
	}
	else if  (m_type == TYPE_ALTERATION)
	{
		label << "_" << m_subtype << m_value << "_" << m_pitch << "_" << m_oct;
	}
	else if ((m_type == TYPE_CUSTOS) || (m_type == TYPE_POINT))
	{
		label << "_" << m_pitch << "_" << m_oct;
	}
	else if ((m_type == TYPE_MESURE) || (m_type == TYPE_SYMBOLE))
	{
		label << "_" << m_subtype;
	}
	
	m_cache = label;
	return label;
}

//----------------------------------------------------------------------------
// CmpMLFOutput
//----------------------------------------------------------------------------

CmpMLFOutput::CmpMLFOutput( MusDoc *file, wxString filename, wxString model_symbol_name ) :
    MusMLFOutput(  file, filename, NULL, model_symbol_name  )
{
	// temporary, cannot be modified, but should be...
	m_ignore_clefs = true;
	m_ignore_custos = true;
	m_ignore_keys = true;
}

//CmpMLFOutput::CmpMLFOutput( MusDoc *file, wxFile *wxfile, wxString filename, wxString model_symbol_name ) :
//    MusFileOutputStream( file, wxfile )
CmpMLFOutput::CmpMLFOutput( MusDoc *file, int fd, wxString filename, wxString model_symbol_name ) :
	MusMLFOutput( file, fd, filename, NULL, model_symbol_name )
{
	// idem previous constructor
	m_ignore_clefs = true;
	m_ignore_custos = true;
	m_ignore_keys = true;
}

CmpMLFOutput::~CmpMLFOutput()
{
}

void CmpMLFOutput::StartLabel( )
{
	m_symbols.Clear();
}

void CmpMLFOutput::EndLabel( )
{
	int pos;

	for (int i = 0; i < (int)m_symbols.GetCount(); i++ )
	{
		CmpMLFSymb *current = (CmpMLFSymb*)&m_symbols[i];
		wxString symbol;
		wxString symbol_label = current->GetLabel();
		if ( symbol_label == "" ) // skip empty labels
			continue;
		pos = current->GetPosition();
		symbol << pos  << " ";
		symbol << symbol_label << " " <<  current->m_im_staff << " " << current->m_im_pos << " " << current->m_im_filename << "\n";
		Write( symbol, symbol.Length() );
	}
	//m_symbols.Clear();	
}

bool CmpMLFOutput::WriteStaff( const MusLaidOutStaff *staff )
{
	if (staff->GetElementCount() == 0)
		return true;

    unsigned int k;
	bool added;

	StartLabel();
	
	MusSymbol1 *clef = NULL;
	bool last_is_clef = false;

    for (k = 0;k < staff->GetElementCount() ; k++ )
    {
		added = false;
        if ( staff->m_elements[k].IsNote() )
		{
			last_is_clef = false;

			bool ignore = false;
			MusNote1 *cur = (MusNote1*)&staff->m_elements[k];
			/*
			if (cur->val == CUSTOS)			
			{
				if ( m_ignore_custos ) // not safe because will ignore any alteration after a clef, even if not part of a key ...
					ignore = true;
			}
			*/ // ax2
			
			if (!ignore)
				added = WriteNote( (MusNote1*)&staff->m_elements[k] );
			else
				cur->m_cmp_flag = CMP_MATCH;
        }
		else if ( staff->m_elements[k].IsSymbol() )
		{
			bool ignore = false;
			MusSymbol1 *cur = (MusSymbol1*)&staff->m_elements[k];
			if (cur->flag == CLE)
			{
				last_is_clef = true;
				if (!clef || clef->code != cur->code) // first clef or new clef, we keep it
					clef = cur;
				else if (m_ignore_clefs)
					ignore = true;
			}
			else if (cur->flag == ALTER)
			{
				if ( last_is_clef && m_ignore_keys ) // not safe because will ignore any alteration after a clef, even if not part of a key ...
					ignore = true;
			}
			else
				last_is_clef = false;
			
			if (!ignore)
				added = WriteSymbol( (MusSymbol1*)&staff->m_elements[k] );
			else
				cur->m_cmp_flag = CMP_MATCH;
		}
		if ( added )
		{	
			((CmpMLFSymb*)&m_symbols.Last())->m_im_filename = (&staff->m_elements[k])->m_im_filename;
			((CmpMLFSymb*)&m_symbols.Last())->m_im_staff = (&staff->m_elements[k])->m_im_staff;
			((CmpMLFSymb*)&m_symbols.Last())->m_im_pos = (&staff->m_elements[k])->m_im_pos;
			((CmpMLFSymb*)&m_symbols.Last())->m_index = k;
		}
    }
	//EndLabel( );

    return true;
}



//----------------------------------------------------------------------------
// CmpMLFInput
//----------------------------------------------------------------------------

CmpMLFInput::CmpMLFInput( MusDoc *file, wxString filename ) :
    MusMLFInput( file, filename )
{
}

CmpMLFInput::~CmpMLFInput()
{
}
	
bool CmpMLFInput::ReadLabelStr( wxString label )
{
	if ( label[0] != '"')
		return false;

	if ( label.Freq( '.' ) < 2 )
		return false;

	if ( label.Freq( '_' ) < 1 )
		return false;

	wxString str = label.BeforeLast('.'); // remove .lab"
	//m_segment_label = atoi( str.AfterLast('.').c_str() );
	str = str.BeforeLast('.'); // remove .seg"
	m_staff_label = atoi ( str.AfterLast('_').c_str() );
	
	// filename
	str = str.BeforeLast('_');
	m_cmp_page_label = str.AfterLast('/');

	return true;
}


MusLaidOutStaff* CmpMLFInput::ImportFileInStaff( )
{
	wxString line;
	if ( !ReadLine( &line )  || (line != "#!MLF!#" ))
		return NULL ;
		
	m_cmp_pos = 50;

	MusLaidOutStaff *staff = new MusLaidOutStaff();
	while ( ReadLine( &line ) && ReadLabelStr( line ) )
	{
		ReadLabel( staff );
	}
	MusLaidOutStaff *staff_ut1 = GetNotUt1( staff );
	delete staff;
	staff_ut1->CheckIntegrity();
	return staff_ut1;
}


// offset est la position x relative du label
// normalement donne par imPage si present

bool CmpMLFInput::ReadLabel( MusLaidOutStaff *staff )
{
	bool is_note;
	int pos = 0;
	wxString line;

	while ( ReadLine( &line ) &&  CmpMLFInput::IsElement( &is_note, &line, &pos ) )
	{
		MusElement *e = NULL;
		if ( !is_note )
		{
			MusSymbol1 *s = CmpMLFInput::ConvertSymbol( line );
			if ( s )
			{
				e = s;
				s->m_x_abs = m_cmp_pos;
				staff->m_elements.Add( s );
			}
		}
		else
		{
			MusNote1 *n = CmpMLFInput::ConvertNote( line );
			if ( n )
			{
				e = n;
				n->m_x_abs = m_cmp_pos;
				staff->m_elements.Add( n );
			}
		}
		
		if ( e )
		{
			e->m_im_filename = m_cmp_page_label;
			e->m_im_staff = m_staff_label;
			e->m_im_pos = pos;
		}
		m_cmp_pos += 45; // default step;
	}       
    return true;
}


		#endif // AX_COMPARISON
#endif // AX_RECOGNITION
