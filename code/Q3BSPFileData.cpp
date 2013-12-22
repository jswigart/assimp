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

#include "Q3BSPFileData.h"

namespace Assimp
{
namespace Q3BSP
{

// ------------------------------------------------------------------------------------------------
//	Counts the number of referenced vertices.
size_t Q3BSPModel::countVertices( const std::vector<size_t> & faceIndices ) const
{
	size_t numVerts = 0;
	for ( size_t i = 0; i < faceIndices.size(); ++i )
	{
		ai_assert( faceIndices[ i ] < m_Faces.size() && faceIndices[ i ] >= 0 );

		const sQ3BSPFace & pQ3BSPFace = m_Faces[ faceIndices[ i ] ];
		if ( pQ3BSPFace.iType == Polygon || pQ3BSPFace.iType == TriangleMesh )
		{
			numVerts += pQ3BSPFace.iNumOfFaceVerts;
		}
	}

	return numVerts;
}

// ------------------------------------------------------------------------------------------------
//	Counts the faces with vertices.
size_t Q3BSPModel::countFaces( const std::vector<size_t> & faceIndices ) const
{
	size_t numFaces = 0;
	for ( size_t i = 0; i < faceIndices.size(); ++i )
	{
		ai_assert( faceIndices[ i ] < m_Faces.size() && faceIndices[ i ] >= 0 );

		const sQ3BSPFace & pQ3BSPFace = m_Faces[ faceIndices[ i ] ];
		if ( pQ3BSPFace.iNumOfFaceVerts > 0 )
		{
			numFaces++;
		}
	}

	return numFaces;
}

// ------------------------------------------------------------------------------------------------
//	Counts the number of triangles in a Q3-facearray.
size_t Q3BSPModel::countTriangles( const std::vector<size_t> & faceIndices ) const
{
	size_t numTriangles = 0;
	for ( size_t i = 0; i < faceIndices.size(); ++i )
	{
		ai_assert( faceIndices[ i ] < m_Faces.size() && faceIndices[ i ] >= 0 );

		const sQ3BSPFace & pQ3BSPFace = m_Faces[ faceIndices[ i ] ];
		numTriangles += pQ3BSPFace.iNumOfFaceVerts / 3;
	}

	return numTriangles;
}


} // Namespace Q3BSP
} // Namespace Assimp

#endif // ASSIMP_BUILD_NO_Q3BSP_IMPORTER