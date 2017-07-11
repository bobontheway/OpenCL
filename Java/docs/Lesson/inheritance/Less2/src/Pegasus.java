public class Pegasus extends Horse implements Flyer, Mythical {
    public static void main(String[] args) {
        Pegasus myApp = new Pegasus(); // 飞，天马
        System.out.println(myApp.identifyMyself());
    }
}
