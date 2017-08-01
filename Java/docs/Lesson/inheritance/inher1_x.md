## 1.1继承
在前面的接口一节，可以看到`继承`被多次提及到。在 Java 语言中，类可以从其它的类派生，因此可以从这些类中继承字段和方法。

> 定义：从其它类派生的类称为子类，也称为派生类或扩展类。可以派生出子类的类称为超类，也称为基类或父类。除了 Object 没有超类，每个类都有且仅有一个唯一的超类，这称为单继承。在没有显式指定其它任何超类的情况下，每个类隐式都是 Object 的子类。类可以派生自派生于其它类的类，并最终派生自最上层的 Object 类。例如，类 C 派生自类 B，类 B 派生自类 A，类 A 派生自 Object。

继承的思想不仅简单而且强大。当需要创建新的类时，若已有的类已经包含了一些需要的代码，可以从已有的类派生出新类。这就可以重用现有类的字段和方法，而不必全部重新编写，同时也省去了调试步骤。

子类从超类中继承所有的成员，包括字段、方法以及嵌套类。构造函数不是成员，因此子类不会继承，但是超类的构造函数可以在子类中调用。
// xbdong
### 1.1.1Java 平台类层次结构
在 java.lang 包中定义的 Object 类，定义并实现所有类共有的行为——包括自己编写的代码。在 Java 平台中，许多类直接从 Object 派生，其它类从这些类派生，等等，构成类的层次结构。

**Java 平台中的所有类都是 Object 的后代**

<img src="images/classes-object.gif" width="70%" height="70%">

在层次结构的顶部，Object 提供了所有类中通用的行为。层次结构底部附近的类提供了更专业的行为。

### 1.1.2继承的示例
下面是 Bicycle 类中可能实现的示例代码：

public calss Bicycle {
	// the Bicycle class has three fields
	public int cadence; // 韵律，节奏
	public int gear; // 齿轮
	public int speed; // 速度
	...	...
}

声明为 MountainBike 的类，是 Bicycle 的子类，如下所示：
public class MountainBike extends Bicycle {
	// the MountainBike subclass adds one field
	public int seatHeight; // 座椅高度
}

MountainBike 继承了 Bicycle 的所有字段和方法，并添加了 seatHeight 字段和一个设置该字段的方法。除了构造函数，这就像完全从头编写了一个新的 MountainBike 类，其中有四个字段和五个方法。然而，不必做所有的工作。如果 Bicycle 类中的方法是复杂的并需要大量时间来调试，这将是特别有价值的。

### 1.1.3 可以在子类中做些什么
不管子类在哪个包中，子类从父类继承了所有的 public 和 protected 成员。如何子类和父类在同一个包中，它也继承父类的包私有成员（package-private）。可以使用继承的成员，或者替换它们，隐藏它们，抑或是用新成员补充它们。

- 继承的字段可以直接使用，就像其它的字段一样；
- 可以在子类中声明一个与超类中同名的字段，达到隐藏超类字段的目的（不推荐这么做）；
- 可以在子类中声明一个在超类不存在的新的字段；
- 继承的方法可以直接使用；
- 可以在子类中写一个新的 instance 方法，和超类中的具有相同的签名，以 overriding 超类的方法；
- 可以在子类中写一个新的 static 方法，和超类中的具有相同的签名，以 hiding 超类的方法；
- 可以在子类中声明一个在超类中不存在的新的方法；
- 可以在子类中写一个构造函数来调用超类的构造函数，使用隐式方式或通过 super 关键字。

本文以下部分将在这些主题上展开。

### 1.1.4超类中的私有成员
子类并不会继承父类中的私有（private）成员。然而，如果超类包含 public 或 protected 方法来访问它的私有成员，这些私有成员也可以间接的被子类使用。

// 翻译问题？
嵌套的类可以访问包含它的类的所有成员——包括字段和方法。因此，继承了的 public 或
protected 的嵌套类的子类可以间接访问超类的所有私有成员。

### 1.1.5 对象转换（Casting Objects）
我们已经看到，一个对象是类的数据类型，该类是它实例化的类。例如，如下所示：
```java
public MountainBike myBike = new MountainBike();
```
那么 myBike 是 MountainBike 类型。

MountainBike 起源于（来自于） Bicycle 和 Object。因此，MountainBike 是Bicycle，并且也是 Object，它可以用于 Bicycle 或 Object 对象需要的时候。

反过来则不一定是真的：Bicycle 可能是 MountainBike，但不是一定的。类似的一个 Object 可能是 Bicycle 或 MountainBike，但它并不是一定的。

强制转换显示了在继承和和实现所允许的对象中，使用一种类型的对象代替另一种类型。例如，如果我们写：
```java
Object obj = new MountainBike();
```
那么 obj 同时为 Object 和 MountainBike（直到 obj 被安排为另一个不是 MountainBike 的对象），这称作隐式强制转换。

另一方面，我们写：
```java
MountainBike myBike = obj;
```
这会得到一个编译错误，因为编译器不知道 obj 是 MountainBike。然而，通过显式
强制转换，我们可以告诉编译器，我们会安排 MountainBike 给 obj：
```java
MountainBike myBike = (MountainBike)obj;
```
此强制转换将插入一个运行时检查，即 obj 被安排（分配）为 MountainBike，以便编译器可以安全地假定 obj 是 MountainBike。如果在运行时 obj 不是 MountainBike，将抛出一个异常。

> 注意：可以使用 instanceof 操作符，做一个是否为特定对象类型的逻辑测试。由于不正确的强制转换，这可以避免运行时错误。例如：


```java
if (obj instanceof MountainBike) {
	MountainBike myBike = (MountainBike)obj;
}
```

> 这里，instanceof 操作符验证 obj 引用了 Mountainbike，这样我们就可以做强制转换，知道不会引发运行时异常抛出。

## 1.2状态、实现和类型的多继承
类和接口之间的一个显著区别是类可以有字段，而接口不能。此外，可以实例化一个类来创建一个对象，而这是不能用在接口上的。一个对象将它的状态存储在字段中，这写字段在类中定义。Java 编程语言不允许扩展多个类的一个原因是为了避免状态的多重继承问题，这是从多个类继承字段的能力。例如，假设能够定义一个扩展自多个类的新类。通过实例化该类创建对象时，该对象将从该类所有超类中继承字段。要是不同超类的方法或构造函数实例化同一个字段该怎么办？哪个方法或构造函数优先？（问题：这里的同一个字段指哪里的字段？父类还是子类？重名的字段如何处理？）由于接口不包含字段，因此不必担心由多个状态继承导致的问题。

实现的多重继承是从多个类中继承已定义方法的能力。这种类型的多重继承（如命名冲突和歧义）会出现问题。当支持此多重继承编程语言的编译器遇到包含同名方法的超类时，它们有时无法确定到底该访问或调用哪个成员或方法。此外，程序员可能在向超类中添加新方法时不知不觉的引入了命名冲突。default 方法引入了实现多重继承的一种形式。类可以实现多个接口，接口可以包含同名的 default 方法。Java 编译器提供了一些规则来确定哪个 default 方法特定的类会使用。

Java 编程语言支持类型的多重继承，这是类实现多个接口的能力。一个对象可以有多种类型：它自己的类的类型和类所实现的所有接口的类型。这意味着，如果将某个变量声明为接口的类型，那么它的值可以引用任何实现该接口的任何类的实例化对象。这在 Using an Interface as a Type 中有讨论。

与实现的多重继承一样，类可以继承它扩展的接口中定义的方法的不同实现（default 或 static）。在这种情况下，编译器或用户必须决定使用哪一个。

## 1.3重写和隐藏方法（Overriding and Hiding Methods）

### 1.3.1 实例方法（Instance Methods）
子类中实例方法和父类中的实例方法有相同的签名（方法名称，加上参数的类型和数目）和返回类型时，将重写（overrides）超类的方法。

子类重写方法的能力允许类从超类继承，它们的行为“足够接近”，然后根据需要修改行为。重写方法具有相同的名称、参数的数目和类型，以及返回类型作为其重写的方法。重写后的方法还可以返回被重写方法返回类型的子类型。该子类型成为协变（covariant）返回类型。

当重写方法，可能需要使用 @Override 注释来提示编译器要重写超类中的方法。如果由于某种原因，编译器检查到该方法在超类中不存在，它将生成一个错误。有关 @Override 的详细信息，请参阅注释。

### 1.3.2 静态方法（Static Methods）
如果子类定义的静态方法与超类中的静态方法具有相同的签名，那么子类中的方法将隐藏（hides）超类中的方法。

隐藏静态方法和重写实例方法的区别具有重要意义：

- 被调用的重写实例方法为子类中的；
- 被调用的隐藏静态方法取决于该调用是来自于子类还是超类。

考虑包含两个类的示例。第一个是 Animal，它包含一个实例方法和一个静态方法：
```java
public class Animal {
	public static void testClassMethod() {
		System.out.println("The static method in Animal");
	}

	public void testInstanceMethod() {
		System.out.println("The instance method in Animal");
	}
}
```
第二个类，Animal 的子类，叫 Cat：
```java
public class Cat extends Animal {
	public static void testClassMethod() {
		System.out.println("The static method in Cat");
	}

	public void testInstanceMethod() {
		System.out.println("The instance method in Cat");
	}

	public static void main(String[] args) {
		Cat myCat = new Cat();
		Animal myAnimal = myCat;
		Animal.testClassMethod(); // Animal -> Invoked from superclass
		myAnimal.testInstanceMethod(); // Cat -> From subclass
	}
}
```
Cat 类重写了 Animal 中的实例方法，并在 Animal 中隐藏了静态方法。此类中的 main 方法创建了一个 Cat 实例，并基于类调用了 testClassMethod() 方法，基于实例调用了 testInstanceMethod 方法。该程序的输入如下：

```java
The static method in Animal
The instance method in Cat
```
正如所描述的那样，被调用的隐藏静态方法来自于超类中，而被调用的重写实例方法来自于子类中。

### 1.3.3 接口方法（Interface Methods）
接口中的 default 方法和 abstract 方法像实例方法一样被继承。但是，当类或接口的类型（超类型）提供具有相同签名的多个 default 方法时，Java 编译器遵循继承规则来解决命名冲突。这些规则由以下两个原则驱动：

- 实例方法优先于接口中的 default 方法。考虑以下的类和接口：

```java
public class Horse {
	public String identifyMyself() {
		return "I am a horse";
	}
}

public interface Flyer {
	default public String identifyMyself() {
		return "I am able to fly";
	}
}

public interface Mythical {
	default public String identifyMyself() {
		return "I am a mythical creature.";
	}
}

public class Pegasus extends Horse implements Flyer, Mythical {
	public static void main(String[] args) {
		Pegasus myApp = new Pegasus(); // 飞，天马
		System.out.println(myApp.identifyMyself());
	}
}
```
方法 Pegasus.identifyMyself 返回 “I am a horse“ 的字符串。

- 已经被其它候选项重写的方法将被忽略。当超类型共享一个共同的祖先（ancestor）时，就会出现这种情况。

考虑下面的接口和类：
```java
public interface Animal {
	default public String identifyMyself() {
		return "I am an animal.";
	}
}

public interface EggLayer extends Animal { // 蛋
	default public String identifyMyself() {
		return "I am able to lay eggs.";
	}
}

public interface FireBreather extends Animal {}

public class Dragon implents EggLayer, FireBreather { // 火焰
	public static void main(String[] args) {
		Dragon myApp = new Dragon();
		System.out.println(myApp.identifyMyself());
	}
}
```
方法 Dragon.identifyMyself 返回 “I am able to lay eggs” 的字符串。（Animal 中的方法被 EggLayer 中的方法重写，将被忽略）

如果两个或多个独立定义的 default 方法产生冲突，或者 default 方法与 abstract方法发生冲突，那么 Java 编译器将生成编译器错误。必须显示重写超类中的方法。

考虑现在可以飞行的计算机控制的汽车的例子。有两个接口（OperateCar 和 FlyCar），它们为同一方法（startEngine）提供了默认实现：
```java
public interface OperateCar {
	// ...
	default public int startEngine(EncryptedKey key) {
		// Implementation
	}
}

public interface FlyCar {
	// ...
	default public int startEngine(EncryptedKey key) {
		// Implementation
	}
}
```
同时实现 OperateCar 和 FlyCar 接口的类必须重写 startEngine 方法。可以使用 super 关键字来调用任何 default 实现。
```java
public class FlyingCar implements OperateCar, FlyCar {
	// ...
	public int startEngine(EncryptedKey key) {
		FlyCar.super.startEngine(key);
		OperateCar.super.startEngine(key);
	}
}
```
super 前面的名称（在该例子中，FlyCar 或 OperateCar）必须直接引用超接口（superinterface），它定义或继承 default 方法用来被调用。这种形式的方法调用不限于区分在实现的多个包含相同签名的 default 方法接口之间。（翻译？）

从类继承的实例方法可以重写抽象接口方法。考虑下面的接口和类：
```java
public interface Mammal {
	String identifyMyself();
}

public class Horse {
	public String identifyMyself() {
		return "I am a horse.";
	}
}

public class Mustang extends Horse implements Mammal { // 野马， 哺乳动物
	public static void main(String[] args) {
		Mustang myApp = new Mustang();
		System.out.println(myApp.identifyMyself());
	}
}
```
方法 Mustang.identifyMyself 返回 “I am a horse.” 字符串。类 Mustang 从 Horse 类继承了 identifyMyself 方法，它重写了 Mammal 接口中具有相同名称的 abstract 方法。

注意：接口中的 static 方法将永远不会被继承。

1.3.4 修饰（Modifiers）

重写方法的访问说明符可以比被重写的方法允许更多，但不是更少的访问。例如，超类中的 protected 实例方法可以在子类中调整为 public，而不是 private。

如果尝试将超类中的实例方法更改为子类中的静态方法，将会出现编译错误，反之亦然。

### 1.3.5 小结（Summary）
下表总结了当定义一个和超类中的方法具有相同的签名时所发生的情况：

定义一个和超类中的方法具有相同签名的方法

  x            |    方法          | 超类静态方法
---------------|-----------------|-------------------
子类的实例方法   |    重写		   | 生成编译错误
子类的静态方法   |  生成编译错误     | 隐藏

> 注意：在子类中，可以重载（Overload）从超类继承的方法。这样的重载方法既不隐藏也不重写超类实例方法——它们是新的方法，对子类是唯一的。

问题：
什么是重载？
- 函数名一样，参数个数不一样？
答：方法名称相同，签名不同（参数数目和类型）

## 1.4多态（Polymorphism）
对多态的定义为是指生物界中，一个生物体或物种可以拥有不同的形态（形式）或阶段（变态阶段）。这一原则也可以应用于面向对象编程和语言，例如 Java 编程语言。类的子类可以定义它们自己的唯一行为，同时也能共享父类中的某些相同功能。

多态性可以通过对 Bicycle 类的轻微修改来证明。例如，可以在类中添加 printDescription 方法，显示当前存储在实例中的所有数据。
```java
public void printDescription() {
	System.out.println("\nBike is " + "in gear" + this.gear +
		"with a cadence of " + this.cadence +
		" and travelling at a speed of " + this.speed + ".");
}
```
为了演示 Java 语言中的多态特性，使用 MountainBike 和 RoadBike 类来扩展 Bicycle 类。对于 MountainBike，添加一个字段用于悬浮（暂停），它是一个字符串值，指示自行车前面是否有减震气（前减震），或者该自行车同时包括前后减震器（双减震器）。

下面是更新后的类：
```java
public class MountainBike extends Bicycle {
	private String suspension;

	public MountainBike(int startCadence, int startSpeed,
		int startGear, String suspensionType) {
		super(startCadence, startSpeed, starGear);
		this.setSuspension(suspendsionType);
	}

	public String getSuspension() {
		return this.suspension;
	}

	public void setSuspension(String suspensionType) {
		this.suspension = suspensionType;
	}

	public void printDescription() {
		super.printDescription();
		System.out.println("The " + "MountainBike has a" +
			getSuspension() + " suspension.");
	}
}
```
注意被重写的 printDescription 方法。除了之前提供的信息外，有关 suspension 的其他数据也包括在输出中。

接下来，创建 RoadBike 类。由于道路或赛车自行车有瘦轮胎，添加一个属性用来表示轮胎宽度。以下是 RoadBike 类：
```java
public class RoadBike extends Bicycle {
	// In millimeters(mm，毫米)
	private int tireWidth;

	public RoadBike(int startCadence, int startSpeed,
		int startGear, int newTireWidth) {
		super(startCadence, startSpeed, startGear);
		this.setTireWidth(newTireWidth);
	}

	public int getTireWidth() {
		return this.newTireWidth;
	}

	public void setTireWidth(int newTireWidth) {
		this.tireWidth = newTireWidth;
	}

	public void printDescription() {
		super.printDescription();
		System.out.println("The RoadBike" + "has " +
			getTireWidth() + "  MM tires.");
	}
}
```
注意：printDescription 方法被重写了。此时，将显示有关轮胎宽度的信息。

总之，有三个类：Bicycle，MountainBike 和 RoadBike。两个子类重写了 printDescription 方法，并打印（输出）唯一的信息。（这里的唯一信息指类特有的信息）

下面是一个测试程序，创建了三个 Bicycle 变量。每个变量被分配三个 bicycle 类中的其中一个。接着每个变量打印输出信息。
```java
public class TestBikes {
	public static void main(String[] args) {
		Bicycle bike01, bike02, bike03;

		bike01 = new Bicycle(20, 10, 1);
		bike02 = new MountainBike(20, 10, 5, "Dual");
		bike03 = new RoadBike(40, 20, 8, 23);

		bike01.printDescription();
		bike02.printDescription();
		bike03.printDescription();
	}
}
```
以下是测试程序的输出（参见　Less5）：
```bash
xbdong@xbdong-opencl:~/Project/github/OpenCL/Java/docs/Lesson/inheritance/Less5$
sh build.sh

Bike is in gear 1 with a cadence of 20 and travelling at a speed of 10. 

Bike is in gear 5 with a cadence of 20 and travelling at a speed of 10. 
The MountainBike has a Dual suspension.

Bike is in gear 8 with a cadence of 40 and travelling at a speed of 20. 
The RoadBikehas 23  MM tires.
```
Java 虚拟机（JVM）为每个变量中引用的对象调用适当的方法。它不调用由（通过）变量类型定义的方法。此行为称为虚方法调用，并演示 Java 语言中重要的多态功能的一个方面。


## 1.6隐藏字段（Hiding Fields）
在一个类中，与超类中的字段同名的字段将隐藏超类的字段，即使它们的类型不同。在子类中，超类中的字段不能简单的通过名称引用。相反，必须通过 super 访问该字段，在下一节将对其介绍。总的来说，不建议隐藏字段，因为它使代码难以阅读。

## 1.7 使用 super 关键字
### 1.7.1 访问超类的成员
如果方法重写了超类中的方法，则可以通过使用 super 关键字调用被重写的方法。还可以使用 super 来引用隐藏的字段（尽管不鼓励隐藏字段），考虑下面的超类：
```java
public class Superclass {
	public void printMethod() {
		System.out.println("Printed in Superclass.");
	}
}
```
下面是一个称为 Subclass 的子类，它重写了 printMethod() 方法：
```java
public class Subclass extends Superclass {
	// overrides printMethod in Superclass
	public void printMethod() {
		super.printMethod();
		System.out.println("Printed in Subclass");
	}

	public static void main(String[] args) {
		Subclass s = new Subclass();
		s.printMethod();
	}
}
```
在子类中，简单的名称 printMethod 是指在子类中声明的那个，它重写了超类中同名的方法。因此，要引用从超类继承的 printMethod() 方法，子类必须使用一个限定符，使用 super 来表示。编译并执行子类打印如下内容（参考 Less6）：

```bash
Printed in Superclass.
Printed in Subclass
```

### 1.7.2 子类构造函数
下面的示例阐述了如何使用 super 关键字来调用超类的构造函数。回忆 Bicycle 示例，MountainBike 是 Bicycle 的子类。下面是 MountainBike（子类）构造函数调用超类的构造函数，然后添加自己的初始化代码：
```java
public MountainBike()int startHeight, int startCadence, int startSpeed,
	   int startGear) {
		   super(startCadence, startSpeed, startGear);
		   seatHeight = startHeight;
}
```
对超类构造函数的调用必须在子类构造函数中的第一行。调用超类构造函数的语法为：

	super();

或
	super(参数列表);

使用 super()，将调用超类中没有参数的构造函数。使用 super(参数列表)，具有匹配参数列表的超类构造将被调用。

> 注意：如果构造函数没有明确（显式）的调用超类的构造函数，Java 编译器将自动插入一个对超类中没有参数构造函数的调用。如果超类中没有包含不带参数的构造函数，则会的到编译错误。Object 确实有这样的构造函数，因此如果 Object 是唯一的超类，则没有问题。

如果子类构造函数调用了超类的构造函数，不管是显示调用还是隐式调用，可以认为会有一个整个构造函数链的调用，所有的路径最终将调用 Object 的构造函数。事实上，情况就是这样的。它被称作构造函数链，当有长的类时，需要了解（意识、关注）它。（由于类的继承是单继承的，顺着链条往下走就行）

## 1.8Object 作为超类（Object as a Superclass）

在 java.lang 包中的 Object 类，位于类层次结构树的顶端。每个类都是 Object 类直接或简介的后代。每个使用的或开发的类都继承了 Object 的实例方法。不必使用这些方法中的任何一种，但是如果需要可以在自己的类中重写这些方法。从 Object 继承的方法将在本节中讨论：

- protected Object clone() trhows CloneNotSupportedException
创建并返回此对象的副本；
- public boolean equals(Object obj)
表示其它对象是否和这个相等；
- protected void finalize() throws Throwable
当垃圾回收时，垃圾回收器将在一个对象上调用
- public final Class getClass()
返回对象的运行时（runtime）类
- public int hashCode()
返回对象的哈希代码值（哈希值）
- public String toString()
返回对象的字符串表示形式

对象的 notify，notifyAll 和 wait 方法都扮演相应的角色，以同步程序中独立运行线程的活动，稍后的课程将对此进行讨论，此处不再介绍。有五种方法：

- public final void notify()
- public final void notifyAll()
- public final void wait()
- public final void wait(long timeout)
- public final void wait(long timeout, int nanos)

> 注意：这些方法的中有一些微妙的方面，特别是 clone 方法

### 1.8.1 clone() 方法
如果某个类，或它的超类，实现了 Cloneable 接口，则可以使用 clone() 方法从现有对象创建副本。若要创建副本，请编写：

aCloneableObject.clone();

对象对此方法的实现，将检查调用 clone() 的对象是否是否实现了 Cloneable 接口。如果没有实现，该方法将抛出 CloneNotSupportedException 异常。异常处理将自爱后面的课程中介绍。当前，需要知道 clone() 必须声明为：

```java
protected Object clone() throws CloneNotSupportedException
```
or
```java
public Object clone() throws CloneNotSupportedException
```
如果要编写一个重写 Object 中的 clone() 方法。

如果调用 clone() 的对象实现了 Cloneable 接口，Object 对 clone() 方法的实现将创建一个相同类的对象，和原始对象相同，并初始化新对象的成员变量，它和原始对象对应成员变量的值相同。

使类可克隆（生成副本）最简单的方法是在类声明中添加 Cloneable 的实现。然后，对象就可以调用 clone() 方法。

对于某些类，Object 的 clone() 方法的默认行为工作正常。但是，一个对象包含对外部对象的引用，称作 ObjExternal，就需要重写（override）clone() 方法来的到正确的行为。否则，一个对象在 ObjExternal 中所做的修改将在其克隆（副本）中可见。这意为着原始对象及其克隆对象不是独立的——要解耦它们，必须重写 clone() 方法，以便它克隆对象和 ObjExternal。然后，原始对象引用 ObjExternal，同时克隆对象引用一个克隆的 ObjExternal，因此对象及其克隆是真正独立的。
// 列举个示例？

### 1.8.2 equals() 方法
equals() 方法比较两个对象是否相等，如果相等，则返回 true。Object 类中的 equals() 方法使用标识运算符（==）来确定两个对象是否相等。对于原始数据类型，这可以给出正确结果。但是，对于对象，它不会。由 Object 提供 equals() 方法测试对象引用是否相等——也就是说，比较的对象是否为完全相同的对象。

若要测试两个对象在等效性上是否相等（包含相同的信息），必须重写（override）equals() 方法。下面是重写 equals() 方法的 Book 类：
```java
public class Book {
	...
	public boolean equals(Object obj) {
		if (obj instanceof Book)
			return ISBN.equals((Book)obj.getISBN());
		else
			return false;
	}
}
```
考虑下面的代码，测试 Book 类的两个实例的相等性：
```java
// Swing Turotrial, 2nd edition
Book firstBook = new Book("0201914670");
Book secondBook = new Book("0201914670");

if (firstBook.equals(secondBook)) {
	System.out.println("objects are equeal");
} else {
	System.out.println("objects are not equeal");
}
```
即使 firstBook 和 secondBook 引用两个不同的对象，但程序显示对象是相等的。它们被认为相等是由于比较的对象包含相同的 ISBN 号。

如果标识晕算符不适合时，则应重写 equals() 方法。

> 注意：如果重写了 equals() 方法，则必须也重写 hashCode() 方法。

### 1.8.3 finalize() 方法
Object 类提供了一个回调方法，即 finalize()，在对象变为垃圾时可以对其调用。Object 中对 finalize() 的实现不执行任何操作，可以重写 finalize() 来执行清理，例如释放资源。

finalize() 方法可以由系统自动调用，但当它被调用时，或者即使被调用，也不确定（时间？）。因此，不应该依赖此方法执行清理操作。例如，如果执行完 I/O 操作后，代码中没有关闭文件描述符，并且寄希望于 finalize() 来关闭，则可能会耗尽文件描述符。

### 1.8.4 getClass() 方法
不能重写 getClass 方法。
getClass() 方法返回一个类对象，它具有可用于获取有关类的信息方法，例如它的名称（getSimpleName()），它的超类（getSuperclass()）和它实现的接口（getInterfaces()）。例如，下面的方法获取并显示对象的类名：
```java
void printClassName(Object obj) {
	System.out.println("The object's" + " class is " +
		obj.getClass().getSimplename());
}
```
java.lang 包中的 Class 类，有大量的方法（超过 50 个）。例如，可以测试该类是注释（isAnnotation()），一个接口（isInterface()）或一个枚举（isEnum()）。可以看到对象的字段是什么（getFields()）或它的方法是什么（getMethods()），等等。

### 1.8.5 hashCode() 方法
hashCode() 返回的是对象的哈希代码，它是十六进制表示的对象的内存地址。

根据定义，如果两个对象相等，则它们的哈希代码也必须相等。如果重写 equals() 方法，则更改两个对象的等效方式（相等的条件），并且对象的 hashCode() 实现不再有效。因此，如果重写了 equals() 方法，也必须重写 hashCode() 方法。

### 1.8.6 toString() 方法
始终需要考虑在自己的类中重写 toString() 方法，也就是说，在自己的类中往往需要重写 toString() 方法。

Object 的 toString() 方法返回对象的字符串表示形式，这对于调试非常有用。对象的字符串表示形式完全依赖与对象，这就是需要在类中重写 toString() 的原因。这里是说类的对象变了（子类嘛）？

可以将 toString() 和 System.out.println() 一起用于显示对象的文本表示形式，如书籍的实例：
```java
System.out.println(firstBook.toString());
```
对于正确的重写 toString() 方法，打印一些有用的内容，如下所示：
```bash
ISBN: 0201914670; The Swing Tutorial; A Guide to Constructing GUIs, 2nd Edition
```

## 1.9编写　Final 类和方法
可以声明类的某些或者所有方法为 final 方法。在方法声明时使用 final 关键字表示该方法不能被子类重写。Object 类就这样做了，它的一些方法就是 final 方法。

如果一个方法在实现时不能被改变，并且保持对象的一致性非常关键，那么该方法就应该为 final。例如，可能希望在 ChessAlgorithm 类中讲　getFirstPlayer 声明为 final 方法：
```java
class ChessAlgorithm { // 棋算法
	enum ChessPlayer {WHITE, BLACK}
	...
	final ChessPlayer getFirstPlayer) {
		return ChessPlayer.WHITE;
	}
	...
}
```
从构造函数中调用的方法通常声明为 final。如果构造函数调用非 final 方法，子类可能会以令人惊讶后不希望的结果重新定义该方法。

注意，也可以声明整个类为 final。声明为 final 的类不能被子类（什么意思？不能被继承？没有子类，不能被继承）。例如，在创建像 String 这样不可变的类时，这一点尤其重要。

=========
## 1.10抽象方法和类
抽象类是声明为 abstract 的类——它可能包括或不包括抽象方法。抽象类不能实例化，但可以被继承（子类化）。

抽象方法在声明的时候没有具体实现（不带大括号，后跟分号），如下所示：
```java
abstract void moveTo(double deltaX, double deltaY);
```
如果类包含抽象方法，则类必须声明为抽象类，如下所示：
```java
public abstract class GraphicObject {
	// declare fields
	// declare nonabstract methods
	abstract void draw();
}
```
当抽象类被继承时，子类通常为父类中的所有抽象方法提供具体实现。然而，如果没有，那么子类也必须被声明为抽象的。

> 注意：接口中的方法（参见接口部分），如果没有被声明为 default 或 static 方法的隐式是抽象的，因此 abstract 修饰符不与接口方法一起使用。（可以使用，但是不必要的）

### 1.10.1 抽象类相对于接口
抽象类和接口类似，不能对它们实例化，并且它们可能包含在声明时有或没有实现的混合方法。但是，使用抽象类，可以声明非 static 和 final 的字段，并且定义 public，protected 和 private 的具体方法。对于接口，所有的字段都是 public，static 和 final 的，并且所有的声明或定义的方法（和 default 方法类似）都是 public 的。此外，只能扩展（extends）一个类，无论它是否为抽象的，但是可以实现任意数量的接口。

抽象类和接口，应该使用哪个？

- 如果下面语句中的任何一个适用于当前情况，需要使用抽象类：
（1）需要在多个紧密相关的类之间共享代码；
（2）希望扩展自抽象类的类拥有许多常用的方法和字段，或者需要除 public 之外的访
问修饰符（例如 protected 和 private）；
（3）需要声明非 static 和 final 的字段。这允许定义方法，它们可以访问和修改所属
对象的状态。

- 如果下面语句中的任何一个适用与当前情况，需要使用接口：
（1）需要不相关的类来实现接口。例如，Comparable 和 Cloneable 被许多不相关的类
所实现；
（2）需要指定特定数据类型的行为，但不关心谁实现其行为（多态）；
（3）需要利用类型的多重继承特性。

JDK 中一个抽象类的示例是 AbstractMap，它是 Collections 框架（Framework）的一部分。它的子类（包括 HashMap，TreeMap 和 ConcurrentHashMap）共享许多在 AbstractMap 中定义的方法（包括 get，put，isEmpty，containsKey 和 containsValue）

JDK 中实现多个接口的类的一个示例是 HashMap，它实现了 Serializable，Cloneable和 Map<K, V> 接口。通过读取此接口的列表，可以推断 HashMap 的实例（不管开发人员还是公司实现了这个类）是可以被克隆，是可以序列化的（这意味着它可以转换为字节流；参考 Serializable 对象部分），并且拥有 map 的功能。此外，Map<K, V> 接口已经通过许多 default 方法的到了增强，例如 merge 和 forEach，而已实习该接口的旧的类不必重定义。

注意，许多软件库同时使用了抽象类和接口；HashMap 类实习了多个接口，并且还扩展了抽象类 AbstractMap。

1.10.2 抽象列示例（An Abstract Class Example）
在面向对象的绘图应用中，可以绘制圆、矩形、线条、贝塞尔曲线和许多其它图形对象。这写对象都具有某些共同的状态（例如：位置、方向、线条颜色、填充颜色）和行为（例如：moveTo，rotate，resize，draw）。某些状态和行为对于所有图形对象来说都是相同的（例如：位置，填充颜色 和 moveTo），其它则需要不同的实现（例如：resize 或 draw）所有的图像对象（GraphicObjects）必须能够绘制（draw）和调整自己大小（resize）；它们的不同之处在于它们是如何做的。这对抽象类来说是完美的场景（情况）。可以利用这些相似性，并声明所有图形对象继承自相同的抽象父对象（例如：GraphicObject），如下图所示：

<img src="images/classes-graphicObject.gif" width="70%" height="70%">

继承自 GraphicObject 的 Rectangle，Line，Bezier 和 Circle 类。

首先，声明一个抽象类 GraphicObject，以提供所有子类完全共享的成员变量和方法，如当前位置和 moveTo 方法。GraphicObject 还声明了需要由所有子类实现但必须以不同方式实现的方法（例如，draw 和 resize）抽象方法。GraphicObject 可以是这样的：
```java
abstract class GraphicObject {
	int x, y;
	...
	void moveTo(int newX, int newY) {
		...
	}
	abstract void draw();
	abstract void resize();
}
```
GraphicObject 的每个子类（非抽象类，如 Circle 和 Rectangle）必须为 draw 和 resize 放提供具体的实现。
```
class Circle extends GraphicObject {
	void draw() {
		...
	}
	void resize() {
		...
	}
}

class Rectangle extends GraphicObject {
	void draw() {
		...
	}
	void resize() {
		...
	}
}
```
### 1.10.3 是么时候一个抽象类实现一个接口（When an Abstract Class Implements an Interface）

在 Interfaces 一节部分，有提到实现接口的类必须实现接口的所有方法。但是，如果定义了一个没有实现接口中所有方法的类，那么这个类应该被声明为抽象类。例如：
```java
abstract class X implements Y {
	// implements all but one method of Y
}

class XX extens X {
	// implements the remaining method in Y
}
```
在上面的例子中，类 X 必须为抽象类，因为它并没有完全实现 Y，但是类 XX 实际上实现了 Y。

### 1.10.4 类成员（Class Members）
抽象类可能包含 static 字段和 static 方法。可以将这些静态成员与类引用一起使用（例如，AbstractClass.staticMethod()），就像与任何其他类一样 。（这里的类引用与对象引用对应？）

## 1.11继承小结（Summary of Inheritance）
除了 Object 类，每个类都有一个直接的超类。类直接或间接继承了其所有超类的字段和方法。子类可以重写它继承的方法，也可以隐藏它继承的字段和方法（注意，隐藏字段通常是不好的变成习惯（实践））。

在 "Overriding and Hiding Methods" 章节的表中，显示了声明一个和超类中的方法具有相同签名结果。

Object 类在类层次结构的顶部。所有类都从次类派生出来，并继承了它的方法。从 Object 类继承的有用方法包括 toString()，equals()，clone() 和 getClass()。

在类声明时使用 final 关键字可以防止类被继承。类似的，通过将方法声明为final 方法可以防止它在子类中被重写。

抽象类只能被继承，不能被实例化。抽象类可以包含抽象方法——已声明但为实现的方法。然后，子类为抽象方法提供实现。

问题和练习：继承（Questions and Exercises: inheritance）
问题：
a.重写了子类的方法：methodTwo()；
b.隐藏了子类的方法：methodFour()；
c.编译错误：methodOne() 和 methodThree()。