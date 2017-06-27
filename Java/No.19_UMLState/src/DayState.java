public class DayState implements State {
	// 单例模式
	private static DayState singleton = new DayState();
	private DayState() {
	}

	// 获取唯一的实例
	public static State getInstance() {
		return singleton;
	}

	// 设置时间
	public void doClock(Context context, int hour) {
		if (hour < 9 || hour >= 17){ // 晚上
			context.changeState(NightState.getInstance());
		}
	}

	// 使用金库
	public void doUse(Context context) {
		context.recordLog("使用金库（白天）");
	}

	// 按下警铃
	public void doAlarm(Context context) {
		context.callSecurityCenter("按下警铃（白天）");
	}

	// 正常通话
	public void doPhone(Context context) {
		context.callSecurityCenter("正常通话（白天）");
	}

	// 显示表示该类对象的字符串
	public String toString() {
		return "[白天]";
	}
}
