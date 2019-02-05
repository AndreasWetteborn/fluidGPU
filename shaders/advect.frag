//#extension ARB_texture_rectangle : enable

uniform sampler2DRect velocitySource;
uniform sampler2DRect quantityToAdvect;

uniform float dt;
uniform float rdx;


void main(void) 
{
    // trace velocity backwards in time, 
    // current position - timestemp * 1/grid scalce * velocitydirection  
    vec2 pos = gl_TexCoord[0].st - dt * rdx * texture2DRect(velocitySource, gl_TexCoord[0].st).xy;    

    // calculate grid coordinates
    vec4 coords;
    coords.xy = floor(pos.xy-0.5)+0.5;
    //coords.xy = floor(pos.xy);
    coords.zw = coords.xy + 1.0;

    // get values from texture
    vec4 value11 = texture2DRect(quantityToAdvect, coords.xy);
    vec4 value21 = texture2DRect(quantityToAdvect, coords.zy);
    vec4 value12 = texture2DRect(quantityToAdvect, coords.xw);
    vec4 value22 = texture2DRect(quantityToAdvect, coords.zw);

    // interpolation amount 
    vec2 amount;
    amount = pos - coords.xy;

    // interpolation,  final new value
    // first interpolate between x values 
    // then interpolate between the 2 new values to get the final value	
    vec4 velocity = mix( mix(value11,value21,amount.x) , mix(value12,value22,amount.x) ,amount.y);

     gl_FragColor = velocity;
     //gl_FragColor = vec4(0.0);
}