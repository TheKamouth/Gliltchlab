uniform sampler2D texture;
varying vec2 vTexCoord;

void main(void)
{
    //gl_FragColor = vec4(1,0,0,1);
    gl_FragColor = texture2D(texture, vTexCoord);
}
