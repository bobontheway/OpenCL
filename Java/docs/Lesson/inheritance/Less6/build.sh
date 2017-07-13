javac -Xlint:unchecked -d bin -sourcepath src src/Subclass.java
java -cp bin Subclass

rm -rf bin/*
