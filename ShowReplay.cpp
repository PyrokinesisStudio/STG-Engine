#include "ResourceHandler.h"

#include "ShowReplay.h"
#include "Menu.h"
#include "Stage1.h"
#include "Keyboard.h"
#include "ReplayLoader.h"

#include "GeneralButtonManager.h"

#include <time.h>

namespace RTG
{
	ShowReplay::ShowReplay() : m_FileNo( 0 )
	{
		MAPIL::ZeroObject( m_Name, sizeof( m_Name ) );
	}

	ShowReplay::~ShowReplay()
	{
		MAPIL::ZeroObject( m_Name, sizeof( m_Name ) );
		m_FileNo = 0;
	}

	bool ShowReplay::IsLoading() const
	{
		return false;
	}

	void ShowReplay::Update()
	{
		ResourceHandler* p = ResourceHandler::GetInst();

		if( p->m_pGBManager->IsPushedOnce( GENERAL_BUTTON_ENTER ) ){
			char fileName[ 40 ];
			sprintf( fileName, "replay/entry%d.rpy", m_FileNo );
			ReplayLoader rl;
			rl.Load( fileName );
			rl.GetReplayData( p->m_pReplayEntry );
			p->m_pGBManager->SetReplay( 1 );
			SetNextScene( new Stage1( PLAY_MODE_REPLAY ) );
		}
		else if( p->m_pGBManager->IsPushedOnce( GENERAL_BUTTON_DOWN ) ){
			++m_FileNo;
			if( m_FileNo >= 25 ){
				m_FileNo = 0;
			}
		}
		else if( p->m_pGBManager->IsPushedOnce( GENERAL_BUTTON_UP ) ){
			--m_FileNo;
			if( m_FileNo <= -1 ){
				m_FileNo = 24;
			}
		}
		else if( p->m_pGBManager->IsPushedOnce( GENERAL_BUTTON_CANCEL ) ){
			SetNextScene( new Menu() );
		}
	}

	void ShowReplay::Draw()
	{
		ResourceHandler* p = ResourceHandler::GetInst();

		// 2D�`��J�n
		p->BeginDraw2D();

		for( int i = 0; i < 25; ++i ){
			if( i == m_FileNo ){
				p->DrawString2D( 100.0f, 50.0f + i * 15.0f, 0xFFFFFFFF, m_EntryStr[ i ] );
			}
			else{
				p->DrawString2D( 100.0f, 50.0f + i * 15.0f, 0x66666666, m_EntryStr[ i ] );
			}
		}
		
		// 2D�`��I��
		p->EndDraw2D();
	}


	void ShowReplay::Init()
	{
		ResourceHandler* p = ResourceHandler::GetInst();

		p->RefleshResouces();

		char fileName[ 256 ];
		for( int i = 0; i < 25; ++i ){
			sprintf( fileName, "replay/entry%d.rpy", i );
			char name[ 16 ];
			int score;
			int progress;
			::time_t date;
			std::ifstream fIn( fileName, std::ios::binary | std::ios::in );
			if( !fIn ){
				sprintf( m_EntryStr[ i ], "------------------No Data------------------" );
				continue;
			}
			fIn.read( name, sizeof( name ) );
			fIn.read( (char*)( &score ), sizeof( score ) );
			fIn.read( (char*)( &progress ), sizeof( progress ) );
			fIn.read( (char*)( &date ),sizeof( date ) );
			fIn.close();

			::tm* pDate = ::localtime( &date );
			sprintf(	m_EntryStr[ i ],
						"No.%d -- %s , %d : %d %04d/%02d/%02d %02d:%02d:%02d",
						i,
						name,
						score,
						progress,
						pDate->tm_year + 1900, pDate->tm_mon + 1, pDate->tm_mday, pDate->tm_hour, pDate->tm_min, pDate->tm_sec );
		}
		
	}
}