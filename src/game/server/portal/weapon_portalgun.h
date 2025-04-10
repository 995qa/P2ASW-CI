//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef WEAPON_PORTALGUN_H
#define WEAPON_PORTALGUN_H
#ifdef _WIN32
#pragma once
#endif

#include "portal_shareddefs.h"
#include "weapon_portalbasecombatweapon.h"
#include "weapon_portalgun_shared.h"

#include "prop_portal.h"


class CWeaponPortalgun : public CBasePortalCombatWeapon
{
	DECLARE_DATADESC();

public:
	DECLARE_CLASS( CWeaponPortalgun, CBasePortalCombatWeapon );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

private:
	CNetworkVar( bool,	m_bCanFirePortal1 );	// Is able to use primary fire
	CNetworkVar( bool,	m_bCanFirePortal2 );	// Is able to use secondary fire
	CNetworkVar( int,	m_iLastFiredPortal );	// Which portal was placed last
	CNetworkVar( bool,	m_bOpenProngs );		// Which portal was placed last

	CNetworkVector( m_vecBluePortalPos );
	CNetworkVector( m_vecOrangePortalPos );

	CHandle<CBaseEntity> m_hFreezeEntity;

	CNetworkVar( unsigned char, m_iPortalLinkageGroupID );

public:
	// HACK HACK! Used to make the gun visually change when going through a cleanser!
	CNetworkVar( float,	m_fEffectsMaxSize1 );
	CNetworkVar( float,	m_fEffectsMaxSize2 );

protected:
	void PostAttack( void );

public:
	virtual const Vector& GetBulletSpread( void )
	{
		static Vector cone = VECTOR_CONE_10DEGREES;
		return cone;
	}
	
	virtual void Precache ( void );

	virtual void CreateSounds( void );
	virtual void StopLoopingSounds( void );

	virtual void OnRestore( void );
	virtual void UpdateOnRemove( void );
	void Spawn( void );
	virtual void Activate();
	void DoEffectBlast( CBaseEntity *pOwner, bool bPortal2, int iPlacedBy, const Vector &ptStart, const Vector &ptFinalPos, const QAngle &qStartAngles, float fDelay );
	virtual void OnPickedUp( CBaseCombatCharacter *pNewOwner );

	virtual bool ShouldDrawCrosshair( void );
	void SetLastFiredPortal( int iLastFiredPortal ) { m_iLastFiredPortal = iLastFiredPortal; }
	int GetLastFiredPortal( void ) { return m_iLastFiredPortal; }

	Activity GetPrimaryAttackActivity( void );

	bool Reload( void );
	void FillClip( void );
	void CheckHolsterReload( void );
	void ItemHolsterFrame( void );
	bool Holster( CBaseCombatWeapon *pSwitchingTo = NULL );
	bool Deploy( void );

	void ResetRefireTime( void );

	void SetCanFirePortal1( bool bCanFire = true );
	void SetCanFirePortal2( bool bCanFire = true );
	bool CanFirePortal1( void ) const;
	bool CanFirePortal2( void ) const;

	void UnfreezeEntity();
	void FreezeEntity( CBaseEntity* target );

	void PrimaryAttack( void );
	void SecondaryAttack( void );

	void DelayAttack( float fDelay );

	virtual bool PreThink( void );	
	void GunEffectsThink( void );
    static char *s_szGunEffectsThinkContext;

	void OpenProngs( bool bOpenProngs );

	void InputChargePortal1( inputdata_t &inputdata );
	void InputChargePortal2( inputdata_t &inputdata );
	
	void FirePortal1( void );
	void FirePortal2( void );

	void FirePortal1Helper( inputdata_t& inputdata );
	void FirePortal2Helper( inputdata_t& inputdata );

	void FirePortalDirection1( inputdata_t &inputdata );
	void FirePortalDirection2( inputdata_t &inputdata );

	void ClearPortalPositions( void );
	void PortalPlaced( void );

	bool TraceFirePortal( const Vector &vTraceStart, const Vector &vDirection, bool bPortal2, PortalPlacedBy_t ePlacedBy, TracePortalPlacementInfo_t &placementInfo );
	PortalPlacementResult_t FirePortal( bool bPortal2, Vector *pVector = 0 );

	void SetLinkageGroupID( int iNewID );
	unsigned int GetLinkageGroupID() { return m_iPortalLinkageGroupID; }

	CSoundPatch		*m_pMiniGravHoldSound;

	// Outputs for portalgun
	COutputEvent m_OnFiredPortal1;		// Fires when the gun's first (blue) portal is fired
	COutputEvent m_OnFiredPortal2;		// Fires when the gun's second (red) portal is fired

	void DryFire( void );
	void UseDeny( void );
	virtual float GetFireRate( void ) { return 0.7; };
	void WeaponIdle( void );

	int GetWeaponID( void ) const { return WEAPON_PORTALGUN; }

	CProp_Portal *GetAssociatedPortal( bool bPortal2 );
	void UpdatePortalAssociation( void );
	virtual void	ChangeTeam( int iTeamNum );
	void SetPotatosOnPortalgun( bool bShowPotatos );
	
	void TogglePotatosThink( void );
	static char *s_szTogglePotatosThinkContext;

	bool	m_bShowingPotatos;

	int m_nStartingTeamNum;

protected:

	bool AttemptStealCoopPortal( TracePortalPlacementInfo_t &placementInfo );
	bool  PortalTraceClippedByBlockers( ComplexPortalTrace_t *pTraceResults, int nNumResultSegments, const Vector &vecDirection, bool bIsSecondPortal, TracePortalPlacementInfo_t &placementInfo );
	bool AttemptSnapToPlacementHelper( CProp_Portal *pPortal, ComplexPortalTrace_t *pTraceResults, int nNumResultSegments, PortalPlacedBy_t ePlacedBy, TracePortalPlacementInfo_t &placementInfo );


	void	StartEffects( void );	// Initialize all sprites and beams
	void	StopEffects( bool stopSound = true );	// Hide all effects temporarily
	void	DestroyEffects( void );	// Destroy all sprites and beams

	// Portalgun effects
	void	DoEffect( int effectType, Vector *pos = NULL );

	void	DoEffectClosed( void );
	void	DoEffectReady( void );
	void	DoEffectHolding( void );
	void	DoEffectNone( void );

	CNetworkVar( int,	m_EffectState );		// Current state of the effects on the gun
	
	CNetworkHandle( CProp_Portal, m_hPrimaryPortal );
	CNetworkHandle( CProp_Portal, m_hSecondaryPortal );

public:

	DECLARE_ACTTABLE();

	CWeaponPortalgun(void);

private:
	CWeaponPortalgun( const CWeaponPortalgun & );
};


#endif // WEAPON_PORTALGUN_H
