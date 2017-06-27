// 表示 “观察者” 的接口，由子类来实现
public interface Observer {
	public abstract void update(NumberGenerator generator);
}
