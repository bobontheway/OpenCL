接口存在的意义：
多个组件之间代码复用

## 1.1接口的定义
接口的声明由访问修饰符、interface 关键字、接口名称、逗号分割的父接口（如果有的话），以及接口体组成。例如：
```java
public interface GroupInterface extends Interface1, Interface2, interface3 {
	// 常量声明
	double E = 2.718282;  // 对数底数

	// 方法签名
	void doSomething(int i, double x);
	int doSomethingElse(String s);
}
```
public 访问修饰符表示接口可以被任意的类访问，和类所在的包无关。如果接口的访问修饰符没有指定 public，那么接口只能被同一包中定义的类访问。

接口可以扩展自其它接口，就像一个类从其它的类扩展一样。然而，类只能从其它的单个类扩展，但接口可以从任意数量的接口扩展。接口声明包括一个逗号分隔的列表，列出了它所扩展的所有接口。

### 1.1.1接口体
接口体是大括号中包含的部分。它可以包含抽象方法、默认方法和静态方法。接口中的抽象方法以分号结尾（不含大括号，因为抽象方法不包含实现）；默认方法定义时使用了 `default` 修饰符，同时静态方法使用了 `static` 关键字。接口中所有的抽象、默认和静态方法隐式使用了 public 访问修饰符，因此可以省略 public 修饰符。

还有，接口中可以包含常量声明。接口中定义的所有常量隐式使用了 public、static 和 final 修饰，这就可以省略这些修饰符符。

> xiaobo：1.默认状态下，接口的访问修饰和接口中成员的访问修饰不一样。接口隐式为 protected，接口中的成员隐式为 public。

> xiaobo：2.类和接口在 Java 中使用声明来描述，变量和方法使用根据情况使用声明或定义。定义的话，新东西，或指定存储和固定的值。


## 1.2接口的实现
若要声明实现了接口的类，在声明类时需要包含一个 implements 从句。类可以实现多个接口，因此 implements 关键字后面是类实现的接口列表，接口之间使用逗号隔开。如果有 extends 从句，implements 从句跟在 extends 从句的后面。

### 1.2.1Relatable 接口示例
下面的接口声明了如何比较对象大小的方法。
```java
public interface Relatable {
	public int isLargerThan(Relatable other);
}
```
若要比较相同对象的大小（无论它们是什么对象），这些对象对应的类应该实现 `Relatable` 接口。

如果要比较对象的`大小`，实现了 Relatable 接口的类可以比较这些的数据。对于字符串，它可以是字符数；对于书籍，它可能是页数；对于平面几何对象，可以选择面积（如下面的 RectanglePlus 类），而体积可用于三维几何对象。这些类都需要实现 isLargerThan() 方法。

### 1.2.2实现 Relatable 接口
下面是 Rectangle 类，实现了 Relatable 接口。
```java
public class RectanglePlus implements Relatable {
	public int width = 0;
	public int height = 0;

	public RectanglePlus(int w, int h) {
		width = w;
		height = h;
	}

	// a method for computing the area of the rectangle
	public int getArea() {
		return width * height;
	}

	// a method required to implement the Relatable interface
	public int isLargerThan(Relatable other) {
		RectanglePlus otherRect = (RectanglePlus)other;  // ①
		if (this.getArea() < otherRect.getArea())
			return -1;
		else if (this.getArea() > otherRect.getArea())
			return 1;
		else
			return 0;
	}

	public static void main(String[] args) {
		Relatable r1 = new RectanglePlus(100, 20);
		Relatable r2 = new RectanglePlus(20, 100);
		if (r1.isLargerThan(r2) == 1) {
			System.out.println("r1 > r2");
		} else if (r1.isLargerThan(r2) == 0) {
			System.out.println("r1 = r2");
		} else {
			System.out.println("r1 < r2");
		}
	}
}
```
由于 RectanglePlus 类实现了 Relatable 接口，任意两个 RectanglePlus 对象的大小都可以比较。

注意，在 Relatable 接口中声明的  方法，包含了一个 Relatable 类型的对象作为参数。上面示例中位置 ① 的代码行将 other 转换为 RectanglePlus 实例，类型转换告诉编译器真正的对象是什么。直接在 other 实例上调用 getArea 方法（例如，other.getArea()）将编译失败，这是因为编译器并不知道 other 实际上是 RectanglePlus 类的实例。

## 1.3将接口当作类型来使用
定义新的接口就意味着定义了新的数据类型。可以在任何使用其它数据类型的位置使用接口名称。如果定义的变量的类型是接口，则赋值给它的任何对象都必须是实现该接口的类的实例。

例如，下面定义了在两个对象中查找较大对象的方法，这些对象来自于对实现了 Relatable 接口的类的实例化。
```java
public Object findLargest(Object object1, Object object2) {
	Relatable obj1 = (Relatable)object1;
	Relatable obj2 = (Relatable)object2;
	if ((obj1).isLargerThan(obj2) > 0)
		return object1;
	else
		return object2;
}
```

通过将 object1 转换为 Relatable 类型，就可以调用 isLargerThan 方法。

如果在各种各样的类中实现 Relatable 接口，则可以将从这些类实例化的对象作为 findLargest() 方法参数进行比较，前提是两个对象都是相同类的实例。同理，它们也可以使用下面的方法来比较：
```java
public Object findSmallest(Object object1, Object object2) {
	Relatable obj1 = (Relatable)object1;
	Relatable obj2 = (Relatable)object2;
	if ((obj1).isLargerThan(obj2) < 0)
		return object1;
	else
		return object2;
}

public boolean isEqual(Object object1, Object object2) {
	Relatable obj1 = (Relatable)object1;
	Relatable obj2 = (Relatable)object2;
	if ((obj1).isLargerThan(obj2) == 0)
		return true;
	else
		return false;
}
```
这些方法对任何 `relatable` 对象都起作用，不管它们的类继承关系如何。当类实现 `Relatable` 接口时，传递给上面函数的对象可以是类本身或超类的类型，也可以是 Relatable 类型。这就是多重继承的好处，它们可以同时包含超类和接口的功能。

## 1.4接口的演进
考虑下面已经开发完成的 DoIt 接口：
```java
public interface DoIt {
	void doSomething(int i, double x);
	int doSomethingElse(String s);
}
```
假设以后要将第三个方法添加到 DoIt 接口中，那么接口将变成下面的样子：
```java
public interface DoIt {
	void doSomething(int i, double x);
	int doSomethingElse(String s);
	boolean didItWork(int i, double x, String s);
}
```
如果按照上面的修改，那么之前实现了 DoIt 接口的类都将不能正常工作。由于没有实现 didItWork 方法，将出现错误，这种做法过于粗暴，对开发人员来说很不友好。

在定义接口时，尽量预测接口的所有功能，并从一开始就声明相关的方法。如果以后需要往接口中添加其它方法，有多个方法供选择。首先，可以创建一个扩展自 DoIt 的 DoItPlus 接口。
```java
public interface DoItPlus extends DoIt {
	boolean didItWork(int i, double x, String s);
}
```
这样，实现 DoIt 接口的类可以选择继续使用旧的接口或者更新到新的接口。或者在 DoIt 接口中将新的方法定义为 `default` 方法，下面的示例程序定义了一个名为 didItWork 的 default 方法。
```java
public interface DoIt {
	void doSomething(int i, double x);
	int doSomethingElse(String s);
	default boolean didItWork(int i, double x, String s) {
		// Method body
	}
}
```
注意，必须为 default 方法提供具体的实现。还可以在现有的接口中定义新的 static 方法。在接口中增加了新的 default 或 static 方法后，那些实现了 DoIt 接口的类，无需修改或重新编译照样能正常工作。
































