package com.seniorshop.bot;

public class NativeLib {
    static {
        String[] possiblePaths = {
            "/app/libs/libnative.so",      
            "/app/build/libnative.so",     
            "/app/libnative.so",           
            "./libnative.so"               
        };
        
        boolean loaded = false;
        for (String libPath : possiblePaths) {
            try {
                System.load(libPath);
                System.out.println("JNI loaded: " + libPath);
                loaded = true;
                break;
            } catch (UnsatisfiedLinkError e) {
                System.err.println("Failed to load: " + libPath);
            }
        }
        
        if (!loaded) {
            System.err.println("Could not load native library from any path");
            System.err.println("   Check that libnative.so exists in the container");
        }
    }
    
    public static native boolean init(String dataDir);
    public static native void cleanup();
    public static native boolean login(String username, String password);
    public static native String getAllProducts();
    public static native String getFinancialReport();
    public static native String getAllSupplies();
    public static native boolean applySupply(int supplyNumber);
    public static native boolean addToCart(int article, int count);
    public static native double getCartTotal();
    public static native void clearCart();
    public static native boolean applySale(double finalTotal, String employee);
}
