#ifndef INCLUDED_RTG_SCRIPTCOMPILER_H
#define INCLUDED_RTG_SCRIPTCOMPILER_H

#include <vector>
#include <map>

#include "ScriptEngine/VM.h"

namespace RTG
{
	// ���\�[�X�^�C�v
	enum ResourceType
	{
		RESOURCE_TYPE_BGM	= 0,		// BGM
		RESOURCE_TYPE_SE	= 1,		// ���ʉ�
	};

	// ���\�[�X�^�O
	struct ResourceTag
	{
		int		m_ID;					// ���ʔԍ�
		int		m_Type;					// ���\�[�X�^�C�v
		char	m_FileName[ 1024 ];		// �t�@�C���^�C�v
	};

	// �X�e�[�W�^�O
	struct StageTag
	{
		int		m_StageNo;				// �X�e�[�W�ԍ�
		char	m_FileName[ 1024 ];		// �t�@�C����
	};

	// �X�N���v�g�t�@�C���\���}
	//
	//	Stage�\�����t�@�C�� ------> Stage1�\���t�@�C�� --------> �G�X�N���v�g�t�@�C�� 0 ...
	//							|							|
	//							|							---> �G�e�X�N���v�g�t�@�C�� 0 ...
	//							|							|
	//							|							---> �X�e�[�W�X�N���v�g�t�@�C��
	//							|
	//							---> Stage2�\���t�@�C��
	//							|

	class ScriptCompiler
	{
	private:
		// �ʂ̃X�N���v�g�t�@�C���̂��߂̍\����
		struct ScriptFileTag
		{
			int		m_ID;					// ���ʔԍ�
			char	m_FileName[ 1024 ];		// �t�@�C����
		};
		// �X�N���v�g�t�@�C���̎��
		enum ScriptType
		{
			SCRIPT_TYPE_ENEMY		= 0,
			SCRIPT_TYPE_ENEMY_SHOT	= 1,
			SCRIPT_TYPE_STAGE		= 2,
			SCRIPT_TYPE_RESOURCE	= 3,
		};

		// �X�N���v�g�f�[�^
		struct ScriptData
		{
			int			m_ID;		// ���ʔԍ�
			VM::Data	m_Data;		// �X�N���v�g�f�[�^�{��
		};

		bool							m_Loaded;				// �ǂݍ��݊����t���O

		std::vector < StageTag >		m_StageFileList;		// �X�e�[�W�t�@�C�����̃��X�g

		// �X�N���v�g�{��
		ScriptData*						m_pStageScriptData;			// �X�e�[�W�X�N���v�g
		// map( index, filename ) => vector( index, val->handle)
		std::map < int, std::string >	m_ResourceList;				// ���[�h���郊�\�[�X���X�g
		ScriptData*						m_pEnemyScriptData;			// �G�̃X�N���v�g�{��
		int								m_EnemyScirptTotal;			// �G�̃X�N���v�g��
		ScriptData*						m_pEnemyShotScriptData;		// �G�e�̃X�N���v�g�{��
		int								m_EnemyShotScriptTotal;		// �G�e�̃X�N���v�g��


		// ����J�֐�
		void CompileEnemyShotScript( int id, const char* pFileName );	// �G�e�̃X�N���v�g�̃R���p�C��
		void CompileEnemyScript( int id, const char* pFileName );		// �G�̃X�N���v�g�̃R���p�C��
		void CompileStageScript( const char* pFileName );				// �X�e�[�W�̃X�N���v�g�̃R���p�C��
		void CompileResourceScript( const char* pFileName );			// ���\�[�X�X�N���v�g�̃R���p�C��
		void Cleanup();													// ���݂̃X�e�[�W�\���X�N���v�g���폜
	public:
		ScriptCompiler();
		~ScriptCompiler();
		void BuildFileStructure( const char* pFileName );		// �X�N���v�g�t�@�C���̍\�����쐬
		void Compile( int stage );								// �X�e�[�W���w�肵�ăR���p�C��
		VM::Data* GetStageScript();									// �X�e�[�W�X�N���v�g���擾
		VM::Data* GetEnemyScript( int index );						// �G�̃X�N���v�g���擾
		VM::Data* GetEnemyShotScript( int index );					// �G�e�̃X�N���v�g���擾
		bool Loaded() const;									// �ǂݍ��݂�����������
	};
}

#endif