习题 7-2
修改 Builder 类、TextBuilder 类和 HTMLBuilder 类，确保在调用
makeString 方法、makeItems 方法和 close 方法之前必须且只能调
用一次 makeTitle 方法。

问题：
0.确保该怎么理解？是报错还是需要执行？
  - 报错即可。如果不满足下面的条件就报错；
1.怎么确保 makeTitle 在调用其它函数之前调用？
  - 查看静态变量的值，不能为 0；
2.怎么确保 makeTitle 函数只能调用一次？
  - 加个静态变量来指定，静态变量初始值为 0；

实际解答：
（1）定义一个私有变量，用来控制各个函数是否有调用。
（2）私有变量和私有静态变量，在继承的子类中有何区别？即，如果
将私有变量定义为私有静态变量是否会影响程序的执行结果？即，静态
是否会在子类之间共享？