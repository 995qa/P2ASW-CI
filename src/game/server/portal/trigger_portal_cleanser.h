//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: trigger_portal_cleanser server class. Keeps a global list loaded in from the map
//			and provides an interface with which prop_portal can get this list and avoid successfully
//			creating portals partially inside the volume.
// 
//			Some parts of this code come from P1 and other parts were reverse engineered.
//
//
// $NoKeywords: $
//======================================================================================//

#ifndef TRIGGER_PORTAL_CLEANSER_H
#define TRIGGER_PORTAL_CLEANSER_H

#include "cbase.h"
#include "triggers.h"

static char *g_pszPortalNonCleansable[] = 
{ 
	"func_door", 
	"func_door_rotating", 
	"prop_door_rotating",
	"func_tracktrain",
	"env_ghostanimating",
	"physicsshadowclone",
	"prop_energy_ball",
	"npc_personality_core",
	NULL,
};

static char *g_pszFizzlingVortexObjects[] =
{
	"prop_weighted_cube",
	"npc_portal_turret_floor",
	"simple_physics_prop",
	"prop_monster_box",
	"hot_potato",
	NULL,
};

// These probably were defined here...
// NOTE: If you want to raise max vortex objects for a mod, it's not as simple as just increasing this.
// You'll need to define additional network handles in the class for more objects, modify all the places that get/set those
// to use the additional ones, and modify the shader to also support more than 2 objects.
#define MAX_FIZZLER_VORTEX_OBJECTS 2
#define MAX_FIZZLER_SEARCH_OBJECTS 32

struct FizzlerVortexObjectInfo_t
{
	FizzlerVortexObjectInfo_t()
	{
		m_flDistanceSq = 0;
		m_hEnt = NULL;
	}
	
	float m_flDistanceSq;
    EHANDLE m_hEnt;
};

class FizzlerMultiOriginSoundPlayer : public CBaseEntity
{
	DECLARE_CLASS( FizzlerMultiOriginSoundPlayer, CBaseEntity );
	DECLARE_DATADESC();
public:
	DECLARE_SERVERCLASS();
    
    FizzlerMultiOriginSoundPlayer();
	~FizzlerMultiOriginSoundPlayer();

	void Spawn();
    
	static FizzlerMultiOriginSoundPlayer *Create( IRecipientFilter &filter, const char *soundName );
    int UpdateTransmitState();

private:
    CSoundPatch *m_pSound;
    void RemoveThink();	
};

class CleanserVortexTraceEnum : public ICountedPartitionEnumerator
{
public:
	CleanserVortexTraceEnum( CBaseEntity **pList, int listMax, CBaseTrigger *pCleanser );
	IterationRetval_t EnumElement( IHandleEntity *pHandleEntity );
	int GetCount() const { return m_count; }
    bool AddToList( CBaseEntity *pEntity );
private:
	CBaseEntity **m_pList;
    int m_listMax;
    int m_count;
    CBaseTrigger *m_pCleanser;
};

DECLARE_AUTO_LIST( ITriggerPortalCleanserAutoList );

//-----------------------------------------------------------------------------
// Purpose: Removes anything that touches it. If the trigger has a targetname,
//			firing it will toggle state.
//-----------------------------------------------------------------------------
class CTriggerPortalCleanser : public CBaseTrigger, public ITriggerPortalCleanserAutoList
{
public:
	DECLARE_CLASS( CTriggerPortalCleanser, CBaseTrigger );   
	DECLARE_SERVERCLASS();
	
    CTriggerPortalCleanser();

	CBaseEntity *GetEntity() { return this; }
	
	virtual void Spawn();
	virtual void Precache();
	virtual void Activate();
    virtual void Touch( CBaseEntity *pOther );
	virtual void UpdateOnRemove( void );
	int UpdateTransmitState();

	virtual void Enable( void );
	virtual void Disable( void );
	bool IsEnabled() { return !m_bDisabled; }

	static void FizzleBaseAnimating( CTriggerPortalCleanser *pFizzler, CBaseAnimating *pBaseAnimating );
    void SearchThink();
    void SetPortalShot();
    void PlayerPassesTriggerFiltersThink();
	
    static char *s_szPlayerPassesTriggerFiltersThinkContext;

	DECLARE_DATADESC();
	
	CNetworkVar( bool, m_bDisabled );	
	
private:	
	
    bool IsCloserThanExistingObjects( FizzlerVortexObjectInfo_t &info, int &iIndex );
    void ClearVortexObjects();
    void StartAmbientSounds();
    void StopAmbientSounds();
    void FizzleTouchingPortals();
    void InputFizzleTouchingPortals( inputdata_t &inputdata );
    
	COutputEvent m_OnDissolve;
    COutputEvent m_OnFizzle;
    COutputEvent m_OnDissolveBox;
    Vector m_vecSearchBoxMins;
    Vector m_vecSearchBoxMaxs;
    
	FizzlerVortexObjectInfo_t m_VortexObjects[MAX_FIZZLER_VORTEX_OBJECTS];	
	
	CNetworkVar( bool, m_bVisible );
	
	CNetworkVar( float, m_flPortalShotTime );
	
	CNetworkVar( bool, m_bObject1InRange );
	CNetworkVar( bool, m_bObject2InRange );
	CNetworkVar( bool, m_bUseScanline );
	CNetworkVar( bool, m_bPlayersPassTriggerFilters );
	
	CNetworkHandle( CBaseEntity, m_hObject1 );
	CNetworkHandle( CBaseEntity, m_hObject2 );
	
    CHandle<FizzlerMultiOriginSoundPlayer> s_FizzlerAmbientSoundPlayer;

public:
	// pdbripper says this was at the end...
	~CTriggerPortalCleanser();
};

#endif