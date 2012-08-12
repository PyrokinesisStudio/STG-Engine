#include "ScriptedStage.h"
#include "Menu.h"

#include "Math.h"

#include "NormalPlayer.h"
#include "ReflectedShot1.h"

namespace RTG
{
	// �Փ˔���
	static bool DoesCollide( CircleObj* pObj1, CircleObj* pObj2 )
	{
		MAPIL::Vector2 < double > diff = pObj1->m_Pos - pObj2->m_Pos;
		if( diff.GetNorm() < pObj1->m_Radius + pObj2->m_Radius ){
			return true;
		}
		return false;
	}

	ScriptedStage::ScriptedStage( ScriptCompiler* pCompiler, int stage ) :	m_pCompiler( pCompiler ),
																			m_pScriptCmd( NULL ),
																			m_Frame( 0 ),
																			m_Cleared( false ),
																			m_GameOvered( false ),
																			m_Paused( false ),
																			m_PlayerList(),
																			m_EnemyList(),
																			m_EnemyShotList(),
																			m_ReflectedShotList(),
																			m_Effect2DList(),
																			m_StageNo( stage ),
																			m_KillTotal( 0 ),
																			m_ReflectTotal( 0 ),
																			m_HitTotal( 0 )
	{
	}

	ScriptedStage::~ScriptedStage()
	{
		// �S���\�[�X�̊J��
		for( unsigned int i = 0; i < m_ResourceMap.m_TextureMap.size(); ++i ){
			MAPIL::DeleteTexture( m_ResourceMap.m_TextureMap[ i ] );
		}

		m_StageNo = 0;
		m_pScriptCmd = NULL;
		m_Frame = 0;
		m_Cleared = false;
		m_GameOvered = false;
		m_Paused = false;
		m_KillTotal = 0;
		m_ReflectTotal = 0;
		m_HitTotal = 0;

		DeleteTaskList( &m_PlayerList );
		DeleteTaskList( &m_EnemyList );
		DeleteTaskList( &m_EnemyShotList );
		DeleteTaskList( &m_ReflectedShotList );
		DeleteTaskList( &m_Effect2DList );

		MAPIL::DeleteStaticBuffer( m_BombbedSE );
		MAPIL::DeleteStaticBuffer( m_ReflectSE );
		//MAPIL::DeleteStreamingBuffer( m_StageBGM );
	}

	void ScriptedStage::Update()
	{
		ResourceHandler* p = ResourceHandler::GetInst();
		
		if( !m_Paused ){

			MAPIL::BeginRendering2DGraphics();

			UpdatePlayer();					// �v���C���[�̏����X�V
			UpdateEnemy();					// �G�̏����X�V
			UpdateEnemyShot();				// �G�̒e�̏����X�V
			UpdateReflectedShot();			// ���˒e�̏����X�V
			UpdateEffect2D();				// �G�t�F�N�g�̍X�V
			CollidePlayerAndEnemy();		// �v���C���[�ƓG�Փ˔���
			CollideEnemyAndReflectedShot();	// �G�Ɣ��˒e�Ƃ̏Փ˔���

			MAPIL::EndRendering2DGraphics();

			// �X�N���v�g�R�}���h�̎��s
			// �X�N���v�g���I�����Ă��Ȃ����`�F�b�N
			m_VirtualMachine.Run();
			if( m_VirtualMachine.Terminated() ){
				SetNextScene( new Menu() );
				return;
			}

			++m_Frame;
		}
		else{
			// �|�[�Y���̏���
		}
	}

	void ScriptedStage::UpdatePlayer()
	{
		CirclePlayerList::Iterator it( &m_PlayerList );
		CirclePlayerList::Iterator itEnd( &m_PlayerList );
		itEnd.End();

		// �v���C���[�̏��̍X�V
		for( it.Begin(); it != itEnd; ++it ){
			if( !( *it ).Update() ){
				it.Remove();
				m_GameOvered = true;	// �Q�[���I�[�o�[
			}
			else{
				ResourceHandler* p = ResourceHandler::GetInst();
				p->m_RdmSeed.m_PosX = ( *it ).GetPos().m_X;
				p->m_RdmSeed.m_PosY = ( *it ).GetPos().m_Y;
				p->m_RdmSeed.m_Score = p->m_pScore->Get();
				p->m_RdmSeed.m_HitTotal = m_HitTotal;
				p->m_RdmSeed.m_ReflectTotal = m_ReflectTotal;
				p->m_RdmSeed.m_Frame = m_Frame;
			}
		}
	}

	void ScriptedStage::UpdateEnemy()
	{
		CircleEnemyList::Iterator it( &m_EnemyList );
		CircleEnemyList::Iterator itEnd( &m_EnemyList );
		itEnd.End();

		// �G�̏����X�V
		for( it.Begin(); it != itEnd; ++it ){
			if( !( *it ).Update() ){
				it.Remove();
			}
		}
	}

	void ScriptedStage::UpdateEnemyShot()
	{
		CircleEnemyShotList::Iterator it( &m_EnemyShotList );
		CircleEnemyShotList::Iterator itEnd( &m_EnemyShotList );
		itEnd.End();

		// �G�̒e�̏����X�V
		for( it.Begin(); it != itEnd; ++it ){
			if( !( *it ).Update() ){
				it.Remove();
			}
		}	
	}

	void ScriptedStage::UpdateReflectedShot()
	{
		CircleReflectedShotList::Iterator it( &m_ReflectedShotList );
		CircleReflectedShotList::Iterator itEnd( &m_ReflectedShotList );
		itEnd.End();

		// ���˒e�̏����X�V
		for( it.Begin(); it != itEnd; ++it ){
			if( !( *it ).Update() ){
				it.Remove();
			}
		}
	}

	void ScriptedStage::UpdateEffect2D()
	{
		Effect2DList::Iterator it( &m_Effect2DList );
		Effect2DList::Iterator itEnd( &m_Effect2DList );
		itEnd.End();

		// ���˒e�̏����X�V
		for( it.Begin(); it != itEnd; ++it ){
			if( !( *it ).Update() ){
				it.Remove();
			}
		}
	}

	void ScriptedStage::CollidePlayerAndEnemy()
	{
		CirclePlayerList::Iterator itPlayer( &m_PlayerList );
		CirclePlayerList::Iterator itPlayerEnd( &m_PlayerList );
		CircleEnemyShotList::Iterator itEnemyShot( &m_EnemyShotList );
		CircleEnemyShotList::Iterator itEnemyShotEnd( &m_EnemyShotList );

		itPlayerEnd.End();

		for( itPlayer.Begin(); itPlayer != itPlayerEnd; ++itPlayer ){
			itEnemyShotEnd.End();
			for( itEnemyShot.Begin(); itEnemyShot != itEnemyShotEnd; ++itEnemyShot ){
				if( DoesCollide( &( *itPlayer ), &( *itEnemyShot ) ) ){
					if( ((NormalPlayer*)&(*itPlayer))->IsReflectMode() ){
						((NormalPlayer*)&(*itPlayer))->DecEnergy( 100 );
						m_ReflectedShotList.Add( new ReflectedShot1( *itEnemyShot, *(NormalPlayer*)&(*itPlayer), (*itEnemyShot).GetImgID() ) );
						MAPIL::PlayStaticBuffer( m_ReflectSE );
					}
					else{
						((NormalPlayer*)&(*itPlayer))->Damage( 1 );
					}
					itEnemyShot.Remove();
				}
			}
		}
	}

	void ScriptedStage::CollideEnemyAndReflectedShot()
	{
		CircleEnemyList::Iterator itEnemy( &m_EnemyList );
		CircleEnemyList::Iterator itEnemyEnd( &m_EnemyList );
		CircleReflectedShotList::Iterator itReflectedShot( &m_ReflectedShotList );
		CircleReflectedShotList::Iterator itReflectedShotEnd( &m_ReflectedShotList );

		itEnemyEnd.End();
		itReflectedShotEnd.End();

		for( itReflectedShot.Begin(); itReflectedShot != itReflectedShotEnd; ++itReflectedShot ){
			for( itEnemy.Begin(); itEnemy != itEnemyEnd; ++itEnemy ){
				if( DoesCollide( &(*itEnemy), &(*itReflectedShot) ) ){
					itReflectedShot.Remove();
					( *itEnemy ).Damage( 1 );
					++m_ReflectTotal;
					MAPIL::PlayStaticBuffer( m_BombbedSE );
					// 2�d���肪�N����\��������A�v�C��
					// �|�������ɓ��_������悤�ɂ��铙
					if( ( *itEnemy ).GetHP() <= 0 ){
						//p->m_pScore->Add( ( *itEnemy ).GetScore() );
						++m_KillTotal;
						/*m_pEffect2DList->Add( new BombedEffect1(	p->m_Sprite,
																	p->m_Texture[ 12 ],
																	MAPIL::Vector2 < float > (	static_cast < float > ( ( *itEnemy ).GetPos().m_X ),
																								static_cast < float > ( ( *itEnemy ).GetPos().m_Y ) ),
																	0.3f,
																	0.1 ) );*/
						if( ( *itEnemy ).IsBoss() ){
							m_Cleared = true;
						}
					}
					else{
						//p->m_pScore->Add( 100 );
					}
				}
			}
		}
	}

	void ScriptedStage::Draw()
	{
	}

	void ScriptedStage::Init()
	{
		ResourceHandler* p = ResourceHandler::GetInst();

		// �폜�v���̃I�u�W�F�N�g�����S�폜
		MAPIL::RefleshResources();

		// �X�N���v�g�̃R���p�C��
		p->m_pCompiler->Compile( m_StageNo );
		p->SetupHandle();


		// �X�e�[�W���̐ݒ�
		MAPIL::ZeroObject( &m_StageInfo, sizeof( m_StageInfo ) );
		m_StageInfo.m_pCompiler = m_pCompiler;
		m_StageInfo.m_pEnemyList = &m_EnemyList;
		m_StageInfo.m_pEnemyShotList = &m_EnemyShotList;
		m_StageInfo.m_pFrame = &m_Frame;
		m_StageInfo.m_pPlayerList = &m_PlayerList;
		m_StageInfo.m_pEffect2DList = &m_Effect2DList;
		m_StageInfo.m_pRandSeed = &p->m_RdmSeed;
		// �X�N���v�g�R�}���h�̎擾
		m_pScriptCmd = m_pCompiler->GetStageScript();
		// ���z�}�V���̃Z�b�g�A�b�v
		m_VirtualMachine.Init( m_pScriptCmd, &m_StageInfo );


		// �e���X�g�̏�����
		m_PlayerList.Init();
		m_EnemyList.Init();
		m_EnemyShotList.Init();
		m_ReflectedShotList.Init();
		m_Effect2DList.Init();

		// �v���C���[�̍쐬
		NormalPlayer* pNewPlayer = new NormalPlayer( MAPIL::Vector2 < double > ( 320.0, 430.0f ) );
		pNewPlayer->Init();
		m_PlayerList.Add( pNewPlayer );

		// ���\�[�X�̓ǂݍ���
		m_ReflectSE = MAPIL::CreateStaticBuffer( "Resource/rtg_se1.wav" );
		m_BombbedSE = MAPIL::CreateStaticBuffer( "Resource/rtg_se2.wav" );
		//m_StageBGM = MAPIL::CreateStreamingBuffer( "Resource/rtg_stage2.wav" );

		// �����_���V�[�h�̏�����
		ResetRandSeed();

		//MAPIL::PlayStreamingBuffer( m_StageBGM );
	}

	bool ScriptedStage::IsLoading() const
	{
		bool ret;

		ret = m_pCompiler->Loaded();

		return !ret;
	}



	
}