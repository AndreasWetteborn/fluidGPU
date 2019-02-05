uniform sampler2DRect density;
uniform sampler2DRect currentVelocity;

uniform float scale;

void main(void) 
{
    vec2 desityTemp = texture2DRect(density, gl_TexCoord[0].st).xy; 
    vec4 velocity = texture2DRect(currentVelocity, gl_TexCoord[0].st);

    velocity.xy += scale * (-5.*desityTemp.x + desityTemp.y ) * vec2(0.0, 1.0);

    gl_FragColor = velocity;
}