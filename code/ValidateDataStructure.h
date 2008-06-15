/*
Open Asset Import Library (ASSIMP)
----------------------------------------------------------------------

Copyright (c) 2006-2008, ASSIMP Development Team
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

* Neither the name of the ASSIMP team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the ASSIMP Development Team.

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

/** @file Defines a post processing step to validate the loader's
 * output data structure (for debugging)
 */
#ifndef AI_VALIDATEPROCESS_H_INC
#define AI_VALIDATEPROCESS_H_INC

#include "BaseProcess.h"

struct aiBone;
struct aiMesh;
struct aiAnimation;
struct aiBoneAnim;
struct aiTexture;
struct aiMaterial;
struct aiNode;

namespace Assimp
{

// ---------------------------------------------------------------------------
/** Validates the ASSIMP data structure
 */
class ValidateDSProcess : public BaseProcess
{
	friend class Importer;

protected:
	/** Constructor to be privately used by Importer */
	ValidateDSProcess();

	/** Destructor, private as well */
	~ValidateDSProcess();

public:
	// -------------------------------------------------------------------
	/** Returns whether the processing step is present in the given flag field.
	 * @param pFlags The processing flags the importer was called with. A bitwise
	 *   combination of #aiPostProcessSteps.
	 * @return true if the process is present in this flag fields, false if not.
	*/
	bool IsActive( unsigned int pFlags) const;

	// -------------------------------------------------------------------
	/** Executes the post processing step on the given imported data.
	* A process should throw an ImportErrorException* if it fails.
	* @param pScene The imported data to work at.
	*/
	void Execute( aiScene* pScene);

protected:

	// -------------------------------------------------------------------
	/** Report a validation error. This will throw an exception,
	 *  control won't return.
	 * @param msg Format string for sprintf().
	 */
	void ReportError(const char* msg,...);

	// -------------------------------------------------------------------
	/** Report a validation warning. This won't throw an exception,
	 *  control will return to the callera.
	 * @param msg Format string for sprintf().
	 */
	void ReportWarning(const char* msg,...);

	// -------------------------------------------------------------------
	/** Validates a mesh
	 * @param pMesh Input mesh
	 */
	void Validate( const aiMesh* pMesh);

	// -------------------------------------------------------------------
	/** Validates a bone
	 * @param pMesh Input mesh
	 * @param pBone Input bone
	 */
	void Validate( const aiMesh* pMesh,const aiBone* pBone);

	// -------------------------------------------------------------------
	/** Validates an animation
	 * @param pAnimation Input animation
	 */
	void Validate( const aiAnimation* pAnimation);

	// -------------------------------------------------------------------
	/** Validates a material
	 * @param pMaterial Input material
	 */
	void Validate( const aiMaterial* pMaterial);

	// -------------------------------------------------------------------
	/** Search the material data structure for invalid or corrupt
	 *  texture keys.
	 * @param pMaterial Input material
	 * @param szType Type of the texture (the purpose string that
	 *  occurs in material keys, e.g. "diffuse", "ambient")
	 */
	void SearchForInvalidTextures(const aiMaterial* pMaterial,
		const char* szType);

	// -------------------------------------------------------------------
	/** Validates a texture
	 * @param pTexture Input texture
	 */
	void Validate( const aiTexture* pTexture);

	// -------------------------------------------------------------------
	/** Validates a bone animation channel
	 * @param pAnimation Input animation
	 * @param pBoneAnim Input bone animation
	 */
	void Validate( const aiAnimation* pAnimation,
		const aiBoneAnim* pBoneAnim);

	// -------------------------------------------------------------------
	/** Validates a node and all of its subnodes
	 * @param Node Input node
	 */
	void Validate( const aiNode* pNode);

private:

	aiScene* mScene;
};

} // end of namespace Assimp

#endif // AI_VALIDATEPROCESS_H_INC