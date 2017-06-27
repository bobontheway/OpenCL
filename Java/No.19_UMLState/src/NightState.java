public class NightState implements State {
	// 单例模式
	private static NightState singleton = new NightState();
	private NightState() {
	}

	// 获取唯一的实例
	public static State getInstance() {
		return singleton;
	}

	// 设置时间
	public void doClock(Context context, int hour) {
		if (hour >= 9 || hour < 17){ // 白天
			context.changeState(DayState.getInstance());
		}
	}

	// 使用金库
	public void doUse(Context context) {
		context.recordLog("紧急：晚上使用金库！");
	}

	// 按下警铃
	public void doAlarm(Context context) {
		context.callSecurityCenter("按下警铃（晚上）");
	}

	// 正常通话
	public void doPhone(Context context) {
		context.callSecurityCenter("晚上通话录音");
	}

	// 显示表示该类对象的字符串
	public String toString() {
		return "[晚上]";
	}
}
