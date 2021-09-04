#ifndef eventtest1_ObjcWrapper_h
#define eventtest1_ObjcWrapper_h

#include "DelegateImpl.h"
#include <objc/objc.h>
#include "WrapperImps.h"

class ObjcWrapper
{
    id _objCobj;
    Class _class;
    
    bool regImplFunc(SEL sel,void *handler,IMP imp, void (*handlerDeleter)(void *handler));
    
    template<typename TH>
    bool regImplFunc(SEL sel,TH *handler,IMP imp)
    {
        void (*handlerDeleter)(void *handler) = &deleter<TH>;
        return regImplFunc(sel, handler, imp, handlerDeleter);
    }
    
    Class createClass(const char *name, const char *superClass);
    id createInstance(Class cls);
    
    template<typename T> 
    static void deleter(void* p)
    {
        delete static_cast<T*>(p);
    }
    void init(const char *className,const char *baseClass);
    
public:
    ObjcWrapper(const char *className,const char *baseClass="NSObject");
    ObjcWrapper(const char *className, Class baseClass = [NSObject class]);

    //class members
    template <typename ... Targs, typename Tobj, typename Tret>
    void add(SEL sel,Tobj *obj, Tret (Tobj::*handler)(Targs...) )
    {
        IMP i=(IMP)&Impl<Tret,Targs...>::impl_bridge;
        regImplFunc(sel, new DelegateImpl<Tret (Tobj::*)(Targs...)>(obj, handler),i);
    }
    template <typename ... Targs, typename Tobj, typename Tret>
    void add(const char *methodSig,Tobj *obj, Tret (Tobj::*handler)(Targs...) )
    {
        add(sel_registerName(methodSig),obj,handler);
    }
    
    //functors & lambdas
    template<typename Tret,typename ... Targs,typename Tobj> 
    void add(SEL sel,Tobj obj)
    {
        IMP i=(IMP)&Impl<Tret,Targs...>::impl_bridge;
        regImplFunc(sel, new DelegateImpl<Tret (Targs...),Tobj>(obj),i);
    }
    template<typename Tret,typename ... Targs,typename Tobj> //functors & lambdas
    void add(const char *methodSig,Tobj obj)
    {
        add(sel_registerName(methodSig),obj);
    }
    
    //regular functions
    template<typename Tret,typename ... Targs> //functions
    void add(SEL sel,Tret (*fn)(Targs...))
    {
        IMP i=(IMP)&Impl<Tret,Targs...>::impl_bridge;
        regImplFunc(sel, new DelegateImpl<Tret (Targs...),Tret (*)(Targs...)>(fn),i);
    }
    template<typename Tret,typename ... Targs> //functions
    void add(const char *methodSig,Tret (*fn)(Targs...))
    {
        add(sel_registerName(methodSig),fn);
    }
    
    //NOT AVAIL. YET!
    void remove(SEL sel);
    void remove(const char *method)
    {
        remove(sel_registerName(method));
    }
    
    id instance() const {return _objCobj;}
    
    ~ObjcWrapper();
    
};

#endif
