javac -Xlint:unchecked -d bin -sourcepath src src/Dragon.java
java -cp bin Dragon

rm -rf bin/*
