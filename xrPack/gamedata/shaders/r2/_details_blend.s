function normal		(shader, t_base, t_second, t_detail)
	shader:begin	("deffer_detail_w_aref_flat","deffer_base_aref_flat")
			: fog		(false)
			: blend		(false,blend.srcalpha,blend.one)
			: aref 		(false,2)
			: sorting	(1, false)
			: zb 		(true,true)
			
	shader:sampler	("s_base")      	:texture	(t_base)
	shader:sampler	("s_bump")      	:texture	(t_base.."_bump")
	shader:sampler	("s_bumpX")     	:texture	(t_base.."_bump#")
end
