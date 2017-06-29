package drawer;

import command.*;

import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

// 实现了 Drawable 接口，它是 Canvas 的子类。实现绘制对象的类。
// 画布（Canvas）组件表示屏幕上的一个空白矩形区域，应用程序可以
// 在这个举行区域上进行绘制；或者在这个区域上，应用程序可以从用
// 户那里捕获输入事件
// 组件（Componet）是具有图形表示的对象，它可以在屏幕上显示，并
// 可以和用户交互。组件的示例是典型图形用户界面的按钮、复选框和
// 滚动条
public class DrawCanvas extends Canvas implements Drawable {
	// 颜色
	private Color color = Color.red;
	// 要绘制的原点的半径
	private int radius = 6;
	// 命令的历史记录
	private MacroCommand history;

	// 构造函数
	public DrawCanvas(int width, int height, MacroCommand history) {
		setSize(width, height);
		setBackground(Color.white);
		this.history = history;
	}

	// 重新全部绘制。重写了父类的方法
	public void paint(Graphics g) {
		history.execute();
	}

	// 绘制
	public void draw(int x, int y) {
		Graphics g = getGraphics();
		// 指定图形上下文的当前颜色。随后使用该图形上下文的图形操作使
		// 用该指定的颜色
		g.setColor(color);
		// 绘制椭圆的轮廓。结果是适合于x、y、width 和 height 参数指定
		// 的矩形内的圆或椭圆
		g.fillOval(x - radius, y - radius, radius * 2, radius * 2);
	}
}
