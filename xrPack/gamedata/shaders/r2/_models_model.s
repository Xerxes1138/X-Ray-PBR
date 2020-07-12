function normal		(shader, t_base, t_second, t_detail)
	shader:begin  	("deffer_model_bump","deffer_base_bump")
		: zb        (false,false)
	shader:sampler	("s_base")      :texture  (t_base)
end
