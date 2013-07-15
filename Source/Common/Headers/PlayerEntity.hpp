#ifndef __UNDEADANIMUS_PLAYERENTITY_HPP__
#define __UNDEADANIMUS_PLAYERENTITY_HPP__

#include <GameEntity.hpp>
#include <Arithmetic/Matrix4x4.hpp>
#include <Renderer/Model.hpp>
#include <Renderer/Shader.hpp>

namespace UndeadAnimus
{
	class PlayerEntity : public GameEntity
	{
	public:
		PlayerEntity( const ZED::Renderer::Renderer *p_pRenderer );
		virtual ~PlayerEntity( );

		virtual ZED_UINT32 Initialise( );

		virtual void Update( const ZED_UINT64 p_ElapsedGameTime );

		virtual void Render( );

		void Move( const ZED::Arithmetic::Vector3 &p_Velocity );

		ZED_INLINE void CameraYawRotation( const ZED_FLOAT32 p_CameraYaw )
			{ m_CameraYaw = p_CameraYaw; }

	private:
		ZED_FLOAT32					m_CameraYaw;
		ZED::Arithmetic::Matrix4x4	m_WorldMatrix;
		ZED::Renderer::Model		*m_pModel;
		ZED::Renderer::Shader		*m_pShader;
	};
}

#endif

