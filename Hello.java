public class Hello {

	boolean b;
	int i;
	long l;

	Hello() {}

	Hello(boolean b, int i, long l) {
		this.i = i;
		this.b = b;
		this.l = l;
	}

	public void printInfo() {
		System.out.println("b: " + this.b +
			", i: " + this.i + ", l: " + this.l);
	}

	public void printArr(String[] arr) {
		for (int i = 0; i < arr.length; i++) {
			System.out.println(arr[i]);
		}
	}

	public void printArr(int[][] arr) {
		for (int i = 0; i < arr.length; i++) {
			for (int j = 0; j < arr[i].length; j++) {
				System.out.print(arr[i][j]);
			}
			System.out.println();
		}
	}

	public static void main(String[] args) {
		Hello h = new Hello();
		//h.javaCall_rgs();
		h.testLocalRefUpperBound();
		//h.register2native(h);
		//System.out.println("same obj: " + h.checkIsSameObj(h));
		//h.printArr(h.getInt2DArr(3, 1));
		//h.printArr(h.getStrArr(3));
		//int[] arr = {1, 2, 3}; 
		//System.out.println("result of sumArr: " + h.sumArr(arr));
		//h.setArr(arr);
		//System.out.println("result of sumArr: " + h.sumArr(arr));
		//String str = h.getStr();
		//System.out.println("str from cpp: " + str);
		//h.sayHello();
		//System.out.println(h);
		//staticFn();
		//h.passStr2cpp("陈志菲");
	}

// --------- native method -------------

	public native void sayHello();

	public static native void staticFn();

	public native void passStr2cpp(String s);

	public native String getStr();

	public native int sumArr(int[] arr);

	public native void setArr(int[] arr);

	public native String[] getStrArr(int len);

	public native int[][] getInt2DArr(int len1, int len2);

	public native void register2native(Hello h);

	public native boolean checkIsSameObj(Hello h);

	public native void testLocalRefUpperBound();

	public native void javaCall_rgs();

	static {
		System.loadLibrary("hello");
	}
}
