#include "MainFrame.h"

#include "ResourceHandler.h"

#include "Menu.h"
#include "GeneralButtonManager.h"
#include "ScreenShot.h"

#include "GameManager.h"

#include "ScriptCompiler.h"

#include <iostream>

namespace RTG
{
	MainFrame::MainFrame() : m_pScene( NULL ), m_FPS()
	{
	}

	MainFrame::~MainFrame()
	{
		RTG::ResourceHandler* p = RTG::ResourceHandler::GetInst();

		MAPIL::SafeDelete( m_pScene );

		MAPIL::SafeDelete( p->m_pSaveDataManager );

		MAPIL::SafeDelete( p->m_pGBManager );

		MAPIL::SafeDelete( p->m_pReplaySaver );

		MAPIL::SafeDelete( p->m_pCompiler );
	}

	void MainFrame::Init()
	{
		RTG::ResourceHandler* p = RTG::ResourceHandler::GetInst();
		
		// �ݒ�t�@�C���̓ǂݍ���
		p->m_pGameManager = new GameManager();
		p->m_pGameManager->Init();
		p->m_pGameManager->LoadConfigFile( "config/config.dat" );

		// ������
		p->Init();
		// �O���[�o�����\�[�X�̐ݒ�
		p->SetupGlobalResources();


		p->m_pSaveDataManager = new SaveDataManager( "save/RTG.dat" );
		p->m_pSaveDataManager->Load();

		// ���v���C�G���g���̍쐬
		p->m_pReplayEntry = new ReplayEntry;

		// �{�^���̊��蓖��
		unsigned short buttons[ GENERAL_BUTTON_TOTAL ];
		p->m_pGameManager->GetButtonMaps( buttons );
		p->m_pGBManager = new GeneralButtonManager();
		p->m_pGBManager->Init( p->m_Keyboard, p->m_pReplayEntry );
		for( int i = 0; i < GENERAL_BUTTON_TOTAL; ++i ){
			p->m_pGBManager->Assign( i, ( buttons[ i ] >> 8 ) & 0xFF, buttons[ i ] & 0xFF );
		}

		// ���v���C�쐬�p�N���X�̐���
		p->m_pReplaySaver = new ReplaySaver();

		// �R���p�C���̏�����
		p->m_pCompiler = new ScriptCompiler;
		p->m_pCompiler->BuildFileStructure( "script/test.isc" );


		// ���j���[��ʂ֑J��
		m_pScene = new Menu();
		m_pScene->Init();
	}

	void MainFrame::ShowLoading()
	{
		static int cnt = 0;
		ResourceHandler* p = ResourceHandler::GetInst();
		p->m_Sprite->Begin();
		char str[ 256 ];
		TCHAR tstr[ 256 ];
		sprintf( str, "%d", cnt );
		MAPIL::ConvertToTChar( str, 256, tstr, 256 );
		p->m_Sprite->DrawString( p->m_Font, tstr, MAPIL::IMAGE_TRANSFORMATION_METHOD_MOVE, MAPIL::Vector2 < float > ( 100.0f, 100.0f ), 0xFFFFFFFF );
		p->m_Sprite->End();
		++cnt;

		p->m_pGBManager->Reset();
	}

	void MainFrame::Run()
	{
		RTG::ResourceHandler* p = RTG::ResourceHandler::GetInst();

		while( !p->m_MainWnd->ProcessMessage() ){
			if( m_FPS.DoesElapseNextTime() ){

				// �{�^���̏�Ԃ��擾
				p->m_pGBManager->Update();

				// �X�N���[���V���b�g
				if( p->m_pGBManager->IsPushedOnce( GENERAL_BUTTON_SS ) ){
					ScreenShot( TSTR( "screencapture" ), TSTR( "ScreenShot" ) );
				}

				// �I������
				if( p->m_pGBManager->IsPushedOnce( GENERAL_BUTTON_TERM ) ){
					break;
				}
				if( p->m_pGameManager->HasTermSig() ){
					break;
				}

				// �`��ݒ�
				p->m_GraphicsCtrl->BeginRendering();
				p->m_GraphicsCtrl->EnableBlend( true );
				p->m_GraphicsCtrl->EnableLight( false );
				p->m_GraphicsCtrl->EnableZBuffer( false );
				p->m_GraphicsCtrl->SetTextureMode( MAPIL::TEXTURE_MODE_2D );
				p->m_GraphicsCtrl->SetCullMode( MAPIL::CULL_MODE_DISABLED );
				//p->m_GraphicsCtrl->SetAlphaBlendMode( MAPIL::ALPHA_BLEND_MODE_NO_TRANSPARENT );
				p->m_GraphicsCtrl->SetAlphaBlendMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
				//p->m_GraphicsCtrl->SetAlphaBlendMode( MAPIL::ALPHA_BLEND_MODE_ADD );
				p->m_GraphicsCtrl->SetViewport( 0, 0, 640, 480 );

				// �V�[���J��
				if( m_pScene->HasNextScene() ){
					Scene* pDeleteScene = m_pScene;
					m_pScene = m_pScene->GetNextScene();
					MAPIL::SafeDelete( pDeleteScene );
					m_pScene->Init();
				}			
	
				// ���[�f�B���O���
				if( m_pScene->IsLoading() ){
					ShowLoading();
				}
				// �ʏ���
				else{
					m_pScene->Draw();
					m_pScene->Update();
				}

				p->m_GraphicsCtrl->EndRendering();
			}
			else{
				Sleep( 1 );
			}
		}
	}
}