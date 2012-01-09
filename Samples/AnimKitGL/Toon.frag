precision mediump float;

varying vec3 v_Normal;
varying vec4 v_Color;
varying mediump vec2 v_TexCoord;

uniform sampler2D s_texture;
uniform vec4 u_Color;

#define USE_UNIFORM_COLOR 0.1
#define USE_VARIANT_COLOR 0.2
#define BLACK_OUTLINE 0.01
#define NO_OUTLINE 0.99 

void main (void)
{
    vec2 uv = v_TexCoord;
    float Edge = 0.45;
    float Phong = 0.95;
    
	vec3 n = normalize( v_Normal );
    
	float f = dot(vec3(0,0,1),n);
    
    vec4 texture;
    // supporting texture, variant color or uniform color
    if (u_Color.a < USE_UNIFORM_COLOR) {
        texture = u_Color;
    } else if (u_Color.a < USE_VARIANT_COLOR) {
        texture = v_Color;
    } else {
        texture = texture2D(s_texture, uv);
    }

    gl_FragColor = texture;
    
    if (abs(f) < Edge) {
        // this magical number means use black for outline
        if (u_Color.a == BLACK_OUTLINE) {
            vec4 black = vec4(0, 0, 0, 0);
            gl_FragColor = black;
        } else if (u_Color.a == NO_OUTLINE) {
            gl_FragColor = texture;
        }
        else {
            gl_FragColor = texture * 0.45;
        }
    } else if (f < Phong) {
        vec4 color1 = texture;
        gl_FragColor = color1;
    } else if (u_Color.a < USE_UNIFORM_COLOR){
        gl_FragColor = texture;
    }
    else {
        vec4 PhongColor = vec4(1.25, 1.25, 1.25, 1.0);
        gl_FragColor = PhongColor * texture;
    }
}

