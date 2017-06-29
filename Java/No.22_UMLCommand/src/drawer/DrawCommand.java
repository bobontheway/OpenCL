package drawer;

import command.Command;
import java.awt.Point;

// 实现了 Command 接口，表示绘制一个点的命令
public class DrawCommand implements Command {
	// 绘制对象
	protected Drawable drawable;
	// 绘制位置
	private Point position;
	// 构造函数
	public DrawCommand(Drawable drawable, Point position) {
		this.drawable = drawable;
		this.position = position;
	}

	// 执行
	public void execute() {
		drawable.draw(position.x, position.y);
	}
}
