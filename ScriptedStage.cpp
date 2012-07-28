#include "ScriptedStage.h"
#include "Menu.h"

#include "Math.h"

namespace RTG
{
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
																			m_StageNo( stage )
	{
	}

	ScriptedStage::~ScriptedStage()
	{
		m_StageNo = 0;
		m_pScriptCmd = NULL;
		m_Frame = 0;
		m_Cleared = false;
		m_GameOvered = false;
		m_Paused = false;

		DeleteTaskList( &m_PlayerList );
		DeleteTaskList( &m_EnemyList );
		DeleteTaskList( &m_EnemyShotList );
		DeleteTaskList( &m_ReflectedShotList );
		DeleteTaskList( &m_Effect2DList );
	}

	void ScriptedStage::Update()
	{
		ResourceHandler* p = ResourceHandler::GetInst();
		
		if( !m_Paused ){

			p->m_Sprite->Begin();

			UpdatePlayer();					// �v���C���[�̏����X�V
			UpdateEnemy();					// �G�̏����X�V
			UpdateEnemyShot();				// �G�̒e�̏����X�V
			UpdateReflectedShot();			// ���˒e�̏����X�V
			UpdateEffect2D();				// �G�t�F�N�g�̍X�V
			CollidePlayerAndEnemy();		// �v���C���[�ƓG�Փ˔���
			CollideEnemyAndReflectedShot();	// �G�Ɣ��˒e�Ƃ̏Փ˔���

			p->m_Sprite->End();

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
		for( it.Begin(); it != itEnd; ++itEnd ){
			if( !( *it ).Update() ){
				it.Remove();
				m_GameOvered = true;	// �Q�[���I�[�o�[
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

		for( itEnemyShot.Begin(); itEnemyShot != itEnemyShotEnd; ++itEnemyShot ){
			// �쐬�r��
		}
	}

	void ScriptedStage::CollideEnemyAndReflectedShot()
	{
		CircleEnemyList::Iterator itEnemy( &m_EnemyList );
		CircleEnemyList::Iterator itEnemyEnd( &m_EnemyList );
		CircleReflectedShotList::Iterator itReflectedShot( &m_ReflectedShotList );
		CircleReflectedShotList::Iterator itReflectedShotEnd( &m_ReflectedShotList );

		for( itReflectedShot.Begin(); itReflectedShot != itReflectedShotEnd; ++itReflectedShot ){
			for( itEnemy.Begin(); itEnemy != itEnemyEnd; ++itEnemy ){
				// �쐬�r��
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
		p->RefleshResouces();

		// �e�X�g�p�B
		p->m_Texture[ 0 ] = p->m_pGraphicsFactory->CreateTexture( TSTR( "Resource/Enemy1.png" ) );
		p->m_Texture[ 0 ]->Create( TSTR( "Resource/Enemy1.png" ) );
		p->m_Texture[ 1 ] = p->m_pGraphicsFactory->CreateTexture( TSTR( "Enemy Shot 1" ) );
		p->m_Texture[ 1 ]->Create( TSTR( "Resource/rtg_enemy_shot_3.png" ) );

		p->m_pCompiler->Compile( m_StageNo );

		// �X�e�[�W���̐ݒ�
		MAPIL::ZeroObject( &m_StageInfo, sizeof( m_StageInfo ) );
		m_StageInfo.m_pCompiler = m_pCompiler;
		m_StageInfo.m_pEnemyList = &m_EnemyList;
		m_StageInfo.m_pEnemyShotList = &m_EnemyShotList;
		m_StageInfo.m_pFrame = &m_Frame;
		// �X�N���v�g�R�}���h�̎擾
		m_pScriptCmd = m_pCompiler->GetStageScript();
		// ���z�}�V���̃Z�b�g�A�b�v
		m_VirtualMachine.Init( m_pScriptCmd, &m_StageInfo );

		// �e�탊�\�[�X�̓ǂݍ���

		// �e���X�g�̏�����
		m_PlayerList.Init();
		m_EnemyList.Init();
		m_EnemyShotList.Init();
		m_ReflectedShotList.Init();
		m_Effect2DList.Init();

		// �����_���V�[�h�̏�����
		ResetRandSeed();
	}

	bool ScriptedStage::IsLoading() const
	{
		bool ret;

		ret = m_pCompiler->Loaded();

		return !ret;
	}



	// �Փ˔���
	//bool DoesCollide( CircleObj* pObj1, CircleObj* pObj2 )
	//{
	//	MAPIL::Vector2 < double > diff = pObj1->m_Pos - pObj2->m_Pos;
	//	if( diff.GetNorm() < pObj1->m_Radius + pObj2->m_Radius ){
	//		return true;
	//	}
	//	return false;
	//}
}