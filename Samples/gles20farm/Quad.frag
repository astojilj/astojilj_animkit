uniform sampler2D  sColorBufferTexture;	
varying mediump vec2 t1;

void main()
{
	gl_FragColor = texture2D(sColorBufferTexture, t1);
}