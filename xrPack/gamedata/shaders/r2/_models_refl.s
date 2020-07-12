function normal		(shader, t_base, t_second, t_detail)
	shader:begin  	("deffer_simple","deffer_simple")
		: zb        (true, true)
	shader:sampler	("s_base")      :texture  (t_base)
end
