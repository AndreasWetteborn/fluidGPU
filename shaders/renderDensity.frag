//#extension ARB_texture_rectangle : enable

uniform sampler2DRect densityR;

void main(void) 
{	        
    vec4 X = texture2DRect(densityR, gl_TexCoord[0].st);      

   // gl_FragColor = vec4(X.x, X.x*0.7 , X.x *0.1, 1.0);
	gl_FragColor = vec4(X.x);      
	//gl_FragColor = vec4(gl_TexCoord[0].x/512.0 , gl_TexCoord[0].y/256.0 , X.x/100 , 1.0);
	//gl_FragColor = vec4(1 , 1 , X.x/100 , 1.0);
}