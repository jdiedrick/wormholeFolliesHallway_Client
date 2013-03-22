uniform sampler2DRect tex0;

uniform float scale;
const vec4 black = vec4(vec3(0.), 1.);
const vec4 white = vec4(vec3(1.), 1.);
const float aspectRatio = .75;
void main(){

    vec2 textureCoord = gl_TexCoord[0].xy;

vec4 srcPixel= texture2DRect( tex0, textureCoord);

//vec2 wrapped = mod(gl_TexCoord[0].xy - vec2(.5), scale);

vec2 normalized = gl_TexCoord[0].xy;

normalized.x *=aspectRatio;

//vec2 wrapped = mod(normalized - vec2(.5), scale);

vec2 wrapped = mod(normalized, scale);

wrapped /= scale;

if(wrapped.x < .5){
	if(wrapped.y < .5){
		gl_FragColor = vec4 (0.0, srcPixel.b,srcPixel.g, 0.5); //black;
		}	else	{
		gl_FragColor = vec4 ( srcPixel.b, 2.0*(srcPixel.r), srcPixel.g, 1.0); //white;
		}
		} else{
			if(wrapped.y<.5){
				gl_FragColor = vec4 (srcPixel.g,1.0 , 1.0, 0.5); //white;
		} else{
				gl_FragColor = vec4 (srcPixel.g, srcPixel.g, srcPixel.g, 0.5); //black;
		}
	}
}

