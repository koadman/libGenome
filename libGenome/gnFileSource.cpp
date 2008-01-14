/////////////////////////////////////////////////////////////////////////////
// File:            gnFileSource.h
// Purpose:         Implements generic gnBaseSource methods
// Description:     Provides a general implementation for accessing DNA
//                  sequence contig data files.
// Changes:        
// Version:         libGenome 0.5.1 
// Author:          Aaron Darling 
// Modified by:     
// Copyright:       (c) Aaron Darling 
// Licenses:        See COPYING file for details
/////////////////////////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "libGenome/gnFileSource.h"
#include <fstream>
#include "libGenome/gnFilter.h"


using namespace std;
namespace genome {

gnFileSource::gnFileSource() :
m_pFilter(gnFilter::fullDNASeqFilter())
{}

//copy constructor
gnFileSource::gnFileSource(const gnFileSource& gnfs){
	m_openString = gnfs.m_openString;
	m_pFilter = gnfs.m_pFilter;
	m_newlineType = gnfs.m_newlineType;
	m_newlineSize = gnfs.m_newlineSize;
	m_ifstream.open( m_openString.c_str(), ios::in | ios::binary );
	if( !m_ifstream.is_open() )
		m_ifstream.clear();
	else{
		omp_init_lock( &file_lock );
	}
}

// Open, Close	
void gnFileSource::Open( string openString )
{
	m_ifstream.open(openString.c_str(), ios::in | ios::binary );
	if( m_ifstream.is_open() )
	{
		omp_init_lock( &file_lock );
		omp_guard rex( file_lock );
		m_openString = openString;
		if( ParseStream(m_ifstream) )
		{
			;
		}
		else{
			m_ifstream.clear();
			m_ifstream.close();
			omp_destroy_lock( &file_lock );
		}
	}else{
		m_ifstream.clear();
		Throw_gnEx(FileNotOpened());
	}
}
void gnFileSource::Open( )
{
	m_ifstream.open( m_openString.c_str(), ios::in | ios::binary );
	if( !m_ifstream.is_open() ){
		m_ifstream.clear();
		Throw_gnEx(FileNotOpened());
	}
	omp_init_lock( &file_lock );
}
void gnFileSource::Close()
{
	m_ifstream.close();
	omp_destroy_lock( &file_lock );
	if( m_ifstream.is_open() )
		Throw_gnEx(IOStreamFailed());
}

boolean gnFileSource::Read( const uint64 pos, char* buf, gnSeqI& bufLen) 
{
	omp_guard rex( file_lock );
	m_ifstream.seekg(pos, ios::beg);
	m_ifstream.read(buf, bufLen);
	if(m_ifstream.fail()){
		m_ifstream.clear();
		return false;
	}
	return true;
}

void gnFileSource::DetermineNewlineType()
{
	omp_guard rex( file_lock );
	// set default values
	m_newlineType = gnNewlineUnix;
	m_newlineSize = 1;

	//decide what type of newlines we have
	char buf[ BUFFER_SIZE ];
	m_ifstream.getline( buf, BUFFER_SIZE);
	m_ifstream.seekg(-2, ios::cur);
	m_ifstream.read( buf, 2);
	m_ifstream.seekg(0);
	if(buf[1] == '\n'){
		if(buf[0] == '\r'){
			m_newlineType = gnNewlineWindows;
			m_newlineSize = 2;
		}else{ 
			if(buf[1] == '\r')
				m_newlineType = gnNewlineMac;
			else
				m_newlineType = gnNewlineUnix;
		}
	}
}

}	// end namespace genome

