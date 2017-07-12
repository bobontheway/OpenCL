javac -Xlint:unchecked -d bin -sourcepath src src/TestBikes.java
java -cp bin TestBikes

rm -rf bin/*
