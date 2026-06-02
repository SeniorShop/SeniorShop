package com.seniorshop.bot;

public class NativeLib {
    static {
        String os = System.getProperty("os.name").toLowerCase();
        String libPath;
        
        if (os.contains("win")) {
            libPath = "/app/libs/native.dll"; 
        } else {
            libPath = "/app/libs/libnative.so";
        }
        
        try {
            System.load(libPath);
            System.out.println("JNI loaded: " + libPath);
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Failed to load: " + e.getMessage());
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
