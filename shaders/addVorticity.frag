uniform sampler2DRect velocitySource;
uniform sampler2DRect vorticitySource;
uniform sampler2DRect NSource;

uniform float eps;



void main(void)
{

      
   // calculate
   vec4 velocity = texture2DRect(velocitySource , gl_TexCoord[0].st);
   vec2 N =  texture2DRect(NSource , gl_TexCoord[0].st).xy;
   float vorticity = texture2DRect(vorticitySource , gl_TexCoord[0].st).x;
   
	velocity.xy += eps * vec2(N.x * vorticity , N.y*vorticity );


   //gl_FragColor = eps * vec4( (velocity.x + N.x*vorticity) , (velocity.y + N.y*vorticity) , 0.0 , 0.0 );
   gl_FragColor = velocity;
   //gl_FragColor = vec4(N.x, N.y , 0.0 , 0.0 );
  
 }