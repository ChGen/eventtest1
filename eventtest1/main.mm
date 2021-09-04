#include "ObjcWrapper.h"
#include "DelegateImpl.h"
#include "CppDelegate.h"
#include <math.h>
#import <Foundation/Foundation.h>

typedef  int CppClass;

@interface Obj1 : NSObject {
    CppClass* instance;
}
-(id)initWithCppInstance:(CppClass*)ins;
-(NSString*)concatString:(NSString*)s1 withString:(NSString*)s2;
-(int)methodWithoutArg;

-(int)method1:(int)x :(int)y;
-(void)method2:(float)v;

@end

@implementation Obj1
-(id)initWithCppInstance:(CppClass*)ins {
    if(self = [super init])
        instance = ins;
    return self;
}

-(NSString*)concatString:(NSString*)s1 withString:(NSString*)s2 {
    return [s1 stringByAppendingString:s2];
}

-(int)methodWithoutArg
{
    return 4;
}
-(int)method1:(int)x :(int)y
{
    return x+y;
}

-(void)method2:(float)v
{
    NSLog(@"method2: %f",v);
}

@end

float ff(int v)
{
    return sqrtf(v);
}
void gg(int v)
{
    printf("gg()= %f\n",sqrtf(v));
}

struct CC1
{
    void f(int a)
    {
        printf("from CC1::f()) %d\n",a);
    }
};

int f(int x,int y)
{
    printf("from f())");
    return x-y;
}
int fint()
{
    printf("from f())");
    return 1;
}

class Method2
{
public:
    void operator()(float v)
    {
        printf("NEW method2: %f\n",v+1);
    }
};

void ff3(id o)
{
    NSLog(@"obj: %p\n",o);
}

int main(int argc, const char * argv[])
{
    {
        ObjcWrapper w("MyObj11", "Obj1");
    }
    
    delete new ObjcWrapper("MyObj11", "Obj1");
    
    if(1)
    {
        // 2 variants
        //ObjcWrapper w("MyObj1", "Obj1");
        ObjcWrapper w("MyObj1", [Obj1 class]);
        
        // 2 variants
        //w.add("methodWithoutArg",fint);
        w.add("methodWithoutArg",(int (*)())[]
        {
            printf("from int lambda()\n");
            return 0;
        });
        
        w.add(@selector(method1::),f);
        //w.add<void,float>(@selector(method2:),Method2()); //as functor
        Method2 m2;
        w.add(@selector(method2:),&m2,&Method2::operator()); //as object's member
        id obj = w.instance();
        
        int var = [obj methodWithoutArg];
        
        NSLog(@"var=%d\n",var);
        NSLog(@"r=%d\n",[obj method1:5 :7]);
        NSLog(@"r=%d\n",[obj method1:5 :7]);
        [obj method2:3.14159265];
        
        NSLog(@"r=%d\n",[obj method1:5 :7]);

    }
    Func<void,id> *fn = _newfn<void,id>(
                                        [](id obj)
                                        {
                                            NSLog(@"Target-action Lambda handler test!: %p\n",obj);
                                        });
    
    CppDelegate *del = [[CppDelegate alloc] initWithEvent:fn];
    
    [del invoke:nil];
    [del release];
    
    return 0;
}
