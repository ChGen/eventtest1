#include "ObjcWrapper.h"
#include "SelsContainer.h"
#include <vector>
#include <string>
#include <map>
#include <objc/runtime.h>
#include <assert.h>
#include <objc/objc-api.h>
#include <objc/objc-class.h>
#include <objc/objc-runtime.h>

struct objc_method_list //NOT AVAIL. IN OBJC 2.0?
{
    struct objc_method_list *obsolete;
    int method_count;
    Method* method_list[1];
};

using namespace std;

Class ObjcWrapper::createClass(const char *name, const char *superClass)
{
    id sup = objc_getClass(superClass);
    assert(sup);
    if(!sup) 
        return nil;
    
    Class cls = objc_allocateClassPair(sup, name, sizeof(void*));
    
    if(!cls)
    {
        NSLog(@"Warning: Class %s already registered.\n",name);
        cls = objc_getClass(name);
    }
    
    assert(cls);
    
    objc_registerClassPair(cls);        
    return cls;
}

id ObjcWrapper::createInstance(Class cls)
{
    id obj = class_createInstance(cls, sizeof(void*));
    assert(obj);        
    return obj;
}

void ObjcWrapper::remove(SEL sel)
{
    assert(!"not avalable!");
    SelsContainer *c = SelsContainer::getContainer(_objCobj);
    assert(c);
    SelsMap::iterator it = c->sels.find(sel);
    if(it == c->sels.end())
        return;
    if(it->second.handlerDeleter)
        it->second.handlerDeleter(it->second.handler);
    IMP oldImp = it->second.oldImp;
    if(oldImp)
    {
        method_setImplementation(class_getInstanceMethod(_class, sel), oldImp);
    }
    else 
    {
        objc_method_list list;
        list.obsolete = 0;
        list.method_count=1;
        Method m = class_getInstanceMethod(_class, sel);
        list.method_list[0] = &m;
        //class_removeMethods(_class, &list); //NOT AVAIL?
    }
    c->sels.erase(it);
}

bool ObjcWrapper::regImplFunc(SEL sel,void *handler,IMP imp,void (*handlerDeleter)(void *handler))
{
    IMP oldImp=nil;
    const char *typeEncoding="";
    bool b = class_addMethod(_class, sel, imp, typeEncoding);
    if(!b) {    
        Method m = class_getInstanceMethod(_class, sel);
        if(!m)
        {
            //failed to get method...
            return false;
        }
        oldImp = method_getImplementation(m);
        method_setImplementation(m, imp);
    }

    //register associated objects
    SelectorData data;
    data.handler = handler;
    data.handlerDeleter = handlerDeleter;
    data.oldImp = oldImp;
    SelsContainer *c = SelsContainer::getContainer(_objCobj);
    SelsMap::iterator it;
    if((it = c->sels.find(sel)) != c->sels.end())
    {
        //delete old handler
        if(it->second.handlerDeleter)
            it->second.handlerDeleter(it->second.handler);
    }
    c->sels[sel]=data;
    
    return true;
}

ObjcWrapper::ObjcWrapper(const char *className,const char *baseClass)
{
    init(className, baseClass);
}

ObjcWrapper::ObjcWrapper(const char *className, Class baseClass)
{
    init(className, class_getName(baseClass));
}

void ObjcWrapper::init(const char *className,const char *baseClass)
{
    _objCobj = nil;
    _class = nil;
    _class = createClass(className, baseClass);
    _objCobj = createInstance(_class);
    SelsContainer *c=new SelsContainer;
    SelsContainer::setContainer(_objCobj, c);
}

ObjcWrapper::~ObjcWrapper()
{
    SelsContainer *c = SelsContainer::getContainer(_objCobj);
    for(SelsMap::iterator it = c->sels.begin();it != c->sels.end(); ++it)
    {
        SelectorData &data=it->second;
        void (*hd)(void *handler) = data.handlerDeleter;
        void *handler=data.handler;
        if(hd)
            hd(handler);
    }
    delete c;
    [_objCobj release];
    [_class release];
}
