//===== Copyright � 1996-2005, Valve Corporation, All rights reserved. ======//
//
//  Purpose: (IDA Pro + 2014 dev leak PDB = this file made from scratch)
//
//============================================================================//

#include "cbase.h"
#include "baseentity.h"
#include "mathlib/vector.h"
#include "func_tank.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: Point entity that fires outputs from ping tool based on whether a player is on TEAM_BLUE or TEAM_RED
//-----------------------------------------------------------------------------
class CInfo_Player_Ping_Detector : public CPointEntity, public CGameEventListener
{
public:
	virtual void Spawn();
	//virtual void UpdateOnRemove();
	virtual void FireGameEvent( IGameEvent *event );

	void InputToggle(inputdata_t& inputdata);
	void InputEnable(inputdata_t& inputdata);
	void InputDisable(inputdata_t& inputdata);

protected:

	void Toggle();
	void Enable();
	void Disable();

private:

	string_t m_iszFuncTankName;
	bool m_bEnabled;
	bool m_bDetectedNewPing;
	bool m_bLookAtPlayerPings;
	int m_nTeamToLookAt;
	Vector m_vecPingLocation;

	COutputEvent m_OnPingDetected;

	// seems like these were at the end of the class for some reason?
	DECLARE_DATADESC();
	DECLARE_CLASS( CInfo_Player_Ping_Detector, CPointEntity );
};

void CInfo_Player_Ping_Detector::Spawn()
{
	BaseClass::Spawn();
	if (m_bEnabled)
	{
		ListenForGameEvent( "portal_player_ping" );
	}
}

void CInfo_Player_Ping_Detector::FireGameEvent( IGameEvent *event )
{
	if (!m_bEnabled)
		return;

	const char* name = event->GetName();
	if (!V_strcmp( name, "portal_player_ping" ))
	{
		int iUserID = event->GetInt("userid");
		Vector vecPingPos(event->GetFloat("ping_x"), event->GetFloat("ping_y"), event->GetFloat("ping_z"));

		int TeamNumber = -1;
		for (int i = 1; i <= MAX_PLAYERS; ++i)
		{
			CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);
			if (pPlayer && pPlayer->GetUserID() == iUserID)
			{
				TeamNumber = pPlayer->GetTeamNumber();
				break;
			}
		}

		// 1 means any here
		if (m_nTeamToLookAt == 1 || m_nTeamToLookAt == TeamNumber)
		{
			m_vecPingLocation = vecPingPos;
			m_bDetectedNewPing = true;

			CBaseEntity* pNewEntity = gEntList.FindEntityByName( NULL, m_iszFuncTankName );
			while (pNewEntity)
			{
				CFuncTank* pTank = dynamic_cast<CFuncTank*>(pNewEntity);

				if (pTank)
					pTank->AimAtTargetPosition(vecPingPos);

				pNewEntity = gEntList.FindEntityByName( pNewEntity, m_iszFuncTankName );
			}

			m_OnPingDetected.FireOutput(NULL, NULL, 0.0f);
		}
	}
}

void CInfo_Player_Ping_Detector::Toggle()
{
	if (m_bEnabled)
		Disable();
	else
		Enable();
}

void CInfo_Player_Ping_Detector::Enable()
{
	m_bEnabled = true;
	ListenForGameEvent("portal_player_ping");
}

void CInfo_Player_Ping_Detector::Disable()
{
	m_bEnabled = false;
	StopListeningForAllEvents();
}

void CInfo_Player_Ping_Detector::InputToggle(inputdata_t& inputdata)
{
	Toggle();
}

void CInfo_Player_Ping_Detector::InputEnable(inputdata_t& inputdata)
{
	Enable();
}

void CInfo_Player_Ping_Detector::InputDisable(inputdata_t& inputdata)
{
	Disable();
}

BEGIN_DATADESC( CInfo_Player_Ping_Detector )

	DEFINE_FIELD(m_vecPingLocation, FIELD_VECTOR),

	// Keyvalues
	DEFINE_KEYFIELD(m_bEnabled, FIELD_BOOLEAN, "Enabled"),
	DEFINE_KEYFIELD(m_iszFuncTankName, FIELD_STRING, "FuncTankName"),
	DEFINE_KEYFIELD(m_bLookAtPlayerPings, FIELD_BOOLEAN, "LookAtPlayerPings"),
	DEFINE_KEYFIELD(m_nTeamToLookAt, FIELD_INTEGER, "TeamToLookAt"),

	// Inputs
	DEFINE_INPUTFUNC(FIELD_VOID, "Toggle", InputToggle),
	DEFINE_INPUTFUNC(FIELD_VOID, "Enable", InputEnable),
	DEFINE_INPUTFUNC(FIELD_VOID, "Disable", InputDisable),

	// Outputs
	DEFINE_OUTPUT(m_OnPingDetected, "OnPingDetected"),
END_DATADESC()

LINK_ENTITY_TO_CLASS( info_player_ping_detector, CInfo_Player_Ping_Detector );
