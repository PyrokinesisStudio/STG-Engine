#include "ResourceHandler.h"
#include "Util.h"

#include "GameManager.h"


namespace RTG
{
	ResourceHandler::ResourceHandler() :	m_Cons(),
											m_Alloc(),
											m_pSaveDataManager( NULL ),
											m_pGBManager( NULL ),
											m_pReplayEntry( NULL ),
											m_pGameManager( NULL )
	{
		m_pScore = new Score();
	}

	ResourceHandler::~ResourceHandler()
	{

		MAPIL::SafeDelete( m_pScore );
		MAPIL::SafeDelete( m_pSaveDataManager );
		MAPIL::SafeDelete( m_pReplayEntry );
		MAPIL::SafeDelete( m_pGameManager );
	}


	void* ResourceHandler::AllocMemory()			// �������̊��蓖��
	{
		return m_Alloc.Alloc();
	}

	void ResourceHandler::ReleaseMemory( void* p )			// �������̊J��
	{
		m_Alloc.Delete( p );
	}

	void ResourceHandler::SetupHandle()
	{
		// �e�탊�\�[�X�̓ǂݍ���
		const int INITIAL_TEXTURE_MAP_RESERVE_CAP = 50;			// �����̃e�N�X�`��MAP���e��
		const int INITIAL_SE_MAP_RESERVE_CAP = 50;				// ������SEMAP���e��
		const int INITIAL_BGM_MAP_RESERVE_CAP = 50;				// ������BGMMAP���e��
		m_ResourceMap.m_TextureMap.resize( INITIAL_TEXTURE_MAP_RESERVE_CAP );
		m_ResourceMap.m_SEMap.resize( INITIAL_SE_MAP_RESERVE_CAP );
		m_ResourceMap.m_BGMMap.resize( INITIAL_BGM_MAP_RESERVE_CAP );
		m_ResourceScriptData = m_pCompiler->GetResourceScriptData();
		// �e�N�X�`���̓ǂݍ���
		typedef std::map < int, std::string > ::iterator	TextureIter;
		for(	TextureIter it = m_ResourceScriptData.m_TextureList.begin();
				it != m_ResourceScriptData.m_TextureList.end();
				++it ){
			// ���e�l�𒴂����C���f�b�N�X���K�v�ȏꍇ�́A�w�肳�ꂽ�C���f�b�N�X��2�{�̃T�C�Y��resize����B
			if( it->first > m_ResourceMap.m_TextureMap.size() ){
				m_ResourceMap.m_TextureMap.resize( it->first * 2 );
			}
			m_ResourceMap.m_TextureMap[ it->first ] = MAPIL::CreateTexture( it->second.c_str() );
		}
		// SE�̓ǂݍ���
		typedef std::map < int, std::string > ::iterator	SEIter;
		for(	SEIter it = m_ResourceScriptData.m_SEList.begin();
				it != m_ResourceScriptData.m_SEList.end();
				++it ){
			if( it->first > m_ResourceMap.m_SEMap.size() ){
				m_ResourceMap.m_SEMap.resize( it->first * 2 );
			}
			m_ResourceMap.m_SEMap[ it->first ] = MAPIL::CreateStaticBuffer( it->second.c_str() );
		}
		// BGM�̓ǂݍ���
		typedef std::map < int, std::string > ::iterator	BGMIter;
		for(	BGMIter it = m_ResourceScriptData.m_BGMList.begin();
				it != m_ResourceScriptData.m_BGMList.end();
				++it ){
			if( it->first > m_ResourceMap.m_BGMMap.size() ){
				m_ResourceMap.m_BGMMap.resize( it->first * 2 );
			}
			m_ResourceMap.m_BGMMap[ it->first ] = MAPIL::CreateStreamingBuffer( it->second.c_str() );
		}
	}

	int ResourceHandler::GetTextureHandle( int index )
	{
		return m_ResourceMap.m_TextureMap[ index ];
	}

	int ResourceHandler::GetBGMHandle( int index )
	{
		return m_ResourceMap.m_BGMMap[ index ];
	}

	int ResourceHandler::GetSEHandle( int index )
	{
		return m_ResourceMap.m_SEMap[ index ];
	}


	
	//float ResourceHandler::GetPlayerPositionY( int index )
	//{
	//	CirclePlayerList::Iterator it( &m_PlayerList );
	//	CirclePlayerList::Iterator itEnd( &m_PlayerList );
	//	itEnd.End();

	//	int cnt = 0;

	//	// �v���C���[�̏��̍X�V
	//	for( it.Begin(); it != itEnd; ++itEnd ){
	//		if( cnt == index ){
	//			return ( *it ).GetPos().m_Y;
	//		}
	//		++cnt;
	//	}

	//	return 0.0f;		// �ړI�̃v���C���[�͌�����Ȃ������B
	//}
}