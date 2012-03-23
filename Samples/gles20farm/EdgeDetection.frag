uniform sampler2D  sColorBufferTexture;	// Texture containing color info in rgb and an object ID value in the alpha channel.
varying mediump vec2 t1;				// Texture coordinate for this fragment.

varying mediump vec2 t2;				// Texel directly above this fragment's.
varying mediump vec2 t3;				// Texel directly to the right of this fragment's.

const lowp vec3 black = vec3(0.0);

void main()
{
	// Gets the color from the render texture.
	lowp vec4 Color = texture2D(sColorBufferTexture, t1);
    
	// Temporary color store to be written to, data transferred to gl_FragColor at the end.
	lowp vec3 newColor=Color.rgb;
	
	// Reads in values from the color texture, for two surrounding texels;
	lowp vec4 upFrag = texture2D(sColorBufferTexture, t2);
	lowp vec4 rightFrag = texture2D(sColorBufferTexture, t3);
	
	// If the object IDs covering this area differ, draw an edge
	if(upFrag.a-Color.a!=0.0 || rightFrag.a-Color.a!=0.0)
	{
		//newColor=((Color.rgb+upFrag.rgb)*0.25+(Color.rgb+rightFrag.rgb)*0.25);
		//newColor=(Color.rgb*0.5+(upFrag.rgb+rightFrag.rgb)*0.25);
		// Sets edge to black
		newColor=black;
	}
    
	// Finally assigns gl_FragColor, with a default alpha value of 1.
	gl_FragColor=vec4(newColor,1.0);
}