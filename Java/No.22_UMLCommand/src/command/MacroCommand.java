package command;

import java.util.Stack;
import java.util.Iterator;

// 表示由多条命令合成的命令
public class MacroCommand implements Command {
	// 命令的集合，存放到堆栈中
	private Stack<Command> commands = new Stack<Command>();

	// 执行命令（通过迭代器遍历命令）
	public void execute() {
		Iterator it = commands.iterator();
		while (it.hasNext()) {
			((Command)it.next()).execute();
		}
	}

	// 添加命令
	public void append(Command cmd) {
		if (cmd != this) { // 防止进入无限循环
			commands.push(cmd);
		}
	}

	// 删除最后一条命令
	public void undo() {
		if (!commands.empty()) { // 命令队列不为空
			commands.pop();
		}
	}

	// 删除所有命令
	public void clear() {
		commands.clear();
	}
}
