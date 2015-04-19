#version 330 core

in vec3 position;
in vec4 color;
in vec2 uv;

out vec4 Color;
out vec2 UV;

uniform mat4 Projection;
//uniform mat4 Transf;

void main(){
   gl_Position = Projection * vec4(position, 1.0f);
   Color = color;
   UV = uv;
}