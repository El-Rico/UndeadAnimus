#ifndef __UNDEADANIMUS_GAME_HPP__
#define __UNDEADANIMUS_GAME_HPP__

#include <DataTypes.hpp>

namespace UndeadAnimus
{
	class Game
	{
	public:
		Game( );
		~Game( );

		ZED_UINT32 Initialise( );
		void Update( const ZED_UINT64 p_ElapsedGameTime );
		void Render( );

		ZED_UINT32 Execute( );

		ZED_INLINE ZED_BOOL Running( ) const { return m_Running; }

	private:
		ZED_BOOL	m_Running;
	};
}

#endif

