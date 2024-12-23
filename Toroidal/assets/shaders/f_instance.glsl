#version 150

uniform vec4 uColor;
uniform vec3 uEyePos;

in vec3	Normal;
in vec3 Color;

out vec4 			oColor;

void main( void )
{
	vec3 light = vec3(0,1,0);
	vec3 normal = normalize( Normal );
	float diffuse = max( dot( normal, light ), 0 );
	float spec = pow((dot(reflect(light, normal), normalize(uEyePos))), 20.5) ;
	
	oColor = uColor * diffuse + max(0,spec)+vec4(Color,1.0);
	oColor.a = 1;
}