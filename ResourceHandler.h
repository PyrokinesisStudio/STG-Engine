#ifndef INCLUDED_RTG_RESOURCEHANDLER_H
#define INCLUDED_RTG_RESOURCEHANDLER_H

#include "Defines.h"

#include "Score.h"
#include "SaveDataManager.h"
#include "ReplaySaver.h"

namespace RTG
{
	struct RandomSeed
	{
		int		m_PosX;
		int		m_PosY;
		int		m_Score;
		int		m_HitTotal;
		int		m_ReflectTotal;
		int		m_Frame;
	};

	class GeneralButtonManager;
	struct ReplayEntry;
	class GameManager;
	class ScriptCompiler;
	class ResourceHandler : public MAPIL::Singleton < ResourceHandler >
	{
	private:
		// �f�o�b�O�֘A
		MAPIL::Console						m_Cons;		// �R���\�[���f�o�C�X

		// �f�o�C�X�E�t�@�N�g���֘A
		MAPIL::IGUIDevice					m_GUIDev;				// GUI�f�o�C�X
		MAPIL::GUIFactory*					m_pGUIFactory;			// GUI�t�@�N�g��
		MAPIL::IGraphicsDevice				m_GraphicsDev;			// �O���t�B�b�N�X�f�o�C�X
		//MAPIL::GraphicsFactory*				m_pGraphicsFactory;		// �O���t�B�b�N�X�t�@�N�g��
		MAPIL::IInputDevice					m_InputDev;				// ���̓f�o�C�X
		MAPIL::InputFactory*				m_pInputFactory;		// ���̓t�@�N�g��
		MAPIL::ISoundDevice					m_SoundDev;				// �T�E���h�f�o�C�X
		//MAPIL::SoundFactory*				m_pSoundFactory;		// �T�E���h�t�@�N�g��
	
		// �O���[�o�����\�[�X�֘A
		MAPIL::TTFFile*						m_FontFile;				// �t�H���g�t�@�C��

		// �������֘A
		MAPIL::Allocator < 3000, 4000 >		m_Alloc;				// �A���P�[�^
	public:
		friend class MAPIL::Singleton < ResourceHandler >;

		MAPIL::GraphicsFactory*				m_pGraphicsFactory;		// �O���t�B�b�N�X�t�@�N�g��
		MAPIL::SoundFactory*				m_pSoundFactory;		// �T�E���h�t�@�N�g��

		// �E�B���h�E�֘A
		MAPIL::IWindow						m_MainWnd;				// �E�B���h�E�I�u�W�F�N�g
		MAPIL::IGraphicsContext				m_GLContext;			// �O���t�B�b�N�X�R���e�L�X�g

		
		MAPIL::IStaticBuffer				m_StaticBuf[ 40 ];
		MAPIL::IStreamingBuffer				m_StreamingBuf;

		
		MAPIL::IGraphicsController			m_GraphicsCtrl;
		MAPIL::ICamera						m_Camera3D;
		MAPIL::ICamera						m_Camera;
		MAPIL::ICanvas2D					m_Canvas;
		MAPIL::ISprite						m_Sprite;
		MAPIL::IGraphicsFont				m_Font;

		MAPIL::ITexture						m_Texture[ 50 ];

		MAPIL::IRectangle3D					m_Rectangle[ 50 ];
		MAPIL::IPointSprite					m_PointSprite;

		

		MAPIL::IKeyboard					m_Keyboard;

		Score*								m_pScore;

		SaveDataManager*					m_pSaveDataManager;

		GeneralButtonManager*				m_pGBManager;
		ReplayEntry*						m_pReplayEntry;
		ReplaySaver*						m_pReplaySaver;
		GameManager*						m_pGameManager;

		ScriptCompiler*						m_pCompiler;

		RandomSeed							m_RdmSeed;

		ResourceHandler();
		~ResourceHandler();
		
		void Init();
		void SetupGlobalResources();				// �O���[�o���ȃ��\�[�X�̃Z�b�g�A�b�v
		MAPIL::ISprite CreateSpriteL( const char* pStr );
		MAPIL::IGraphicsFont CreateFontL( const char* pStr );
		void BeginDraw2D();
		void EndDraw2D();
		void DrawString2D( float x, float y, const char* pStr );
		void DrawString2D( float x, float y, int color, const char* pStr, ... );
		void RefleshResouces();
		void* AllocMemory();					// �������̊��蓖��
		void ReleaseMemory( void* p );			// �������̊J��
	};
}

#endif