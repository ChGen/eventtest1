#ifndef eventtest1_DelegateImpl_h
#define eventtest1_DelegateImpl_h

//base and abstract event class
template<typename Tret, typename ... Targs>
class Func
{
public:
    virtual ~Func(void) {}
    Func() {}
    Func(const Func&) {}
    virtual Tret fire(Targs ... args) = 0;
};

template<typename Tobj, typename Tret,typename ... Targs >
class DelegateImplBaseHelper: public Func<Tret,Targs ... >
{
public:
    Tret fire(Targs ... args)
    {
        return (oo->*hh)(args ...);
    }
    typedef Tret (Tobj::*Handler)(Targs ...);
protected:
    Tobj *oo;
    Handler hh;
};

template<typename Tobj, typename ... Targs>
class DelegateImplBaseHelper<Tobj,void , Targs ...>: public Func<void, Targs ... >
{
    
public:
    void fire(Targs ... args)
    {
        (oo->*hh)(args ...);
    }
    typedef void (Tobj::*Handler)(Targs ...);
protected:
    Tobj *oo;
    Handler hh;
};

template<typename ... Args> class DelegateImpl;

//event handler for any member of class: "void <Class>::member(...)"
template<typename Tobj, typename Tret, typename ... Targs>
class DelegateImpl<Tret (Tobj::*)(Targs...)>: public DelegateImplBaseHelper<Tobj, Tret,Targs... >
{
    typedef Tret (Tobj::*Handler)(Targs ...);
    
public:
    DelegateImpl(Tobj *o, Handler h)
    {
        DelegateImplBaseHelper< Tobj, Tret,Targs...>::oo = o;
        DelegateImplBaseHelper<Tobj, Tret,Targs... >::hh = h;
    }
};

//helper classes for void ret. val.
template<typename Tobj, typename Tret,typename ... Targs>
class DelegateImplFBaseHelper: public Func<Tret,Targs...>
{
protected:
    Tobj o;
public:
    DelegateImplFBaseHelper(Tobj o): o(o)
    {
    }
    Tret fire(Targs ... arg)
    {
        return o(arg...);
    }
};

template<typename Tobj,typename ... Targs>
class DelegateImplFBaseHelper<Tobj, void,Targs...>: public Func<void,Targs...>
{
protected:
    Tobj o;
public:
    DelegateImplFBaseHelper(Tobj o): o(o)
    {
    }
    void fire(Targs ... arg)
    {
        o(arg...);
    }
};

template<typename Tobj, typename Tret,typename ... Targs>
class DelegateImpl<Tret (Targs...),Tobj> : public DelegateImplFBaseHelper<Tobj, Tret,Targs...>
{
    
public:
    DelegateImpl(Tobj o): DelegateImplFBaseHelper<Tobj, Tret,Targs...>(o)
    {
    }
};

//helpers

template<typename Tret,typename ... Targs,typename Tobj> //for functors & lambdas
Func<Tret, Targs...>* _newfn(Tobj obj)
{
    return new DelegateImpl<Tret (Targs...),Tobj>(obj);
}

template<typename Tret,typename ... Targs> //for regular functions
Func<Tret, Targs...>* _newfn(Tret (*fn)(Targs...))
{
    return new DelegateImpl<Tret (Targs...),Tret (*)(Targs...)>(fn);
}

template <typename ... Targs, typename Tobj, typename Tret> //for class members
Func<Tret, Targs...>* _newfn(Tobj *obj, Tret (Tobj::*handler)(Targs...))
{
    return new DelegateImpl<Tret (Tobj::*)(Targs...)>(obj, handler);
}


#endif
