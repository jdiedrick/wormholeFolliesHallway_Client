//kyle's checkerboard

uniform float scale;
const vec4 black = vec4(vec3(0.), 1.);
const vec4 white = vec4(vec3(1.), 1.);
const float aspectRatio = .75;
void main(){
    
    //vec2 wrapped = mod(gl_TexCoord[0].xy - vec2(.5), scale);
    
    vec2 normalized = gl_TexCoord[0].xy;
    
    normalized.y *=aspectRatio;
    
    //vec2 wrapped = mod(normalized - vec2(.5), scale);
    
    vec2 wrapped = mod(normalized, scale);
    
    wrapped /= scale;
    
    if(wrapped.x < .5){
        if(wrapped.y < .5){
            gl_FragColor = black;
		}	else	{
            gl_FragColor = white;
		}
    } else{
        if(wrapped.y<.5){
            gl_FragColor = white;
		} else{
            gl_FragColor = black;
		}
	}
}

