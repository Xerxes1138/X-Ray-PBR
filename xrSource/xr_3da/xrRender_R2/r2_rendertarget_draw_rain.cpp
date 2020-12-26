#include "stdafx.h"
#include "..\igame_persistent.h"
#include "..\environment.h"

float ps_r3_dyn_wet_surf_near = 5.f; // 10.0f
float ps_r3_dyn_wet_surf_far = 50.f; // 30.0f
int ps_r3_dyn_wet_surf_sm_res = 4096; // 256

void CRenderTarget::draw_rain( light &RainSetup )
{
	float	fRainFactor	= g_pGamePersistent->Environment().CurrentEnv.rain_density;

	// Common calc for quad-rendering
	u32		Offset;
	u32		C					= color_rgba	(255,255,255,255);
	float	_w					= float			(Device.dwWidth);
	float	_h					= float			(Device.dwHeight);
	Fvector2					p0,p1;
	p0.set						(.5f/_w, .5f/_h);
	p1.set						((_w+.5f)/_w, (_h+.5f)/_h );
	float	d_Z	= EPS_S, d_W = 1.f;

	// Common constants (light-related)
	Fvector		L_dir;
	Device.mView.transform_dir	(L_dir,RainSetup.direction);
	L_dir.normalize				();

	Fvector		W_dirX;
	Device.mView.transform_dir	(W_dirX,Fvector().set(1.0f, 0.0f, 0.0f));
	W_dirX.normalize				();

	Fvector		W_dirZ;
	Device.mView.transform_dir	(W_dirZ,Fvector().set(0.0f, 0.0f, 1.0f));
	W_dirZ.normalize				();

	if (1)
	{
		// Fill vertex buffer
		FVF::TL* pv					= (FVF::TL*)	RCache.Vertex.Lock	(4,g_combine->vb_stride,Offset);
		pv->set						(EPS,			float(_h+EPS),	d_Z,	d_W, C, p0.x, p1.y);	pv++;
		pv->set						(EPS,			EPS,			d_Z,	d_W, C, p0.x, p0.y);	pv++;
		pv->set						(float(_w+EPS),	float(_h+EPS),	d_Z,	d_W, C, p1.x, p1.y);	pv++;
		pv->set						(float(_w+EPS),	EPS,			d_Z,	d_W, C, p1.x, p0.y);	pv++;
		RCache.Vertex.Unlock		(4,g_combine->vb_stride);
		RCache.set_Geometry			(g_combine);
	}

	// recalculate d_Z, to perform depth-clipping
	const float fRainFar = ps_r3_dyn_wet_surf_far;
	Fvector	center_pt;
	center_pt.mad(Device.vCameraPosition, Device.vCameraDirection, fRainFar);
	Device.mFullTransform.transform(center_pt);
	d_Z							= center_pt.z;

	// Perform lighting
	{
		// texture adjustment matrix
		float			fTexelOffs			= (.5f / float(RImplementation.o.smapsize));
		float			fRange				= ps_r2_hao_depth_near_scale;
		float			fBias				= ps_r2_hao_depth_near_bias;
		float			smapsize			= float(RImplementation.o.smapsize);
        float view_dimX = float(RainSetup.X.D.maxX - RainSetup.X.D.minX) / smapsize;
        float view_dimY = float(RainSetup.X.D.maxX - RainSetup.X.D.minX) / smapsize;
        float view_sx = float(RainSetup.X.D.minX) / smapsize;
        float view_sy = float(RainSetup.X.D.minY) / smapsize;
        Fmatrix m_TexelAdjust = 
		{
			view_dimX / 2.f,							0.0f,										0.0f,							0.0f, 
			0.0f,										-view_dimY / 2.f,							0.0f,							0.0f, 
			0.0f,										0.0f,										fRange,							0.0f, 
			view_dimX / 2.f + view_sx + fTexelOffs,		view_dimY / 2.f + view_sy + fTexelOffs,		fBias,							1.0f
		};

		// compute xforms
		FPU::m64r			();
		Fmatrix				xf_invview;		xf_invview.invert	(Device.mView)	;

		// shadow xform
		Fmatrix				m_shadow;
		{
			Fmatrix			xf_project;		
			xf_project.mul		(m_TexelAdjust,RainSetup.X.D.combine);
			m_shadow.mul	(xf_project,	xf_invview);
			FPU::m24r		();
		}

		// clouds xform
		Fmatrix				m_clouds_shadow;
		{
			static	float	w_shift		= 0;
			Fmatrix			m_xform;
			Fvector			normal	;	normal.setHP(1,0);
			m_xform.identity();
			Fvector			localnormal;m_xform.transform_dir(localnormal,normal); localnormal.normalize();
			m_clouds_shadow.mul			(m_xform,xf_invview)		;
			m_xform.scale				(1.f,1.f,1.f)				;
			m_clouds_shadow.mulA_44		(m_xform)					;
			m_xform.translate			(localnormal.mul(w_shift))	;
			m_clouds_shadow.mulA_44		(m_xform)					;
		}

		// Make jitter texture
		Fvector2					j0,j1;
		float	scale_X				= float(Device.dwWidth)	/ float(TEX_jitter);
		float	offset				= (.5f / float(TEX_jitter));
		j0.set						(offset,offset);
		j1.set						(scale_X,scale_X).add(offset);

		struct v_rain	
		{
			Fvector4	p;
			Fvector2	uv0;
		};

		float	_w					= float(Device.dwWidth);
		float	_h					= float(Device.dwHeight);

		p0.set(.5f/_w, .5f/_h);
		p1.set((_w+.5f)/_w, (_h+.5f)/_h );

		// Fill vertex buffer
		v_rain* pv = (v_rain*) RCache.Vertex.Lock(4, g_rain->vb_stride, Offset);
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

		RCache.Vertex.Unlock(4, g_rain->vb_stride);
		RCache.set_Geometry(g_rain);

		RCache.set_Shader			(s_rain);
		RCache.set_c				("Ldynamic_dir",		L_dir.x,L_dir.y,L_dir.z,0		);
		RCache.set_c				("WorldX",				W_dirX.x,W_dirX.y,W_dirX.z,0		);
		RCache.set_c				("WorldZ",				W_dirZ.x,W_dirZ.y,W_dirZ.z,0		);
		RCache.set_c				("m_rain_shadow",			m_shadow						);
		RCache.set_c				("m_sunmask",			m_clouds_shadow					);
		RCache.set_c				("RainDensity",			fRainFactor, 0, 0, 0			);
		RCache.set_c				("RainFallof",			ps_r3_dyn_wet_surf_near, ps_r3_dyn_wet_surf_far, 0, 0);

		u_setrt	(rt_Position, rt_Normal, rt_Color, rt_SpecularColor, HW.pBaseZB);
		RCache.set_ColorWriteEnable	();
		RCache.set_Stencil(TRUE, D3DCMP_EQUAL, 0x01, 0x01, 0);
		RCache.Render(D3DPT_TRIANGLELIST, Offset, 0, 4, 0, 2);
		RCache.set_ColorWriteEnable	(FALSE);
	}
}
