#import "CppDelegate.h"

@implementation CppDelegate

- (id)initWithEvent:(Func<void,id>*) ev
{
    self = [super init];
    if (self) {
        _ev=ev;
    }
    return self;
}

- (void)dealloc
{
    delete _ev;
    [super dealloc];
}

- (void) invoke:(id)sender
{
    _ev->fire(sender);
}

- (void) setHandler:(Func<void,id> *)ev
{
    self->_ev=ev;
}

@end
