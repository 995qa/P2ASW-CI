//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "iclientmode.h"
#include "engine/IEngineSound.h"
#include "vgui_controls/AnimationController.h"
#include "vgui_controls/Controls.h"
#include "vgui_controls/Panel.h"
#include "vgui/ISurface.h"
#include "c_portal_player.h"
#include "c_weapon_portalgun.h"
#include "igameuifuncs.h"
#include "iinput.h"
#include "radialmenu.h"
#include "ivieweffects.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define	HEALTH_WARNING_THRESHOLD	25


static ConVar	hud_quickinfo( "hud_quickinfo", "1", FCVAR_ARCHIVE );
static ConVar	hud_quickinfo_swap( "hud_quickinfo_swap", "0", FCVAR_ARCHIVE );

extern ConVar crosshair;

#define QUICKINFO_EVENT_DURATION	1.0f
#define	QUICKINFO_BRIGHTNESS_FULL	255
#define	QUICKINFO_BRIGHTNESS_DIM	64
#define	QUICKINFO_FADE_IN_TIME		0.5f
#define QUICKINFO_FADE_OUT_TIME		2.0f

// P2ASW TODO: This was probably an existing function that got inlined, but I couldn't find one which matched
// It's needed for the crosshair colors to match P2 - Kelsey
int QuickInfo_ColorTransform( int input )
{
	if ( input < 5 )
		return 5;
	
	float retVal = input + ((1.0 - (input / 255.0)) * 64.0);
	if ( retVal > 255.0 )
		return 255;
		
	return retVal;
}

/*
==================================================
CHUDQuickInfo 
==================================================
*/

class CHUDQuickInfo : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CHUDQuickInfo, vgui::Panel );
public:    
	CHUDQuickInfo( const char *pElementName );
    ~CHUDQuickInfo();
	void Init( void );
	void VidInit( void );
	bool ShouldDraw( void );
	//virtual void OnThink();
	virtual void Paint();
	
	virtual void ApplySchemeSettings( vgui::IScheme *scheme );	
private:

	void 	DrawCrosshair( Color color, float flApparentZ );
    void 	DrawPortalHint( Vector& vecPosition, bool bBluePortal );
    void 	DrawPortalHints();
	void	DrawWarning( int x, int y, CHudTexture *icon, float &time );
    void 	UpdateEventTime();
    bool 	EventTimeElapsed();
		
	float	m_flLastEventTime;
	
	float	m_fLastPlacedAlpha[2];
	bool	m_bLastPlacedAlphaCountingUp[2];

	CHudTexture	*m_icon_rbn;	// right bracket
	CHudTexture	*m_icon_lbn;	// left bracket

	CHudTexture	*m_icon_rb;		// right bracket, full
	CHudTexture	*m_icon_lb;		// left bracket, full
	
    int m_nArrowTexture;
    int m_nCursorRadius;
    int m_nPortalIconOffsetX;
    int m_nPortalIconOffsetY;
	
    float m_flPortalIconScale;
};

DECLARE_HUDELEMENT( CHUDQuickInfo );

CHUDQuickInfo::CHUDQuickInfo( const char *pElementName ) :
	CHudElement( pElementName ), BaseClass( NULL, "HUDQuickInfo" )
{
	vgui::Panel *pParent = GetClientMode()->GetViewport();
	SetParent( pParent );

	SetHiddenBits( HIDEHUD_CROSSHAIR );

	m_fLastPlacedAlpha[0] = m_fLastPlacedAlpha[1] = 0.0f;
	m_nCursorRadius = m_nPortalIconOffsetX = m_nPortalIconOffsetY = 0;
	m_bLastPlacedAlphaCountingUp[0] = m_bLastPlacedAlphaCountingUp[1] = true;
	m_nArrowTexture = -1; // TODO should this be a constant?
	m_flPortalIconScale = 1.0f;
}

CHUDQuickInfo::~CHUDQuickInfo()
{
	if (vgui::surface() && m_nArrowTexture != -1)
	{
		vgui::surface()->DestroyTextureID( m_nArrowTexture );
		m_nArrowTexture = -1;
	}
}

void CHUDQuickInfo::ApplySchemeSettings( vgui::IScheme *scheme )
{
	BaseClass::ApplySchemeSettings( scheme );

	m_nCursorRadius = 10;
	m_nPortalIconOffsetX = 6;
	m_nPortalIconOffsetY = 10;
	m_flPortalIconScale = 1.0f;

	SetPaintBackgroundEnabled( false );
}


void CHUDQuickInfo::Init( void )
{
	m_flLastEventTime   = 0.0f;

	if ( m_nArrowTexture == -1 )
	{
		m_nArrowTexture = vgui::surface()->CreateNewTextureID();
		vgui::surface()->DrawSetTextureFile(m_nArrowTexture, "HUD/hud_icon_arrow", 1, false);
	}
}


void CHUDQuickInfo::VidInit( void )
{
	Init();
	
	m_icon_rb = HudIcons().GetIcon( "portal_crosshair_right_valid" );
	m_icon_lb = HudIcons().GetIcon( "portal_crosshair_left_valid" );
	m_icon_rbn = HudIcons().GetIcon( "portal_crosshair_right_invalid" );
	m_icon_lbn = HudIcons().GetIcon( "portal_crosshair_left_invalid" );
}


void CHUDQuickInfo::DrawWarning( int x, int y, CHudTexture *icon, float &time )
{
	float scale	= (int)( fabs(sin(gpGlobals->curtime*8.0f)) * 128.0);

	// Only fade out at the low point of our blink
	if ( time <= (gpGlobals->frametime * 200.0f) )
	{
		if ( scale < 40 )
		{
			time = 0.0f;
			return;
		}
		else
		{
			// Counteract the offset below to survive another frame
			time += (gpGlobals->frametime * 200.0f);
		}
	}
	
	// Update our time
	time -= (gpGlobals->frametime * 200.0f);
	Color caution = GetHud().m_clrCaution;
	caution[3] = scale * 255;

	icon->DrawSelf( x, y, caution );
}

//-----------------------------------------------------------------------------
// Purpose: Save CPU cycles by letting the HUD system early cull
// costly traversal.  Called per frame, return true if thinking and 
// painting need to occur.
//-----------------------------------------------------------------------------
bool CHUDQuickInfo::ShouldDraw( void )
{
	if (!m_icon_rb || !m_icon_rbn || !m_icon_lb || !m_icon_lbn)
		return false;

	if ( !GetClientMode()->ShouldDrawCrosshair() )
		return false;

	C_BasePlayer *player = C_BasePlayer::GetLocalPlayer();
	if ( player == NULL )
		return false;

	if ( !crosshair.GetBool() )
		return false;

	if ( IsRadialMenuOpen() )
		return false;

	if ( player->GetPlayerRenderMode( player->GetSplitScreenPlayerSlot() ) == PLAYER_RENDER_THIRDPERSON ||
		 ( player->GetViewEntity() && player->GetViewEntity() != player ) )
		return false;

	return ( CHudElement::ShouldDraw() && !engine->IsDrawingLoadingImage() );
}

void CHUDQuickInfo::DrawPortalHint( Vector& vecPosition, bool bBluePortal )
{
	// TODO: Unused function. Only found in Linux bins, optimized out of others.
	// I couldn't be bothered to figure this out for now, but it seems to share a lot of code with
	// CHudCoopPingIndicator::DrawIndicatorHint(), which IS compiled into Windows/Mac - Kelsey
#if 0
	if ( vecPosition == vec3_invalid )
		return;

	Vector vecScreen;
	ScreenTransform(vecPosition, vecScreen);

	// math
	float xCenter = ScreenWidth() / 2;
	float yCenter = ScreenWidth() / 2;

	float posX = xCenter + ScreenWidth() * ( vecScreen.x * 0.5f ) + 0.5f;
	float posY = yCenter - ScreenHeight() * ( vecScreen.y * 0.5f ) + 0.5f;

	float offsetX = posX - xCenter;
	float offsetY = posY - yCenter;

	float flDist = sqrtf( ( offsetY * offsetY ) + ( offsetX * offsetX ) );

	float flInnerCircle;
	if ( ScreenWidth() < ScreenHeight() )
		flInnerCircle = ( ScreenWidth() * 0.9f ) * 0.5f;
	else
		flInnerCircle = ( ScreenHeight() * 0.9f ) * 0.5f;

	float atan2Result = atan2(offsetY, offsetX);

	float ca = cos(atan2Result);
	float sa = sin(atan2Result);

	if ( flInnerCircle <= ( flDist * 0.5f ) )
		offsetY = flInnerCircle;
	else
		offsetY = flDist * 0.5f;

	float finalX = ( ScreenWidth() / 2 ) + (offsetY * ca);

	C_BasePlayer* pPlayer = GetSplitScreenViewPlayer( GET_ACTIVE_SPLITSCREEN_SLOT() );
	Assert(pPlayer);
	Color colorPortal = UTIL_Portal_Color( bBluePortal ? 1 : 2, pPlayer->GetTeamNumber() );
	colorPortal[3] = 255;

	// numbers
	vgui::Vertex_t verts[4];

	verts[0].m_TexCoord.Init( 0, 0 );
	verts[1].m_TexCoord.Init( 1, 0 );
	verts[2].m_TexCoord.Init( 1, 1 );
	verts[3].m_TexCoord.Init( 0, 1 );

	vgui::surface()->DrawSetColor( colorPortal );
	vgui::surface()->DrawSetTexture( m_nArrowTexture );
	vgui::surface()->DrawTexturedPolygon( 4, verts );

	// TEMP
	//vgui::surface()->DrawOutlinedCircle( ( vecScreen.x + 1 ) / 2 * ScreenWidth(), ( -vecScreen.y + 1 ) / 2 * ScreenHeight(), 30, 30 );
#endif
}

void CHUDQuickInfo::DrawPortalHints()
{
	C_Portal_Player *pPortalPlayer = C_Portal_Player::GetLocalPortalPlayer();
	if ( !pPortalPlayer )
		return;
	
	C_BaseCombatWeapon *pWeapon = pPortalPlayer->GetActiveWeapon();
	if ( !pWeapon )
		return;

	C_WeaponPortalgun *pPortalgun = dynamic_cast<C_WeaponPortalgun*>( pWeapon );
	if ( !pPortalgun )
		return;

	DrawPortalHint( pPortalgun->m_vecBluePortalPos, true );
	DrawPortalHint( pPortalgun->m_vecOrangePortalPos, false );
}

void CHUDQuickInfo::DrawCrosshair( Color color, float flApparentZ )
{
	int centerX = ScreenWidth() / 2;
	int centerY = ScreenHeight() / 2;

#ifndef P2ASW // Not in Swarm
	vgui::surface()->DrawSetApparentDepth(flApparentZ);
#endif
	vgui::surface()->DrawSetColor(color);

	g_pVGuiSurface->DrawFilledRect( centerX, centerY, centerX + 1, centerY + 1);
	g_pVGuiSurface->DrawFilledRect( centerX + m_nCursorRadius,centerY,centerX + m_nCursorRadius + 1,centerY + 1);
	g_pVGuiSurface->DrawFilledRect( centerX - m_nCursorRadius,centerY,centerX - m_nCursorRadius + 1,centerY + 1);
	g_pVGuiSurface->DrawFilledRect( centerX,centerY + m_nCursorRadius,centerX + 1,centerY + m_nCursorRadius + 1);
	g_pVGuiSurface->DrawFilledRect( centerX,centerY - m_nCursorRadius,centerX + 1,centerY - m_nCursorRadius + 1);

#ifndef P2ASW
	vgui::surface()->DrawClearApparentDepth();
#endif
}

void CHUDQuickInfo::Paint()
{
	C_Portal_Player *pPortalPlayer = (C_Portal_Player*)( GetSplitScreenViewPlayer( GET_ACTIVE_SPLITSCREEN_SLOT() ) );
	if ( pPortalPlayer == NULL )
		return;

	C_BaseCombatWeapon *pWeapon = pPortalPlayer->GetActiveWeapon();

	// Find any full-screen fades
	byte color[4];
	bool blend;
	GetViewEffects()->GetFadeParams( &color[0], &color[1], &color[2], &color[3], &blend );
	float flFadeAlpha = color[3];
	Color clrCrosshair(255, 255, 255, 255 - flFadeAlpha);

#ifndef P2ASW
	float flApparentZ = vgui::STEREO_NOOP;
	if ( materials->IsStereoActiveThisFrame() )
	{
		// P2P2 TODO
		// Do we even need to support this? It's for some nvidia stereoscopic 3D thing
	}
#else
	float flApparentZ = 1.0f;
#endif

	// Crosshair dots are drawn by the quickinfo hud element in portal2
	DrawCrosshair(clrCrosshair, flApparentZ);

	if ( pWeapon == NULL )
		return;

	int		xCenter	= ( ScreenWidth() ) / 2;
	int		yCenter = ( ScreenHeight() ) / 2;

	C_WeaponPortalgun *pPortalgun = dynamic_cast<C_WeaponPortalgun*>( pWeapon );

	if ( !hud_quickinfo.GetInt() || !pPortalgun || ( !pPortalgun->CanFirePortal1() && !pPortalgun->CanFirePortal2() ) )
	{
		// no quickinfo or we can't fire either portal, just draw the basic crosshair dots
		return;
	}

	int iTeamNumber = pPortalPlayer->GetTeamNumber();
	bool bCanFireBoth = pPortalgun->CanFirePortal1() && pPortalgun->CanFirePortal2();
	bool bSwaped = hud_quickinfo_swap.GetBool();

	if ( input->ControllerModeActive() && bCanFireBoth )
		bSwaped = !bSwaped;

	const unsigned char iAlphaStart = 150;	   
	
	Color portal1Color = UTIL_Portal_Color( bSwaped ? 2 : 1, iTeamNumber );
	Color portal2Color = UTIL_Portal_Color( bSwaped ? 1 : 2, iTeamNumber );

	// p2asw todo
	portal1Color[0] = QuickInfo_ColorTransform( portal1Color[0] );
	portal1Color[1] = QuickInfo_ColorTransform( portal1Color[1] );
	portal1Color[2] = QuickInfo_ColorTransform( portal1Color[2] );
	portal2Color[0] = QuickInfo_ColorTransform( portal2Color[0] );
	portal2Color[1] = QuickInfo_ColorTransform( portal2Color[1] );
	portal2Color[2] = QuickInfo_ColorTransform( portal2Color[2] );

	portal1Color[ 3 ] = iAlphaStart;
	portal2Color[ 3 ] = iAlphaStart;

	const int iBaseLastPlacedAlpha = 128;
	Color lastPlaced1Color = Color( portal1Color[0], portal1Color[1], portal1Color[2], iBaseLastPlacedAlpha );
	Color lastPlaced2Color = Color( portal2Color[0], portal2Color[1], portal2Color[2], iBaseLastPlacedAlpha );

	if ( pPortalgun->CanFirePortal1() && pPortalgun->CanFirePortal2() )
	{
		bool bPortal1Placed = ( bSwaped ? pPortalgun->m_vecOrangePortalPos : pPortalgun->m_vecBluePortalPos ) != vec3_invalid;
		bool bPortal2Placed = ( bSwaped ? pPortalgun->m_vecBluePortalPos : pPortalgun->m_vecOrangePortalPos ) != vec3_invalid;

		int iPortal1Alpha = 255 - flFadeAlpha;
		if ( !bPortal1Placed )
			iPortal1Alpha = 0;
		lastPlaced1Color[3] = iPortal1Alpha;

		int iPortal2Alpha = 255 - flFadeAlpha;
		if ( !bPortal2Placed )
			iPortal2Alpha = 0;
		lastPlaced2Color[3] = iPortal2Alpha;
	}
	//can't fire both portals, and we want the crosshair to remain somewhat symmetrical without being confusing
	// This might not be totally accurate, but it seems to be functionally the same - Kelsey
	else if ( !pPortalgun->CanFirePortal1() )
	{
		// clone portal2 info to portal 1
		portal1Color = portal2Color;
		lastPlaced1Color = lastPlaced2Color;
	}
	else if ( !pPortalgun->CanFirePortal2() )
	{
		// clone portal1 info to portal 2
		portal2Color = portal1Color;
		lastPlaced2Color = lastPlaced1Color;
	}

	// P2 code seems to draw all of these always, but the placed indicators have alpha 0 when not placed
	// The ifdefs here really suck for readability... P2P2 FIXME: remove
	m_icon_lbn->DrawSelf( xCenter - m_nPortalIconOffsetX - ( m_icon_lbn->EffectiveWidth( m_flPortalIconScale ) / 2 ),
						  yCenter - m_nPortalIconOffsetY - ( m_icon_lbn->EffectiveHeight( m_flPortalIconScale ) / 2 ),
						  m_icon_lbn->EffectiveWidth( m_flPortalIconScale ), m_icon_lbn->EffectiveHeight( m_flPortalIconScale ), portal1Color
#ifndef P2ASW
						  , flApparentZ
#endif
						  );

	m_icon_rbn->DrawSelf( xCenter + m_nPortalIconOffsetX - ( m_icon_rbn->EffectiveWidth( m_flPortalIconScale ) / 2 ),
						  yCenter + m_nPortalIconOffsetY - ( m_icon_rbn->EffectiveHeight( m_flPortalIconScale ) / 2 ),
						  m_icon_rbn->EffectiveWidth( m_flPortalIconScale ), m_icon_rbn->EffectiveHeight( m_flPortalIconScale ), portal2Color
#ifndef P2ASW
						  , flApparentZ
#endif
						  );

	m_icon_lb->DrawSelf( xCenter - m_nPortalIconOffsetX - ( m_icon_lb->EffectiveWidth( m_flPortalIconScale ) / 2 ),
						  yCenter - m_nPortalIconOffsetY - ( m_icon_lb->EffectiveHeight( m_flPortalIconScale ) / 2 ),
						  m_icon_lb->EffectiveWidth( m_flPortalIconScale ), m_icon_lb->EffectiveHeight( m_flPortalIconScale ), lastPlaced1Color
#ifndef P2ASW
						  , flApparentZ
#endif
						  );

	m_icon_rb->DrawSelf( xCenter + m_nPortalIconOffsetX - ( m_icon_rb->EffectiveWidth( m_flPortalIconScale ) / 2 ),
						  yCenter + m_nPortalIconOffsetY - ( m_icon_rb->EffectiveHeight( m_flPortalIconScale ) / 2 ),
						  m_icon_rb->EffectiveWidth( m_flPortalIconScale ), m_icon_rb->EffectiveHeight( m_flPortalIconScale ), lastPlaced2Color
#ifndef P2ASW
						  , flApparentZ
#endif
						  );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHUDQuickInfo::UpdateEventTime( void )
{
	m_flLastEventTime = gpGlobals->curtime;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CHUDQuickInfo::EventTimeElapsed( void )
{
	if (( gpGlobals->curtime - m_flLastEventTime ) > QUICKINFO_EVENT_DURATION )
		return true;

	return false;
}

