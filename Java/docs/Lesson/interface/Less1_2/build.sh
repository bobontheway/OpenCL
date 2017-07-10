javac -Xlint:unchecked -d bin -sourcepath src src/SimpleTimeClient.java
java -cp bin SimpleTimeClient

rm -rf bin/*
