#include "stdafx.h"

#include "..\igame_persistent.h"
#include "..\environment.h"

void CRenderTarget::phase_motionVector ()
{
	// Previous matrix
	Fmatrix		m_previous, m_current;

	static Fmatrix		m_saved_viewproj;
		
	Fmatrix	m_invview;
	m_invview.invert(Device.mView);

	m_previous.mul(m_saved_viewproj, m_invview);

	m_current.set(Device.mProjectUnJittered);

	m_saved_viewproj.mul(Device.mProjectUnJittered, Device.mView);

	if(1)
	{
		CEnvDescriptorMixer& environment = g_pGamePersistent->Environment().CurrentEnv;

		u32			Offset = 0;
		Fvector2	p0,p1;

		struct v_motionVector
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
		v_motionVector* pv = (v_motionVector*) RCache.Vertex.Lock(4, g_motionVector->vb_stride, Offset);
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
		
		RCache.Vertex.Unlock(4, g_motionVector->vb_stride);

		RCache.set_Shader	(s_motionVector);

		Fmatrix	ViewMatrix; ViewMatrix.set(Device.mView);
		Fmatrix	ProjectioMatrix; ProjectioMatrix.set(Device.mProjectUnJittered);
		Fmatrix	InverseProjectioMatrix; InverseProjectioMatrix.invert(Device.mProjectUnJittered);

		Fmatrix	ViewProjectionMatrix;
		ViewProjectionMatrix.mul(ViewMatrix, ProjectioMatrix);

		Fmatrix	InverseViewProjectionMatrix;
		InverseViewProjectionMatrix.invert(ViewProjectionMatrix);

		Fmatrix	CameraToWorldMatrix; CameraToWorldMatrix.invert(Device.mView);

		// TODO : Move this to standard binding
		RCache.set_c("dx_matrix_Projection", ProjectioMatrix);
		RCache.set_c("dx_matrix_PreviousProjection", m_previous);
		RCache.set_c("dx_matrix_CameraToWorld", CameraToWorldMatrix);
		RCache.set_c("dx_matrix_InverseProjection", InverseProjectioMatrix);
		RCache.set_c("dx_matrix_ViewProjection", ViewProjectionMatrix);
		RCache.set_c("dx_matrix_InverseViewProjection",	InverseViewProjectionMatrix);

		u_setrt	(rt_motionVector, NULL, NULL, NULL, HW.pBaseZB);

		RCache.set_CullMode( CULL_NONE );
		RCache.set_Stencil(FALSE);
		RCache.set_ColorWriteEnable	();
		RCache.set_Geometry			(g_motionVector);
		RCache.Render				(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
		RCache.set_ColorWriteEnable	(FALSE);
	}
}