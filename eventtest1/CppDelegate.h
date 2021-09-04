#include "DelegateImpl.h"
//#include "BaseTypes.h"

@interface CppDelegate : NSObject
{
     Func<void, id> *_ev;
}

- (id)initWithEvent:(Func<void, id>*) ev;
- (void) invoke:(id)sender;
- (void) setHandler:(Func<void, id> *)ev;
@end

template <typename Targ,typename T> //Both args could be automatically inferred
CppDelegate* _newCppDelEvh(T *obj,void (T::*handler)(Targ)) {
    return [[CppDelegate alloc] initWithEvent: new DelegateImpl<void (T::*)(Targ)>(obj,handler)];
}
