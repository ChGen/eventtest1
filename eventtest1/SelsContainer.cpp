#include "SelsContainer.h"
#include <assert.h>

void SelsContainer::setContainer(id obj, const SelsContainer *container)
{
    void* data = object_getIndexedIvars(obj);
    assert(data);
    assert(data != container);
    (*static_cast<void**>(data)) = static_cast<void*>(const_cast<SelsContainer*>(container));
}

SelsContainer* SelsContainer::getContainer(id obj)
{
    void* data = object_getIndexedIvars(obj);
    assert(data);
    return static_cast<SelsContainer*>(*static_cast<void**>(data));
}

void SelsContainer::deleteContainer(id obj, SEL sel)
{
    assert(!"Not implemented.");
}
