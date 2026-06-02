package com.seniorshop.bot;

import org.telegram.telegrambots.meta.TelegramBotsApi;
import org.telegram.telegrambots.meta.exceptions.TelegramApiException;
import org.telegram.telegrambots.updatesreceivers.DefaultBotSession;

public class Main {
    public static void main(String[] args) {
        String token = System.getenv("TELEGRAM_BOT_TOKEN");
        String username = System.getenv("TELEGRAM_BOT_USERNAME");
        
        if (token == null || token.isEmpty()) {
            System.err.println("TELEGRAM_BOT_TOKEN not set!");
            System.err.println("Set environment variable: TELEGRAM_BOT_TOKEN=your_token");
            System.exit(1);
        }
        
        if (username == null || username.isEmpty()) {
            username = "SeniorShopBot";
        }
        
        System.out.println("Starting bot: " + username);
        System.out.println("Token: " + token.substring(0, 10) + "...");
        
        try {
            TelegramBotsApi botsApi = new TelegramBotsApi(DefaultBotSession.class);
            botsApi.registerBot(new ShopBot(token, username));
            System.out.println("Bot is running!");
        } catch (TelegramApiException e) {
            System.err.println("Failed to start bot: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
