#include "cbase.h"
#include "c_projectedwallentity.h"

#undef CProjectedWallEntity

IMPLEMENT_AUTO_LIST( IProjectedWallEntityAutoList )

IMPLEMENT_CLIENTCLASS_DT( C_ProjectedWallEntity, DT_ProjectedWallEntity, CProjectedWallEntity )
	RecvPropFloat( RECVINFO( m_flLength ) ),
	RecvPropFloat( RECVINFO( m_flHeight ) ),
	RecvPropFloat( RECVINFO( m_flWidth ) ),
	RecvPropFloat( RECVINFO( m_flSegmentLength ) ),
	RecvPropFloat( RECVINFO( m_flParticleUpdateTime ) ),
	
	RecvPropBool( RECVINFO( m_bIsHorizontal ) ),
	RecvPropInt( RECVINFO( m_nNumSegments ) ),
	
	RecvPropVector( RECVINFO( m_vWorldSpace_WallMins ) ),
	RecvPropVector( RECVINFO( m_vWorldSpace_WallMaxs ) ),
END_RECV_TABLE()

BEGIN_PREDICTION_DATA( C_ProjectedWallEntity )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( projected_wall_entity, C_ProjectedWallEntity )

C_ProjectedWallEntity::C_ProjectedWallEntity()
{
	m_hHitPortal = NULL;
	m_hSourcePortal = NULL;
	m_hChildSegment = NULL;
	m_hPlacementHelper = NULL;
}

void C_ProjectedWallEntity::Spawn( void )
{
	//C_BaseEntity::ThinkSet(&v1, this, (BASEPTR)349LL, 0.0, 0);

	// Skip BaseClass
	C_BaseEntity::Spawn();
}

void C_ProjectedWallEntity::OnDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnDataChanged( updateType );
}

void C_ProjectedWallEntity::OnPreDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnPreDataChanged( updateType );
}

void C_ProjectedWallEntity::PostDataUpdate( DataUpdateType_t updateType )
{
	BaseClass::PostDataUpdate( updateType );
}

void C_ProjectedWallEntity::OnProjected( void )
{
	BaseClass::OnProjected();
}

void C_ProjectedWallEntity::ProjectWall( void )
{
}

void C_ProjectedWallEntity::RestoreToToolRecordedState( KeyValues *pKV )
{

}

void C_ProjectedWallEntity::SetPaintPower( int nSegment, PaintPowerType power )
{

}

void C_ProjectedWallEntity::UpdateOnRemove( void )
{
	BaseClass::UpdateOnRemove();
}

bool C_ProjectedWallEntity::TestCollision( const Ray_t &ray, unsigned int mask, trace_t& trace )
{
	return BaseClass::TestCollision( ray, mask, trace );
}

bool C_ProjectedWallEntity::TestHitboxes( const Ray_t &ray, unsigned int fContentsMask, trace_t& tr )
{
	return BaseClass::TestCollision( ray, fContentsMask, tr );
}

const QAngle &C_ProjectedWallEntity::GetRenderAngles( void )
{
	return vec3_angle;
}

void C_ProjectedWallEntity::ClientThink( void )
{
	BaseClass::ClientThink();
}

bool C_ProjectedWallEntity::InitMaterials( void )
{
	return false;
}

RenderableTranslucencyType_t C_ProjectedWallEntity::ComputeTranslucencyType( void )
{
	return RENDERABLE_IS_TRANSLUCENT;
}

ConVar cl_projected_wall_debugdraw( "cl_projected_wall_debugdraw", "0", FCVAR_NONE );
int C_ProjectedWallEntity::DrawModel( int flags, const RenderableInstance_t &instance )
{
	if ( cl_projected_wall_debugdraw.GetBool() )
	{
		//NDebugOverlay::BoxAngles( vec3_origin, m_vWorldSpace_WallMins, m_vWorldSpace_WallMaxs, angles, 0, 128, 255, 64, gpGlobals->frametime );
		NDebugOverlay::Box( vec3_origin, m_vWorldSpace_WallMins, m_vWorldSpace_WallMaxs, 0, 128, 255, 64, gpGlobals->frametime );
	}
	return 1;
}

void C_ProjectedWallEntity::ComputeWorldSpaceSurroundingBox( Vector *pWorldMins, Vector *pWorldMaxs )
{
	*pWorldMins = m_vWorldSpace_WallMins;
	*pWorldMaxs = m_vWorldSpace_WallMaxs;
}

void C_ProjectedWallEntity::GetRenderBounds( Vector& vecMins, Vector& vecMaxs )
{
	vecMins = m_vWorldSpace_WallMins - GetRenderOrigin();
	vecMaxs = m_vWorldSpace_WallMaxs - GetRenderOrigin();
}

void C_ProjectedWallEntity::GetProjectionExtents( Vector &outMins, Vector& outMaxs )
{
	GetExtents( outMins, outMaxs, 0.5 );
}

bool C_ProjectedWallEntity::ShouldDraw( void )
{
	return true;
}

CollideType_t C_ProjectedWallEntity::GetCollideType( void )
{
	return ENTITY_SHOULD_COLLIDE;
}

void C_ProjectedWallEntity::GetToolRecordingState( KeyValues *msg )
{
	BaseClass::GetToolRecordingState( msg );
}