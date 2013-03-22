#version 120

//uniform sampler2DRect texture;
uniform sampler2D texture;
void main(){
	
    
       // vec4 rgb = texture2DRect(texture, gl_TexCoord[0].st) ;
        vec4 rgb = texture2D(texture, gl_TexCoord[0].st) ;
       // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        gl_FragColor = vec4(rgb, 1.0);

    
   
	
}

