//#extension ARB_texture_rectangle : enable

uniform sampler2DRect densitySource;
uniform float coords[2];

uniform float dt;
uniform float rdx;

void main(void) 
{
    float deltaS = coords[0] - gl_TexCoord[0].s ;
    float delatT = coords[1] - gl_TexCoord[0].t;
    float distance = sqrt(deltaS*deltaS + delatT*delatT);

    vec4 X = texture2DRect(densitySource, gl_TexCoord[0].st);

    if(distance < 0.01 / rdx)
    {
        X.x +=  1.0 * dt;
        X.y = 10000.0 * dt;
    }

    gl_FragColor =X;
}