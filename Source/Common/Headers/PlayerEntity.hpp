#ifndef __UNDEADANIMUS_PLAYERENTITY_HPP__
#define __UNDEADANIMUS_PLAYERENTITY_HPP__

#include <GameEntity.hpp>
#include <Arithmetic/Matrix4x4.hpp>
#include <Arithmetic/Vector3.hpp>
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

		void BoundingBox( ZED::Arithmetic::AABB *p_pBoundingBox )
			{ m_pModel->BoundingBox( p_pBoundingBox ); }

		ZED_INLINE void CameraYawRotation( const ZED_FLOAT32 p_CameraYaw )
			{ m_CameraYaw = p_CameraYaw; }

	private:
		ZED_FLOAT32					m_CameraYaw;
		ZED::Arithmetic::Matrix4x4	m_WorldMatrix;
		ZED::Renderer::Model		*m_pModel;
		ZED::Renderer::Shader		*m_pShader;
		ZED::Arithmetic::Vector3	m_CameraDirection;
		ZED::Arithmetic::Vector3	m_CameraPosition;
	};
}

#endif

