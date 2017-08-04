// 接口 Iterator 用于遍历集合中的元素，其作用相当于循环语句中的循环变量。
// 该接口声明了两个抽象方法，分别为 hasNext 和 next。
public interface Iterator {
	boolean hasNext();
	Object next();
}
