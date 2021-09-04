#ifndef eventtest1_ObjcWrapperMaps_h
#define eventtest1_ObjcWrapperMaps_h

#include "DelegateImpl.h"
#include "SelsContainer.h"

template<typename Tret, typename ... Targs>
        Func<Tret,Targs... >* getImplFunc(id obj,SEL sel)
{
    SelsContainer *c = SelsContainer::getContainer(obj);
    SelsMap::iterator it = c->sels.find(sel);
    assert(it != c->sels.end());
    return static_cast<Func<Tret,Targs...>* >(it->second.handler);
}

#endif
