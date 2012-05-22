attribute highp	  vec3 inVertex;		//Vertex coordinates
attribute mediump vec2 inTexCoord;		//Texture coordinates in.
varying   mediump vec2 t1;				//Texture coordinate passed to fragment.

void main()
{
	//Pass through texture coordinates.
	t1 = inTexCoord;
    
	// Set vertex position.
	gl_Position = vec4(inVertex,  1.0);
    
}
