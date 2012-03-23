attribute highp	  vec3 inVertex;		//Vertex coordinates
attribute mediump vec2 inTexCoord;		//Texture coordinates in.
varying   mediump vec2 t1;				//Texture coordinate passed to fragment.

uniform mediump vec2 u_PixelSize;			//Relative size of a pixel (in texels) for this program.
varying mediump vec2 t2;				//Texture location for fragment directly above.
varying mediump vec2 t3;				//Texture location for fragment directly to the right.

void main()
{
	//Pass through texture coordinates.
	t1 = inTexCoord;
    
	// Sets texture coordinates for surrounding texels (up and right);
	t2 = vec2(inTexCoord.x, inTexCoord.y+u_PixelSize.y);
	t3 = vec2(inTexCoord.x+u_PixelSize.x, inTexCoord.y);
    
	// Set vertex position.
	gl_Position = vec4(inVertex,  1.0);
    
}
