function normal		(shader, t_base, t_second, t_detail)
	shader:begin  	("deffer_base_aref_flat","deffer_base_aref_flat")
		: zb        (true, true)
	shader:sampler	("s_base")      :texture  (t_base)
end