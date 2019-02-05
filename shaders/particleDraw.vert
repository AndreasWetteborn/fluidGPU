//uniform mat4 ProjectionMatrix;
//uniform mat4 ViewMatrix;
//uniform mat4 ModelMatrix;

uniform sampler2DRect particleTex;

void main(void)
{
	vec4 particle = texture2DRect(particleTex, gl_Color.st);
	
   gl_PointSize = 2.0;
   //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  // gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_Color.x ,gl_Color.y ,particle.x, 1.0);
   gl_Position = gl_ModelViewProjectionMatrix * vec4(particle.x ,particle.y ,0.0, 1.0);
   
}