#ifndef NPC_SECURITY_CAMERA_H
#define NPC_SECURITY_CAMERA_H

//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "ai_basenpc.h"
#include "ai_senses.h"
#include "ai_memory.h"
#include "engine/IEngineSound.h"
#include "ammodef.h"
#include "Sprite.h"
#include "soundenvelope.h"
#include "explode.h"
#include "IEffects.h"
#include "animation.h"
#include "props.h"
#include "basehlcombatweapon_shared.h"
#include "iservervehicle.h"
#include "physics_prop_ragdoll.h"
#include "portal_util_shared.h"
#include "prop_portal.h"
#include "portal_player.h"
#include "world.h"
#include "ai_baseactor.h"		// for Glados ent playing VCDs
#include "sceneentity.h"		// precacheing vcds

#define	SECURITY_CAMERA_MODEL		"models/props/security_camera.mdl"
#define SECURITY_CAMERA_BC_YAW		"aim_yaw"
#define SECURITY_CAMERA_BC_PITCH	"aim_pitch"
#define	SECURITY_CAMERA_RANGE		1500
#define SECURITY_CAMERA_SPREAD		VECTOR_CONE_2DEGREES
#define	SECURITY_CAMERA_MAX_WAIT	5
#define	SECURITY_CAMERA_PING_TIME	1.0f	//LPB!!

#define SECURITY_CAMERA_GLOW_SPRITE	"sprites/glow1.vmt"

//Aiming variables
#define	SECURITY_CAMERA_MAX_NOHARM_PERIOD	0.0f
#define	SECURITY_CAMERA_MAX_GRACE_PERIOD	3.0f

//Spawnflags
#define SF_SECURITY_CAMERA_AUTOACTIVATE		0x00000020
#define SF_SECURITY_CAMERA_STARTINACTIVE	0x00000040
#define SF_SECURITY_CAMERA_NEVERRETIRE		0x00000080
#define SF_SECURITY_CAMERA_OUT_OF_AMMO		0x00000100

// Normal turn speed
#define	SECURITY_CAMERA_YAW_SPEED	7.0f
// Turn speed when looking at coop pings
#define	SECURITY_CAMERA_YAW_SPEED_PING	8.0f

//Turret states
enum cameraState_e
{
	CAMERA_SEARCHING,
	CAMERA_AUTO_SEARCHING,
	CAMERA_ACTIVE,
	CAMERA_DEPLOYING,
	CAMERA_RETIRING,
	CAMERA_DEAD,
};

//
// Security Camera
//

class CNPC_SecurityCamera : public CNPCBaseInteractive<CAI_BaseNPC>, public CDefaultPlayerPickupVPhysics, public CGameEventListener
{
	DECLARE_CLASS( CNPC_SecurityCamera, CNPCBaseInteractive<CAI_BaseNPC> );
public:
	
	CNPC_SecurityCamera( void );
	~CNPC_SecurityCamera( void );

	void			Precache( void );
	virtual void	CreateSounds( void );
	virtual void	StopLoopingSounds( void );
	virtual void	Spawn( void );
	virtual void	Activate( void );
	bool			CreateVPhysics( void );
	virtual void	UpdateOnRemove( void );
	virtual void	FireGameEvent(IGameEvent* event);
	virtual int		ObjectCaps( void );
	void			Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

	// Portal 2 stuff
	virtual void NotifyPortalEvent(PortalEvent_t nEventType, CPortal_Base2D* pNotifier);
	void TauntedByPlayer(CPortal_Player* pPlayer);
	void TauntedByPlayerFinished(CPortal_Player* pPlayer);

	// TODO: Use m_bEnabled or m_bActive?
	bool IsActive( void ) { return m_bEnabled; }

	// Think functions
	void	Retire( void );
	void	Deploy( void );
	void	ActiveThink( void );
	void	DormantThink( void );
	void	SearchThink( void );
	void	DeathThink( void );

	// Inputs
	void	InputToggle( inputdata_t &inputdata );
	void	InputEnable( inputdata_t &inputdata );
	void	InputDisable( inputdata_t &inputdata );
	void	InputRagdoll( inputdata_t &inputdata );
	void	InputLookAtBlue( inputdata_t &inputdata );
	void	InputLookAtOrange( inputdata_t &inputdata );
	void	InputLookAllTeams( inputdata_t &inputdata );

	void	SetLastSightTime();

	int		OnTakeDamage( const CTakeDamageInfo &inputInfo );
	virtual void	PlayerPenetratingVPhysics( void );
	bool	OnAttemptPhysGunPickup( CBasePlayer *pPhysGunUser, PhysGunPickup_t reason );

	bool	ShouldSavePhysics() { return true; }

	virtual bool CanBeAnEnemyOf( CBaseEntity *pEnemy );

	Class_T	Classify( void ) 
	{
		if( m_bEnabled ) 
			return CLASS_COMBINE;

		return CLASS_NONE;
	}
	
	bool	FVisible( CBaseEntity *pEntity, int traceMask = MASK_BLOCKLOS, CBaseEntity **ppBlocker = NULL );

	Vector	EyeOffset( Activity nActivity ) 
	{
		Vector vForward;

		GetVectors( &vForward, 0, 0 );

		return vForward * 10.0f;
	}

	Vector	EyePosition( void )
	{
		return GetAbsOrigin() + EyeOffset(GetActivity());
	}


protected:
	
	bool	PreThink( cameraState_e state );
	void	Ping( void );	
	void	Toggle( void );
	void	Enable( void );
	void	Disable( void );

	void	EyeOn( void );
	void	EyeOff( void );

	bool	UpdateFacing( void );

	void	Ragdoll();
	bool	CheckRestingSurfaceForPortals();

private:

	CHandle<CSprite>		m_hEyeGlow;

	bool	m_bAutoStart;
	bool	m_bActive;		//Denotes the turret is deployed and looking for targets
	bool	m_bBlinkState;
	bool	m_bEnabled;		//Denotes whether the turret is able to deploy or not

	// Portal 2 stuff
	bool    m_bDetectedNewPing;
	bool    m_bLookAtPlayerPings;
	int     m_nTeamToLookAt;
	int     m_nTeamPlayerToLookAt;
	
	float	m_flLastSight;
	float	m_flPingTime;

	QAngle	m_vecGoalAngles;
	QAngle	m_vecCurrentAngles;
	Vector	m_vNoisePos;
	int		m_iTicksTillNextNoise;

	Vector  m_vecPingLocation;

	CSoundPatch		*m_pMovementSound;

	COutputEvent m_OnDeploy;
	COutputEvent m_OnRetire;

	COutputEvent m_OnTaunted;
	COutputEvent m_OnTauntedBlue;
	COutputEvent m_OnTauntedOrange;
	COutputEvent m_OnTauntedFinished;
	COutputEvent m_OnTauntedBlueFinished;
	COutputEvent m_OnTauntedOrangeFinished;

	DECLARE_DATADESC();
};
#endif // NPC_SECURITY_CAMERA_H