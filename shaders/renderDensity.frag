//#extension ARB_texture_rectangle : enable

uniform sampler2DRect densityR;

void main(void) 
{
    vec4 X = texture2DRect(densityR, gl_TexCoord[0].st); 

    gl_FragColor = vec4(X.x);
}