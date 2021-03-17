function normal		(shader, t_base, t_second, t_detail)
	shader:begin	("null","postprocess")
		: fog	(false)
		: zb 	(false,false)

		shader:sampler	("s_base0")		:texture("$user$generic0")	: clamp() : f_linear ()
		shader:sampler	("s_base1")    	:texture("$user$generic0"	)	: clamp() : f_linear ()
		shader:sampler	("s_noise")    	:texture("fx\\fx_noise2")	: f_linear ()
end
