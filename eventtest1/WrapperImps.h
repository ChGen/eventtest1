#ifndef eventtest1_WrapperImps_h
#define eventtest1_WrapperImps_h

#include "ObjcWrapperMaps.h"

//same template args as in ObjcWrapper.h

template<typename Tret, typename ... Targs>
struct Impl
{
    static Tret impl_bridge(id obj,SEL sel, Targs ... args)
    {
        return getImplFunc<Tret, Targs...>(obj, sel)->fire(args...);
    }
};

template<typename ... Targs>
struct Impl<void,Targs...>
{
    static void impl_bridge(id obj,SEL sel, Targs ... args)
    {
        getImplFunc<void, Targs...>(obj, sel)->fire(args...);
    }
};

#endif
