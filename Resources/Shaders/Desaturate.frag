//#version 330 core

in vec2 vTexCoord;
uniform sampler2D texture;
// [0 ; desaturationModeCount]
uniform int desaturationMode;
// [0 ; 1]
uniform float desaturationValue;

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
	vec3 rgbDesaturatedColor;

	switch(desaturationMode)
	{

	case 0:
		// Lightness
		float lightness = hsvTexColor.b;
		rgbDesaturatedColor.r = 255;
		rgbDesaturatedColor.g = lightness;
		rgbDesaturatedColor.b = lightness;
		break;

	case 1:
		// Average
		float averageRgb = (texColor.r + texColor.g + texColor.b) / 3.0;
		rgbDesaturatedColor.r = averageRgb;
		rgbDesaturatedColor.g = averageRgb;
		rgbDesaturatedColor.b = averageRgb;
		break;

	case 2:
		// Min
		float minRgb = min( texColor.r, min( texColor.r, texColor.b));
		rgbDesaturatedColor.r = minRgb;
		rgbDesaturatedColor.g = minRgb;
		rgbDesaturatedColor.b = minRgb;
		break;

	case 3:
		// Max
		float maxRgb = max( texColor.r, max( texColor.g, texColor.b));
		rgbDesaturatedColor.r = maxRgb;
		rgbDesaturatedColor.g = maxRgb;
		rgbDesaturatedColor.b = maxRgb;
		break;

		// Modes that are glitchy:
	case 4:
		// HslAverageToRgb
		float averageHsv = (hsvTexColor.r + hsvTexColor.g + hsvTexColor.b) / 3.0;
		hsvTexColor.r = averageHsv;
		hsvTexColor.g = averageHsv;
		hsvTexColor.b = averageHsv;
		rgbDesaturatedColor = hsv2rgb(hsvTexColor);
		break;

	case 5:
		// HslMinToRgb
		float maxHsv = max( hsvTexColor.r, max( hsvTexColor.g, hsvTexColor.b));
		hsvTexColor.r = maxHsv;
		hsvTexColor.g = maxHsv;
		hsvTexColor.b = maxHsv;
		rgbDesaturatedColor = hsv2rgb(hsvTexColor);
		break;

	case 6:
		//  HslMaxToRgb
		float minHsv = min( hsvTexColor.r, min( hsvTexColor.g, hsvTexColor.b));
		hsvTexColor.r = minHsv;
		hsvTexColor.g = minHsv;
		hsvTexColor.b = minHsv;
		rgbDesaturatedColor = hsv2rgb(hsvTexColor);
		break;

	case 7:
		// Debug
		rgbDesaturatedColor = vec3(255.0,0.,255.0);
		break;
	}

	vec3 attenuatedColor = desaturationValue * rgbDesaturatedColor;
	vec3 allRed = vec3(255.0, 0.0, 255.0);

	//gl_FragColor = vec4( allRed, 1.0f);
	gl_FragColor = vec4( attenuatedColor, 1.0f);
}
