/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

    Contributor(s): astojilj.
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/

#include "akEntity.h"

#include "akAnimatedObject.h"
#include "akMesh.h"
#include "akSkeleton.h"
#include "akSkeletonPose.h"
#include "akDualQuat.h"
#include "akPose.h"

#include "piper.h"
#if OPENGL_ES_2_0
#include "piperGL20.h"
#endif

#define GL_ARRAY_BUFFER_ARB GL_ARRAY_BUFFER
#define GL_ELEMENT_ARRAY_BUFFER_ARB GL_ELEMENT_ARRAY_BUFFER
#define glBindBufferARB glBindBuffer
#define glPushMatrix(); Piper::instance()->pushMatrix(Piper::MODEL);
#define glPopMatrix(); Piper::instance()->popMatrix(Piper::MODEL);
#define glMultMatrixf(a) Piper::instance()->multMatrix(a,Piper::MODEL)

akEntity::akEntity(const utHashedString &name) : m_name(name), m_mesh(0), m_skeleton(0), m_animatedObject(0),
	m_useDualQuatSkinning(false), m_useGPUSkinning(false), m_posAnimated(false), m_morphAnimated(false), m_useVbo(false)
{
    m_useVbo = true;
}

akEntity::~akEntity()
{	
	m_matrixPalette.clear();
	m_dualquatPalette.clear();
	
	m_posnoVertexVboIds.clear();
	m_staticVertexVboIds.clear();
	m_staticIndexVboIds.clear();
    
    m_boneWeightVboIds.clear();
    m_boneIndexVboIds.clear();
	
	m_textures.clear();
}

// FIXME
// in iOS 4.1 simulator, glDrawElements(GL_TRIANGLES, cnt, GL_UNSIGNED_INT did not work - did not show anything on screen
// while GL_UNSIGNED_SHORT works. Using this as temporary workaround until trying with new sdk or anyway, change types
// in animkit code (no reason to use unsigned int anyway).
static void convertUint2Ushort(unsigned int *source, unsigned short *target, unsigned int count)
{
    for (int i = 0; i < count; i++) {
        target[i] = (unsigned short) source[i];
    }
}

#ifdef OPENGL_ES_2_0
    #define OPENGL_ES_2_0_GPU_SKINNING			
#endif

void akEntity::init(bool useVbo, akDemoBase* demo)
{
	m_useVbo = useVbo;
	
	if(m_skeleton)
	{
		if(m_animatedObject==0)
			m_animatedObject = new akAnimatedObject();
		m_animatedObject->getPose()->setSkeleton(m_skeleton);
		
		m_matrixPalette.resize(m_skeleton->getNumJoints());
		m_dualquatPalette.resize(m_skeleton->getNumJoints());
	}
	
	if(isMeshDeformed())
		m_mesh->addSecondPositionBuffer();
	
	int nsub = m_mesh->getNumSubMeshes();
	
	if(m_useVbo)
	{
		m_posnoVertexVboIds.resize(nsub);
		m_staticVertexVboIds.resize(nsub);
		m_staticIndexVboIds.resize(nsub);

#ifdef OPENGL_ES_2_0_GPU_SKINNING
        if (isMeshDeformedBySkeleton()) {
            m_boneIndexVboIds.resize(nsub);
            m_boneWeightVboIds.resize(nsub);
            glGenBuffers(nsub, &m_boneIndexVboIds[0]);
            glGenBuffers(nsub, &m_boneWeightVboIds[0]);            
        }
#endif
		glGenBuffers(nsub, &m_posnoVertexVboIds[0]);
		glGenBuffers(nsub, &m_staticVertexVboIds[0]);
		glGenBuffers(nsub, &m_staticIndexVboIds[0]);
        
		
		for(int i=0; i<nsub; i++)
		{
			akSubMesh* sub = m_mesh->getSubMesh(i);
			UTuint32 nv = sub->getVertexCount();
			UTuint32 ni = sub->getIndexCount();
			
			void *posnodata = sub->getPosNoDataPtr();
			void *staticdata = sub->getStaticVertexDataPtr();
			void *idata = sub->getIndexDataPtr();
			UTuint32 posnodatas = sub->getPosNoDataStride();
			UTuint32 staticdatas = sub->getStaticVertexDataStride();
			UTuint32 idatas = sub->getIndexDataStride();

            glBindBuffer(GL_ARRAY_BUFFER_ARB, m_posnoVertexVboIds[i]);
			glBufferData(GL_ARRAY_BUFFER_ARB, nv*posnodatas, posnodata, GL_STATIC_DRAW);
			
			glBindBuffer(GL_ARRAY_BUFFER_ARB, m_staticVertexVboIds[i]);
			glBufferData(GL_ARRAY_BUFFER_ARB, nv*staticdatas, staticdata, GL_STATIC_DRAW);
			
            unsigned short indexData2UShort[ni];
            convertUint2Ushort((unsigned int*) idata, indexData2UShort, ni);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, m_staticIndexVboIds[i]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, ni*(idatas/2), indexData2UShort, GL_STATIC_DRAW);
#ifdef OPENGL_ES_2_0_GPU_SKINNING
            if (isMeshDeformedBySkeleton()) {
                void *boneIndexData = sub->getBoneIndexDataPtr();
                void *boneWeightsData = sub->getBoneWeightsDataPtr();
                
                glBindBuffer(GL_ARRAY_BUFFER_ARB, m_boneIndexVboIds[i]);
                glBufferData(GL_ARRAY_BUFFER_ARB, nv*4*sizeof(UTuint8), boneIndexData, GL_STATIC_DRAW);
                
                glBindBuffer(GL_ARRAY_BUFFER_ARB, m_boneWeightVboIds[i]);
                glBufferData(GL_ARRAY_BUFFER_ARB, nv*4*sizeof(float), boneWeightsData, GL_STATIC_DRAW);
            }
#endif
		}
	}
	
	m_textures.resize(nsub);
    for(int i=0; i<nsub; i++)
	{
		akSubMesh* sub = m_mesh->getSubMesh(i);
		m_textures[i] = 0;
		
		if(sub->getMaterial().m_mode & akMaterial::MA_HASFACETEX)
		{
			const utString& texname = sub->getMaterial().m_textures.at(0).m_image;
			m_textures[i] = demo->getTexture(texname);
		}
	}
	
	// Activate the first action of each object
	akAnimationPlayer* player = m_animatedObject ? m_animatedObject->getAnimationPlayers()->getAnimationPlayer(0) : 0;
	if(player)
	{
		player->setEnabled(true);
		player->setWeight(1.0f);
		player->setMode(akAnimationPlayer::AK_ACT_LOOP);
	}
}

void akEntity::updateVBO(void)
{
	if(m_useVbo)
	{
		int nsub = m_mesh->getNumSubMeshes();
		
		for(int i=0; i<nsub; i++)
		{
			akSubMesh* sub = m_mesh->getSubMesh(i);
			UTuint32 nv = sub->getVertexCount();
			void *codata = sub->getSecondPosNoDataPtr();
			UTuint32 datas = sub->getPosNoDataStride();
			
			glBindBuffer(GL_ARRAY_BUFFER_ARB, m_posnoVertexVboIds[i]);
			glBufferData(GL_ARRAY_BUFFER_ARB, nv*datas, NULL, GL_STREAM_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER_ARB, 0, nv*datas, codata);
		}
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}
}

void akEntity::update(int dualQuat, int normalsMethod)
{
	//m_pose->reset(akSkeletonPose::SP_BINDING_SPACE);
	//m_players.stepTime(dt);
	//m_players.evaluate(m_pose);
	
	if(m_animatedObject)
	{
		akPose* pose = m_animatedObject->getPose();
		
		if(isPositionAnimated())
			setTransformState(pose->getTransform());
		
		if(isMeshDeformed())
		{
			// Animations are in binding space (imported "as is" from Blender)
			// The skeleton pose (result of the animations blending) is therefore in binding space
			// We need to tell that so it gets converted to model space OK
			akSkeletonPose* skelpose = pose->getSkeletonPose();
			skelpose->setSpace(akSkeletonPose::SP_BINDING_SPACE);
			
			// Manul conversion to model space is not needed anymore
			// On the fly conversion is done at the time of filling the matrix palette if necessary
			// But the skeleton pose space must be set correctly for this to work
			// Note: If you need to get a bone pose in model space, try to avoid doing the conversion twice.
			//skelpose->toModelSpace(skelpose);
			
			int dq ;
			switch(dualQuat)
			{
			case 0:
				if(getUseDualQuatSkinning())
					dq = akGeometryDeformer::GD_SO_DUAL_QUAT;
				else
					dq = akGeometryDeformer::GD_SO_MATRIX;
				break;
			case 1:
				dq = akGeometryDeformer::GD_SO_MATRIX;
				break;
			case 2:
				dq = akGeometryDeformer::GD_SO_DUAL_QUAT;
				break;
			case 3:
				dq = akGeometryDeformer::GD_SO_DUAL_QUAT_ANTIPOD;
				break;
			}
			int nm;
			switch(normalsMethod)
			{
			case 0:
				nm = akGeometryDeformer::GD_NO_NONE;
				break;
			case 1:
				nm = akGeometryDeformer::GD_NO_NOSCALE;
				break;
			case 2:
				nm = akGeometryDeformer::GD_NO_UNIFORM_SCALE;
				break;
			case 3:
				nm = akGeometryDeformer::GD_NO_FULL;
				break;
			}
			
			if( dq != akGeometryDeformer::GD_SO_MATRIX )
				pose->fillDualQuatPalette(m_dualquatPalette, m_matrixPalette);
			else
				pose->fillMatrixPalette(m_matrixPalette);

            
#ifdef OPENGL_ES_2_0_GPU_SKINNING                
            if(m_animatedObject && PiperGL20::instance())
            {
                if(isMeshDeformed() && !m_mesh->hasMorphTargets() && pose && pose->getSkeletonPose())
                    // if GPU skinning, no need to copy invertices -> outvertices and updateVBO
                    return;
            }
            
			m_mesh->deform((akGeometryDeformer::SkinningOption)dq, (akGeometryDeformer::NormalsOption)nm,
						   pose, 0, 0);
            
#else            
			m_mesh->deform((akGeometryDeformer::SkinningOption)dq, (akGeometryDeformer::NormalsOption)nm,
						   pose, &m_matrixPalette, &m_dualquatPalette);
#endif			
			updateVBO();
		}
	}
}

void akEntity::draw(bool drawNormal, bool drawColor, bool textured, bool useVbo, bool shaded, bool drawskel)
{
	glPushMatrix();
	
	akMatrix4 mat = m_transform.toMatrix();

    glMultMatrixf((GLfloat*)&mat);
	
	if(m_mesh)
	{
		for(unsigned int j=0; j< m_mesh->getNumSubMeshes(); j++)
		{
			akSubMesh* sm = m_mesh->getSubMesh(j);
			const akBufferInfo* vbuf = sm->getVertexBuffer();
			const akBufferInfo* ibuf = sm->getIndexBuffer();
			UTuint32 tot = ibuf->getSize();
			
			const akBufferInfo::Element *posbuf, *norbuf, *idxbuf, *uvbuf, *colorbuf;
			
            bool useGPUSkinning(false);
#ifdef OPENGL_ES_2_0_GPU_SKINNING                
            if(isMeshDeformedBySkeleton())
                useGPUSkinning = true;
#endif            
            if(isMeshDeformed() && !(useGPUSkinning && !isMeshDeformedByMorphing()))
			{
				posbuf = vbuf->getElement(akBufferInfo::BI_DU_VERTEX, akBufferInfo::VB_DT_3FLOAT32, 2);
				norbuf = vbuf->getElement(akBufferInfo::BI_DU_NORMAL, akBufferInfo::VB_DT_3FLOAT32, 2);
			}
			else
			{
				posbuf = vbuf->getElement(akBufferInfo::BI_DU_VERTEX, akBufferInfo::VB_DT_3FLOAT32, 1);
				norbuf = vbuf->getElement(akBufferInfo::BI_DU_NORMAL, akBufferInfo::VB_DT_3FLOAT32, 1);
			}
			
			uvbuf = vbuf->getElement(akBufferInfo::BI_DU_UV, akBufferInfo::VB_DT_2FLOAT32, 1);
			colorbuf = vbuf->getElement(akBufferInfo::BI_DU_COLOR, akBufferInfo::VB_DT_4UINT8, 1);
			
			idxbuf = ibuf->getElement(akBufferInfo::BI_DU_ELEMENT, akBufferInfo::VB_DT_UINT32, 1);
			
			if(!posbuf ||! idxbuf)
				continue;
			
			bool color = colorbuf && drawColor && sm->hasVertexColor();
			bool hasnormals = norbuf && sm->hasNormals();
			bool texture = uvbuf && m_textures[j] && textured && sm->getUVLayerCount() >=1;
	
			if(texture)
			{
                glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, m_textures[j]);

				// internal tricks - alpha == 0.99 means no outline
                Piper::instance()->glColor4f(1.f, 0.f, 0.f, m_skeleton ? 1.0f : 0.99f);
			}
			
			if(m_useVbo && useVbo)
			{
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_posnoVertexVboIds[j]);
				glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(3, GL_FLOAT, posbuf->stride, (GLvoid*)posbuf->getOffset());
				if(hasnormals)
				{
					glNormalPointer(GL_FLOAT, norbuf->stride, (GLvoid*)norbuf->getOffset());
					glEnableClientState(GL_NORMAL_ARRAY);
				}
				
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_staticVertexVboIds[j]);
				
                if(color)
				{
					glEnableClientState(GL_COLOR_ARRAY);
					glColorPointer(4, GL_UNSIGNED_BYTE, colorbuf->stride, (GLvoid*)colorbuf->getOffset());
                    if (!texture)
                        // using alpha to instruct shader what to use, among other things
                        Piper::instance()->glColor4f(1.f, 0.f, 0.f, 0.15f);
				}
				if(texture)
				{
                    glTexCoordPointer(2, GL_FLOAT, uvbuf->stride, (GLvoid*)uvbuf->getOffset());
                    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                }
                
                
#ifdef OPENGL_ES_2_0_GPU_SKINNING                
                if(m_animatedObject && PiperGL20::instance())
                {
                    akPose* pose = m_animatedObject->getPose();
                    if(isMeshDeformedBySkeleton()) {
                        
                        const akBufferInfo::Element *indicesbuf, *weightsbuf;
                        
                        indicesbuf = vbuf->getElement(akBufferInfo::BI_DU_BONE_IDX, akBufferInfo::VB_DT_4UINT8, 1);
                        weightsbuf = vbuf->getElement(akBufferInfo::BI_DU_BONE_WEIGHT, akBufferInfo::VB_DT_4FLOAT32, 1);
                        
                        // Number of bones influencing one vertex in the mesh
                        glUniform1i(PiperGL20::instance()->currentShader()->BoneCountHandle, 4);
                        
                        // matrix palete and normals
                        akMatrix4 *matrixData = new akMatrix4[m_matrixPalette.size()];
                        for (int matrixIndex = 0; matrixIndex < m_matrixPalette.size(); matrixIndex++) {
                            matrixData[matrixIndex] = m_matrixPalette[matrixIndex];
                        }
                        glUniformMatrix4fv(PiperGL20::instance()->currentShader()->BoneMatricesHandle, m_matrixPalette.size(), GL_FALSE, (float*)matrixData);
                        delete [] matrixData;
                        
				        glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_boneWeightVboIds[j]);
                        glEnableVertexAttribArray(GL_BONEWEIGHT_ARRAY);
                        glVertexAttribPointer(GL_BONEWEIGHT_ARRAY, 4, GL_FLOAT, GL_FALSE, weightsbuf->stride, (GLvoid*) weightsbuf->getOffset());
                        
				        glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_boneIndexVboIds[j]);
                        glEnableVertexAttribArray(GL_BONEINDEX_ARRAY);
                        glVertexAttribPointer(GL_BONEINDEX_ARRAY, 4, GL_UNSIGNED_BYTE, GL_FALSE, indicesbuf->stride, (GLvoid*) indicesbuf->getOffset());

                    }
                }

				if (!useGPUSkinning && PiperGL20::instance()) {
                    // since we use the same shader, turing off matrix skinning this way
                    glUniform1i(PiperGL20::instance()->currentShader()->BoneCountHandle, 0);
                }
#endif                
                glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_staticIndexVboIds[j]);
                Piper::instance()->glDrawElements(GL_TRIANGLES, tot, GL_UNSIGNED_SHORT, (GLvoid*)idxbuf->getOffset());

				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_NORMAL_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                if (useGPUSkinning) {
                    glDisableClientState(GL_BONEINDEX_ARRAY);
                    glDisableClientState(GL_BONEWEIGHT_ARRAY);
                }
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
				glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
			}
			else
			{
                assert(false);
			}
			glBindTexture(GL_TEXTURE_2D, 0);
			
			
		}
	}
	
	if(m_skeleton && drawskel)
	{
        akSkeletonPose* skelpose = m_animatedObject->getPose()->getSkeletonPose();
		
        glDisable(GL_DEPTH_TEST);
		
		if(skelpose)
		{
			skelpose->toModelSpace(skelpose);
			
			glDisable(GL_LIGHTING);
			glDisable(GL_DEPTH_TEST);
			
			int i, tot;
			tot = skelpose->getNumJoints();
			for(i=0; i<tot; i++)
			{
				glPushMatrix();
				
				akSkeletonPose* pose = m_animatedObject->getPose()->getSkeletonPose();
				const akTransformState* jointpose = pose->getJointPose(i);
				akMatrix4 mat = jointpose->toMatrix();
				
				glMultMatrixf((GLfloat*)&mat);
                /*            Piper::instance()->glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
                 GLfloat indices[] = {
                 0.05,0,0,
                 0,0,0,
                 0,0.05,0,
                 0,0,0,
                 0,0,0.05,
                 0,0,0
                 };
                 
                 Piper::instance()->glDrawElements(GL_LINES,3, GL_FLOAT, indices);
                 Piper::instance()->glColor4f(0.f, 0.f, 0.f, 1.0f);
                 */ 
				
				glPopMatrix();
			}
			glEnable(GL_DEPTH_TEST);
			
		}
	}
	
	glPopMatrix();
}



