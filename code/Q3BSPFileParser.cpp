/*
Open Asset Import Library (assimp)
----------------------------------------------------------------------

Copyright (c) 2006-2008, assimp team
All rights reserved.

Redistribution and use of this software in source and binary forms, 
with or without modification, are permitted provided that the 
following conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------
*/
#include "AssimpPCH.h"

#ifndef ASSIMP_BUILD_NO_Q3BSP_IMPORTER

#include "Q3BSPFileParser.h"
#include "DefaultIOSystem.h"
#include "Q3BSPFileData.h"
#include "Q3BSPZipArchive.h"
#include <vector>

namespace Assimp
{

using namespace Q3BSP;

// ------------------------------------------------------------------------------------------------
Q3BSPFileParser::Q3BSPFileParser( const std::string &rMapName, Q3BSPZipArchive *pZipArchive ) :
	m_sOffset( 0 ),
	m_Data(),
	m_pModel( NULL ),
	m_pZipArchive( pZipArchive )
{
	ai_assert( NULL != m_pZipArchive );
	ai_assert( !rMapName.empty() );

	if ( !readData( rMapName ) )
		return;

	m_pModel = new Q3BSPModel;
	m_pModel->m_ModelName = rMapName;
	if ( !parseFile() )
	{
		delete m_pModel;
		m_pModel = NULL;
	}
}

// ------------------------------------------------------------------------------------------------
Q3BSPFileParser::~Q3BSPFileParser()
{
	delete m_pModel;
	m_pModel = NULL;
}

// ------------------------------------------------------------------------------------------------
Q3BSP::Q3BSPModel *Q3BSPFileParser::getModel() const
{
	return m_pModel;
}

// ------------------------------------------------------------------------------------------------
bool Q3BSPFileParser::readData( const std::string &rMapName )
{
	if ( !m_pZipArchive->Exists( rMapName.c_str() ) )
		return false;

	IOStream *pMapFile = m_pZipArchive->Open( rMapName.c_str() );
	if ( NULL == pMapFile )
		return false;
		
	const size_t size = pMapFile->FileSize();
	m_Data.resize( size );

	const size_t readSize = pMapFile->Read( &m_Data[0], sizeof( char ), size );
	if ( readSize != size )
	{
		m_Data.clear();
		return false;
	}
	m_pZipArchive->Close( pMapFile );

	return true;
}

// ------------------------------------------------------------------------------------------------
bool Q3BSPFileParser::parseFile()
{
	if ( m_Data.empty() )
	{
		return false;
	}

	if ( !validateFormat() )
	{
		return false;
	}

	// Imports the dictionary of the level
	getLumps();

	CopyLump( m_pModel->m_Vertices, kVertices );
	CopyLump( m_pModel->m_Indices, kMeshVerts );
	CopyLump( m_pModel->m_Faces, kFaces );
	CopyLump( m_pModel->m_Models, kModels );
	CopyLump( m_pModel->m_Textures, kTextures );
	CopyLump( m_pModel->m_Lightmaps, kLightmaps );

	// Load the entities
	getEntities();

	return true;
}

// ------------------------------------------------------------------------------------------------
bool Q3BSPFileParser::validateFormat()
{
	memcpy( &m_Header, &m_Data[ 0 ], sizeof( m_Header ) );
	m_sOffset += sizeof( sQ3BSPHeader );

	// Version and identify string validation
	if (m_Header.strID[ 0 ] != 'I' || 
		m_Header.strID[ 1 ] != 'B' || 
		m_Header.strID[ 2 ] != 'S' || 
		m_Header.strID[ 3 ] != 'P') 
	{
		return false;
	}

	return true;
}

// ------------------------------------------------------------------------------------------------
void Q3BSPFileParser::getLumps()
{
	// note some games may use a different bsp file version that have a different number
	// of chunks. it might be a good idea to be explicit about which bsp versions we support
	// if not for the difficulty of finding information about the chunk structure of all potential versions
	// without being picky about the versions we choose to process, there is a possibility that files
	// with different chunk structures may crash. it would be useful to report this bsp version
	// out to error logging at least so that there would be some information prior to the attempts to export
	// that would help troubleshoot it if a file with a different version crashed while working versions didn't
	//ai_assert( m_Header.iVersion == 47 );
	std::stringstream ss;
	ss << "Q3BSPFileParser attempting to load version " << m_Header.iVersion << " with " << kMaxLumps << " data lumps" << std::endl;
	DefaultLogger::get()->info( ss.str() );
	
	m_pModel->m_Lumps.resize( kMaxLumps );

	const size_t Offset = m_sOffset;
	const size_t CopySize = sizeof( sQ3BSPLump ) * kMaxLumps;
	memcpy( &m_pModel->m_Lumps[ 0 ], &m_Data[ Offset ], CopySize );
}

// ------------------------------------------------------------------------------------------------
void Q3BSPFileParser::getEntities()
{
	const int CopySize = m_pModel->m_Lumps[ kEntities ].iSize;
	m_pModel->m_EntityData.resize( CopySize );
	if ( CopySize > 0 ) 
	{
		const size_t Offset = m_pModel->m_Lumps[ kEntities ].iOffset;
		memcpy( &m_pModel->m_EntityData[ 0 ], &m_Data[ Offset ], CopySize );
	}
}

// ------------------------------------------------------------------------------------------------

} // Namespace Assimp

#endif // ASSIMP_BUILD_NO_Q3BSP_IMPORTER