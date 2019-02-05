//#extension ARB_texture_rectangle : enable

uniform sampler2DRect velocityR;

void main(void) 
{	        
    vec4 x = texture2DRect(velocityR, gl_TexCoord[0].st);      

    gl_FragColor = vec4(abs(x.r)*5.0 , abs(x.y)*5.0, 0.0 ,1.0);                      
}