#version 330 core

in vec2 tex_coord;
out vec4 color;

uniform sampler2D objTexture;



void main(void){

    color = texture(objTexture, tex_coord);

}
