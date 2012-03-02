/*
-------------------------------------------------------------------------------
	This file is part of http://gamekit.googlecode.com/
	
	Copyright (c) 2012 Aleksandar Stojiljkovic
	
	Contributor(s): none yet.
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

attribute highp   vec4 a_Vertex;
attribute mediump vec2 a_MultiTexCoord0;
attribute mediump vec4 a_Normal;
attribute lowp  vec4 a_Color;
attribute mediump vec4 a_BoneIndices;
attribute mediump vec4 a_BoneWeights;

uniform highp   mat4	u_ModelViewProjectionMatrix;
uniform mediump mat4	u_TrasposeInverseModelMatrix;
uniform mediump mat4	u_ViewMatrix;
uniform mediump	int	    u_BoneCount;
uniform highp   mat4    u_BoneMatrices[8];

varying mediump vec2 v_TexCoord;
varying vec3 v_Normal;
varying vec4 v_Color;

void main(void)
{
    mediump ivec4 indexOfBone = ivec4(a_BoneIndices);
    highp vec4 weightOfBone = a_BoneWeights;

    highp vec4 vertex;
	mediump vec4 normal;
        
    if (u_BoneCount > 0) {
        highp mat4 mat = u_BoneMatrices[indexOfBone.x] * weightOfBone.x;
        for (int i = 1; i < u_BoneCount; i++) {
            // rotate to use indexOfBone.x in the loop
            indexOfBone = indexOfBone.yzwx;
            weightOfBone = weightOfBone.yzwx;
            if (weightOfBone.x > 0.0) {
                mat = mat + (u_BoneMatrices[indexOfBone.x] * weightOfBone.x);
            }
             
        }
        
        // resulting position after applying skinning
        vertex = mat * a_Vertex;
        normal = mat * a_Normal;
    } else {
        vertex = a_Vertex;
	    normal = a_Normal;        
    }
	
    normal = u_TrasposeInverseModelMatrix * normal;
    v_Normal = normalize(vec3(u_ViewMatrix * normal));
    gl_Position = u_ModelViewProjectionMatrix * vertex;
    
    // pass through color and texture coordinates
    v_Color = a_Color;
    v_TexCoord = a_MultiTexCoord0;
}
