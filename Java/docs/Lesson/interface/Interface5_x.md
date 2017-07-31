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