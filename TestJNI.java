// TestJNI.java
public class TestJNI {
    static {
        // Загружаем библиотеку из папки jni_build
        System.load("/home/fr1zb3e/SeniorShop/jni_build/libnative.so");
    }
    
    // Имена методов должны совпадать с теми, что в JNI-библиотеке
    public static native boolean init(String dataDir);
    public static native void cleanup();
    public static native boolean login(String username, String password);
    public static native String getAllProducts();
    public static native String getUserRole();
    public static native String getCurrentUser();
    public static native boolean addProduct(String name, String category,
                                             double price, int article,
                                             String beginDate, String endDate,
                                             int count, String country,
                                             String supplier);
    public static native boolean deleteProduct(int article);
    public static native boolean updateProductCount(int article, int newCount);
    public static native boolean updateProductPrice(int article, double newPrice);
    public static native boolean addToCart(int article, int count);
    public static native double getCartTotal();
    public static native void clearCart();
    public static native boolean applySale(double finalTotal, String employee);
    
    public static void main(String[] args) {
        System.out.println("=== Тестирование JNI ===\n");
        
        String dir = System.getProperty("user.dir");
        System.out.println("Working dir: " + dir);
        
        // Инициализация
        boolean initResult = init(dir);
        System.out.println("Init: " + initResult);
        
        if (!initResult) {
            System.err.println("Init failed!");
            cleanup();
            return;
        }
        
        // Тест авторизации
        System.out.println("\n--- Login test ---");
        boolean loginResult = login("SuperAdmin", "admin***123");
        System.out.println("Login result: " + loginResult);
        
        if (loginResult) {
            System.out.println("Current user: " + getCurrentUser());
            System.out.println("User role: " + getUserRole());
        }
        
        // Тест получения товаров
        System.out.println("\n--- Products test ---");
        String products = getAllProducts();
        System.out.println("Products: " + products);
        
        // Очистка
        cleanup();
        System.out.println("\n=== Test completed ===");
    }
}
