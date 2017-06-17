习题 2-2

关于 Properties 类，继承自 Hashtable<Object, Object>

类 Properties 表示一组持久属性，该属性可以保存到流中，或者从流加载。
每个键值（key）和它对应的值（value）在属性列表中是以字符串表示。

一个属性列表可以包含其它的属性列表来作为它的 “defaults”；如果属性的
键值（key）在原始的属性列表中没有找到，那么将搜索第二个属性列表。

由于 Properties 继承自 Hashtable，put 和 putAll 方法可以应用到
Properties 对象中。强烈不建议这么做，这是由于它们允许调用调用者插入
键值和值不是字符串的表项，应该使用 setProperty 方法来代替。如果 store
或 save 方法在包含一个非字符串的键或值的 “compromised”  Properties 对象上调用，
那么该调用将失败。类似的，在调用 propertyNames 或者列表方法时，如果被调用的
“compromised” 属性对象包含了一个非字符串的键值，将调用失败。

load(Reader)/store(Write, String)方法，加载或存储属性从一个字符流中，格式如下。
...	...

