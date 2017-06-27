// 负责管理状态和联系警报中心的接口
public interface Context {
	public abstract void setClock(int hour);
	public abstract void changeState(State state);
	public abstract void callSecurityCenter(String string);
	public abstract void recordLog(String string);
}
