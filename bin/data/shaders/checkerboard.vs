varying float  TexCoordx;
varying float TexCoordy;

void main()
{
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
	TexCoordx = gl_TexCoord[0].x;
    TexCoordy = gl_TexCoord[0].y;
}
