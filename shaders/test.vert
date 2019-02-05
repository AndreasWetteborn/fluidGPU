//varying vec2 myTexCood; 
void main() 
{ 
  // We compute the vertex position as the fixed function does. 
  gl_Position = ftransform(); 
  // We fill our varying variable with the texture 
  //coordinate related to the texture unit 0 (gl_MultiTexCoord0 refers to the TU0   
  //interpolator). 
 
 // myTexCoord = vec2(gl_MultiTexCoord0);   
}