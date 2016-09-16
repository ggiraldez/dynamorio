package sample;

public class First {
    static {
        System.loadLibrary("first");
    }

    public native int findMax(int x, int y);

    public void callIntoNative() {
        int result = findMax(10, 20);
        System.out.println("findMax(10,20) = " + result);

        result = findMax(10,10);
        System.out.println("findMax(10,10) = " + result);

        result = findMax(20,10);
        System.out.println("findMax(20,10) = " + result);
    }

    public static void main(String args[]) {
        System.out.println("Hello from Java");

        First f = new First();
        f.callIntoNative();
    }

    public static void basicBlockCallback(long tag) {
        System.out.println("basicBlockCallback " + tag);
    }
}
