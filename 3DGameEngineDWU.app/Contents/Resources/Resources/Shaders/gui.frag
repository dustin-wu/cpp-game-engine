#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D objTexture;
uniform vec3 textColor;

void main()
{
    color = texture(objTexture, TexCoords);
//    color = vec4(vec3(color.a), 1.0);
}
