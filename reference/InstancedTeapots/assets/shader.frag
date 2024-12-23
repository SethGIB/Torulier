#version 150

uniform sampler2D uTex0;
uniform vec4 uColor;

in vec3	Normal;
in vec2	TexCoord;

out vec4 			oColor;

void main( void )
{
	vec3 normal = normalize( -Normal );
	float diffuse = max( dot( normal, vec3( 0, 0, -1 ) ), 0 );
	oColor = uColor * diffuse;
}