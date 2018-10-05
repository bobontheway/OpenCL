## explicit的作用

### 示例一，显示构造函数
C++ 为类(Class)提供了许多默认函数。如果自己没有申明，编译器会为我们提供一个copy构造函数、一个copy assignment操作符和一个析构函数。此外，如果没有申明任何构造函数，编译器会为我们申明一个default构造函数。很像下面的Empty类：
```cpp
class Empty{
    public:
        Empty();
        Empty(const Empty &rhs);
        ~Empty();
        Empty& operator=(const Empty &rhs);
};
```
就像Effective C++说的，如果不想使用编译器自动生成的函数，就应该明确拒绝。

#### 1.隐式构造函数
如果c++类的其中一个构造函数有一个参数，那么在编译的时候就会有一个缺省的转换操作：将该构造函数（参数）对应数据类型的数据转换为该类对象。

#### 2.显式构造函数（Explicit Constructors）
为了避免上面提到的只有一个参数的构造函数采用的缺省转换操作，在构造函数前，使用Explicit 关键字修饰即可。

```cpp
#include <iostream>
using namespace std;
class B{
    public:
        int data;
        B(int _data):data(_data){}
        //explicit B(int _data):data(_data){}
};

int main(){
    B temp=5;
    cout<<temp.data<<endl;
    return 0;
}
```

程序第11行，将int转换为B类型的对象，就是使用了隐式构造函数。因为B中存在一个只有一个参数的构造函数，且参数类型也是int。
如果在构造函数前加explicit ，表示为显示构造函数，则第11行编译不能通过。因为这时，就没有隐式构造函数啦。

### 示例二，explicit的作用
```cpp
#include <iostream>
using namespace std;

class A
{
public:
    A(int i = 5)
    {
        m_a = i;
     }
private:
    int m_a;
};

int main()
{
    A s;
    //我们会发现,我们没有重载'='运算符,但是去可以把内置的int类型赋值给了对象A.
    s = 10;
    //实际上,10被隐式转换成了下面的形式,所以才能这样.
    //s = A temp(10);

    system("pause");
    return 0;
}
```
> 我们发现成员变量的值被修改了。

由此可知：当类构造函数的参数只有一个的时候,或者所有参数都有默认值的情况下,类A的对象时可以直接被对应的内置类型隐式转换后去赋值的,这样会造成错误,所以接下来会体现出explicit这个关键词的作用。

```cpp
#include <iostream>
using namespace std;

class A
{
public:
    //这里用explicit关键词来修饰类构造函数.
    explicit A(int i = 5, int j = 10)
    {
        m_a = i;
        m_b = j;
    }
private:
    int m_a;
    int m_b;
};

int main()
{
    A s;
    //这样直接赋值,会被提示错误,因为explicit抑制隐式转换的进行
    s = 10;//这样会报错!!!
    //当然显示转换还是可以的.
    s = A(20);

    system("pause");
    return 0;
}
```
通过两个例子我们知道:explicit可以抑制内置类型隐式转换,所以在类的构造函数中,最好尽可能多用explicit关键字,防止不必要的隐式转换。

### Reference From

- https://www.cnblogs.com/xudong-bupt/p/3671972.html
- https://blog.csdn.net/qq_37233607/article/details/79051075
