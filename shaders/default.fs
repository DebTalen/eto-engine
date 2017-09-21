#version 330 core

in  vec2 texturePos;
out vec4 color;

uniform sampler2D texture2D;
uniform vec3 	  textureColor;


void main()
{
// 	vec4(textureColor, 1.0f) * ...
 	color = texture(texture2D, texturePos);
}
