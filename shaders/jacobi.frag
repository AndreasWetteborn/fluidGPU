//#extension ARB_texture_rectangle : enable
// pressure , initial value in first iteration is 0
uniform sampler2DRect x;
// divergence of the velocityfield
uniform sampler2DRect b;

//uniform float beta; = 1/4
uniform float alpha ;

void main(void)
{
   vec4 xLeft	= texture2DRect(x , gl_TexCoord[0].st - vec2(1,0));
   vec4 xRight	= texture2DRect(x , gl_TexCoord[0].st + vec2(1,0));
   vec4 xBottom = texture2DRect(x , gl_TexCoord[0].st - vec2(0,1));
   vec4 xTop	= texture2DRect(x , gl_TexCoord[0].st + vec2(0,1));
   
   vec4 bCenter = texture2DRect(b , gl_TexCoord[0].st);
   
   
   
   gl_FragColor  = (xLeft + xRight + xBottom + xTop + alpha * bCenter) * 0.25;
  // gl_FragColor  = (xLeft + xRight + xBottom + xTop  - bCenter) * 0.25;
  
 }