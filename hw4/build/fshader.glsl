#version 150 

in vec4 color;
in float yCoord;
out vec4 fColor;

void main(){
    if(yCoord<0.1f) discard;
    fColor = color;
}