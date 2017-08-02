## 1.1继承
在前面的接口一节，可以看到`继承`被多次提及到。在 Java 语言中，类可以从其它的类派生，因此可以从这些类中继承字段和方法。

> 定义：从其它类派生的类称为子类，也称为派生类或扩展类。可以派生出子类的类称为超类，也称为基类或父类。除了 Object 没有超类，每个类都有且仅有一个唯一的超类，这称为单继承。在没有显式指定其它任何超类的情况下，每个类隐式都是 Object 的子类。类可以派生自派生于其它类的类，并最终派生自最上层的 Object 类。例如，类 C 派生自类 B，类 B 派生自类 A，类 A 派生自 Object。

继承的思想不仅简单而且强大。当需要创建新的类时，若已有的类已经包含了一些需要的代码，可以从已有的类派生出新类。这就可以重用现有类的字段和方法，而不必全部重新编写，同时也省去了调试步骤。

子类从超类中继承所有的成员，包括字段、方法以及嵌套类。构造函数不是成员，因此子类不会继承，但是超类的构造函数可以在子类中调用。

### 1.1.1Java 平台类层次结构
在 java.lang 包中定义的 Object 类，它定义并实现了所有类共有的行为，这些类包括自己编写的类。在 Java 平台中，许多类直接派生自 Object ，其它类从这些类派生，以此类推。这就构成类的层次结构。

<center><img src="images/classes-object.gif" width="70%" height="70%"></center>

<center>**Java 平台中的所有类都是 Object 的后代**</center>

在类层次结构的顶部，Object 提供了所有类的通用行为。接近类层次结构底部的类提供更专业的行为。

### 1.1.2继承的示例
下面是 Bicycle 类对应的示例代码：
```java
public class Bicycle {

	// the Bicycle class has three fields
	public int cadence;
	public int gear; // 齿轮
	public int speed; // 速度

	// the Bicycle class has one constructor
	public Bicycle(int startCadence, int startSpeed, int startGear) {
		gear = startGear;
		cadence = startCadence;
		speed = startSpeed;
	}

	// the Bicycle class has four methods
	public void setCadence(int newValue) {
		cadence = newValue;
	}

	public void setGear(int newValue) {
		gear = newValue;
	}

	public void applyBrake(int decrement) {
		speed -= decrement;
	}

	public void speedUp(int increment) {
		speed += increment;
	}
}
```
声明为 MountainBike 的类，它是 Bicycle 的子类，如下所示：
```java
public class MountainBike extends Bicycle {

	// the MountainBike subclass adds one field
	public int seatHeight; // 坐垫高度

	// the MountainBike subclass has one constructor
	public MountainBike(int startHeight,
		int startCadence,
		int startSpeed,
		int startGear) {
		super(startCadence, startSpeed, startGear);
		seatHeight = startHeight;
	}

	// the MountainBike subclass adds one method
	public void setHeight(int newValue) {
		seatHeight = newValue;
	}
}
```
MountainBike 继承了 Bicycle 的所有字段和方法，并添加了 seatHeight 字段和一个设置该字段的方法。除了构造函数，这就像完全重新编写了一个新的 MountainBike 类，它包含四个字段和五个方法，但是，实际上并不需要做所有的这些工作。如果 Bicycle 类中的方法非常复杂并且需要大量的时间来调试，继承现有操作将特别有效。

### 1.1.3 可以在子类中做些什么
不管子类在哪个包中，子类从父类继承了所有的 public 和 protected 成员。如果子类和父类在同一个包中，它也继承父类中包的私有成员。在子类中，可以直接使用继承的成员，或者替换它们，隐藏它们，抑或是用新成员来做一些扩展。

- 继承的字段可以直接使用，就像使用类中的其它字段一样；
- 在子类中声明一个与超类中同名的字段，达到隐藏超类字段的目的（不推荐这么做）；
- 在子类中声明一个在超类不存在的新的字段；
- 继承的方法可以直接使用；
- 在子类中编写新的 instance 方法，该方法和超类中的某个方法具有相同签名，以重写（overriding）超类的方法；
- 在子类中编写新的 static 方法，该方法和超类中的某个方法具有相同签名，用以隐藏（hiding）超类的方法；
- 在子类中声明一个超类中不存在的新方法；
- 在子类中编写构造函数来调用超类的构造函数，这可以使用隐式方式或通过 super 关键字。

// xbdong - 隐藏和重写有什么区别？方法签名包含那些东西？

下面的部分将在这些主题上展开。

### 1.1.4超类中的私有成员
子类并不会继承父类中的私有成员。但是，如果超类包含的 public 或 protected 方法会访问它的私有成员，那么这些私有成员也可以间接的被子类使用。

嵌套类中内部内可看作是外部内的一个成员，因此内部内可以访问外部类的所有私有成员，包括其中的字段和方法。于是，继承自 public 或 protected 内部类的子类可以间接访问超类中所有的私有成员。

### 1.1.5 对象转换（Casting Objects）
通过前面的学习，我们已经知道一个对象是类的数据类型，此类被实例化。例如，在下面的代码中：
```java
public MountainBike myBike = new MountainBike();
```
myBike 是 MountainBike 类型。

MountainBike 扩展自 Bicycle 和 Object 类。因此，MountainBike 是 Bicycle、并且也是 Object 类型。当需要的时候，它可以应用于 Bicycle 或 Object 对象。反过来则不一定成立，Bicycle 可能是 MountainBike 类型，但不是一定的。同理，Object 可能是 Bicycle 或 MountainBike 类型，但这并不是一定的。

强制转换体现了在`继承`和`实现`所允许的对象中，使用一种类型代替另一种类型的对象。例如，我们可以这么写：
```java
Object obj = new MountainBike();
```
那么 obj 可以同时为 Object 和 MountainBike 对象，直到 obj 被赋值为另一个不是 MountainBike 的对象，这称作隐式的强制转换。相反，如果按照下面的方式：
```java
MountainBike myBike = obj;
```
将会得到一个编译错误，因为编译器不知道 obj 是 MountainBike 类型的对象。然而，通过显式的强制转换，我们可以告诉编译器，将 MountainBike 类型的对象赋值给 myBike：
```java
MountainBike myBike = (MountainBike)obj;
```
当 obj 被赋值给 MountainBike 类型对象时， 强制转换将插入一个运行检查，这样编译器可以安全地认为 obj 是 MountainBike 类型的对象。如果在运行时 obj 不是 MountainBike 类型对象，将抛出一个异常。

注意：可以使用 instanceof 操作符来测试特定对象类型，这可以避免由于不正确的强制转换导致的运行时错误。如下：
```java
if (obj instanceof MountainBike) {
	MountainBike myBike = (MountainBike)obj;
}
```
这里，instanceof 操作符验证 obj 是否为 Mountainbike 类型对象，如果成立就做强制转换，这样就不会导致运行异常的发生。

## 1.2状态、实现和类型的多继承
类和接口之间的一个显著区别是类可以包含字段，而接口则没有。此外，可以通过实例化类来创建对象，而这不能用在接口上。一个对象将它的状态存储在字段中，这些字段定义在类中。Java 编程语言不允许扩展多个类的一个原因是为了避免状态（字段）的多重继承问题，这可以从多个类继承字段。例如，假设可以定义一个扩展自多个类的新类，通过实例化该类来创建对象时，此对象将从该类的所有超类中继承字段。要是不同超类的方法或构造函数实例化同一个字段该怎么办？哪个方法或构造函数优先？由于接口不包含字段，因此不必担心继承多个状态所产生的问题。

实现的多重继承是指从多个类中继承已定义方法。这种类型的多重继承可能会出现一些问题，例如命名冲突和歧义。当支持这种多重继承编程语言的编译器遇到包含同名方法的超类时，它们有时无法确定到底该访问或调用哪个成员或方法。此外，程序员可能在向超类中添加新方法时不知不觉的引入了命名冲突。接口中的 default 方法引入了对多重继承实现的支持。类可以实现多个接口，多个接口可以包含同名的 default 方法，Java 编译器提供了一些规则来决定哪个 default 方法会被使用。

Java 编程语言支持类型的多重继承（接口也是一种类型），也就是说一个类就可以同时实现多个接口。一个对象可以有多种类型，它自己的类的类型以及类所实现的所有接口的类型。这意味着，如果将某个变量声明为接口类型，那么该变量可以引用任何实现该接口的类的实例化对象。

与实现的多重继承一样，类可以继承它实现的所有接口中定义方法的具体实现（default 或 static）。在这种情况下，编译器或用户必须决定使用哪个接口中实现的方法。

## 1.3重写和隐藏方法

### 1.3.1 实例方法
子类中的实例方法和父类中的实例方法如果具有有相同的签名（方法名称、参数类型和数目）和返回类型，将重写（overrides）超类的方法。重写方法还可以返回被重写方法的返回类型的子类型。当重写方法时，可以使用 `@Override` 注释来提示编译器要重写超类中的方法。如果编译器检查到该方法在超类中不存在，它将生成编译错误。

### 1.3.2 静态方法
如果子类定义的静态方法与超类中的静态方法具有相同的签名，那么子类中的方法将隐藏超类中的方法。

弄清楚隐藏静态方法和重写实例方法的区别具有重要意义：

- 被调用的重写实例方法处于子类中；
- 被调用的隐藏静态方法取决于该调用是来自于子类还是超类。

下面是两个类的示例程序，第一个是 Animal，它包含一个实例方法和一个静态方法：
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
第二个类是 Animal 的子类 Cat：
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
Cat 类重写了 Animal 中的实例方法，并隐藏了在 Animal 中的静态方法。此类中的 main 方法创建了一个 Cat 实例，并基于类调用了 testClassMethod() 方法，基于实例调用了 testInstanceMethod 方法。该程序的输出如下所示（代码参见 Less1）：
```bash
The static method in Animal
The instance method in Cat
```
正如前面描述的那样，被调用的隐藏静态方法来自于超类，而被调用的重写实例方法来自于子类。

### 1.3.3 接口方法
接口中的 default 方法和 abstract 方法和实例方法一样被继承。但是，当类的接口（超类型）提供具有相同签名的多个 default 方法时，Java 编译器遵循继承规则来解决命名冲突。这些规则由以下包括下面两个原则：

- 实例方法优先于接口中的 default 方法。在下面的类和接口中：

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
		Pegasus myApp = new Pegasus(); // 能飞，天马
		System.out.println(myApp.identifyMyself());
	}
}
```
方法 `Pegasus.identifyMyself` 返回字符串 `I am a horse` 。

- 已经被其它接口重写的方法将被忽略。当超类型（接口）继承自同一个接口时，就会出现这种情况。

请看下面的接口和类：
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
方法 `Dragon.identifyMyself` 返回字符串 `I am able to lay eggs`。Animal 中的方法被 EggLayer 中的定义的方法重写，将被忽略。

如果两个或多个在独立的接口中定义的 default 方法产生冲突，或者 default 方法与 abstract方法发生冲突，那么 Java 编译器将生成编译器错误，此时必须显式的重写超类型中的方法。

以可以飞行的计算机控制的汽车为例。有两个接口，分别是 OperateCar 和 FlyCar，它们为同一方法 `startEngine` 提供了默认实现：
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
这时，同时实现了 OperateCar 和 FlyCar 接口的类就必须重写 startEngine 方法。针对不同的超类型，可以使用 super 关键字来调用它们具体的 default 实现。
```java
public class FlyingCar implements OperateCar, FlyCar {
	// ...
	public int startEngine(EncryptedKey key) {
		FlyCar.super.startEngine(key);
		OperateCar.super.startEngine(key);
	}
}
```
super 前面的名称（在上面的例子中，FlyCar 或 OperateCar）必须直接引用超类型对应的接口，它定义或继承的 default 方法将被调用。这种形式的方法调用可以使用在实现了多个包含相同签名的 default 方法接口的类中。

从类继承的实例方法可以重写接口中的抽象方法。如下面的接口和类：
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
方法 `Mustang.identifyMyself` 返回字符串 `I am a horse.`。类 Mustang 从 Horse 类继承了 identifyMyself 方法，它重写了 Mammal 接口中具有相同名称的 abstract 方法。

> 注意：接口中的 static 方法将永远不会被继承。

1.3.4 访问修饰符

重写的方法对应的访问修饰符只能比被重写的方法允许更多的访问，而不是更少的访问。例如，超类中的 protected 实例方法可以在子类中调整为 public，而不能变为 private。还有，如果尝试将超类中的实例方法更改为子类中的静态方法，将会出现编译错误，反之亦然。

### 1.3.5 小结
下表总结了当子类中定义的方法和超类的方法具有相同签名时所发生的情况：

  -             |    超类的实例方法 　| 超类的静态方法
-----------------|------------------|-------------------
**子类的实例方法** |    重写		      | 生成编译错误
**子类的静态方法** |  生成编译错误       | 隐藏

> 注意：在子类中，可以重载（Overload）从超类继承的方法。这些的重载方法既不隐藏也不重写超类的实例方法,它们是新的方法，对子类来说是唯一的。所谓重载就是子类的实例方法和父类中的实例方法具有相同的名称，但是签名（参数数目或类型）不同。要注意和重写（Override）的区别，重写是方法签名和返回类型都相同。

问题：
什么是重载？
- 函数名一样，参数个数不一样？
答：方法名称相同，签名不同（参数数目和类型）

## 1.4多态（Polymorphism）
多态的定义为是指在生物界中，一个生物体或物种拥有不同的形态（形式）或阶段（变态阶段）。这一原则也可以应用于面向对象编程和语言，例如 Java 编程语言。子类可以定义自己的行为，同时也能共享父类中的某些相同功能。

可以通过对 Bicycle 类做一些修改来说明多态特性。例如，可以在类中添加 printDescription 方法，显示当前存储在实例中的所有数据。
```java
public void printDescription() {
	System.out.println("\nBike is " + "in gear" + this.gear +
		"with a cadence of " + this.cadence +
		" and travelling at a speed of " + this.speed + ".");
}
```
为了演示 Java 语言中的多态特性，使用了扩展自 Bicycle 类的 MountainBike 和 RoadBike 类。对于 MountainBike，添加一个　suspension　字段，它是一个字符串类型值，指示自行车是否有前减震器，或者该自行车同时包括前后减震器（双减震器）。

下面是更新后的类：
```java
public class MountainBike extends Bicycle {
	private String suspension;　// 减震器

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
注意被重写的 printDescription 方法。除了之前提供的信息外，有关 ｀suspension｀ 的数据也包括在输出中。接下来，创建 RoadBike 类。由于赛车自行车拥有窄轮胎，添加一个 tireWidth 属性用来表示轮胎宽度。以下是 RoadBike 类：
```java
public class RoadBike extends Bicycle {
	// In millimeters(毫米)
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

总的来说有三个类，Bicycle，MountainBike 和 RoadBike。两个子类都重写了 printDescription 方法，并输出子类特有的信息。下面是一个测试程序，创建了三个 Bicycle 变量。每个变量被分配三个 bicycle 类中的其中一个。接着每个变量打印输出信息。
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
在上面的示例中，Java 虚拟机（JVM）为每个变量引用的对象调用了对应的方法，可以看出它并没有调用由变量类型定义的方法。这种行为称为虚方法调用，它是 Java 语言中多态功能的一个重要方面。

## 1.6隐藏字段
在子类中，与超类同名的字段将隐藏超类的字段，即使它们的类型不同。如果要在子类中引用超类的同名字段，不能简单的通过名称来引用，而是必须通过 super 关键字来访问该字段，下面将对其展开介绍。总的来说，不建议隐藏字段，因为它会导致代码难以阅读。

## 1.7 使用 super 关键字
### 1.7.1 访问超类的成员
如果子类中的方法重写了超类的方法，可以通过使用 super 关键字调用被重写的方法。还可以使用 super 来引用隐藏的字段（不建议隐藏字段），例如下面的超类：
```java
public class Superclass {
	public void printMethod() {
		System.out.println("Printed in Superclass.");
	}
}
```
下面是称为 Subclass 的子类，它重写了 printMethod() 方法：
```java
public class Subclass extends Superclass {
	// overrides printMethod in Superclass
	public void printMethod() {
		super.printMethod();
		System.out.println("Printed in Subclass.");
	}

	public static void main(String[] args) {
		Subclass s = new Subclass();
		s.printMethod();
	}
}
```
在子类中，方法名称 `printMethod` 在子类中声明，它重写了超类中同名的方法。因此，若要引用从超类继承的 printMethod() 方法，子类必须使用一个限定符，通过 super 来表示。编译并执行该程序，在子类中输出了如下内容：

```bash
Printed in Superclass.
Printed in Subclass.
```
（参考 Less6）

### 1.7.2 子类构造函数
下面的示例阐述了如何使用 super 关键字来调用超类的构造函数。回想在之前的 Bicycle 示例中，MountainBike 是 Bicycle 的子类。下面是在 MountainBike 构造函数中调用超类的构造函数，然后执行其它的初始化代码：
```java
public MountainBike()int startHeight, int startCadence, int startSpeed,
	   int startGear) {
		   super(startCadence, startSpeed, startGear);
		   seatHeight = startHeight;
}
```
对超类构造函数的调用必须在子类构造函数中的第一行执行。调用超类构造函数的语法为：
```java
	super();
```
或
```java
	super(参数列表);
```
使用 `super()`，将调用超类中不包含参数的构造函数；如果使用 `super(参数列表)`，超类中具有匹配参数列表的构造函数将被调用。

> 注意：如果子类的构造函数没有显式的调用超类的构造函数，Java 编译器将自动插入一个对超类中没有参数列表的构造函数的调用。如果这时超类中不包含没有带参数的构造函数，则会的到编译错误。Object 类的实现中包含了没有带参数的构造函数，如果 Object 是唯一超类，则不会出现编译错误。

如果子类构造函数调用了超类的构造函数，不管是显示调用还是隐式调用，那么会有一条整个构造函数链的调用，整条路径最终将调用 Object 类的构造函数。实际情况就是这样的，这被称作构造函数链。当包含较长的构造函数链时，需要了解它们的执行流程（由于类的继承是单继承，顺着链条往下走就行）。

## 1.8超类 Object
在 java.lang 包中的 Object 类，位于类层次结构的顶端。所有类都是 Object 直接或间接的子类，每个类都继承了 Object 的实例方法。不一定要使用这些方法，但如果需要可以在子类中重写这些方法。这里将讨论从 Object 类继承的方法：

- protected Object clone() trhows CloneNotSupportedException
创建并返回该对象的副本；
- public boolean equals(Object obj)
判断其它对象是否和此对象相等；
- protected void finalize() throws Throwable
当回收垃圾时，垃圾回收器将在该对象上调用
- public final Class getClass()
返回对象的运行时对应的类
- public int hashCode()
返回对象的哈希值
- public String toString()
返回对象的字符串表示形式

Object 类的 notify、notifyAll 和 wait 方法用于程序中不同线程之间的同步，后面将对其详细介绍。总共有 5 个相关方法：

- public final void notify()
- public final void notifyAll()
- public final void wait()
- public final void wait(long timeout)
- public final void wait(long timeout, int nanos)

### 1.8.1 clone() 方法
如果子类或其超实现了 Cloneable 接口，则可以使用 clone() 方法根据现有的对象创建一个副本。若要创建副本，代码如下：
```java
aCloneableObject.clone();
```
Object 类中实现的 clone() 方法将检查调用 clone() 方法的对象是否是否实现了 Cloneable 接口，如果没有实现该接口，将抛出 CloneNotSupportedException 异常，关于异常处理将在后面介绍。若要重写 Object 类中的 clone() 方法，clone() 必须按照如下方式声明：

```java
protected Object clone() throws CloneNotSupportedException
```
或
```java
public Object clone() throws CloneNotSupportedException
```
如果调用 clone() 的对象已经实现了 Cloneable 接口，Object 类中的 clone() 方法将创建一个相同类的对象，和原始对象相同，并初始化新对象的成员变量，它和原始对象对应成员变量的值相同。

使类可克隆（生成副本）最简单的方法是在类声明中添加 Cloneable 的实现。然后，对象就可以调用 clone() 方法。

对于某些类，Object 的 clone() 方法的默认行为工作正常。但是，一个对象包含对外部对象的引用，称作 ObjExternal，就需要重写（override）clone() 方法来的到正确的行为。否则，一个对象在 ObjExternal 中所做的修改将在其克隆（副本）中可见。这意为着原始对象及其克隆对象不是独立的——要解耦它们，必须重写 clone() 方法，以便它克隆对象和 ObjExternal。然后，原始对象引用 ObjExternal，同时克隆对象引用一个克隆的 ObjExternal，因此对象及其克隆是真正独立的。
// 列举个示例？

### 1.8.2 equals() 方法
equals() 方法比较两个对象是否相等，如果相等则返回 true。Object 类中的 equals() 方法使用标识运算符`==`来确定两个对象是否相等。对于原始数据类型，这可以得到正确的结果；但是，对于对象则不会。Object 类提供 equals() 方法测试对象引用是否相等，它比较的对象是否为完全相同的两个对象。若要测试两个对象是否相等（包含相同的信息），必须重写 equals() 方法。下面是重写了 equals() 方法的 Book 类：
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
在下面的代码中，测试 Book 类的两个实例是否相等：
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
即使 firstBook 和 secondBook 引用了两个不同的对象，但结果显示对象是相等的。它们被认为相等是由于比较的对象包含了相同的 ISBN 编码。如果标识运算符不能满足比较需求，则应重写 equals() 方法。

> 注意：如果重写了 equals() 方法，则必须也重写 hashCode() 方法。hashCode() 返回对象哈希码，它是对象的十六进制表示的内存地址。

### 1.8.3 finalize() 方法
Object 类提供了一个 finalize() 回调方法，在对象变为垃圾时可以对其调用。Object 类中关于 finalize() 的实现不执行任何操作，可以通过重写 finalize() 来执行清理操作，例如释放资源。

finalize() 方法可以由系统自动调用，但被调用的时间是不确定的。因此，不应该依赖系统自动对此方法的调用来执行清理操作。例如，如果执行完 I/O 操作后代码中没有关闭文件描述符，并且寄希望于系统自动调用 finalize() 来关闭，最终可能会耗尽文件描述符。

### 1.8.4 getClass() 方法
不能重写 Object 类的 getClass 方法，该方法在声明时使用了 final 修饰符。getClass() 方法返回一个类对象，该对象包含了可用于获取有关类的信息的方法，例如类名称`getSimpleName()`，类的超类`getSuperclass()`和类实现的接口`getInterfaces()`。例如，下面的方法获取并显示对象的类名：
```java
void printClassName(Object obj) {
	System.out.println("The object's" + " class is " +
		obj.getClass().getSimplename());
}
```
java.lang 包中的 Class 类，包含了大量的方法，超过 50 个。例如，可以测试该类是否为 annotation  类型`isAnnotation()`，是否为接口类型`isInterface()`或是否为枚举类型`isEnum()`。可以看到对象包含了哪些字段`getFields()`或有哪些方法`getMethods()`，等等。

### 1.8.5 hashCode() 方法
hashCode() 返回对象的哈希码，它是通过十六进制来表示的对象的内存地址。根据定义，如果两个对象相等，则它们的哈希码也必须相等。如果重写了 equals() 方法，则更改了两个对象是否相等的前提条件，于是对象的 hashCode() 实现不再有效。因此，如果重写了 equals() 方法，也必须重写 hashCode() 方法。

### 1.8.6 toString() 方法
在子类中，通常需要重写 toString() 方法。Object 类的 toString() 方法返回对象的字符串表示，这对于调试非常有用。对象的字符串表示完全依赖于对象，这就是需要在子类中重写 toString() 的原因。可以将 toString() 和 System.out.println() 结合，用于显示对象的字符串表示，如书籍的实例：
```java
System.out.println(firstBook.toString());
```
也可以重写 toString() 方法，打印一些有用的信息，如下所示：
```bash
ISBN: 0201914670; The Swing Tutorial; A Guide to Constructing GUIs, 2nd Edition
```

## 1.9编写 Final 类和方法
可以将类的部分或全部方法声明为 final 方法，在方法声明时如果使用了 final 关键字表示该方法不能被子类重写。Object 类就这样做了，它的一些方法就是 final 方法。

如果一个方法在实现时希望以后不会被改变，并且该方法对于保持对象的一致性非常关键，那么就应该将它声明为 final。例如，将 ChessAlgorithm 类中的 getFirstPlayer 声明为 final 方法：
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
从构造函数中调用的方法通常声明为 final 方法。如果构造函数调用了非 final 方法，子类可能会重新定义该方法，并出现预料之外的结果。还有，也可以将整个类声明为 final 类，声明为 final 的类不能被继承。例如，在创建像 String 这样的类时就这样做了，这一点非常重要。

## 1.10抽象方法和类
抽象类是声明为 abstract 的类，它可能包括抽象方法，也可以不包括抽象方法。抽象类不能实例化，但可以被子类继承。抽象方法在声明的时候不包含具体实现，它不带大括号，后面直接跟分号，如下所示：
```java
abstract void moveTo(double deltaX, double deltaY);
```
如果类中包含了抽象方法，则该类必须声明为抽象类，如下所示：
```java
public abstract class GraphicObject {
	// declare fields
	// declare nonabstract methods
	abstract void draw();
}
```
当抽象类被继承时，子类通常会为父类中的所有抽象方法提供具体的实现。如果没有实现父类中所有的抽象方法，那么子类也必须被声明为抽象类。

> 注意：接口中的方法如果没有被声明为 default 或 static 方法，则隐式为抽象方法。因此， 通常 abstract 修饰符不与接口方法一起使用。也就说可以使用，但是没必要。

### 1.10.1 抽象类和接口
抽象类和接口类似，不能对它们实例化，并且它们在声明时都可能同时包含已实现和没有实现的方法。但是，使用抽象类，可以声明非 static 和 final 的字段，并且定义 public、protected 或 private 的具体方法。对于接口，所有的字段都同时具有 public、static 和 final 属性，并且所有的声明或定义的方法（如 default 方法）都是 public 属性。还有，只能扩展一个类，无论它是否为抽象类，但是可以同时实现多个接口。

关于抽象类和接口，应该按照下面的方式来选择。

- 当遇到下面的情况，需要使用抽象类：
（1）需要在多个密切相关的类之间共享代码；
（2）希望扩展自抽象类的子类拥有许多共同的方法和字段，或者这些成员需要除 public 之外的其它访问修饰符（例如 protected 和 private）；
（3）需要声明非 static 和 final 的字段。这就允许在类中定义一些方法，它们可以访问或修改所属对象的状态。

- 当遇到下面的情况，需要使用接口：
（1）不相关的类来实现接口。例如，接口 Comparable 和 Cloneable 被许多不相关的类所实现；
（2）需要指定特定数据类型的行为，但不关心谁来实现具体的行为；
（3）需要利用类型的多重继承特性。

JDK 中一个抽象类的示例是 AbstractMap，它是 Collections 框架的一部分。它的子类（包括 HashMap，TreeMap 和 ConcurrentHashMap）共享许多在 AbstractMap 中定义的方法（包括 get，put，isEmpty，containsKey 和 containsValue）。

JDK 中实现多个接口的示例是 HashMap 类，它实现了 Serializable、Cloneable 和 Map 接口。通过读取这些接口的方法列表，可以推断 HashMap 的实例可以被克隆，可以被序列化（这意味着它可以转换为字节流；参考 Serializable 对象部分），并且拥有 map 的功能。此外，Map 接口已经通过定义多个 default 方法得到了功能的增强，例如 merge 和 forEach，之前已实现了 Map 接口的旧的类不必重新定义。

注意，许多软件库同时使用了抽象类和接口。例如，HashMap 类实现了多个接口，并且还扩展了 AbstractMap 抽象类。

### 1.10.2 抽象类示例
在面向对象的绘图应用中，可以绘制圆、矩形、线条、贝塞尔曲线和许多其它图形对象。这写对象都具有某些共同的状态（例如：位置、方向、线条颜色、填充颜色）和行为（例如：moveTo，rotate，resize，draw）。某些状态和行为对于所有图形对象来说都是相同的（例如：位置，填充颜色 和 moveTo），其它则需要不同的实现（例如：resize 或 draw）所有的图像对象（GraphicObjects）必须能够绘制（draw）和调整自己大小（resize）；它们的不同之处在于它们是如何做的。这对抽象类来说是完美的场景（情况）。可以利用这些相似性，并声明所有图形对象继承自相同的抽象父对象（例如：GraphicObject），如下图所示：

<center><img src="images/classes-graphicObject.gif" width="70%" height="70%"></center>

<center>继承自 GraphicObject 的 Rectangle，Line，Bezier 和 Circle 类。</center>

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