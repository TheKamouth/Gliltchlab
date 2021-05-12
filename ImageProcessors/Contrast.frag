uniform sampler2D texture;
in int desaturationMode;
varying vec2 vTexCoord;

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}


void main(void)
{
    vec4 texColor = texture2D(texture, vTexCoord);
    vec3 hsvTexColor = rgb2hsv( vec3(texColor.x, texColor.y, texColor.z));

    switch(desaturationMode)
    {
    case 0:
        // No saturation
        hsvTexColor.y = 0.0;
        break;
    case 1:
        // Max
        hsvTexColor.r = max( hsvTexColor.r, max( hsvTexColor.r, hsvTexColor.b));
        hsvTexColor.g = hsvTexColor.r;
        hsvTexColor.b = hsvTexColor.r;
        break;
    case 2:
        // Min
        hsvTexColor.r = max( hsvTexColor.r, max( hsvTexColor.r, hsvTexColor.b));
        hsvTexColor.g = hsvTexColor.r;
        hsvTexColor.b = hsvTexColor.r;
        break;
    }

    vec3 rgbDesaturatedColor = hsv2rgb(hsvTexColor);

    gl_FragColor = vec4( rgbDesaturatedColor.x, rgbDesaturatedColor.y, rgbDesaturatedColor.z, 1.0f);

    //gl_FragColor = texColor;
}

