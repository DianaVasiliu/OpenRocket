#version 400

in vec4 ex_Color;
in vec2 tex_Coord;

vec4 red = vec4(1.0,0.0,0.0,1.0);
vec4 green= vec4(0.0,1.0,0.0,1.0);

uniform int codCol;
uniform sampler2D myTexture;
 
out vec4 out_Color;

void main(void)
{
	out_Color = texture(myTexture, tex_Coord);
}
