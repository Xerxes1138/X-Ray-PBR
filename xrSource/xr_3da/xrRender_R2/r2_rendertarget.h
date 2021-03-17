#pragma once

class light;

#define DU_SPHERE_NUMVERTEX 92
#define DU_SPHERE_NUMFACES	180
#define DU_CONE_NUMVERTEX	18
#define DU_CONE_NUMFACES	32

class CRenderTarget		: public IRender_Target
{
private:
	u32							dwWidth;
	u32							dwHeight;
	u32							dwAccumulatorClearMark;
public:
	u32							dwLightMarkerID;
	// 
	IBlender*					b_occq;
	IBlender*					b_accum_mask;
	IBlender*					b_accum_direct;
	IBlender*					b_accum_point;
	IBlender*					b_accum_spot;
	IBlender*					b_accum_reflected;
	IBlender*					b_bloom;
	IBlender*					b_luminance;
	IBlender*					b_combine;
	IBlender*					b_blit;


#ifdef DEBUG
	struct		dbg_line_t		{
		Fvector	P0,P1;
		u32		color;
	};
	xr_vector<std::pair<Fsphere,Fcolor> >		dbg_spheres;
	xr_vector<dbg_line_t>						dbg_lines;
	xr_vector<Fplane>							dbg_planes;
#endif

	// MRT-path

	ref_rt						rt_Position;
	ref_rt						rt_Normal;
	ref_rt						rt_Color;
	ref_rt						rt_SpecularColor;

	ref_rt						rt_AO;
	ref_rt						rt_AO_Resolve;
	ref_rt						rt_AO_Temporal;

	// 
	ref_rt						rt_Accumulator;
	ref_rt						rt_AccumulatorSpecular;

	ref_rt						rt_Accumulator_temp;

	ref_rt						rt_SSR_Raycast;

	ref_rt						rt_SSGI_Raycast;
	ref_rt						rt_SSGI_Resolve;
	ref_rt						rt_SSGI_Combine;
	ref_rt						rt_SSGI;
	ref_rt						rt_SSGI_Previous;

	ref_rt						rt_currentColor;
	ref_rt						rt_previousColor;

	ref_rt						rt_motionVector;

	//ref_rt						rt_ssr_reflection;
	ref_rt						rt_ssr_resolve;
	ref_rt						rt_ssr_reflection_previous;

	ref_rt						rt_distortion;

	ref_rt						rt_Generic_0;		// 32bit		(r,g,b,a)				// post-process, intermidiate results, etc.
	ref_rt						rt_Generic_1;		// 32bit		(r,g,b,a)				// post-process, intermidiate results, etc.
	ref_rt						rt_Bloom_1;			// 32bit, dim/4	(r,g,b,?)
	ref_rt						rt_Bloom_2;			// 32bit, dim/4	(r,g,b,?)
	ref_rt						rt_LUM_64;			// 64bit, 64x64,	log-average in all components
	ref_rt						rt_LUM_8;			// 64bit, 8x8,		log-average in all components

	ref_rt						rt_LUM_pool	[4]	;	// 1xfp32,1x1,		exp-result -> scaler
	ref_texture					t_LUM_src		;	// source
	ref_texture					t_LUM_dest		;	// destination & usage for current frame

	// env
	ref_texture					t_envmap_0		;	// env-0
	ref_texture					t_envmap_1		;	// env-1

	// smap
	ref_rt						rt_smap_surf;	// 32bit,		color
	ref_rt						rt_smap_depth;	// 24(32) bit,	depth 
	IDirect3DSurface9*			rt_smap_ZB;		//

	// Textures
	IDirect3DVolumeTexture9*	t_material_surf;
	ref_texture					t_material;

	IDirect3DTexture9*			t_noise_surf	[TEX_jitter_count];
	ref_texture					t_noise			[TEX_jitter_count];
private:
	// OCCq
	ref_shader					s_occq;

	// Accum
	ref_shader					s_accum_hemi	;
	ref_shader					s_accum_mask	;
	ref_shader					s_accum_direct	;
	ref_shader					s_accum_point	;
	ref_shader					s_accum_spot	;
	ref_shader					s_accum_reflected;

	// AO
	IBlender*					b_postprocess_AO;
	ref_shader					s_postprocess_AO;
	ref_geom					g_postprocess_AO;

	IBlender*					b_postprocess_AO_resolve;
	ref_shader					s_postprocess_AO_resolve;
	ref_geom					g_postprocess_AO_resolve;

	IBlender*					b_postprocess_AO_temporal;
	ref_shader					s_postprocess_AO_temporal;
	ref_geom					g_postprocess_AO_temporal;
	//

	// Accum Ambient
	IBlender*					b_accum_ambient;
	ref_shader					s_accum_ambient;
	ref_geom					g_accum_ambient;
	//

	// Motion Vector
	IBlender*					b_motionVector;
	ref_shader					s_motionVector;
	ref_geom					g_motionVector;

	// Post Process
	IBlender*					b_postprocess_ssgi;
	ref_shader					s_postprocess_ssgi;
	ref_geom					g_postprocess_ssgi;

	IBlender*					b_postprocess_ssgi_resolve;
	ref_shader					s_postprocess_ssgi_resolve;
	ref_geom					g_postprocess_ssgi_resolve;

	IBlender*					b_postprocess_ssgi_temporal;
	ref_shader					s_postprocess_ssgi_temporal;
	ref_geom					g_postprocess_ssgi_temporal;

	IBlender*					b_postprocess_ssr;
	ref_shader					s_postprocess_ssr;
	ref_geom					g_postprocess_ssr;

	IBlender*					b_postprocess_ssr_resolve;
	ref_shader					s_postprocess_ssr_resolve;
	ref_geom					g_postprocess_ssr_resolve;

	IBlender*					b_postprocess_reflection;
	ref_shader					s_postprocess_reflection;
	ref_geom					g_postprocess_reflection;

	IBlender*					b_postprocess_ssr_temporal;
	ref_shader					s_postprocess_ssr_temporal;
	ref_geom					g_postprocess_ssr_temporal;

	IBlender*					b_postprocess_bloom_prepass;
	ref_shader					s_postprocess_bloom_prepass;
	ref_geom					g_postprocess_bloom_prepass;

	IBlender*					b_postprocess_stochastic_bloom;
	ref_shader					s_postprocess_stochastic_bloom;
	ref_geom					g_postprocess_stochastic_bloom;

	IBlender*					b_postprocess_combine;
	ref_shader					s_postprocess_combine;
	ref_geom					g_postprocess_combine;

	IBlender*					b_postprocess_fog_scattering;
	ref_shader					s_postprocess_fog_scattering;
	ref_geom					g_postprocess_fog_scattering;

	IBlender*					b_postprocess_taa;
	ref_shader					s_postprocess_taa;
	ref_geom					g_postprocess_taa;

	IBlender*					b_postprocess_tonemap_exposure;
	ref_shader					s_postprocess_tonemap_exposure;
	ref_geom					g_postprocess_tonemap_exposure;

	IBlender*					b_postprocess_tonemap;
	ref_shader					s_postprocess_tonemap;
	ref_geom					g_postprocess_tonemap;
	//

	ref_geom					g_accum_point	;
	ref_geom					g_accum_spot	;
	ref_geom					g_accum_omnipart;

	IDirect3DVertexBuffer9*		g_accum_point_vb;
	IDirect3DIndexBuffer9*		g_accum_point_ib;

	IDirect3DVertexBuffer9*		g_accum_omnip_vb;
	IDirect3DIndexBuffer9*		g_accum_omnip_ib;

	IDirect3DVertexBuffer9*		g_accum_spot_vb	;
	IDirect3DIndexBuffer9*		g_accum_spot_ib	;

	// Bloom
	ref_geom					g_bloom_build;
	ref_geom					g_bloom_filter;
	ref_shader					s_bloom_dbg_1;
	ref_shader					s_bloom_dbg_2;
	ref_shader					s_bloom;
	float						f_bloom_factor;

	// Luminance
	ref_shader					s_luminance;
	float						f_luminance_adapt;

	// Combine
	ref_geom					g_combine;
	ref_geom					g_combine_VP;		// xy=p,zw=tc
	ref_geom					g_combine_2UV;
	ref_geom					g_aa_blur;
	ref_geom					g_aa_AA;
	ref_shader					s_combine_dbg_0;
	ref_shader					s_combine_dbg_1;
	ref_shader					s_combine_dbg_Accumulator;
	ref_shader					s_combine;

	// Rain
	ref_shader					s_rain;
	IBlender*					b_rain;
	ref_geom					g_rain;

	// Blit
	ref_shader					s_blit;
	ref_geom					g_blit;

	IBlender*					b_blit_ssao;
	ref_shader					s_blit_ssao;
	ref_geom					g_blit_ssao;

	IBlender*					b_blit_ssgi;
	ref_shader					s_blit_ssgi;
	ref_geom					g_blit_ssgi;

	IBlender*					b_blit_ssgi_recursive;
	ref_shader					s_blit_ssgi_recursive;
	ref_geom					g_blit_ssgi_recursive;

	IBlender*					b_blit_ssr;
	ref_shader					s_blit_ssr;
	ref_geom					g_blit_ssr;

	IBlender*					b_blit_taa;
	ref_shader					s_blit_taa;
	ref_geom					g_blit_taa;

public:
	ref_shader					s_postprocess;
	ref_geom					g_postprocess;
	ref_shader					s_menu;
	ref_geom					g_menu;

private:
	float						im_noise_time;
	u32							im_noise_shift_w;
	u32							im_noise_shift_h;
	
	float						param_blur;
	float						param_gray;
	float						param_duality_h;
	float						param_duality_v;
	float						param_noise;
	float						param_noise_scale;
	float						param_noise_fps;
	u32							param_color_base;
	u32							param_color_gray;
	u32							param_color_add;
public:
								CRenderTarget			();
								~CRenderTarget			();
	void						accum_point_geom_create	();
	void						accum_point_geom_destroy();
	void						accum_omnip_geom_create	();
	void						accum_omnip_geom_destroy();
	void						accum_spot_geom_create	();
	void						accum_spot_geom_destroy	();

	void						u_stencil_optimize		(BOOL		common_stencil=TRUE);
	void						u_compute_texgen_screen	(Fmatrix&	dest);
	void						u_compute_texgen_jitter	(Fmatrix&	dest);
	void						u_setrt					(const ref_rt& _1, const ref_rt& _2, const ref_rt& _3, const ref_rt& _4, IDirect3DSurface9* zb);

	void						u_setrt					(u32 W, u32 H, IDirect3DSurface9* _1, IDirect3DSurface9* _2, IDirect3DSurface9* _3, IDirect3DSurface9* _4, IDirect3DSurface9* zb);
	void						u_calc_tc_noise			(Fvector2& p0, Fvector2& p1);
	void						u_calc_tc_duality_ss	(Fvector2& r0, Fvector2& r1, Fvector2& l0, Fvector2& l1);
	BOOL						u_need_PP				();
	BOOL						u_DBT_enable			(float zMin, float zMax);
	void						u_DBT_disable			();

	void						phase_scene_prepare		();
	void						phase_scene_begin		();
	void						phase_scene_end			();
	void						phase_occq				();

	void						phase_motionVector		();
	void						phase_ssr_raycast		();
	void						phase_deferred_resolve	();
	void						phase_UI				();
	void						phase_postprocess_AO	();
	void						phase_postprocess_reflection		();
	void						phase_postprocess_combine();
	void						phase_postprocess_fog_scattering();
	void						phase_forward();
	void						phase_rain();
	void						draw_rain(light &RainSetup);
	void						phase_postprocess_TAA	();
	void						phase_wallmarks			();
	void						phase_smap_direct		(light* L,	u32 sub_phase);
	void						phase_smap_direct_tsh	(light* L,	u32 sub_phase);
	void						phase_smap_spot_clear	();
	void						phase_smap_spot			(light* L);
	void						phase_smap_spot_tsh		(light* L);
	void						phase_accumulator		();
	void						phase_accumulator_temp		();
	void						shadow_direct			(light* L, u32 dls_phase);
	
	BOOL						enable_scissor			(light* L);		// true if intersects near plane
	void						enable_dbt_bounds		(light* L);

	void						disable_aniso			();

	void						draw_volume				(light* L);
	void						accum_direct			(u32	sub_phase);
	void						accum_direct_f			(u32	sub_phase);
	void						accum_direct_lum		();
	void						accum_direct_blend		();
	void						accum_point				(light* L);
	void						accum_spot				(light* L);
	void						accum_reflected			(light* L);
	void						accum_raytraced_shadows		();
	void						accum_ambient			();
	void						phase_bloom				();
	void						RenderFaces();
	void						phase_postprocess_tonemap();

	void						phase_copy				(const ref_rt& source, const ref_rt& destination);

	void						phase_luminance			();
	void						phase_combine			();
	void						phase_pp				();

	virtual void				set_blur				(float	f)		{ param_blur=f;						}
	virtual void				set_gray				(float	f)		{ param_gray=f;						}
	virtual void				set_duality_h			(float	f)		{ param_duality_h=_abs(f);			}
	virtual void				set_duality_v			(float	f)		{ param_duality_v=_abs(f);			}
	virtual void				set_noise				(float	f)		{ param_noise=f;					}
	virtual void				set_noise_scale			(float	f)		{ param_noise_scale=f;				}
	virtual void				set_noise_fps			(float	f)		{ param_noise_fps=_abs(f)+EPS_S;	}
	virtual void				set_color_base			(u32	f)		{ param_color_base=f;				}
	virtual void				set_color_gray			(u32	f)		{ param_color_gray=f;				}
	virtual void				set_color_add			(u32	f)		{ param_color_add=f;				}

	virtual u32					get_width				()				{ return dwWidth;					}
	virtual u32					get_height				()				{ return dwHeight;					}

#ifdef DEBUG
	IC void						dbg_addline				(Fvector& P0, Fvector& P1, u32 c)					{
		dbg_lines.push_back		(dbg_line_t());
		dbg_lines.back().P0		= P0;
		dbg_lines.back().P1		= P1;
		dbg_lines.back().color	= c;
	}
	IC void						dbg_addplane			(Fplane& P0,  u32 c)								{
		dbg_planes.push_back(P0);
	}
#else
	IC void						dbg_addline				(Fvector& P0, Fvector& P1, u32 c)					{}
	IC void						dbg_addplane			(Fplane& P0,  u32 c)								{}
#endif
};
