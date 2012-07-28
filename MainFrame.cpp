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
		MAPIL::InitMAPIL( "Confetti", 640, 480 );

		// �Z�[�u�f�[�^�Ǘ��N���X�̏�����
		p->m_pSaveDataManager = new SaveDataManager( "save/RTG.dat" );
		p->m_pSaveDataManager->Load();

		// ���v���C�G���g���̍쐬
		p->m_pReplayEntry = new ReplayEntry;

		// �{�^���̊��蓖��
		unsigned short buttons[ GENERAL_BUTTON_TOTAL ];
		p->m_pGameManager->GetButtonMaps( buttons );
		p->m_pGBManager = new GeneralButtonManager();
		p->m_pGBManager->Init( p->m_pReplayEntry );
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
		MAPIL::BeginRendering2DGraphics();
		char str[ 256 ];
		sprintf( str, "%d", cnt );
		MAPIL::DrawString( 100.0f, 100.0f, str );
		MAPIL::EndRendering2DGraphics();
		++cnt;

		p->m_pGBManager->Reset();
	}

	void MainFrame::Run()
	{
		RTG::ResourceHandler* p = RTG::ResourceHandler::GetInst();

		while( !MAPIL::ProcessMessage() ){
			if( m_FPS.DoesElapseNextTime() ){

				// �{�^���̏�Ԃ��擾
				p->m_pGBManager->Update();

				// �X�N���[���V���b�g
				if( p->m_pGBManager->IsPushedOnce( GENERAL_BUTTON_SS ) ){
					ScreenShot( "screencapture", "ScreenShot" );
				}

				// �I������
				if( p->m_pGBManager->IsPushedOnce( GENERAL_BUTTON_TERM ) ){
					break;
				}
				if( p->m_pGameManager->HasTermSig() ){
					break;
				}

				// �`��ݒ�
				MAPIL::BeginRendering();
				MAPIL::EnableBlending();
				MAPIL::EnableLighting();
				MAPIL::EnableZBuffering();
				MAPIL::SetTextureMode( MAPIL::TEXTURE_MODE_2D );
				MAPIL::SetCullingMode( MAPIL::CULL_MODE_DISABLED );
				MAPIL::SetAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
				//p->m_GraphicsCtrl->SetAlphaBlendMode( MAPIL::ALPHA_BLEND_MODE_ADD );
				MAPIL::SetViewPort( 0, 0, 640, 480 );

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

				MAPIL::EndRendering();
			}
			else{
				Sleep( 1 );
			}
		}
	}
}