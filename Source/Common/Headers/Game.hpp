#ifndef __UNDEADANIMUS_GAME_HPP__
#define __UNDEADANIMUS_GAME_HPP__

#include <System/DataTypes.hpp>
#include <System/Window.hpp>
#include <Renderer/Renderer.hpp>
#include <System/InputManager.hpp>
#include <System/Keyboard.hpp>
#include <Renderer/CanvasDescription.hpp>
#include <cstdio>

static const ZED_CHAR8 *g_pWindowTitle =
#ifdef ZED_BUILD_DEBUG
"Undead Animus [DEBUG]";
#elif defined ZED_BUILD_PROFILE
"Undead Animus [PROFILE]";
#elif defined ZED_BUILD_RELEASE
"Undead Animus";
#else
"";
#error Unknown build profile
#endif

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
#if ( ZED_PLATFORM_PANDORA_LINUX || ZED_PLATFORM_LINUX )
		FILE	*m_pConfigFile;
#elif ( ZED_PLATFORM_WINDOWS )
		HANDLE	m_ConfigFile;
		static LRESULT CALLBACK WindowProc( HWND p_HWND, UINT p_Message,
			WPARAM p_WPARAM, LPARAM p_LPARAM );
#endif

		ZED_BOOL	m_Running;
		ZED_BOOL	m_FullScreen;

		ZED::Renderer::Renderer				*m_pRenderer;
		ZED::Renderer::CanvasDescription	m_Canvas;
		ZED::System::Window					*m_pWindow;
		ZED::System::InputManager			*m_pInputManager;
		ZED::System::Keyboard				m_Keyboard;
	};
}

#endif

