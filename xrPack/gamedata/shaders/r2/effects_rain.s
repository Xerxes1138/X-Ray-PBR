//local tex_base                	= "fx\\fx_rain"
local tex_nmap                	= "fx\\fx_rain_bump"

local tex_blueNoise             = "engine\\LDR_RGBA_0"

local tex_sky0                	= "$user$sky0"
local tex_sky1                	= "$user$sky1"

local tex_env0                	= "$user$env_s0"   
local tex_env1             		= "$user$env_s1"

local tex_rt_depth				= "$user$depth"
local tex_rt_position			= "$user$position"
local tex_rt_sceneColor			= "$user$ssgi_combine"

function normal		(shader, t_base, t_second, t_detail)
shader:begin                	("rain", "rain")
	: sorting        			(2, true)
	: blend                		(true, blend.srcalpha, blend.invsrcalpha)
	: zb        				(true, false)
	: distort        			(false)
	: fog                		(false)
	shader:sampler				("s_base")      	:texture(t_base)
	//shader:sampler        		("s_bump")       	:texture(tex_nmap)
	
	shader:sampler	("s_bump")      	:texture	(t_base.."_bump")
	//shader:sampler	("s_bumpX")     	:texture	(t_base.."_bump#")
	
	shader:sampler        		("s_specular_s0")       	:texture(tex_sky0) 				: clamp()
	shader:sampler        		("s_specular_s1")       	:texture(tex_sky1) 				: clamp()
	  
	shader:sampler        		("s_diffuse_s0")       	:texture(tex_env0) 				: clamp()
	shader:sampler        		("s_diffuse_s1")       	:texture(tex_env1) 				: clamp()
	  
	shader:sampler       		("s_depth") 		:texture(tex_rt_depth) 			: clamp()
	shader:sampler       		("s_position") 		:texture(tex_rt_position) 		: clamp()
	shader:sampler        		("s_sceneColor") 	:texture(tex_rt_sceneColor) 	: clamp()
	
	shader:sampler				("s_blueNoise")     :texture(tex_blueNoise) 		:f_none()
end