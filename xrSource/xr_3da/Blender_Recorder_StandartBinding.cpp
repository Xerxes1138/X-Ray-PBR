#include "stdafx.h"
#pragma hdrstop

#pragma warning(push)
#pragma warning(disable:4995)
#include <d3dx9.h>
#pragma warning(pop)

#include "ResourceManager.h"
#include "blenders\Blender_Recorder.h"
#include "blenders\Blender.h"

#include "igame_persistent.h"
#include "environment.h"

// matrices
#define	BIND_DECLARE(xf)	\
class cl_xform_##xf	: public R_constant_setup {	virtual void setup (R_constant* C) { RCache.xforms.set_c_##xf (C); } }; \
	static cl_xform_##xf	binder_##xf

/*#define	BIND_DECLARE_PREVIOUS(xf)	\
class cl_xform_previous_##xf	: public R_constant_setup {	virtual void setup (R_constant* C) { RCache.xforms.set_c_previous_##xf (C); } }; \
	static cl_xform_previous_##xf	binder_previous_##xf*/

BIND_DECLARE(w);
BIND_DECLARE(v);
BIND_DECLARE(p);
/*BIND_DECLARE_PREVIOUS(v);
BIND_DECLARE_PREVIOUS(p);*/
BIND_DECLARE(wv);
BIND_DECLARE(vp);
BIND_DECLARE(wvp);

// fog
class cl_fog_plane	: public R_constant_setup {
	u32			marker;
	Fvector4	result;
	virtual void setup(R_constant* C)
	{
		if (marker!=Device.dwFrame)
		{
			// Plane
			Fvector4		plane;
			Fmatrix&	M	= Device.mFullTransform;
			plane.x			= -(M._14 + M._13);
			plane.y			= -(M._24 + M._23);
			plane.z			= -(M._34 + M._33);
			plane.w			= -(M._44 + M._43);
			float denom		= -1.0f / _sqrt(_sqr(plane.x)+_sqr(plane.y)+_sqr(plane.z));
			plane.mul		(denom);

			// Near/Far
			float A			= g_pGamePersistent->Environment().CurrentEnv.fog_near;
			float B			= 1/(g_pGamePersistent->Environment().CurrentEnv.fog_far - A);

			result.set		(-plane.x*B, -plane.y*B, -plane.z*B, 1 - (plane.w-A)*B	);								// view-plane
		}
		RCache.set_c	(C,result);
	}
};
static cl_fog_plane		binder_fog_plane;

// fog-params
class cl_fog_params	: public R_constant_setup {
	u32			marker;
	Fvector4	result;
	virtual void setup(R_constant* C)
	{
		if (marker!=Device.dwFrame)
		{
			// Density
			float	d	= g_pGamePersistent->Environment().CurrentEnv.fog_density;
			// Near/Far
			float	n		= g_pGamePersistent->Environment().CurrentEnv.fog_near	;
			float	f		= g_pGamePersistent->Environment().CurrentEnv.fog_far		;
			float	r		= 1/(f-n);
			float	e		= g_pGamePersistent->Environment().CurrentEnv.fog_exp;

			result.set		(-n*r, e, d, r);
		}
		RCache.set_c	(C,result);
	}
};	static cl_fog_params	binder_fog_params;

// fog-color
class cl_fog_color	: public R_constant_setup {
	u32			marker;
	Fvector4	result;
	virtual void setup	(R_constant* C)	{
		if (marker!=Device.dwFrame)	{
			CEnvDescriptor&	desc	= g_pGamePersistent->Environment().CurrentEnv;
			result.set				(desc.fog_color.x,	desc.fog_color.y, desc.fog_color.z,	0);
		}
		RCache.set_c	(C,result);
	}
};	static cl_fog_color		binder_fog_color;

// times
class cl_times		: public R_constant_setup {
	virtual void setup(R_constant* C)
	{
		float 		t	= Device.fTimeGlobal;
		RCache.set_c	(C,t,t*10,t/10,_sin(t))	;
	}
};
static cl_times		binder_times;

// eye-params
class cl_eye_P		: public R_constant_setup {
	virtual void setup(R_constant* C)
	{
		Fvector&		V	= Device.vCameraPosition;
		RCache.set_c	(C,V.x,V.y,V.z,1);
	}
};
static cl_eye_P		binder_eye_P;

// eye-params
class cl_eye_D		: public R_constant_setup {
	virtual void setup(R_constant* C)
	{
		Fvector&		V	= Device.vCameraDirection;
		RCache.set_c	(C,V.x,V.y,V.z,0);
	}
};
static cl_eye_D		binder_eye_D;

// eye-params
class cl_eye_N		: public R_constant_setup {
	virtual void setup(R_constant* C)
	{
		Fvector&		V	= Device.vCameraTop;
		RCache.set_c	(C,V.x,V.y,V.z,0);
	}
};
static cl_eye_N		binder_eye_N;

// D-Light0
class cl_sun0_color	: public R_constant_setup {
	u32			marker;
	Fvector4	result;
	virtual void setup	(R_constant* C)	{
		if (marker!=Device.dwFrame)	{
			CEnvDescriptor&	desc	= g_pGamePersistent->Environment().CurrentEnv;
			result.set				(desc.sun_color.x,	desc.sun_color.y, desc.sun_color.z,	0);
		}
		RCache.set_c	(C,result);
	}
};	static cl_sun0_color		binder_sun0_color;
class cl_sun0_dir_w	: public R_constant_setup {
	u32			marker;
	Fvector4	result;
	virtual void setup	(R_constant* C)	{
		if (marker!=Device.dwFrame)	{
			CEnvDescriptor&	desc	= g_pGamePersistent->Environment().CurrentEnv;
			result.set				(desc.sun_dir.x,	desc.sun_dir.y, desc.sun_dir.z,	0);
		}
		RCache.set_c	(C,result);
	}
};	static cl_sun0_dir_w		binder_sun0_dir_w;
class cl_sun0_dir_e	: public R_constant_setup {
	u32			marker;
	Fvector4	result;
	virtual void setup	(R_constant* C)	{
		if (marker!=Device.dwFrame)	{
			Fvector D;
			CEnvDescriptor&	desc		= g_pGamePersistent->Environment().CurrentEnv;
			Device.mView.transform_dir	(D,desc.sun_dir);
			D.normalize					();
			result.set					(D.x,D.y,D.z,0);
		}
		RCache.set_c	(C,result);
	}
};	static cl_sun0_dir_e		binder_sun0_dir_e;

//
class cl_amb_color	: public R_constant_setup {
	u32			marker;
	Fvector4	result;
	virtual void setup	(R_constant* C)	{
		if (marker!=Device.dwFrame)	{
			CEnvDescriptorMixer&	desc	= g_pGamePersistent->Environment().CurrentEnv;
			result.set				(desc.ambient.x, desc.ambient.y, desc.ambient.z, desc.weight);
		}
		RCache.set_c	(C,result);
	}
};	static cl_amb_color		binder_amb_color;
class cl_hemi_color	: public R_constant_setup {
	u32			marker;
	Fvector4	result;
	virtual void setup	(R_constant* C)	{
		if (marker!=Device.dwFrame)	{
			CEnvDescriptor&	desc	= g_pGamePersistent->Environment().CurrentEnv;
			result.set				(desc.hemi_color.x, desc.hemi_color.y, desc.hemi_color.z, desc.hemi_color.w);
		}
		RCache.set_c	(C,result);
	}
};	static cl_hemi_color		binder_hemi_color;

// NEW 2019 - IMPORTANT : for consistency, each new shader constants or variables should us the prefix dx_ (stands for directX)
// Screen params
class cl_screen_params		: public R_constant_setup {
	virtual void setup(R_constant* C)
	{
		float		w	= float(Device.dwWidth);
		float		h	= float(Device.dwHeight);
		RCache.set_c(C, w,h,1.0f / w, 1.0f / h);
	}
};
static cl_screen_params		binder_screen_params;

// Rain
class cl_weather_params		: public R_constant_setup {
	virtual void setup(R_constant* C)
	{
		float		rainFactor	= g_pGamePersistent->Environment().CurrentEnv.rain_density;
		RCache.set_c(C, rainFactor, 0.0f, 0.0f, 0.0f);
	}
};
static cl_weather_params		binder_weather_params;

//
static class cl_pos_decompress_params : public R_constant_setup{
    virtual void setup(R_constant* C)
	{
        float VertTan = -1.0f * tanf(deg2rad(Device.fFOV / 2.0f));
        float HorzTan = -VertTan / Device.fASPECT;

        RCache.set_c(
            C, HorzTan, VertTan, (2.0f * HorzTan) / (float)Device.dwWidth, (2.0f * VertTan) / (float)Device.dwHeight);
    }
} 
binder_pos_decompress_params;

static class cl_projection_params : public R_constant_setup{
    virtual void setup(R_constant* C)
	{
		float nearPlane = VIEWPORT_NEAR;
		float farPlane = g_pGamePersistent->Environment().CurrentEnv.far_plane;

		float    h, w, Q;
		h = 1.0f / tan(deg2rad(Device.fFOV) * 0.5f);
		w = Device.fASPECT * h;
		Q = ((farPlane * nearPlane) / (nearPlane - farPlane)) * 2.0f;

		Fvector4 projectionParams = {-w, h, Q, 0.0f};

        RCache.set_c(C, projectionParams);
    }
} 
binder_projection_params;

static class cl_viewport_params : public R_constant_setup{
    virtual void setup(R_constant* C)
	{
		float nearPlane = VIEWPORT_NEAR;
		float farPlane = g_pGamePersistent->Environment().CurrentEnv.far_plane;

		Fvector4 viewportParams = {nearPlane, farPlane, 0.0f, 0.0f};

        RCache.set_c(C, viewportParams);
    }
} 
binder_viewport_params;

static class cl_previous_projection : public R_constant_setup{
    virtual void setup(R_constant* C)
	{
		Fmatrix		m_previous;

		static Fmatrix		m_saved_viewproj;

		Fmatrix	m_invview;
		m_invview.invert(RCache.get_xform_view());

		m_previous.mul(m_saved_viewproj, m_invview);

		m_saved_viewproj.mul(RCache.get_xform_project(), RCache.get_xform_view());

        RCache.set_c(C, m_previous);
    }
} 
binder_previous_projection;

static class cl_current_projection : public R_constant_setup
{
    virtual void setup(R_constant* C)
	{
		Fmatrix m_current;

		m_current.set(Device.mProjectUnJittered);

        RCache.set_c(C, m_current);
    }
} 
binder_current_projection;

static class cl_skyParams : public R_constant_setup
{
    virtual void setup(R_constant* C)
	{
		float skyRotation = g_pGamePersistent->Environment().CurrentEnv.sky_rotation;

		Fvector4 skyParams = {skyRotation, 0.0f, 0.0f, 0.0f};

        RCache.set_c(C, skyParams);
    }
} 
binder_skyParams;

static class cl_previous_World : public R_constant_setup
{
    virtual void setup(R_constant* C)
	{
		Fmatrix	m_previous_W;
		Fmatrix m_current_W;

		static Fmatrix m_saved_m_W;

		m_previous_W.set(m_saved_m_W);

		m_current_W.set(RCache.get_xform_world());

		m_saved_m_W.set(m_current_W);

		RCache.set_c(C, m_previous_W);

		//RCache.xforms.set_c_previous_v(C); 
    }
} 
binder_previous_World;

static class cl_previous_View : public R_constant_setup
{
    virtual void setup(R_constant* C)
	{
		Fmatrix	m_previous_V;
		Fmatrix m_current_V;

		static Fmatrix m_saved_m_V;

		m_previous_V.set(m_saved_m_V);

		//m_current_V.mul_43(RCache.get_xform_view(), RCache.get_xform_world());
		m_current_V.set(RCache.get_xform_view());

		m_saved_m_V.set(m_current_V);

		RCache.set_c(C, m_previous_V);

		//RCache.xforms.set_c_previous_v(C); 
    }
} 
binder_previous_View;

static class cl_previous_Projection : public R_constant_setup
{
    virtual void setup(R_constant* C)
	{
		/*Fmatrix m_previous_P;
		Fmatrix m_current_P;

		static Fmatrix m_saved_m_P;

		m_previous_P.set(m_saved_m_P);

		m_current_P.set(RCache.get_xform_project());

		m_saved_m_P.set(m_current_P);

        RCache.set_c(C, m_previous_P);*/
		//RCache.xforms.set_c_previous_p(C); 

		Fmatrix		m_previous, m_current;

		static Fmatrix		m_saved_viewproj;
		
		Fmatrix	m_invview;
		m_invview.invert(RCache.get_xform_view());

		m_previous.mul(m_saved_viewproj, m_invview);

		m_current.set(RCache.get_xform_project());

		m_saved_viewproj.mul(Device.mProjectUnJittered, Device.mView);
		 RCache.set_c(C, m_previous);
    }
} 
binder_previous_Projection;

// Standart constant-binding
void	CBlender_Compile::SetMapping	()
{
	// matrices
	r_Constant				("m_W",				&binder_w);
	r_Constant				("m_V",				&binder_v);
	r_Constant				("m_P",				&binder_p);

	r_Constant				("m_WV",			&binder_wv);
	r_Constant				("m_VP",			&binder_vp);
	r_Constant				("m_WVP",			&binder_wvp);

	r_Constant				("m_previous_W",	&binder_previous_World);
	r_Constant				("m_previous_V",	&binder_previous_View);
	r_Constant				("m_previous_P",	&binder_previous_Projection);

	// fog-params
	r_Constant				("fog_plane",		&binder_fog_plane);
	r_Constant				("fog_params",		&binder_fog_params);
	r_Constant				("fog_color",		&binder_fog_color);

	// time
	r_Constant				("timers",			&binder_times);

	// eye-params
	r_Constant				("eye_position",	&binder_eye_P);
	r_Constant				("eye_direction",	&binder_eye_D);
	r_Constant				("eye_normal",		&binder_eye_N);

	// global-lighting (env params)
	r_Constant				("L_sun_color",		&binder_sun0_color);
	r_Constant				("L_sun_dir_w",		&binder_sun0_dir_w);
	r_Constant				("L_sun_dir_e",		&binder_sun0_dir_e);
//	r_Constant				("L_lmap_color",	&binder_lm_color);
	r_Constant				("L_hemi_color",	&binder_hemi_color);
	r_Constant				("L_ambient",		&binder_amb_color);

	// detail
	if (bDetail	&& detail_scaler)
		r_Constant			("dt_params",		detail_scaler);

	// other common
	for (u32 it=0; it<Device.Resources->v_constant_setup.size(); it++)
	{
		std::pair<shared_str,R_constant_setup*>	cs	= Device.Resources->v_constant_setup[it];
		r_Constant			(*cs.first,cs.second);
	}

	// NEW 2019
	r_Constant				("dx_ScreenParams", &binder_screen_params);
	r_Constant				("dx_WeatherParams", &binder_weather_params);
	r_Constant				("dx_Pos_Decompression_Params", &binder_pos_decompress_params);
	r_Constant				("dx_Projection_Params", &binder_projection_params);
	r_Constant				("dx_Viewport_Params", &binder_viewport_params);

	r_Constant				("dx_Current_Projection", &binder_current_projection);
	r_Constant				("dx_Previous_Projection", &binder_previous_projection);
	r_Constant				("dx_SkyRotation", &binder_skyParams);
}
