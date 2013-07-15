#include <PlayerEntity.hpp>
#include <System/File.hpp>
#include <System/Memory.hpp>
#include <cstring>
#ifdef ZED_PLATFORM_LINUX
#include <Renderer/OGL/GLModel.hpp>
#include <Renderer/OGL/GLShader.hpp>
#endif

namespace UndeadAnimus
{
	PlayerEntity::PlayerEntity( const ZED::Renderer::Renderer *p_pRenderer )
	{
		m_pRenderer = const_cast< ZED::Renderer::Renderer * >( p_pRenderer );

		m_pModel = ZED_NULL;
		m_pShader = ZED_NULL;
	}

	PlayerEntity::~PlayerEntity( )
	{
		zedSafeDelete( m_pModel );
		zedSafeDelete( m_pShader );
	}

	ZED_UINT32 PlayerEntity::Initialise( )
	{
		ZED_RENDERER_BACKEND RendererBackEnd = m_pRenderer->BackEnd( );
		ZED_CHAR8	*pBinDir = new ZED_CHAR8[ 256 ];
		FILE		*m_pFile = ZED_NULL;

		switch( RendererBackEnd )
		{
			case ZED_RENDERER_BACKEND_OPENGL:
			{
				m_pModel = new ZED::Renderer::GLModel( m_pRenderer );
				m_pShader = new ZED::Renderer::GLShader( );
				break;
			}
			default:
			{
				return ZED_GRAPHICS_ERROR;
			}
		}

		ZED::System::GetExecutablePath( &pBinDir, 256 );

		ZED_CHAR8	*pVertexShader = new ZED_CHAR8[ 256 ];
		memset( pVertexShader, '\0', sizeof( ZED_CHAR8 )*256 );
		strcat( pVertexShader, pBinDir );
		strcat( pVertexShader, "GameMedia/Shaders/Player.vsh" );

		if( m_pShader->Compile(
			const_cast< const ZED_CHAR8 ** >( &pVertexShader ),
			ZED_VERTEX_SHADER, ZED_TRUE ) != ZED_OK )
		{
			zedSafeDeleteArray( pBinDir );
			zedSafeDeleteArray( pVertexShader );

			zedTrace( "[UndeadAnimus::PlayerEntity::Initialise] <ERROR> "
				"Failed to load vertex shader: \"%s\"\n", pVertexShader );

			return ZED_FAIL;
		}
		zedSafeDeleteArray( pVertexShader );

		ZED_CHAR8	*pFragmentShader = new ZED_CHAR8[ 256 ];
		memset( pFragmentShader, '\0', sizeof( ZED_CHAR8 )*256 );
		strcat( pFragmentShader, pBinDir );
		strcat( pFragmentShader, "GameMedia/Shaders/Player.fsh" );

		if( m_pShader->Compile(
			const_cast< const ZED_CHAR8 ** >( &pFragmentShader ),
			ZED_FRAGMENT_SHADER, ZED_TRUE ) != ZED_OK )
		{
			zedSafeDeleteArray( pBinDir );
			zedSafeDeleteArray( pFragmentShader );

			zedTrace( "[UndeadAnimus::PlayerEntity::Initialise] <ERROR> "
				"Failed to load fragment shader: \"%s\"\n", pFragmentShader );

			return ZED_FAIL;
		}
		zedSafeDeleteArray( pFragmentShader );

		ZED_SHADER_VERTEXATTRIBUTE_GL Attributes[ 2 ];
		Attributes[ 0 ].Index = 0;
		Attributes[ 0 ].pName = "vPosition";
		Attributes[ 1 ].Index = 1;
		Attributes[ 1 ].pName = "vColour";
		m_pShader->SetVertexAttributeTypes( Attributes, 2 );

		ZED_SHADER_CONSTANT_MAP Constants[ 10 ];

		zedSetConstant( Constants, 0, ZED_MAT4X4, "uWVP" );
		zedSetConstant( Constants, 1, ZED_FLOAT3, "uGlobalAmbient" );
		zedSetConstant( Constants, 2, ZED_FLOAT3, "uLightColour" );
		zedSetConstant( Constants, 3, ZED_FLOAT3, "uLightPos" );
		zedSetConstant( Constants, 4, ZED_FLOAT3, "uEyePos" );
		zedSetConstant( Constants, 5, ZED_FLOAT3, "uEmissive" );
		zedSetConstant( Constants, 6, ZED_FLOAT3, "uAmbient" );
		zedSetConstant( Constants, 7, ZED_FLOAT3, "uDiffuse" );
		zedSetConstant( Constants, 8, ZED_FLOAT3, "uSpecular" );
		zedSetConstant( Constants, 9, ZED_FLOAT1, "uShininess" );

		m_pShader->SetConstantTypes( Constants, 10 );

		ZED_CHAR8	*pModelPath = new ZED_CHAR8[ 256 ];
		memset( pModelPath, '\0', sizeof( ZED_CHAR8 )*256 );
		strcat( pModelPath, pBinDir );
		strcat( pModelPath, "GameMedia/Models/Player.zed" );

		zedTrace( "[UndeadAnimus::PlayerEntity::Initialise] <INFO> "
			"Attempting to load model file: \"%s\" ... ", pModelPath );

		if( m_pModel->Load( pModelPath ) != ZED_OK )
		{
			zedTrace( "[FAIL]\n" );
			zedTrace( "[UndeadAnimus::PlayerEntity::Initialise] <ERROR> "
				"Failed to open file: \"%s\"\n", pModelPath );
			zedSafeDeleteArray( pBinDir );
			zedSafeDeleteArray( pModelPath );
			return ZED_FAIL;
		}

		zedTrace( "[OK]\n" );

		zedSafeDeleteArray( pBinDir );
		zedSafeDeleteArray( pModelPath );

		ZED_FLOAT32 Shininess = 0.0f;

		ZED::Arithmetic::Vector3 GlobalAmbient( 0.1f, 0.1f, 0.1f );
		ZED::Arithmetic::Vector3 LightColour( 1.0f, 1.0f, 1.0f );
		ZED::Arithmetic::Vector3 Emissive( 0.0f, 0.0f, 0.0f );
		ZED::Arithmetic::Vector3 Ambient( 0.0f, 0.0f, 0.0f );
		ZED::Arithmetic::Vector3 Diffuse( 0.0f, 1.0f, 0.0f );
		ZED::Arithmetic::Vector3 Specular( 0.0f, 0.0f, 0.0f );

		m_pShader->Activate( );
		m_pShader->SetConstantData( 1, ( void * )( &GlobalAmbient ) );
		m_pShader->SetConstantData( 2, ( void * )( &LightColour ) );
		m_pShader->SetConstantData( 5, ( void * )( &Emissive ) );
		m_pShader->SetConstantData( 6, ( void * )( &Ambient ) );
		m_pShader->SetConstantData( 7, ( void * )( &Diffuse ) );
		m_pShader->SetConstantData( 8, ( void * )( &Specular ) );
		m_pShader->SetConstantData( 9, ( void * )( &Shininess ) );

		return ZED_OK;
	}

	void PlayerEntity::Update( const ZED_UINT64 p_ElapsedGameEntity )
	{
		ZED::Arithmetic::Matrix4x4 WVP;
		ZED::Arithmetic::Matrix4x4 PerspProj;
		ZED::Arithmetic::Vector3 Position( 0.0f, 0.0f, 10.0f );
		ZED::Arithmetic::Vector3 Look( 0.0f, 0.0f, 0.0f );
		ZED::Arithmetic::Vector3 Up( 0.0f, 1.0f, 0.0f );

		m_pRenderer->SetViewLookAt( Position, Look, Up );
		m_pRenderer->PerspectiveProjectionMatrix( &PerspProj );
		m_pRenderer->GetWVP( &m_WorldMatrix );

		WVP = PerspProj*m_WorldMatrix;

		ZED_FLOAT32 Matrix[ 16 ];
		WVP.AsFloat( Matrix );
		ZED::Arithmetic::Vector3 LightPosition( 0.0f, 200.0f, 0.0f );

		m_pShader->Activate( );
		m_pShader->SetConstantData( 0, Matrix );
		m_pShader->SetConstantData( 3, ( void * )( &LightPosition ) );
		m_pShader->SetConstantData( 4, ( void * )( &Position ) );
	}

	void PlayerEntity::Render( )
	{
		// There should be some kind of object to represent the player
		m_pModel->Render( );
	}

	void PlayerEntity::Move( const ZED::Arithmetic::Vector3 &p_Velocity )
	{
		// The player will be moving relative to the camera's orientation
		// Which will require knowledge of the camera's yaw
		ZED_FLOAT32 Sin = 0.0f, Cos = 0.0f;
		ZED::Arithmetic::SinCos( m_CameraYaw, Sin, Cos );

		if( p_Velocity[ 0 ] > ZED_Epsilon )
		{
			m_Position[ 0 ] += Cos * p_Velocity[ 0 ];
			m_Position[ 2 ] -= Sin * p_Velocity[ 0 ];
		}
		if( p_Velocity[ 0 ] < -ZED_Epsilon )
		{
			m_Position[ 0 ] -= Cos * p_Velocity[ 0 ];
			m_Position[ 2 ] += Sin * p_Velocity[ 0 ];
		}

		if( p_Velocity[ 2 ] > ZED_Epsilon )
		{
			m_Position[ 0 ] -= Sin * p_Velocity[ 2 ];
			m_Position[ 2 ] -= Cos * p_Velocity[ 2 ];
		}
		if( p_Velocity[ 2 ] < -ZED_Epsilon )
		{
			m_Position[ 0 ] += Sin * p_Velocity[ 2 ];
			m_Position[ 2 ] += Cos * p_Velocity[ 2 ];
		}
	}
}

