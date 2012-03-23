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

precision mediump float;

varying vec3 v_Normal;
varying vec4 v_Color;
varying mediump vec2 v_TexCoord;

uniform sampler2D s_texture;
uniform vec4 u_Color;

#define USE_UNIFORM_COLOR 0.1
#define USE_VARIANT_COLOR 0.2
#define NO_OUTLINE 0.99

void main (void)
{
    vec2 uv = v_TexCoord;
//    float Edge = 0.5;
    float Phong = 0.8;
    
	vec3 n = normalize(v_Normal);
//    float edge = dot(vec3(0,0,1),n);
    
    vec4 texture;
    // supporting texture or variant color
    if (u_Color.a < USE_VARIANT_COLOR) {
        texture = v_Color;
    } else {
        texture = texture2D(s_texture, uv);
    }

    gl_FragColor = texture;
    
    if (u_Color.a == NO_OUTLINE) {
            gl_FragColor = texture;
//    } else if (abs(edge) < Edge) {
//        gl_FragColor = texture * 0.4;
    } else {
        // take (0,1,1) as light vector
	    float f = dot(vec3(0,0.707,0.707),n);
        if (f < Phong) {
            gl_FragColor = texture;
        } else if (u_Color.a < USE_UNIFORM_COLOR){
            gl_FragColor = texture;
        }
        else {
            vec4 PhongColor = vec4(1.25, 1.25, 1.25, 1.0);
            gl_FragColor = PhongColor * texture;
        }
    }
    gl_FragColor.a = u_Color.a;
}

