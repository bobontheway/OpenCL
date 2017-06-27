import java.util.ArrayList;
import java.util.Iterator;

public abstract class NumberGenerator {
	private ArrayList<Observer> observers = new ArrayList<Observer>();

	// 注册观察者对象
	public void addObserver(Observer observer) {
		observers.add(observer);
	}

	// 删除观察者对象
	public void deleteObserver(Observer observer) {
		observers.remove(observer);
	}

	// 向观察者发送通知
	public void notifyObservers() {
		Iterator it = observers.iterator();
		while (it.hasNext()) {
			Observer o = (Observer)it.next();
			o.update(this);
		}
	}

	// 抽象方法，需要子类去实现
	public abstract int getNumber(); // 获取数值 
	public abstract void execute(); // 生成数值 
}
