## C++类继承中，虚函数、纯虚函数和普通函数的区别

### 虚函数

- 父类中实现的虚函数为提供了函数的默认实现；
- 子类可以重写父类的虚函数，实现子类的特殊化。

### 纯虚函数

- C++中包含纯虚函数的类，被称为抽象类。抽象类不能被实例化，只有实现了这个纯虚函数的子类才能被实例化；
- C++中纯虚函数只提供了声明，没有具体的实现，是对子类的约束，是接口继承。

### 普通函数

- 普通函数是静态编译的，没有运行时多态。只会根据指针或引用的类对象，调用自己的普通函数；
- 普通函数是父类为子类提供的强制实现；
- 在继承关系中，子类不应该重写父类的普通函数，因为函数的调用和类对象的名称有关。


### 程序示例

```cpp
#include <iostream>
using namespace std;

class A
{
public:
    virtual void out1() = 0; ///由子类实现
    virtual ~A() {};
    virtual void out2() ///默认实现
    {
        cout << "A(out2)" << endl;
    }
    void out3() ///强制实现
    {
        cout << "A(out3)" << endl;
    }
};

class B: public A
{
public:
    virtual ~B() {};
    void out1()
    {
        cout << "B(out1)" << endl;
    }
    void out2()
    {
        cout << "B(out2)" << endl;
    }
    void out3()
    {
        cout << "B(out3)" << endl;
    }
};

int main()
{
    A *ab = new B;
    ab->out1();
    ab->out2();
    ab->out3();
    cout << "************************" << endl;
    B *bb = new B;
    bb->out1();
    bb->out2();
    bb->out3();

    delete ab;
    delete bb;
    return 0;
}
```

### Reference From

- https://www.cnblogs.com/xudong-bupt/p/3570304.html