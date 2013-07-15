#ifndef __UNDEADANIMUS_GAMEENTITY_HPP__
#define __UNDEADANIMUS_GAMEENTITY_HPP__

#include <System/DataTypes.hpp>
#include <Arithmetic/Vector3.hpp>
#include <Renderer/Renderer.hpp>

namespace UndeadAnimus
{
	class GameEntity
	{
	public:
		virtual ~GameEntity( ){ }

		ZED_UINT32 ID( ) const { return m_ID; }
		void ID( const ZED_UINT32 p_ID ){ m_ID = p_ID; }

		virtual ZED_UINT32 Initialise( ) = 0;

		virtual void Update( const ZED_UINT64 p_ElapsedGameTime ) = 0;

		virtual void Render( ) = 0;

		ZED_INLINE void Position( const ZED::Arithmetic::Vector3 &p_Position )
			{ m_Position = p_Position; }

		ZED_INLINE ZED_BOOL Renderable( ) const { return m_Renderable; }
		ZED_INLINE void Renderable( const ZED_BOOL p_Renderable )
			{ m_Renderable = p_Renderable; }

	protected:
		ZED_UINT32 m_ID;

		ZED::Arithmetic::Vector3	m_Position;
		ZED_BOOL					m_Renderable;
		ZED::Renderer::Renderer		*m_pRenderer;
	};
}

#endif

