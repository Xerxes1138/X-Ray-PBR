#include "stdafx.h"

#include "..\igame_persistent.h"
#include "..\environment.h"

void CRenderTarget::phase_UI ()
{
	//CHK_DX(HW.pDevice->SetRenderState	( D3DRS_SRGBWRITEENABLE,			0				));

	u_setrt(Device.dwWidth, Device.dwHeight, HW.pBaseRT, NULL, NULL,NULL, HW.pBaseZB);

	//CHK_DX(HW.pDevice->SetRenderState	( D3DRS_SRGBWRITEENABLE,			0				));

	RCache.set_CullMode(CULL_NONE);
	RCache.set_Stencil(FALSE);


	if (g_pGamePersistent)	g_pGamePersistent->OnRenderPPUI_main()	;	// PP-UI
}