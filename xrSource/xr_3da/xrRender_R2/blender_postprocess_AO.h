#pragma once

class CBlender_postprocess_AO : public IBlender  
{
public:
	virtual		LPCSTR		getComment()	{ return "INTERNAL: combiner";	}
	virtual		BOOL		canBeDetailed()	{ return FALSE;	}
	virtual		BOOL		canBeLMAPped()	{ return FALSE;	}

	virtual		void		Compile			(CBlender_Compile& C);

	CBlender_postprocess_AO();
	virtual ~CBlender_postprocess_AO();
};

class CBlender_postprocess_AO_resolve : public IBlender  
{
public:
	virtual		LPCSTR		getComment()	{ return "INTERNAL: combiner";	}
	virtual		BOOL		canBeDetailed()	{ return FALSE;	}
	virtual		BOOL		canBeLMAPped()	{ return FALSE;	}

	virtual		void		Compile			(CBlender_Compile& C);

	CBlender_postprocess_AO_resolve();
	virtual ~CBlender_postprocess_AO_resolve();
};

class CBlender_postprocess_AO_temporal : public IBlender  
{
public:
	virtual		LPCSTR		getComment()	{ return "INTERNAL: combiner";	}
	virtual		BOOL		canBeDetailed()	{ return FALSE;	}
	virtual		BOOL		canBeLMAPped()	{ return FALSE;	}

	virtual		void		Compile			(CBlender_Compile& C);

	CBlender_postprocess_AO_temporal();
	virtual ~CBlender_postprocess_AO_temporal();
};
