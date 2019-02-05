//#extension ARB_texture_rectangle : enable

uniform sampler2DRect velocitySource;

uniform float coords[2];
uniform float direction[2];

uniform float dt;
uniform float rdx;


void main(void) 
{     

	// ska skrivas om men vektorer
	float deltaS = coords[0] - gl_TexCoord[0].s ;
	float delatT = coords[1] - gl_TexCoord[0].t;
	float distance = sqrt(deltaS*deltaS + delatT*delatT);       
	        
    vec4 x = texture2DRect(velocitySource, gl_TexCoord[0].st);    
    
    if(distance < 0.01/rdx )    
    {
		x.x += direction[0]*15.0f * dt;
		x.y += direction[1]*15.0f * dt;
	}	
	
    gl_FragColor =x;                      
}