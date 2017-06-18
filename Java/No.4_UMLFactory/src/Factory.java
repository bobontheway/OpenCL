public abstract class Factory {
	public final Product create(String owner) {
		Product p = createProduct(owner);
		registerProduct(p);
		return p;
	}

	// 声明的抽象方法，需要子类实现
	protected abstract Product createProduct(String owner);
	protected abstract void registerProduct(Product product);
}
