#pragma once

class CBlender_postprocess_bloom_prepass : public IBlender  
{
public:
	virtual		LPCSTR		getComment()	{ return "INTERNAL: combiner";	}
	virtual		BOOL		canBeDetailed()	{ return FALSE;	}
	virtual		BOOL		canBeLMAPped()	{ return FALSE;	}

	virtual		void		Compile			(CBlender_Compile& C);

	CBlender_postprocess_bloom_prepass();
	virtual ~CBlender_postprocess_bloom_prepass();
};