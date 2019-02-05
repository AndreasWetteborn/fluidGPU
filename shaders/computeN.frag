uniform sampler2DRect vorticitySource;

void main(void)
{

   // sample surrounding pixels 
   float vorticityLeft	= texture2DRect(vorticitySource , gl_TexCoord[0].st - vec2(1,0)).x;
   float vorticityRight	= texture2DRect(vorticitySource , gl_TexCoord[0].st + vec2(1,0)).x;
   float vorticityBottom = texture2DRect(vorticitySource , gl_TexCoord[0].st - vec2(0,1)).x;
   float vorticityTop	= texture2DRect(vorticitySource , gl_TexCoord[0].st + vec2(0,1)).x;
      
   // calculate N 
   vec2 n = vec2((abs(vorticityRight) - abs(vorticityLeft) )  , (abs(vorticityTop) - abs(vorticityBottom) ));
   vec2 N = normalize(n);
   
   gl_FragColor = vec4(N.x, N.y , 0.0 , 0.0 );
  
 }