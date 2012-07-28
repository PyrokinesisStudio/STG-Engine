#include "ResourceHandler.h"
#include "Util.h"

#include "GameManager.h"


namespace RTG
{
	ResourceHandler::ResourceHandler() :	m_Cons(),

											m_GUIDev(),
											m_pGUIFactory( NULL ),
											m_GraphicsDev(),
											m_pGraphicsFactory( NULL ),
											m_InputDev(),
											m_pInputFactory( NULL ),
											m_SoundDev(),
											m_pSoundFactory( NULL ),

											m_Alloc(),

											m_FontFile( NULL ),

											m_MainWnd(),
											m_GLContext(),
											m_GraphicsCtrl(),
											m_Camera(),
											m_Camera3D(),
											m_Canvas(),
											m_Font(),
											m_PointSprite(),
											m_Keyboard(),
											m_StreamingBuf(),
											m_pScore( NULL ),
											
											m_pSaveDataManager( NULL ),
											m_pGBManager( NULL ),
											m_pReplayEntry( NULL ),
											m_pGameManager( NULL )
	{
	}

	ResourceHandler::~ResourceHandler()
	{
		MAPIL::SafeDelete( m_FontFile );

		MAPIL::SafeDelete( m_pSoundFactory );
		MAPIL::SafeDelete( m_pInputFactory );
		MAPIL::SafeDelete( m_pGraphicsFactory );
		MAPIL::SafeDelete( m_pGUIFactory );
		

		MAPIL::SafeDelete( m_pScore );
		MAPIL::SafeDelete( m_pSaveDataManager );
		MAPIL::SafeDelete( m_pReplayEntry );
		MAPIL::SafeDelete( m_pGameManager );
	}

	void ResourceHandler::Init()
	{
		//if( MAPIL::InitMAPIL( "Confetti", 640, 480 ) ){
		//	throw std::exception();
		//}

		// GUI�t�@�N�g���̍쐬
		m_GUIDev = MAPIL::CreateGUIDevice( MAPIL::GUI_API_WIN32API );
		m_pGUIFactory = MAPIL::CreateGUIFactory( m_GUIDev );
		// �O���t�B�b�N�X�t�@�N�g���̍쐬
		m_GraphicsDev = MAPIL::CreateGraphicsDevice( MAPIL::GRAPHICS_API_DIRECT3D );
		m_pGraphicsFactory = CreateGraphicsFactory( m_GraphicsDev );
		// ���̓t�@�N�g���̍쐬
		m_InputDev = MAPIL::CreateInputDevice( MAPIL::INPUT_API_WIN32API );
		m_pInputFactory = new MAPIL::WinAPIInputFactory( m_InputDev );
		// �T�E���h�t�@�N�g���̍쐬
		m_SoundDev = MAPIL::CreateSoundDevice( MAPIL::SOUND_API_OPENAL );
		m_pSoundFactory = new MAPIL::ALSoundFactory( m_SoundDev );
	}

	void ResourceHandler::SetupGlobalResources()
	{
		//----------------------------------
		// �E�B���h�E�֌W�̐ݒ�
		//----------------------------------

		// �E�B���h�E�̏�����
		
		m_MainWnd = m_pGUIFactory->CreateWnd( TSTR( "Window" ) );
		m_GLContext = m_pGUIFactory->CreateGraphicsContext( TSTR( "GLContext" ) );
		m_MainWnd->Create( TSTR( "Confetti" ), m_pGameManager->GetWndWidth(), m_pGameManager->GetWndHeight() );
		m_GLContext->Create( TSTR( "Confetti GraphicsContext" ), m_pGameManager->GetWndWidth(), m_pGameManager->GetWndHeight(), m_MainWnd );
		// �A�C�R���̐ݒ�
		m_MainWnd->SetIcon( TSTR( "Resource/Enemy1.ico" ) );

		//----------------------------------
		// �e��f�o�C�X�̃C���X�^���X��
		//----------------------------------

		m_GraphicsDev->Create( m_GLContext );
		m_InputDev->Create();
		m_SoundDev->Create();

		//----------------------------------
		// �O���[�o�����\�[�X�̍쐬
		//----------------------------------

		// �O���t�B�b�N�X�R���g���[���̍쐬
		m_GraphicsCtrl = m_pGraphicsFactory->CreateGraphicsController( TSTR( "Global Graphics Ctrl" ) );
		m_GraphicsCtrl->Create( m_GLContext );
		// Canvas�I�u�W�F�N�g�̍쐬
		m_Canvas = m_pGraphicsFactory->CreateCanvas2D( TSTR( "Global Canvas 2D" ) );
		m_Canvas->Create();
		// Sprite�I�u�W�F�N�g�̍쐬
		m_Sprite = m_pGraphicsFactory->CreateSprite( TSTR( "Global Sprite" ) );
		m_Sprite->Create();
		// 2D�p�̃J�����̍쐬
		m_Camera = m_pGraphicsFactory->CreateCamera( TSTR( "Global Camera 2D" ) );
		// 3D�p�̃J�����̍쐬
		m_Camera3D = m_pGraphicsFactory->CreateCamera( TSTR( "Global Camera 3D" ) );
		// �t�H���g�̍쐬
		m_Font = m_pGraphicsFactory->CreateGraphicsFont( TSTR( "Global Font" ) );
		m_FontFile = new MAPIL::TTFFile;
		m_FontFile->Load( TSTR( "Test.ttf" ) );
		MAPIL::FontFormat fmt;
		MAPIL::ZeroObject( &fmt, sizeof( fmt ) );
		fmt.m_Height = 16;
		fmt.m_Quality = DEFAULT_QUALITY;
		fmt.m_Thickness = FW_NORMAL;
		fmt.m_FontFaceName = TSTR( "Bauhaus Lt BT �׎�" );
		m_Font->Create( fmt );
		// �L�[�{�[�h�I�u�W�F�N�g�̍쐬
		m_Keyboard = m_pInputFactory->CreateKeyboard( TSTR( "Global Keyboard" ) );
		
	}

	MAPIL::ISprite ResourceHandler::CreateSpriteL( const char* pStr )
	{
		MAPIL::ISprite sprite = m_pGraphicsFactory->CreateSprite( ToTCHAR( pStr ) );
		sprite->Create();

		return sprite;
	}

	MAPIL::IGraphicsFont ResourceHandler::CreateFontL( const char* pStr )
	{
		MAPIL::IGraphicsFont font = m_pGraphicsFactory->CreateGraphicsFont( ToTCHAR( pStr ) );
		MAPIL::FontFormat fmt;
		MAPIL::ZeroObject( &fmt, sizeof( fmt ) );
		fmt.m_Height = 16;
		fmt.m_Quality = DEFAULT_QUALITY;
		fmt.m_Thickness = FW_NORMAL;
		fmt.m_FontFaceName = TSTR( "Bauhaus Lt BT �׎�" );
		font->Create( fmt );

		return font;
	}

	void ResourceHandler::BeginDraw2D()
	{
		m_Sprite->Begin();
	}

	void ResourceHandler::EndDraw2D()
	{
		m_Sprite->End();
	}

	void ResourceHandler::DrawString2D( float x, float y, const char* pStr )
	{
		RTG::DrawString2D( m_Sprite, m_Font, x, y, pStr );
	}

	void ResourceHandler::DrawString2D( float x, float y, int color, const char* pStr, ... )
	{
		::va_list list;
		va_start( list, pStr );
		char str[ 1024 ];
		::vsprintf( str, pStr, list );
		va_end( list );

		RTG::DrawString2D( m_Sprite, m_Font, x, y, color, str );
	}

	void ResourceHandler::RefleshResouces()
	{
		m_pGraphicsFactory->Reflesh( 2 );
		m_pGUIFactory->Reflesh( 2 );
		m_pInputFactory->Reflesh( 2 );
		m_pSoundFactory->Reflesh( 2 );
	}

	void* ResourceHandler::AllocMemory()			// �������̊��蓖��
	{
		return m_Alloc.Alloc();
	}

	void ResourceHandler::ReleaseMemory( void* p )			// �������̊J��
	{
		m_Alloc.Delete( p );
	}
}