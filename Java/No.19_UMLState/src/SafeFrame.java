import java.awt.Frame;
import java.awt.Color;
import java.awt.Button;
import java.awt.TextField;
import java.awt.TextArea;
import java.awt.BorderLayout;
import java.awt.Panel;

public class SafeFrame extends Frame implements ActionListener, Context {
	private TextField textClock = new TextField(60); // 显示当前时间
	private TExtArea textScreen = new TextArea(10, 60); // 显示警报中心的记录
	private Button buttonUse = new Button("使用金库"); // 使用金库按钮
	private Button buttonAlarm = new Button("按下警铃"); // 按下警铃按钮
	private Button buttonPhone = new Button("正常通话"); // 正常通话按钮
	private Button buttonExit = new Button("结束"); // 结束按钮

	// 当前的状态，默认为白天
	private State state = DayState.getInstance();

	public SafeFrame(String tittle) {
		// 包含标题和边框的顶层窗口
		super(title);
		// 设置背景颜色，灰色
		setBackground(Color.lightGray);

		// 设置布局格式
		setLayout(new BorderLayout);

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

}
