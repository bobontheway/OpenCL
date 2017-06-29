import command.*;
import drawer.*;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

//public class Main {
public class Main extends JFrame implements ActionListener,
	   MouseMotionListener, WindowListener {
	// 绘制的历史记录
	private MacroCommand history = new MacroCommand();
	// 绘制区域（实现绘制对象的类）
	private DrawCanvas canvas = new DrawCanvas(400, 400, history);
	// 删除按钮
	private JButton clearButton = new JButton("clear");

	// 构造函数
	public Main(String title) {
		super(title);

		// 添加指定的窗口监听器，以便从此窗口接收窗口事件（Window）
		this.addWindowListener(this);
		// 添加指定的鼠标移动监听器，以接收此组件中的鼠标移动事件
		canvas.addMouseMotionListener(this);
		// 为按钮添加监听器
		clearButton.addActionListener(this);

		// 一个轻量级的容器，它使用 BoxLayout 对象作为其布局管理器
		// 创建一个框，它沿着 Y 坐标轴显示其组件
		Box buttonBox = new Box(BoxLayout.Y_AXIS);
		buttonBox.add(clearButton);

		Box mainBox = new Box(BoxLayout.Y_AXIS);
		mainBox.add(buttonBox);
		mainBox.add(canvas);

		// 返回该 frame 的 contentPane 对象（JFrame）
		// contentPane 对象是一个 Container 的容器
		getContentPane().add(mainBox);

		// 使窗口调整大小，以适合其子组件的首选大小和布局
		pack();
		// 让窗口可见（已废弃，使用 setVisible 来代替）
		//show();
		setVisible(true);
	}

	// ActionListener 接口中的方法
	public void actionPerformed(ActionEvent e) {
		if (e.getSource() == clearButton) {
			history.clear();
			// 重新绘制该组建
			canvas.repaint();
		}
	}

	// MouseMotionListener 接口中的方法
	// 当鼠标光标移动到组件上，但是没有按下按钮是调用
	public void mouseMoved(MouseEvent e) {
	}

	// 当按下组件上的鼠标按钮然后拖动时调用
	public void mouseDragged(MouseEvent e) {
		Command cmd = new DrawCommand(canvas, e.getPoint());
		history.append(cmd);
		cmd.execute();
	}

	// WindowListener 接口中的方法
	public void windowClosing(WindowEvent e) {
		System.exit(0);
	}

	public void windowActivated(WindowEvent e) {}
	public void windowClosed(WindowEvent e) {}
	public void windowDeactivated(WindowEvent e) {}
	public void windowDeiconified(WindowEvent e) {}
	public void windowIconified(WindowEvent e) {}
	public void windowOpened(WindowEvent e) {}

	public static void main(String[] args) {
		new Main("Command Pattern Sample");
	}
}

