#include <Game.hpp>

namespace UndeadAnimus
{
	Game::Game( )
	{
		m_pRenderer = ZED_NULL;
		m_pConfigFile = ZED_NULL;
		m_FullScreen = ZED_FALSE;
		m_Running = ZED_FALSE;
		m_pInputManager = ZED_NULL;
		m_pWindow = ZED_NULL;
	}

	Game::~Game( )
	{
		if( m_pInputManager )
		{
			delete m_pInputManager;
			m_pInputManager = ZED_NULL;
		}

		if( m_pRenderer )
		{
			delete m_pRenderer;
			m_pRenderer = ZED_NULL;
		}

		if( m_pWindow )
		{
			delete m_pWindow;
			m_pWindow = ZED_NULL;
		}
	}

	ZED_UINT32 Game::Initialise( )
	{
		return ZED_OK;
	}

	void Game::Update( const ZED_UINT64 p_ElapsedGameTime )
	{
	}

	void Game::Render( )
	{
	}

	ZED_UINT32 Game::Execute( )
	{
		return ZED_OK;
	}
}

