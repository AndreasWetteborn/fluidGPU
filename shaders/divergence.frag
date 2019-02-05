//#extension ARB_texture_rectangle : enable

uniform sampler2DRect velocity;

uniform float halfrdx;

void main(void)
{
   vec4 velLeft	= texture2DRect(velocity, gl_TexCoord[0].st - vec2(1,0));
   vec4 velRight = texture2DRect(velocity, gl_TexCoord[0].st + vec2(1,0));
   vec4 velBottom = texture2DRect(velocity, gl_TexCoord[0].st - vec2(0,1));
   vec4 velTop = texture2DRect(velocity, gl_TexCoord[0].st + vec2(0,1));

   float divergence = halfrdx * ((velRight.x - velLeft.x) + (velTop.y - velBottom.y));
   gl_FragColor  = vec4(divergence);

}