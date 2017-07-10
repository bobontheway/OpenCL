javac -Xlint:unchecked -d bin -sourcepath src src/TestSimpleTimeClient.java
java -cp bin TestSimpleTimeClient

rm -rf bin/*
