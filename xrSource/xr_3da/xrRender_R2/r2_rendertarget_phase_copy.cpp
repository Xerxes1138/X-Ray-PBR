#include "stdafx.h"

void CRenderTarget::phase_copy (const ref_rt&  source, const ref_rt& destination)
{
	{
		u32			Offset					= 0;
		Fvector2	p0,p1;

		u_setrt	(source, 0,0,0,0 );

		CHK_DX(HW.pDevice->SetRenderState(D3DRS_ZENABLE, FALSE));

		struct v_blit	
		{
			Fvector4	p;
			Fvector2	uv0;
		};

		float	_w					= float(Device.dwWidth);
		float	_h					= float(Device.dwHeight);

		p0.set(.5f/_w, .5f/_h);
		p1.set((_w+.5f)/_w, (_h+.5f)/_h );

		// Fill vertex buffer
		v_blit* pv = (v_blit*) RCache.Vertex.Lock(4, g_blit->vb_stride, Offset);
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

		RCache.Vertex.Unlock(4, g_blit->vb_stride);

		RCache.set_Shader(s_blit);
		RCache.set_Geometry	(g_blit);
		RCache.Render(D3DPT_TRIANGLELIST,Offset,0,4,0,2);

		CHK_DX(HW.pDevice->SetRenderState(D3DRS_ZENABLE, TRUE));
	}
}