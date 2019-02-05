uniform sampler2DRect velocitySource;

void main(void)
{

   // sample surrounding pixels 
   float velocityLeft	= texture2DRect(velocitySource , gl_TexCoord[0].st - vec2(1,0)).y;
   float velocityRight	= texture2DRect(velocitySource , gl_TexCoord[0].st + vec2(1,0)).y;
   float velocityBottom = texture2DRect(velocitySource , gl_TexCoord[0].st - vec2(0,1)).x;
   float velocityTop	= texture2DRect(velocitySource , gl_TexCoord[0].st + vec2(0,1)).x;
      
   // calculate vorticity  
   float w = (velocityRight - velocityLeft ) - (velocityTop - velocityBottom );
   
   gl_FragColor = vec4(w, 0.0, 0.0 , 0.0 );
  
 }