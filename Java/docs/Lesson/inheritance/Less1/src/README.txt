问题：
找不到 java.time 包，错误日志如下所示
xbdong@ubuntu:~/Project/src/github/OpenCL/Java/docs/Lesson/Less1_2$ sh
build.sh src/SimpleTimeClient.java:1: error: package java.time does not exist
import java.time.*;

1.查看文件文件
which change_to_v1.8.sh 
/usr/local/bin/change_to_v1.8.sh

2.执行文件中命令
export JAVA_HOME=/home/likewise-open/SPREADTRUM/xiaobo.dong/Personal/Installed/jdk/java-8-openjdk-amd64
export PATH=$JAVA_HOME/bin:$JAVA_HOME/jre/bin:$PATH
export CLASSPATH=$CLASSPATH:.:$JAVA_HOME/lib:$JAVA_HOME/jre/lib
