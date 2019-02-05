//#extension ARB_texture_rectangle : enable

uniform sampler2DRect velocity;
uniform sampler2DRect pressure;

uniform float halfrdx;

void main(void)
{
   float pressureLeft = texture2DRect(pressure, gl_TexCoord[0].st - vec2(1,0)).x;
   float pressureRight = texture2DRect(pressure, gl_TexCoord[0].st + vec2(1,0)).x;
   float pressureBottom = texture2DRect(pressure, gl_TexCoord[0].st - vec2(0,1)).x;
   float pressureTop = texture2DRect(pressure, gl_TexCoord[0].st + vec2(0,1)).x;

   vec4 currentVelocity = texture2DRect(velocity, gl_TexCoord[0].st);

   currentVelocity.xy -= halfrdx * vec2((pressureRight - pressureLeft),(pressureTop -  pressureBottom));
   gl_FragColor = currentVelocity;
}