#version 150

uniform vec4 uColor;

in vec3	Normal;

out vec4 			oColor;

void main( void )
{
	vec3 normal = normalize( -Normal );
	float diffuse = max( dot( normal, vec3( 0, 0, -1 ) ), 0 );
	oColor = uColor * diffuse;
	oColor.a = 1;
}