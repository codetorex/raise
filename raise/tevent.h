#ifndef TEVENT_H
#define TEVENT_H

class delegate
{
public:
	virtual void call () = 0;
};

template <class R>
class delegate0
{
public:
	virtual R call () = 0;
};

template <class R, class AT1>
class delegate1
{
public:
	virtual R call (AT1) = 0;
};

template <class R, class AT1,class AT2>
class delegate2
{
public:
	virtual R call (AT1,AT2) = 0;
};

template <class R, class AT1,class AT2,class AT3>
class delegate3
{
public:
	virtual R call (AT1,AT2,AT3) = 0;
};

template <class R >
class funcdelegate0: public delegate0<R>
{
public:
	typedef R (*FNC)();
	FNC fncptr;
	
	funcdelegate0( FNC _ptr)
	{
		fncptr = _ptr;
	}

	R call ()
	{
		return fncptr();
	}
};

template <class R, class AT1 >
class funcdelegate1: public delegate1<R,AT1>
{
public:
	typedef R (*FNC)(AT1);
	FNC fncptr;

	funcdelegate1( FNC _ptr)
	{
		fncptr = _ptr;
	}

	inline R call (AT1 prm1)
	{
		return fncptr(prm1);
	}
};

template <class R, class AT1,class AT2 >
class funcdelegate2: public delegate2<R,AT1,AT2>
{
public:
	typedef R (*FNC)(AT1,AT2);
	FNC fncptr;

	funcdelegate2( FNC _ptr)
	{
		fncptr = _ptr;
	}

	inline R call (AT1 prm1,AT2 prm2)
	{
		return fncptr(prm1,prm2);
	}
};

template <class R, class AT1,class AT2,class AT3 >
class funcdelegate3: public delegate3<R,AT1,AT2,AT3>
{
public:

	typedef R (*FNC)(AT1,AT2,AT3);
	FNC fncptr;

	funcdelegate3( FNC _ptr)
	{
		fncptr = _ptr;
	}

	inline R call (AT1 prm1,AT2 prm2,AT3 prm3)
	{
		return fncptr(prm1,prm2,prm3);
	}
};

template <class T,class R>
class TMemberDelegate0: public delegate0<R>
{
public:
	typedef R (T::*mfunc)();
	TMemberDelegate0(T* obje,mfunc memfn) : mobj(obje), mobjfnc(memfn) {};

	R call()
	{ 
		return ((mobj)->*mobjfnc)();
	}

private:
	T*		mobj;
	mfunc	mobjfnc;
};

template <class T,class R,class AT1>
class TMemberDelegate1: public delegate1<R,AT1>
{
public:
	typedef R (T::*mfunc)(AT1);
	TMemberDelegate1(T* obje,mfunc memfn) : mobj(obje), mobjfnc(memfn) {};

	R call(AT1 prm1)
	{ 
		return ((mobj)->*mobjfnc)(prm1);
	}

private:
	T*		mobj;
	mfunc	mobjfnc;
};

template <class T,class R,class AT1,class AT2>
class TMemberDelegate2: public delegate2<R,AT1,AT2>
{
public:
	typedef R (T::*mfunc)(AT1,AT2);
	TMemberDelegate2(T* obje,mfunc memfn) : mobj(obje), mobjfnc(memfn) {};

	R call(AT1 prm1,AT2 prm2)
	{ 
		return ((mobj)->*mobjfnc)(prm1,prm2);
	}

private:
	T*		mobj;
	mfunc	mobjfnc;
};

template <class T,class R,class AT1,class AT2,class AT3>
class TMemberDelegate3: public delegate3<R,AT1,AT2,AT3>
{
public:
	typedef R (T::*mfunc)(AT1,AT2,AT3);
	TMemberDelegate3(T* obje,mfunc memfn) : mobj(obje), mobjfnc(memfn) {};

	R call(AT1 prm1,AT2 prm2,AT3 prm3)
	{ 
		return ((mobj)->*mobjfnc)(prm1,prm2,prm3);
	}

private:
	T*		mobj;
	mfunc	mobjfnc;
};



/**
 * TODO: make it T instead of T* so data is stored internally, will be fast to access?
 */
template <class T>
class event
{
public:
	T* Handlers[8];
	int HandlerCount;

	event()
	{
		HandlerCount = 0;
	}


	event& operator += (T* handler)
	{
		Handlers[HandlerCount++] = handler;
		return *this;
	}

	event& operator -= (T* handler)
	{
		int i = HandlerCount;
		while(i--)
		{
			if (Handlers[i] == handler)
			{
				Handlers[i] = Handlers[--HandlerCount];
				break;
			}
		}
		return *this;
	}

	void callSeq()
	{
		for (int i=0;i<HandlerCount;i++)
		{
			Handlers[i]-call();
		}
	}

	inline void call()
	{
		int i = HandlerCount;
		while(i--)
		{
			Handlers[i]->call();
		}
	}

	template <class AT1>
	inline void call(AT1 prm1)
	{
		int i = HandlerCount;
		while(i--)
		{
			Handlers[i]->call(prm1);
		}
	}

	template <class AT1,class AT2>
	inline void call(AT1 prm1,AT2 prm2)
	{
		int i = HandlerCount;
		while(i--)
		{
			Handlers[i]->call(prm1,prm2);
		}
	}

	template <class AT1,class AT2,class AT3>
	inline void call(AT1 prm1,AT2 prm2, AT3 prm3)
	{
		int i = HandlerCount;
		while(i--)
		{
			Handlers[i]->call(prm1,prm2,prm3);
		}
	}
};

/**
* Only difference from event is keeps functions sequentially and calls them sequentially

template<class T>
class functionlist
{
public:
	T* Handlers[8];
	int HandlerCount;

	functionlist()
	{
		HandlerCount = 0;
	}


	functionlist& operator += (T* handler)
	{
		Handlers[HandlerCount++] = handler;
		return *this;
	}

	functionlist& operator -= (T* handler)
	{
		int f = 0;
		for (int i=0;i<HandlerCount;i++)
		{
			if (Handlers[i] == handler)
			{
				f = i;
				break;
			}
		}

		

		for (int i=f;i<HandlerCount;i++)
		{
			Handlers[i] = Handlers[i+1];
		}

		HandlerCount--;

		
		return *this;
	}

	void call()
	{
		for (int i=0;i<HandlerCount;i++)
		{
			Handlers[i]-call();
		}
	}
	template <class AT1>
	inline void call(AT1 prm1)
	{
		for (int i=0;i<HandlerCount;i++)
		{
			Handlers[i]-call(prm1);
		}
	}

	template <class AT1,class AT2>
	inline void call(AT1 prm1,AT2 prm2)
	{
		for (int i=0;i<HandlerCount;i++)
		{
			Handlers[i]-call(prm1,prm2);
		}
	}

	template <class AT1,class AT2,class AT3>
	inline void call(AT1 prm1,AT2 prm2, AT3 prm3)
	{
		for (int i=0;i<HandlerCount;i++)
		{
			Handlers[i]-call(prm1,prm2,prm3);
		}
	}
};*/

/*template <class R, R (*FNC)() >
delegate0<R>* GetHandler( FNC funcaddr )
{
	return new funcdelegate0<R, funcaddr>;
}*/

template <class R>
delegate0<R>* GetHandler( R (*fncptr)() )
{
	return new funcdelegate0<R>(fncptr);
}

template <class R,class AT1>
delegate1<R,AT1>* GetHandler( R (*fncptr)(AT1) )
{
	return new funcdelegate1<R,AT1>(fncptr);
}

template <class R,class AT1,class AT2>
delegate2<R,AT1,AT2>* GetHandler( R (*fncptr)(AT1,AT2) )
{
	return new funcdelegate2<R,AT1,AT2>(fncptr);
}

template <class R,class AT1,class AT2,class AT3>
delegate3<R,AT1,AT2,AT3>* GetHandler( R (*fncptr)(AT1,AT2,AT3) )
{
	return new funcdelegate3<R,AT1,AT2,AT3>(fncptr);
}


template <class R,class M>
delegate0<R>* GetHandler( M* obj, R (M::*fncptr)() )
{
	return new TMemberDelegate0<M,R>(obj,fncptr);
}

template <class R,class M,class AT1>
delegate1<R,AT1>* GetHandler( M* obj, R (M::*fncptr)(AT1) )
{
	return new TMemberDelegate1<M,R,AT1>(obj,fncptr);
}

template <class R,class M,class AT1,class AT2>
delegate2<R,AT1,AT2>* GetHandler( M* obj, R (M::*fncptr)(AT1,AT2) )
{
	return new TMemberDelegate2<M,R,AT1,AT2>(obj,fncptr);
}

#endif