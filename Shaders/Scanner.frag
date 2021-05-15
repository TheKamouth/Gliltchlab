in vec2 vTexCoord;
uniform sampler2D texture;

void main(void)
{
	gl_FragColor = vec4(1,0,0,1);
	//gl_FragColor = texture2D(texture, vTexCoord);
}
