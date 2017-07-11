javac -Xlint:unchecked -d bin -sourcepath src src/Cat.java
java -cp bin Cat

rm -rf bin/*
