#version 150

uniform mat4	ciModelViewProjection;
uniform mat3	ciNormalMatrix;

in vec4		ciPosition;
in vec3		ciNormal;
in vec3		vInstancePosition; // per-instance position variable

out highp vec3	Normal;

void main( void )
{
	gl_Position	= ciModelViewProjection * ( 4.0 * ciPosition + vec4( vInstancePosition, 0 ) );
	Normal		= ciNormalMatrix * ciNormal;
}