#include "stdafx.h"
#include "..\igame_persistent.h"
#include "..\environment.h"

void	CRenderTarget::phase_forward	()
{
	bool	_menu_pp	= g_pGamePersistent?g_pGamePersistent->OnRenderPPUI_query():false;

	CEnvDescriptorMixer& environment = g_pGamePersistent->Environment().CurrentEnv;

	RCache.set_Stencil(FALSE);

	u_setrt	(rt_Generic_1, 0, 0, 0, HW.pBaseZB);
	RCache.set_CullMode				(CULL_CCW);
	RCache.set_Stencil(FALSE);
	RCache.set_ColorWriteEnable	(D3DCOLORWRITEENABLE_RED|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_BLUE);
	RImplementation.render_forward	();
	RCache.set_Stencil	(FALSE);

	// Distortion filter
	BOOL	bDistort	= RImplementation.o.distortion_enabled;				// This can be modified
	{
		if		((0==RImplementation.mapDistort.size()) && !_menu_pp)		bDistort= FALSE;
		if (bDistort)		{
			u_setrt						(rt_distortion,0,0,0,HW.pBaseZB);		// Now RT is a distortion mask
			RCache.set_CullMode			(CULL_CCW);
			RCache.set_Stencil			(FALSE);
			RCache.set_ColorWriteEnable	();
			CHK_DX(HW.pDevice->Clear	( 0L, NULL, D3DCLEAR_TARGET, color_rgba(127,127,0,127), 1.0f, 0L));
			RImplementation.r_dsgraph_render_distort	();
			//if (g_pGamePersistent)	g_pGamePersistent->OnRenderPPUI_PP()	;	// PP-UI
		}
	}
}