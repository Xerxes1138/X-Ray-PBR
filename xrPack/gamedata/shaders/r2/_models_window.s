local tex_base                	= "water\\water_studen"
local tex_nmap                	= "water\\water_studen_normal"

local tex_wnmap                	= "water\\water_soft_normal"
local tex_dist                	= "water\\water_soft_dudv"

local tex_blueNoise             = "engine\\LDR_RGBA_0"

local tex_sky0                	= "$user$sky0"
local tex_sky1                	= "$user$sky1"

local tex_env0                	= "$user$env_s0"   
local tex_env1             		= "$user$env_s1"

local tex_rt_depth				= "$user$depth"
local tex_rt_position			= "$user$position"
local tex_rt_sceneColor			= "$user$generic0"	

local tex_rt_ssao				= "$user$AO_Temporal"

function normal		(shader, t_base, t_second, t_detail)
shader:begin                	("forward_glass", "forward_glass")
	: sorting        			(2, true)
	: blend                		(true, blend.srcalpha, blend.invsrcalpha)
	: zb        				(true, false)
	: distort        			(false)
	: fog                		(false)
	shader:sampler				("s_base")      	:texture(t_base)
	shader:sampler        		("s_nmap")       	:texture(tex_nmap)
	shader:sampler        		("s_wnmap")       	:texture(tex_wnmap)
	
	shader:sampler        		("s_specular_s0")       	:texture(tex_sky0) 				: clamp()
	shader:sampler        		("s_specular_s1")       	:texture(tex_sky1) 				: clamp()
	  
	shader:sampler        		("s_diffuse_s0")       	:texture(tex_env0) 				: clamp()
	shader:sampler        		("s_diffuse_s1")       	:texture(tex_env1) 				: clamp()
	  
	shader:sampler       		("s_depth") 		:texture(tex_rt_depth) 			: clamp()
	shader:sampler       		("s_position") 		:texture(tex_rt_position) 		: clamp()
	shader:sampler        		("s_sceneColor") 	:texture(tex_rt_sceneColor) 	: clamp()
	
	shader:sampler        		("s_AO") 	:texture(tex_rt_ssao) 	: clamp()
		
	shader:sampler				("s_blueNoise")     :texture(tex_blueNoise) 		:f_none()
end
