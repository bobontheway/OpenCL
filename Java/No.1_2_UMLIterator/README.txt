用了不安全操作。比较典型的是：JDK1.5使用集合类，没有使用泛型。 
比如： 
ArrayList list = new ArrayList(); 
list.add( "Element1 "); 搜索
…… 
会报告楼主那种警告信息。 
Java 1.5里面的泛型： 
ArrayList <String> list = new ArrayList <String> (); 
list.add( "Element1 ");
