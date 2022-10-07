# Description
This is a simple weekend IoT project built using ESP8266 and an LCD display which can be controlled via a Telegram bot remotely. The Telegram bot is hosted on the ESP8266 which is connected to WiFi for internet access. The Telegram bot has been configured to take input strings of up to 80 characters from the user and displays it on the LCD via I2C communication.

To prevent unwanted access to the LCD, only verified users will be able to gain access to the LCD. The Telegram bot first checks the User ID of the user accessing the bot and only allows changes to be made to the LCD if the user ID is a part of the predefined list of verified users.

# Demo

# Configuration
All user configurations can be found in `config.h`.

### Verified Users
The Telegram bot can be configured to only allow certain users to have access to the LCD display. This can be added by inserting the user ID of verified users in the line below. User IDs can be found by messaging @userinfobot on Telegram.

```cpp
// Change the following array to include verified user IDs
String accepted_users[] = {"ENTER USER_ID 1 HERE", "ENTER USER_ID 2 HERE"};v
```

### Telegram Bot Token
After creating your Telegram bot, input the Telegram bot ID in the line below.
```cpp
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "INCLUDE YOUR BOT TOKEN HERE"
```

### WiFi Credentials
The ESP8266 needs to be connected to WiFi for internet access. Insert WiFi details in the lines below.
```cpp
// Wifi network station credentials
#define WIFI_SSID "INCLUDE YOUR WIFI_SSID HERE"
#define WIFI_PASSWORD "INCLUDE YOUR WIFI PASSWORD HERE"
```

### LCD Screen Size
Depending on the size of the LCD screen used, the maximum number of characters displayed on the LCD need to be configured as well.
```cpp
// Change the following parameters according to your LCD screen
#define LCD_COLS 20
#define LCD_ROWS 4
```
