//#extension ARB_texture_rectangle : enable


uniform sampler2DRect velocitySource;
uniform sampler2DRect particleSource;

void main(void) 
{

	vec2 pos = texture2DRect(particleSource, gl_TexCoord[0].st).xy;
	vec2 velocity = texture2DRect(velocitySource,  pos ).xy;   	
	
	
	/*
	// calculate grid coordinates
	vec4 coords;
	coords.xy = floor(pos.xy-0.5)+0.5;
	//coords.xy = floor(pos.xy);
	coords.zw = coords.xy + 1.0;
	
	
	// get values from texture
	vec4 value11 =  texture2DRect(velocitySource, coords.xy);  
	vec4 value21 =  texture2DRect(velocitySource, coords.zy);  
	vec4 value12 =  texture2DRect(velocitySource, coords.xw);
	vec4 value22 =  texture2DRect(velocitySource, coords.zw);  
	

	
	// interpolation amount 
	vec2 amount;
	amount = pos - coords.xy;
	
	
	// interpolation,  final new value
	// first interpolate between x values 
	// then interpolate between the 2 new values to get the final value	
	vec4 velocity = mix( mix(value11,value21,amount.x) , mix(value12,value22,amount.x) ,amount.y);
	/**/
	//os = pos +  vec2(0.5, 0.0);
	
	if (length(velocity)> 0.1 )
	{
		pos = pos + velocity.xy * 0.005;
	}
	/*
	if (pos.x < 5 )
	{
		pos.x = pos.x + 5;
	} /**/
	
	 gl_FragColor = vec4(pos.x, pos.y, 0.0, 0.0);
	 //gl_FragColor = vec4(0.0);
	
	
}