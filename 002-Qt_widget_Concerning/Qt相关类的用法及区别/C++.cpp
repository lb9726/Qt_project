#include <cstdio>  
  
class CBase  
{  
public:  
    virtual void A(int ival){printf(" CBase::A() ");}  
    void B(int ival){printf(" CBase::B() ");}  
    void C(int ival){printf(" CBase::C() ");}  
};  
  
class CDerive : public CBase  
{  
public:  
    virtual void A(int ival)    //覆盖父类的A：函数名、参数完全相同，且父类一定要有virtual  
    {printf(" CDerive::A() ");}      
  
    void B(int ival)            //隐藏父类的B：函数名、参数完全相同，且父类无virtual  
    {printf(" CDerive::B() ");}      
  
    void C(float ival)          //隐藏父类的C：函数名相同、与数不同，与virtual无关  
    {printf(" CDerive::C() ");}      
  
};  
  
int main()  
{  
    CDerive d;  
    CBase* pB = &d;  
      
    d.A(1); pB->A(1);printf("\n");   //均调用子类的A  
    d.B(1); pB->B(1);printf("\n");   //分别调用各自的B  
    d.C(1); pB->C(1);printf("\n");   //分别调用各自的C  
  
    return 0;  
} 
