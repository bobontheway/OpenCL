package framework;

public abstract class Factory {
	public final Product create(String owner, String id) {
		Product p = createProduct(owner, id);
		registerProduct(p);
		return p;
	}

	// 声明的抽象方法，需要子类实现
	protected abstract Product createProduct(String owner, String id);
	protected abstract void registerProduct(Product product);
}
