#include <Game.hpp>
#include <Renderer/LinuxRendererOGL3.hpp>
#include <System/LinuxInputManager.hpp>
#include <System/LinuxWindow.hpp>
#include <System/Time.hpp>
#include <System/Memory.hpp>

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
		m_pPlayer = ZED_NULL;
	}

	Game::~Game( )
	{
		if( m_pInputManager )
		{
			delete m_pInputManager;
			m_pInputManager = ZED_NULL;
		}

		if( m_pPlayer )
		{
			zedSafeDelete( m_pPlayer );
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
		ZED_UINT32 X = 0, Y = 0, Width = 0, Height = 0;
		ZED::System::ZED_SCREENSIZE NativeSize;
		ZED::System::ZED_SCREENSIZE *pScreenSizes = ZED_NULL;
		ZED_MEMSIZE ScreenSizeCount = 0;

		m_pRenderer = new ZED::Renderer::LinuxRendererOGL3( );
		m_pWindow = new ZED::System::LinuxWindow( );

		ZED::System::EnumerateScreenSizes( &pScreenSizes, &ScreenSizeCount,
			ZED::System::GetCurrentScreenNumber( ) );

		if( ZED::System::GetNativeScreenSize(
			ZED::System::GetCurrentScreenNumber( ), NativeSize ) != ZED_OK )
		{
			zedTrace( "[UndeadAnimus::Game::Initialise] <ERROR> "
				"Could not get native screen size\n" );

			return ZED_FAIL;
		}

		if( m_FullScreen )
		{
			X = 0;
			Y = 0;
			Width = NativeSize.Width;
			Height = NativeSize.Height;
		}
		else
		{
#ifdef ZED_BUILD_DEBUG
			Width = 1280;
			Height = 720;
			X = ( NativeSize.Width / 2 ) - ( Width / 2 );
			Y = ( NativeSize.Height / 2 ) - ( Height / 2 );
#else
#endif
		}

		m_pWindow->Create( X, Y, Width, Height );

		m_Canvas.Width( Width );
		m_Canvas.Height( Height );

		m_Canvas.BackBufferCount( 1 );
		m_Canvas.DepthStencilFormat( ZED_FORMAT_D24S8 );
		m_Canvas.ColourFormat( ZED_FORMAT_ARGB8 );

		m_pRenderer->Create( m_Canvas, ( *m_pWindow ) );
		m_pRenderer->ClearColour( 0.2f, 0.0f, 0.0f );
		
		ZED::System::StartTime( );

		m_pRenderer->SetRenderState( ZED_RENDERSTATE_CULLMODE,
			ZED_CULLMODE_CCW );
		m_pRenderer->SetRenderState( ZED_RENDERSTATE_DEPTH, ZED_ENABLE );
		m_pRenderer->SetClippingPlanes( 1.0f, 100000.0f );
		m_pRenderer->PerspectiveProjectionMatrix( 45.0f,
			static_cast< ZED_FLOAT32 >( Width ) /
			static_cast< ZED_FLOAT32 >( Height ) );

		ZED::System::ZED_WINDOWDATA WinData = m_pWindow->WindowData( );
		m_pInputManager =
			new ZED::System::LinuxInputManager( WinData.pX11Display );
		m_pInputManager->AddDevice( &m_Keyboard );

		if( pScreenSizes )
		{
			delete [ ] pScreenSizes;
			pScreenSizes = ZED_NULL;
		}

		m_pPlayer = new PlayerEntity( m_pRenderer );

		if( m_pPlayer->Initialise( ) != ZED_OK )
		{
			zedTrace( "[UndeadAnimus::Game::Initialise] <ERROR> "
				"Something went wrong loading the model file\n" );
			return ZED_FAIL;
		}

		m_Running = ZED_TRUE;

		return ZED_OK;
	}

	void Game::Update( const ZED_UINT64 p_ElapsedGameTime )
	{
		m_pPlayer->Update( p_ElapsedGameTime );
	}

	void Game::Render( )
	{
		m_pRenderer->BeginScene( ZED_TRUE, ZED_TRUE, ZED_TRUE );
		m_pPlayer->Render( );
		m_pRenderer->EndScene( );
	}

	ZED_UINT32 Game::Execute( )
	{
		XEvent Events;
		KeySym Key;
		ZED::System::ZED_WINDOWDATA WinData = m_pWindow->WindowData( );
		ZED_UINT64 ElapsedTime = 0ULL;
		ZED_UINT64 TimeStep = 16667ULL;
		ZED_UINT64 OldTime = ZED::System::GetTimeMiS( );
		ZED_UINT64 FrameTime = ZED::System::GetTimeMiS( );
		ZED_MEMSIZE FrameRate = 0;
		ZED_UINT64 Accumulator = 0ULL;

		while( m_Running == ZED_TRUE )
		{
			m_pInputManager->Update( );

			// All of the key checking code needs to be rectified before
			// getting too deep into gameplay
			if( m_Keyboard.IsKeyDown( K_ESCAPE ) )
			{
				m_Running = ZED_FALSE;
			}
			
			if( m_Keyboard.IsKeyDown( 'w' ) )
			{
				m_pPlayer->Move( ZED::Arithmetic::Vector3( 0.0f, 0.0f, 0.1f ) );
			}
			if( m_Keyboard.IsKeyDown( 'd' ) )
			{
				m_pPlayer->Move( ZED::Arithmetic::Vector3( 0.0f, 0.0f, -0.1f ) );
			}
			if( m_Keyboard.IsKeyDown( 's' ) )
			{
				m_pPlayer->Move( ZED::Arithmetic::Vector3( -0.1f, 0.0f, 0.0f ) );
			}
			if( m_Keyboard.IsKeyDown( 'f' ) )
			{
				m_pPlayer->Move( ZED::Arithmetic::Vector3( 0.1f, 0.0f, 0.0f ) );
			}
			if( m_Keyboard.IsKeyDown( 'q' ) )
			{
				m_pPlayer->Position( ZED::Arithmetic::Vector3( 0.0f, 0.0f, 0.0f ) );
			}

			while( XPending( WinData.pX11Display ) > 0 )
			{
				XNextEvent( WinData.pX11Display, &Events );
				switch( Events.type )
				{
					case EnterNotify:
					{
						XGrabKeyboard( WinData.pX11Display, WinData.X11Window,
							True, GrabModeAsync, GrabModeAsync, CurrentTime );
						XGrabPointer( WinData.pX11Display, WinData.X11Window,
							True, EnterWindowMask | LeaveWindowMask |
							PointerMotionMask, GrabModeAsync, GrabModeAsync,
							None, None, CurrentTime );
						m_pWindow->HideCursor( );
						break;
					}
					case LeaveNotify:
					{
						XUngrabPointer( WinData.pX11Display, CurrentTime );
						XUngrabKeyboard( WinData.pX11Display, CurrentTime );
						m_pWindow->ShowCursor( );

						break;
					}
					default:
					{
						break;
					}
				}
			}

			const ZED_UINT64 NewTime = ZED::System::GetTimeMiS( );
			ZED_UINT64 DeltaTime = NewTime - OldTime;

			if( DeltaTime > 250000ULL )
			{
				DeltaTime = 250000ULL;
			}

			OldTime = NewTime;

			Accumulator += DeltaTime;

			while( Accumulator >= TimeStep )
			{
				this->Update( 0.0f );
				ElapsedTime += TimeStep;
				Accumulator -= TimeStep;
			}

			this->Render( );
			++FrameRate;

			if( ( NewTime - FrameTime ) >= 1000000ULL )
			{
				zedTrace( "FPS: %d\n", FrameRate );
				FrameTime = ZED::System::GetTimeMiS( );
				FrameRate = 0;
			}
		}

		return ZED_OK;
	}
}

