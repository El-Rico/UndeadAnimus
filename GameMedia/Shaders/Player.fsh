#version 330

precision highp float;

uniform vec3 uGlobalAmbient;
uniform vec3 uLightColour;
uniform vec3 uLightPos;
uniform vec3 uEyePos;
uniform vec3 uEmissive;
uniform vec3 uAmbient;
uniform vec3 uDiffuse;
uniform vec3 uSpecular;
uniform float uShininess;

in vec4 fPosition;
in vec3 fObjectPos;
in vec3 fNormal;

void main( )
{
	vec3 P = fObjectPos;
	vec3 N = normalize( fNormal );

	vec3 Emissive = uEmissive;

	vec3 Ambient = uAmbient*uGlobalAmbient;

	vec3 L = normalize( uLightPos - P );
	float DiffuseLight = max( dot( N, L ), 0.0 );
	vec3 Diffuse = uDiffuse * uLightColour * DiffuseLight;

	vec3 V = normalize( uEyePos - P );
	vec3 H = normalize( L + V );
	float SpecularLight = pow( max( dot( N, H ), 0.0 ), uShininess );
	if( DiffuseLight <= 0.0 )
	{
		SpecularLight = 0.0;
	}

	vec3 Specular = uSpecular * uLightColour * SpecularLight;

	gl_FragColor.rgb = Emissive + Ambient + Diffuse + Specular;
	gl_FragColor.a = 1.0;

	//gl_FragColor = vec4( fColour, 1.0 );
}
