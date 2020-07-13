local tex_base                	= "fx\\fx_rain"
local tex_nmap                	= "fx\\fx_rain_normal"

local tex_sky0                	= "$user$sky0"
local tex_sky1                	= "$user$sky1"

local tex_env0                	= "$user$env_s0"   
local tex_env1             		= "$user$env_s1"

local tex_rt_position			= "$user$position"
local tex_rt_sceneColor			= "$user$generic0"

function normal                	(shader, t_base, t_second, t_detail)

shader:begin                	("rain", "rain")
	: sorting        			(2, false)
	: blend                		(true, blend.srcalpha, blend.one)
	: zb        				(false, false)
	: distort        			(false)
	: fog                		(false)
	shader:sampler        		("s_base")       	:texture(tex_base)				:fmip_linear ()
	shader:sampler        		("s_nmap")       	:texture(tex_nmap)

	shader:sampler        		("sky_s0")       	:texture(tex_sky0) 				: clamp()
	shader:sampler        		("sky_s1")       	:texture(tex_sky1) 				: clamp()
	  
	shader:sampler        		("env_s0")       	:texture(tex_env0) 				: clamp()
	shader:sampler        		("env_s1")       	:texture(tex_env1) 				: clamp()
	  
	shader:sampler       		("s_position") 		:texture(tex_rt_position) 		: clamp()
	shader:sampler        		("s_sceneColor") 	:texture(tex_rt_sceneColor) 	: clamp()
end