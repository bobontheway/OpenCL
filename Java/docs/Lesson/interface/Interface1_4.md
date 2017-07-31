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
public 访问修饰符表示接口可以被任意的类访问，和类所在的包无关。如果接口的访问修饰符没有指定为 public，那么接口只能被同一包中定义的类访问。

接口可以扩展自其它接口，就像一个类从其它的类扩展一样。然而，类只能从单个类扩展，但接口可以从任意数量的接口扩展。接口声明包括一个逗号分隔的列表，列出了它所扩展的所有接口。

### 1.1.1接口体
接口体是大括号中包含的部分。它可以包含抽象方法、默认方法和静态方法。接口中的抽象方法以分号结尾（不含大括号，因为抽象方法不包含具体实现）；默认方法定义时使用了 `default` 修饰符，同时静态方法使用了 `static` 关键字。接口中所有的抽象、默认和静态方法隐式使用了 public 访问修饰符，因此在声明时可以省略 public 修饰符。

还有，接口中可以包含常量声明。接口中定义的所有常量隐式使用了 public、static 和 final 修饰，这就可以省略这些修饰符。

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
		else if (this.getArea() > otherRect.getArea()) // ② this 表示 isLargerThan 所属对象
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

注意，在 Relatable 接口中声明的方法，包含了一个 Relatable 类型的对象作为参数。上面示例中位置 ① 的代码行将 other 转换为 RectanglePlus 实例，类型转换告诉编译器真正的对象是什么。直接在 other 实例上调用 getArea 方法（例如，other.getArea()）将编译失败，这是因为编译器并不知道 other 实际上是 RectanglePlus 类的实例。

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

## 1.5 Default 方法
若需要向现有的接口添加新的方法，default 方法允许向库的接口中添加新功能的同时，确保曾经为这些旧的接口版本编写的代码具有二进制兼容性，也就是说那些类不需要修改或重新编译照样可以正常工作。

考虑下面的 TimeClient 接口：
```java
import java.time.*;

public interface TimeClient {
	void setTime(int hour, int minute, int second);
	void setDate(int day, int month, int year);
	void setDateAndTime(int day, int month, int year,
		int hour, int minute, int second);
	LocalDateTime getLocalDateTime();
}
```
SimpleTimeClient 类实现了 TimeClient 接口：
```java
import java.time.*;
import java.lang.*;
import java.util.*;

public class SimpleTimeClient implements TimeClient {
	private LocalDateTime dateAndTime;

	public SimpleTimeClient() {
		dateAndTime = LocalDateTime.now();
	}

	public void setTime(int hour, int minute, int second) {
		LocalDate currentDate = LocalDate.from(dateAndTime);
		LocalTime timeToSet = LocalTime.of(hour, minute, second);
		dateAndTime = LocalDateTime.of(currentDate, timeToSet);
	}

	public void setDate(int day, int mouth, int year) {
		LocalDate dateToSet = LocalDate.of(day, mouth, year);
		LocalTime currentTime = LocalTime.from(dateAndTime);
		dateAndTime = LocalDateTime.of(dateToSet, currentTime);
	}

	public void setDateAndTime(int day, int mouth, int year,
		int hour, int minute, int second) {
		LocalDate dateToSet = LocalDate.of(day, mouth, year);
		LocalTime timeToSet = LocalTime.of(hour, minute, second);
		dateAndTime = LocalDateTime.of(dateToSet, timeToSet);
	}

	public LocalDateTime getLocalDateTime() {
		return dateAndTime;
	}

	public String toString() {
		return dateAndTime.toString();
	}

	public static void main(String... args) {
		TimeClient myTimeClient = new SimpleTimeClient();
		System.out.println(myTimeClient.toString());
	}
}
```
（相关代码见 Less1_2）

若要向 TimeClient 接口中添加新的功能。例如，通过使用 ZonedDateTime 对象（和 LocalDateTime 对象类似，除了它存储的是时区信息）来获取时区的能力。
```java
public interface TimeClient {
	void setTime(int hour, int minute, int second);
	void setDate(int day, int month, int year);
	void setDateAndTime(int day, int month, int year,
		int hour, int minute, int second);
	LocalDateTime getLocalDateTime();
	ZoneDateTime getZonedDateTime(String zoneString);
}
```
（相关代码见 Less1_3）
在对 TimeClient 接口进行修改之后，还必须修改 SimpleTimeClient 类并实现 getZonedDateTime 方法。但是，除了将 getZonedDateTime 定义为抽象方法（在上面的示例中），还可以将它定义为 default 方法的实现（注意，抽象方法是在没有具体实现的情况下声明的方法）。
```java
import java.time.*;

public interface TimeClient {
	void setTime(int hour, int minute, int second);
	void setDate(int day, int month, int year);
	void setDateAndTime(int day, int month, int year,
		int hour, int minute, int second);
	LocalDateTime getLocalDateTime();

	static ZoneId getZoneId(String zoneString) {
		try {
			return ZoneId.of(zoneString);
		} catch (DateTimeException e) {
			System.err.println("Invalid time zone: " +
				zoneString + "; using default time zone instead.");
			return ZoneId.systemDefault();
		}
	}

	default ZonedDateTime getZonedDateTime(String zoneString) {
		return ZonedDateTime.of(getLocalDateTime(), getZoneId(zoneString));
	}
}
```
在方法签名的开头使用 default 关键字，可以将接口中定义的方法指定为默认方法。接口中声明的所有方法（包括 default 方法）隐式为 public，因此可以省略 public 访问修饰符。

使用了此接口，就不必修改 SimpleTimeClient 类，而此类（以及实现了 TimeClient 接口的任何类）将包含已定义的 getZonedDateTime 方法。下面的示例中，TestSimpleTimeClient 通过 SimpleTimeClient 实例调用了 getZonedDateTime 方法。
```java
public class {
	public static void main(String[] args) {
		TimeClient myTimeClient = new SimpleTimeClient();
		System.out.println("Current time: " + myTimeClient);
		System.out.println("Time in California: " +
			myTimeClient.getZonedDateTime("Blah blah").toString());
	}
}
```

### 1.5.1扩展包含 default 方法的接口
在扩展包含 default 方法的接口时，可以执行下面的这些操作：

- 不必再管 default 方法，这可以让扩展的接口继承 default 方法；
- 重新声明 default 方法，让它变为抽象方法（abstract）；
- 重新定义 default 方法，执行重写操作。

假如按照如下的方式扩展 TimeClient 接口：
```java
public interface AnotherTimeClient extends TimeClient {
}
```

任何实现 AnotherTimeClient 接口的类，都将包含 TimeClient.getZonedDateTime 的 default 方法实现。

假设按照如下的方式扩展 TimeClient 接口：
```java
public interface AbstractZoneTimeClient extends TimeClient {
	public ZonedDateTime getZonedDateTime(String zoneString);
}
```

任何实现 AbstractZoneTimeClient 接口的类都必须实现 getZonedDateTime 方法。因为它重新声明为抽象方法，这与接口中其它的 nondefault 和 nonstatic 方法类似。

假设按照如下的方式扩展了 TimeClient 接口：
```java
public interface HandleInvalidTimeZoneClient extends TimeClient {
	default public ZonedDateTime getZonedDateTime(String zoneString) {
		try {
			return ZonedDateTime.of(getLocalDateTime(),
				ZoneId.of(zoneString));
		} catch (DateTimeException e) {
			System.err.println("Invalid zone ID: " + zoneString +
				"; using the default time zone instead.");
			return ZonedDateTime.of(getLocalDateTime(), ZoneId.systemDefault());
		}
	}
}
```

任何实现 HandleInvalidTimeZoneClient 的类都将使用此接口中指定的实现，而不是由 TimeClient 指定的实现，这重写了 TimeClient 中的实现。

### 1.5.2静态方法
除了 default 方法外，还可以在接口中定义 static 方法。静态方法是与定义它的类相关联的方法，而不是与对象关联。类的每个实例都共享其静态方法。这可以在同一接口中保留特定于接口的静态方法，而不是在单独的类中。下面的示例定义了一个 static 方法，它检索与时区标识符相对应的 ZoneId 对象。如果没有与特定标识符对应的 ZoneId 对象，则使用系统默认的时区（因此，可以简化 getZonedDateTime 方法）：
```java
public interface TimeClient {
	// ...
	static public ZoneId getZoneId(String zoneString) {
		try {
			return ZoneId.of(zoneString);
		} catch (DateTimeException e) {
			System.err.println("Invalid time zone: " +
				zoneString + "; using default time zone
				instead.");
			return ZoneId.systemDefault();
		}
	}

	default ZoneDateTime getZonedDateTime(String zoneString) {
		return ZonedDateTime.of(getLocalDateTime(), getZoneId(zoneString));
	}
}
```

和类中的 static 方法类似，可以在方法签名的开头添加 static 关键字，以指定接口中定义的方法为 static 方法。所有在接口中声明的方法，包括 static 方法，默认状态为 public，因此可以省略 public 修饰符。

## 1.6接口总结
接口声明时，接口体可以包含抽象方法、默认方法、静态方法和常量定义，其中只有 default 和 static 方法需要实现。实现接口的类必须实现接口中声明的所有方法。接口名称可以用于使用类型的任何地方。






























