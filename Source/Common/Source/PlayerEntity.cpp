#include <PlayerEntity.hpp>

namespace UndeadAnimus
{
	ZED_UINT32 PlayerEntity::Initialise( )
	{
		return ZED_OK;
	}

	void PlayerEntity::Update( const ZED_UINT64 p_ElapsedGameEntity )
	{
		// Pump the player event queue
	}

	void PlayerEntity::Render( )
	{
		// There should be some kind of object to represent the player
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

