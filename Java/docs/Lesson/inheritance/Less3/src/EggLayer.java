public interface EggLayer extends Animal { // 蛋
	default public String identifyMyself() {
		return "I am able to lay eggs.";
	}
}


