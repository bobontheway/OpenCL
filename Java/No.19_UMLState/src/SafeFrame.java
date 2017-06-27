import java.awt.Frame;
import java.awt.Color;
import java.awt.Button;
import java.awt.TextField;
import java.awt.TextArea;
import java.awt.BorderLayout;
import java.awt.Panel;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

public class SafeFrame extends Frame implements ActionListener, Context {
	private TextField textClock = new TextField(60); // 显示当前时间
	private TextArea textScreen = new TextArea(10, 60); // 显示警报中心的记录
	private Button buttonUse = new Button("使用金库"); // 使用金库按钮
	private Button buttonAlarm = new Button("按下警铃"); // 按下警铃按钮
	private Button buttonPhone = new Button("正常通话"); // 正常通话按钮
	private Button buttonExit = new Button("结束"); // 结束按钮

	// 当前的状态，默认为白天
	private State state = DayState.getInstance();

	public SafeFrame(String title) {
		// 包含标题和边框的顶层窗口
		super(title);
		// 设置背景颜色，灰色
		setBackground(Color.lightGray);

		// 设置布局格式
		setLayout(new BorderLayout());

		add(textClock, BorderLayout.NORTH); // 配置 textClock
		textClock.setEditable(false);

		add(textScreen, BorderLayout.CENTER); // 配置 textScreen
		textScreen.setEditable(false);

		// 为界面添加按钮，Panel 是最简单的容器类
		Panel panel = new Panel();
		panel.add(buttonUse);
		panel.add(buttonAlarm);
		panel.add(buttonPhone);
		panel.add(buttonExit);

		// 配置界面
		add(panel, BorderLayout.SOUTH);

		// 显示
		pack();
		show();

		// 设置监听器
		buttonUse.addActionListener(this);
		buttonAlarm.addActionListener(this);
		buttonPhone.addActionListener(this);
		buttonExit.addActionListener(this);
	}

	// 按钮被按下后该方法会被调用
	public void actionPerformed(ActionEvent e) {
		System.out.println(e.toString());

		if (e.getSource() == buttonUse) { // 金库使用按钮
			state.doUse(this);
		} else if (e.getSource() == buttonAlarm) { // 按下警铃按钮
			state.doAlarm(this);
		} else if (e.getSource() == buttonPhone) { // 正常通话按钮
			state.doPhone(this);
		} else if (e.getSource() == buttonExit) { // 结束按钮
			System.exit(0);
		} else {
			System.out.println("?");
		}
	}

	// 设置时间
	public void setClock(int hour) {
		String clockstring = "现在时间是";

		if (hour < 10) {
			clockstring += "0" + hour + ":00";
		} else {
			clockstring += hour + ":00";
		}

		System.out.println(clockstring);
		textClock.setText(clockstring);
		state.doClock(this, hour);
	}

	// 改变状态
	public void changeState(State state) {
		System.out.println("从" + this.state + "状态变为了" + state +
			"状态。");
		this.state = state;
	}

	// 联系警报中心
	public void callSecurityCenter(String msg) {
		textScreen.append("Call!" + msg + "\n");
	}

	// 在警报中心留下记录
	public void recordLog(String msg) {
		textScreen.append("record ... " + msg + "\n");
	}
}
