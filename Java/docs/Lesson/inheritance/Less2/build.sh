javac -Xlint:unchecked -d bin -sourcepath src src/Pegasus.java
java -cp bin Pegasus

rm -rf bin/*
