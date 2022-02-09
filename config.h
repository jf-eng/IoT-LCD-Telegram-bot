#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Wifi network station credentials
#define WIFI_SSID "INCLUDE YOUR WIFI_SSID HERE"
#define WIFI_PASSWORD "INCLUDE YOUR WIFI PASSWORD HERE"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "INCLUDE YOUR BOT TOKEN HERE"

// Change the following parameters according to your LCD screen
#define LCD_COLS 20
#define LCD_ROWS 4

#define CHAR_LIMIT 80 // max chars that can be displayed on LCD

bool verify_user(String user_id);

#endif
