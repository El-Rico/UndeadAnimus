#version 330

in vec3 vPosition;
in vec3 vNormal;

uniform mat4 uWVP;

out vec4 fPosition;
out vec3 fObjectPos;
out vec3 fNormal;
//varying vec3 fColour;

void main( )
{
/*	gl_Position = uWVP*vec4( vPosition, 1.0 );
	vec3 P = gl_Position.xyz;
	vec3 N = vNormal;
	vec3 Emissive = vec3( 0.0, 0.0, 0.0 );
	vec3 Ambient = vec3( 0.0, 0.0, 0.0 );
	vec3 L = normalize( vec3( 0.0, 12.0, 0.0 ) - P );
	float DiffuseLight = max( dot( N, L ), 0.0 );
	vec3 Diffuse = vec3( 1.0, 1.0, 1.0 ) * vec3( 1.0, 1.0, 1.0 ) * DiffuseLight;

	vec3 V = normalize( vec3( 0.0, 0.0, 10.0 ) - P );
	vec3 H = normalize( L + V );
	float SpecularLight = pow( max( dot( N, H ), 0.0 ), 1.0 );
	if( DiffuseLight <= 0.0 )
	{
		SpecularLight = 0.0;
	}

	vec3 Specular = vec3( 0.0, 0.0, 0.0 )*vec3( 1.0, 1.0, 1.0 )*SpecularLight;

	fColour = Emissive + Ambient + Diffuse + Specular;*/

//	fPosition = uWVP*vec4( vPosition, 1.0 );
	fObjectPos = vPosition;
	fNormal = vNormal;

	gl_Position = uWVP*vec4( vPosition, 1.0 );
}
