//#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect x;
uniform float offset[2];
uniform float scale;


void main(void) 
{

	vec2 offsetVector = vec2(offset[0], offset[1]);
	vec4 value = scale * texture2DRect( x, gl_TexCoord[0].st + offsetVector );
	
	gl_FragColor = value;     
	//gl_FragColor =vec4(1.0, 0.0 ,0.0 ,1.0); 	
}