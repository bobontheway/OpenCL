javac -Xlint:unchecked -d bin -sourcepath src src/Main.java
java -cp bin Main

rm -rf bin/*.class
