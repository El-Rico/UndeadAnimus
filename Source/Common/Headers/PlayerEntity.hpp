#ifndef __UNDEADANIMUS_PLAYERENTITY_HPP__
#define __UNDEADANIMUS_PLAYERENTITY_HPP__

#include <GameEntity.hpp>

namespace UndeadAnimus
{
	class PlayerEntity : public GameEntity
	{
	public:
		virtual ZED_UINT32 Initialise( );

		virtual void Update( const ZED_UINT64 p_ElapsedGameTime );

		virtual void Render( );

		void Move( const ZED::Arithmetic::Vector3 &p_Velocity );

		ZED_INLINE void CameraYawRotation( const ZED_FLOAT32 p_CameraYaw )
			{ m_CameraYaw = p_CameraYaw; }

	private:
		ZED_FLOAT32 m_CameraYaw;
	};
}

#endif

