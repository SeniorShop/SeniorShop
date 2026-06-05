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
        System.out.println("Получено обновление: " + update);
    
        if (!update.hasMessage() || !update.getMessage().hasText()) {
            System.out.println("Пропущено: нет текстового сообщения");
            return;
        }
    
    String command = update.getMessage().getText();
    long chatId = update.getMessage().getChatId();
    String userName = update.getMessage().getFrom().getFirstName();
    
    System.out.println("Команда: " + command + " от " + userName);
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
                   "/report - финансовый отчёт\n" +
                   "/supplies - список поставок\n" +
                   "/apply 123 - применить поставку\n" +
                   "/help - помощь";
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
        
        if (command.startsWith("/apply ")) {
            try {
                int num = Integer.parseInt(command.substring(7));
                boolean result = NativeLib.applySupply(num);
                return result ? "Поставка #" + num + " применена" : "Поставка #" + num + " не найдена";
            } catch (NumberFormatException e) {
                return "Неверный формат. Используйте: /apply 123";
            }
        }
        
        if (command.equals("/help")) {
            return "Доступные команды:\n" +
                   "/start - приветствие\n" +
                   "/products - список товаров\n" +
                   "/report - финансовый отчёт\n" +
                   "/supplies - список поставок\n" +
                   "/apply N - применить поставку №N\n" +
                   "/help - эта справка";
        }
        
        return "Неизвестная команда. Введите /help";
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
