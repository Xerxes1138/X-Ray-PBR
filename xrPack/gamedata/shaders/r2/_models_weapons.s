function normal		(shader, t_base, t_second, t_detail)
	shader:begin  	("deffer_model_bump","deffer_model_bump")
		: zb        (true, true)
	shader:sampler	("s_base")      :texture  (t_base)
end
function shadows		(shader, t_base, t_second, t_detail)
	shader:begin  	("shadow_direct_model","shadow_direct_base")
		: zb        (true, true)
	shader:sampler	("s_base")      :texture  (t_base)
end