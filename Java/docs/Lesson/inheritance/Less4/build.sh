javac -Xlint:unchecked -d bin -sourcepath src src/Mustang.java
java -cp bin Mustang

rm -rf bin/*
