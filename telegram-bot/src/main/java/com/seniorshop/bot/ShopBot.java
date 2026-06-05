package com.seniorshop.bot;

import org.telegram.telegrambots.bots.TelegramLongPollingBot;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;
import org.telegram.telegrambots.meta.api.objects.Update;
import org.telegram.telegrambots.meta.exceptions.TelegramApiException;

public class ShopBot extends TelegramLongPollingBot {
    
    private final String botToken;
    private final String botUsername;
    private boolean initialized = false;
    
    public ShopBot(String token, String username) {
        this.botToken = token;
        this.botUsername = username;
    }
    
    @Override
    public void onUpdateReceived(Update update) {
        if (!update.hasMessage() || !update.getMessage().hasText()) return;
        
        String command = update.getMessage().getText();
        long chatId = update.getMessage().getChatId();
        String userName = update.getMessage().getFrom().getFirstName();
        
        if (!initialized) {
            String dataDir = System.getProperty("user.dir");
            boolean ok = NativeLib.init(dataDir);
            initialized = ok;
            sendMessage(chatId, ok ? "Система готова, " + userName : "Ошибка инициализации");
            if (!ok) return;
        }
        
        String response = handleCommand(command);
        sendMessage(chatId, response);
    }
    
    private String handleCommand(String command) {
        if (command.equals("/start")) {
            return "Добро пожаловать в магазин!\n\n" +
                   "Команды:\n" +
                   "/products - список товаров\n" +
                   "/addproduct - добавить товар\n" +
                   "/report - финансовый отчёт\n" +
                   "/supplies - список поставок\n" +
                   "/addsupply - создать поставку\n" +
                   "/apply N - применить поставку №N\n" +
                   "/help - помощь";
        }
        
        if (command.startsWith("/addproduct|")) {
            String[] parts = command.split("\\|");
            
            System.out.println("Add product - parts length: " + parts.length);
            for (int i = 0; i < parts.length; i++) {
                System.out.println("  parts[" + i + "] = '" + parts[i] + "'");
            }
            
            if (parts.length != 9) {
                return "Неверный формат. Ожидается 9 полей, получено: " + parts.length + 
                       "\n\nИспользуйте:\n/addproduct|Название|Категория|Цена|Артикль|Годен до|Количество|Страна|Поставщик\n\n" +
                       "Пример:\n/addproduct|Кола|Напитки|99.99|1001|31.12.2026|50|США|Coca-Cola";
            }
            
            try {
                String name = parts[1];
                String category = parts[2];
                double price = Double.parseDouble(parts[3]);
                int article = Integer.parseInt(parts[4]);
                String endDate = parts[5];
                int count = Integer.parseInt(parts[6]);
                String country = parts[7];
                String supplier = parts[8];
                String beginDate = java.time.LocalDate.now().format(java.time.format.DateTimeFormatter.ofPattern("dd.MM.yyyy"));
                
                System.out.println("Adding product: name=" + name + ", article=" + article + ", price=" + price);
                
                boolean result = NativeLib.addProduct(name, category, price, article, beginDate, endDate, count, country, supplier);
                return result ? "Товар '" + name + "' успешно добавлен!" : "Ошибка добавления товара. Возможно, артикль " + article + " уже существует.";
            } catch (NumberFormatException e) {
                return "Ошибка: неверный формат числа (цена, артикль или количество). " + e.getMessage();
            } catch (Exception e) {
                return "Ошибка: " + e.getMessage();
            }
        }
        
        if (command.equals("/addproduct")) {
            return "Формат добавления товара:\n\n" +
                   "/addproduct|Название|Категория|Цена|Артикль|Годен до|Количество|Страна|Поставщик\n\n" +
                   "Пример:\n/addproduct|Кола|Напитки|99.99|1001|31.12.2026|50|США|Coca-Cola";
        }

        if (command.equals("/products")) {
            return NativeLib.getAllProducts();
        }

        if (command.equals("/report")) {
            return NativeLib.getFinancialReport();
        }

        if (command.equals("/supplies")) {
            return NativeLib.getAllSupplies();
        }

        if (command.startsWith("/addsupply|")) {
            String[] parts = command.split("\\|");
            
            System.out.println("Add supply - parts length: " + parts.length);
            
            if (parts.length != 12) {
                return "Неверный формат. Ожидается 12 полей, получено: " + parts.length + 
                       "\n\nИспользуйте:\n/addsupply|Номер|Поставщик|Ответственный|Товар|Категория|Цена|Артикль|Начало срока|Конец срока|Количество|Страна";
            }
            
            try {
                int number = Integer.parseInt(parts[1]);
                String supplier = parts[2];
                String responsible = parts[3];
                String productName = parts[4];
                String category = parts[5];
                double price = Double.parseDouble(parts[6]);
                int article = Integer.parseInt(parts[7]);
                String beginDate = parts[8];
                String endDate = parts[9];
                int quantity = Integer.parseInt(parts[10]);
                String country = parts[11];
                
                System.out.println("Creating supply: #" + number + ", product=" + productName);
                
                boolean result = NativeLib.createSupply(number, supplier, responsible, productName, category, price, article, beginDate, endDate, quantity, country);
                return result ? "Поставка #" + number + " успешно создана!" : "Ошибка создания поставки";
            } catch (NumberFormatException e) {
                return "❌ Ошибка: неверный формат числа. " + e.getMessage();
            } catch (Exception e) {
                return "❌ Ошибка: " + e.getMessage();
            }
        }
        
        // ========== СПРАВКА ПО СОЗДАНИЮ ПОСТАВКИ ==========
        if (command.equals("/addsupply")) {
            return "Формат создания поставки:\n\n" +
                   "/addsupply|Номер|Поставщик|Ответственный|Товар|Категория|Цена|Артикль|Начало срока|Конец срока|Количество|Страна\n\n" +
                   "Пример:\n/addsupply|1|Coca-Cola|Иванов|Кола|Напитки|99.99|1001|01.06.2026|31.12.2026|50|США";
        }
        
        // ========== ПРИМЕНЕНИЕ ПОСТАВКИ ==========
        if (command.startsWith("/apply ")) {
            try {
                int num = Integer.parseInt(command.substring(7));
                boolean result = NativeLib.applySupply(num);
                return result ? "Поставка #" + num + " применена" : "Поставка #" + num + " не найдена";
            } catch (NumberFormatException e) {
                return "❌ Неверный формат. Используйте: /apply 123";
            }
        }
        
        if (command.equals("/help")) {
            return "Доступные команды:\n\n" +
                   "/start - приветствие\n" +
                   "/products - список товаров\n" +
                   "/addproduct - добавить товар\n" +
                   "/report - финансовый отчёт\n" +
                   "/supplies - список поставок\n" +
                   "/addsupply - создать поставку\n" +
                   "/apply N - применить поставку\n" +
                   "/help - эта справка";
        }
        
        return "❌ Неизвестная команда. Введите /help";
    }
    
    private void sendMessage(long chatId, String text) {
        SendMessage message = new SendMessage();
        message.setChatId(String.valueOf(chatId));
        message.setText(text);
        try {
            execute(message);
        } catch (TelegramApiException e) {
            System.err.println("Failed to send message: " + e.getMessage());
        }
    }
    
    @Override
    public String getBotUsername() { return botUsername; }
    @Override
    public String getBotToken() { return botToken; }
}
