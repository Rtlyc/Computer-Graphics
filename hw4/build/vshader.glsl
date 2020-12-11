#version 150

in  vec3 vVelocity;
in  vec3 vColor;
out vec4 color;
out float yCoord;

uniform mat4 model_view;
uniform mat4 projection;
uniform float  t;

void main() 
{
    float x = 0.001*vVelocity[0]*t;
    float y = 0.1+0.001*vVelocity[1]*t+0.5*(-0.00000049)*t*t;
    float z = 0.001*vVelocity[2]*t;
    vec4 vColor4 = vec4(vColor.r, vColor.g, vColor.b, 1.0); 
    gl_Position = projection * model_view * vec4(x,y,z,1.0);

    yCoord = gl_Position.y;
    color = vColor4;
} 