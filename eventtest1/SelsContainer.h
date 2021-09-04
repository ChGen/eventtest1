#ifndef eventtest1_SelsHolder_h
#define eventtest1_SelsHolder_h

#include <objc/objc.h>
#include <map>

struct SelectorData
{
    SelectorData():handler(0),handlerDeleter(0){}
    void *handler;// Func<>* instance
    void (*handlerDeleter)(void *handler);
    
    IMP oldImp;
    //could be extra data...
};

typedef std::map<SEL, SelectorData> SelsMap;

class SelsContainer
{
public:
    SelsMap sels;
    
public:
    static void setContainer(id obj, const SelsContainer *container);
    static SelsContainer* getContainer(id obj);
    static void deleteContainer(id obj, SEL sel);
};

#endif
