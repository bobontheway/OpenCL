## C++学习笔记 ： friend类和friend函数

类的友元函数（friend函数）在类的作用域外定义，却具有访问类的非public （以及public)成员的权限。独立的函数、整个类或其他类的成员函数都可以被声明为另一个类的友元。

### friend 的声明

- 在类定义中函数原型前加保留字friend, 就将该函数声明为该类的友元（友元函数）；
- 若要将整个类Classtwo 的所有成员函数声明为Classone类的友元（友元类），需在Classone定义中加入如下的声明:

```cpp
friend void setX(fri1 &, int);	// 声明友元函数

friend class ClassTwo;	// 声明友元类，ClassTwo的所有成员函数都是Classone的友元函数
```

### 友元函数的例子
```cpp
#include<iostream>
using namespace std;
class fri1
{
    friend void setX(fri1 &, int);
    /*友元函数声明，setX是类fril的友元函数
      友元函数的声明可以出现在类的任何地方
      private、protected、public这些成员访问说明符标志与友元
      的声明无关，因此友元函数的声明可以出现在类的任何地方*/
public:
    fri1():x(0)
    {
    }
    void printt() const
    {
        cout<<"fri1里的 x: "<<x<<endl;
    }
private:
    int x;
};

void setX(fri1 &f, int val)
{//友元函数的定义
    f.x = val;//修改private数据
    //f.printt();//还可以访问成员函数
}

int main()
{
    fri1 friend1;
    cout<<"创建fri1对象friend1, x的值为: ";
    friend1.printt();
    cout<<endl;
    setX(friend1, 8);
    cout<<"使用友元函数setX后，x的值为: ";
    friend1.printt();
    return 0;
}
```

> 注意，若B是A的友元，则A必须显式地声明B是它的友元。另外友元关系既是不对称的也是不传递的。 （类A是类B的友元，类B是类C的友元，不能推断类B是类A的友元，类A是类C的友元 ) 友元不是成员函数。

### 友元类的例子

```cpp
#include<iostream>
using namespace std;
class fri1
{
    friend class fri2;//fri2是fri1的友元类
public:
    fri1():x(0)
    {
    }
    void printt() const
    {
        cout<<"fri1里的 x: "<<x<<endl;
    }
private:
    int x;
};

class fri2
{
public:
    fri2():y(9)
    {
    }
    void print(fri1& f1,int x) const
    {
        cout<<"fri2里的 y: "<<y<<endl;
        f1.x = x;//修改f1中x的值
        f1.printt();//访问f1的成员函数
    }
private:
    int y;
};

int main()
{
    fri1 friend1;
    cout<<"创建fri1对象friend1, x的值为: ";
    friend1.printt();
    cout<<endl;
    fri2 friend2;
    cout<<"使用friend2.print(fri1,10)后： ";
    friend2.print(friend1, 10);
    return 0;
}
```

值得注意的是友元函数会破坏面向对象类的封装性，如果不是一定要使用，则少用或尽量不用。

> 友元类中的所有函数都可以作为该类友元函数。例如，在Class A中，将Class B申明为Class A的友元类，则Class B中的所有函数都可以访问Class A中的成员，这破坏了面向对象的封装性。

### Reference From

- https://blog.csdn.net/cpohui/article/details/79219166

