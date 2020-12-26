#include "stdafx.h"

#include "..\igame_persistent.h"
#include "..\environment.h"

void CRenderTarget::phase_postprocess_tonemap ()
{
	bool	_menu_pp	= g_pGamePersistent?g_pGamePersistent->OnRenderPPUI_query():false;
	

	u_setrt	(rt_previousColor, 0, 0, 0, HW.pBaseZB);
	RCache.set_CullMode				(CULL_CCW);
	RCache.set_Stencil(FALSE);
	g_pGamePersistent->Environment().RenderLast(); // rain/thunder-bolts
	RCache.set_Stencil	(FALSE);

	//*** exposure-pipeline
	u32			gpu_id	= Device.dwFrame%2;
	{
		t_LUM_src->surface_set		(rt_LUM_pool[gpu_id*2+0]->pSurface);
		t_LUM_dest->surface_set		(rt_LUM_pool[gpu_id*2+1]->pSurface);
	}

	if(1)
	{
		u32			Offset					= 0;
		Fvector2	p0, p1;

		u_setrt	(rt_Generic_0, rt_Generic_1, 0, 0, HW.pBaseZB);
		RCache.set_CullMode(CULL_NONE);
		RCache.set_Stencil(FALSE);

		struct v_postprocess_tonemap_exposure
		{
			Fvector4	position;
			Fvector2	uv0;
		};

		float	_w					= float(Device.dwWidth);
		float	_h					= float(Device.dwHeight);

		float du = 0.5f / _w;
		float dv = 0.5f / _h;

		p0.set(du, dv);
		p1.set((_w + 0.5f) / _w, (_h + 0.5f) / _h);

		// Fill vertex buffer
		v_postprocess_tonemap_exposure* pv = (v_postprocess_tonemap_exposure*)
		
		RCache.Vertex.Lock(4, g_postprocess_tonemap_exposure->vb_stride, Offset);

		pv->position.set(EPS, float(_h + EPS), EPS, 1.f);
		pv->uv0.set(p0.x, p1.y);
		pv++;

		pv->position.set(EPS, EPS,	EPS, 1.f);
		pv->uv0.set(p0.x, p0.y);
		pv++;

		pv->position.set(float(_w + EPS), float(_h + EPS), EPS, 1.f); 
		pv->uv0.set(p1.x, p1.y);
		pv++;

		pv->position.set(float(_w + EPS), EPS, EPS, 1.f); 
		pv->uv0.set(p1.x, p0.y); 
		pv++;

		RCache.Vertex.Unlock(4, g_postprocess_tonemap_exposure->vb_stride);

		// Shader
		RCache.set_Shader(s_postprocess_tonemap_exposure);
		RCache.set_Geometry	(g_postprocess_tonemap_exposure);
		RCache.Render(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
	}


	// Perform blooming filter and distortion if needed
	RCache.set_Stencil	(FALSE);
	phase_bloom			( );												// HDR RT invalidated here

	{
		u32			Offset					= 0;
		Fvector2	p0, p1;

		u_setrt(Device.dwWidth, Device.dwHeight, HW.pBaseRT, NULL, NULL, NULL, HW.pBaseZB);

		

		RCache.set_ColorWriteEnable();

		CHK_DX(HW.pDevice->SetRenderState	( D3DRS_SRGBWRITEENABLE,			0				));

		struct v_postprocess_tonemap
		{
			Fvector4	position;
			Fvector2	uv0;
		};

		float	_w					= float(Device.dwWidth);
		float	_h					= float(Device.dwHeight);

		float du = 0.5f / _w;
		float dv = 0.5f / _h;

		p0.set(du, dv);
		p1.set((_w + 0.5f) / _w, (_h + 0.5f) / _h);

		// Fill vertex buffer
		v_postprocess_tonemap* pv = (v_postprocess_tonemap*)
		
		RCache.Vertex.Lock(4, g_postprocess_tonemap->vb_stride, Offset);

		pv->position.set(EPS, float(_h + EPS), EPS, 1.f);
		pv->uv0.set(p0.x, p1.y);
		pv++;

		pv->position.set(EPS, EPS,	EPS, 1.f);
		pv->uv0.set(p0.x, p0.y);
		pv++;

		pv->position.set(float(_w + EPS), float(_h + EPS), EPS, 1.f); 
		pv->uv0.set(p1.x, p1.y);
		pv++;

		pv->position.set(float(_w + EPS), EPS, EPS, 1.f); 
		pv->uv0.set(p1.x, p0.y); 
		pv++;

		RCache.Vertex.Unlock(4, g_postprocess_tonemap->vb_stride);

		// Shader
		RCache.set_Shader(s_postprocess_tonemap);


		Fvector4 tonemapParams = {ps_r2_tonemap_bias,0,0,0};
		RCache.set_c("dx_Tonemap_Params", tonemapParams);

		RCache.set_Geometry	(g_postprocess_tonemap);
		RCache.Render(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
	}

	{
		// PP enabled ?
		BOOL	PP_Complex		= u_need_PP	();
		if (_menu_pp)			PP_Complex	= FALSE;

		//	PP-if required
		if (PP_Complex)
		{
			phase_pp		();
		}
	}
	
	//	Re-adapt luminance
	RCache.set_Stencil(FALSE);

	//*** exposure-pipeline-clear
	{
		std::swap					(rt_LUM_pool[gpu_id*2+0],rt_LUM_pool[gpu_id*2+1]);
		t_LUM_src->surface_set		(NULL);
		t_LUM_dest->surface_set		(NULL);
	}
}