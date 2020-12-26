#include "stdafx.h"

#include "..\igame_persistent.h"
#include "..\environment.h"

void CRenderTarget::phase_postprocess_fog_scattering ()
{
	bool	_menu_pp	= g_pGamePersistent?g_pGamePersistent->OnRenderPPUI_query():false;

	if(1)
	{
		u32			Offset					= 0;
		Fvector2	p0,p1;

		u_setrt	(rt_Generic_0, 0, 0, 0, HW.pBaseZB);
		RCache.set_CullMode			(CULL_CCW);
		RCache.set_Stencil			(FALSE);
		RCache.set_ColorWriteEnable	();

		CHK_DX(HW.pDevice->SetRenderState(D3DRS_ZENABLE, FALSE));

		struct v_postprocess_fog_scattering
		{
			Fvector4	p;
			Fvector2	uv0;
		};

		float	_w					= float(Device.dwWidth);
		float	_h					= float(Device.dwHeight);

		p0.set(.5f/_w, .5f/_h);
		p1.set((_w+.5f)/_w, (_h+.5f)/_h );

		// Fill vertex buffer
		v_postprocess_fog_scattering* pv = (v_postprocess_fog_scattering*) RCache.Vertex.Lock(4, g_postprocess_fog_scattering->vb_stride, Offset);
		pv->p.set(EPS, float(_h+EPS), EPS, 1.f);
		pv->uv0.set(p0.x, p1.y);
		pv++;

		pv->p.set(EPS, EPS,	EPS, 1.f);
		pv->uv0.set(p0.x, p0.y);
		pv++;

		pv->p.set(float(_w+EPS), float(_h+EPS), EPS, 1.f); 
		pv->uv0.set(p1.x, p1.y);
		pv++;

		pv->p.set(float(_w+EPS), EPS, EPS, 1.f); 
		pv->uv0.set(p1.x, p0.y); 
		pv++;

		RCache.Vertex.Unlock(4, g_postprocess_fog_scattering->vb_stride);

		RCache.set_Shader(s_postprocess_fog_scattering);
		RCache.set_Geometry	(g_postprocess_fog_scattering);
		RCache.Render(D3DPT_TRIANGLELIST,Offset,0,4,0,2);

		CHK_DX(HW.pDevice->SetRenderState(D3DRS_ZENABLE, TRUE));
	}
}