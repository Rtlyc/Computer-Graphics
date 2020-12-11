/*****************************
 * File: fshader42.glsl
 *       A simple fragment shader
 *****************************/

#version 150  // YJC: Comment/un-comment this line to resolve compilation errors
                 //      due to different settings of the default GLSL version

in  vec4 color;
in  vec3 pointPos;
in  vec2 fTexCoord;
in  float fSphereTexCoord;
in  vec2 fSphereTexCoord2;
in  vec2 latticeSphereTexCoord2;
in  float  fSphereLattice;
out vec4 fColor;

uniform float fogState;
uniform sampler2D texture_2D;
uniform sampler1D texture_1D;
uniform int ground_Textured;
uniform int sphere_Textured;

float fogStart = 0.0f;
float fogEnd = 18.0f;
float fogExp = 0.09f;
vec4 fogColor = vec4(0.7,0.7,0.7,0.5);
vec4 redColor = vec4(0.9,0.1,0.1,1.0);

void main() 
{ 
    float z = length(pointPos.xyz);
    float f = (fogEnd-z)/(fogEnd-fogStart);
    f = clamp(f,0,1);
    float fe = exp(-fogExp*z);
    float fe2 = exp(-fogExp*z*fogExp*z);
    fColor = color;
    if(ground_Textured==1) fColor = color * texture(texture_2D,fTexCoord);
    if(fSphereLattice != 0){
        if(fract(4*latticeSphereTexCoord2[0])<0.35 && fract(4*latticeSphereTexCoord2[1])<0.35) discard;
    }
    if(sphere_Textured==1) fColor = color * texture(texture_1D, fSphereTexCoord);
    else if(sphere_Textured==2){
        fColor = color * texture(texture_2D,fSphereTexCoord2);
        if(fColor[0] == 0){
            fColor = color * redColor;
        }
        }
    if(fogState==1) fColor = mix(fogColor,fColor,f);
    else if(fogState==2) fColor = mix(fogColor,fColor,fe);
    else if(fogState==3) fColor = mix(fogColor,fColor,fe2);
    
} 

