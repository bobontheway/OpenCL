## 主机 github 的 ssh 环境配置
### 1.生成新的 ssh key
尝试使一台机器包含多个 `ssh key`，使其不冲突。新建 `github` 目录，生成 `ssh key`，用于 `github`。相关命令如下：
```bash
xbdong@ubuntu:~/.ssh/github$ ssh-keygen -t rsa -b 4096 -C "xiaobo_dong@yeah.net"
```
### 2.添加 ssh key
将新生成的 `ssh key` 添加到 `ssh-agent`。在每次重启系统或切换终端时都要执行该动作。
####（1）启动 ssh-agent
```bash
xbdong@ubuntu:~/.ssh/github$ eval "$(ssh-agent -s)"
```
####（2）添加 ssh key
```bash
xbdong@ubuntu:~/.ssh/github$ ssh-add /home/xiaobo.dong/.ssh/github/id_rsa
```
####（3）添加 key 到 github

### 3.测试 ssh 连接
```bash
ssh -T git@github.com
```

## 服务器 github 环境配置

将 Host 的 `ssh key` 拷贝到服务器上，在服务器上的同一终端执行如下命令，也可访问。相关命令如下

### 1.拷贝 ssh key 到 Server
```bash
bob@ubuntu:~/.ssh$ scp github/* bobdong@server:~/Project/src/graphics/github/
```
### 2.启动 ssh-agent
```bash
bobdong@server:~/Project/src/graphics/github$ eval "$(ssh-agent -s)"
Agent pid 15949
```

### 3.添加 ssh key
```bash
bobdong@server:~/Project/src/graphics/github$ ssh-add ./id_rsa
Identity added: ./id_rsa (./id_rsa)
```

### 4.测试连接
```bash
bobdong@server:~/Project/src/graphics/github$ ssh -T git@github.com
Hi bobontheway! You've successfully authenticated, but GitHub does not provide shell access.
```

## 参考
- https://help.github.com/articles/connecting-to-github-with-ssh/
