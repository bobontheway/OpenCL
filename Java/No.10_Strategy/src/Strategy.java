public interface Strategy {
	public abstract Hand nextHand();	// 获取下一局要出的手势
	public abstract void study(boolean win);	// 学习上一局的手势是否获胜了
}
