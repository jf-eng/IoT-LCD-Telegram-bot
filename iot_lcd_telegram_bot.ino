#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <Wire.h>
#include <TimeLib.h>
#include <hd44780.h>            // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h>  // i2c expander i/o class header
#include "config.h"

const unsigned long BOT_MTBS = 1000; // mean time between scan messages

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime; // last time messages' scan has been done

hd44780_I2Cexp lcd; // declare lcd object: auto locate & config exapander chip

String currently_displayed_string = ""; // contains the currently displayed string on LCD
time_t last_msg_sent;
String last_msg_sent_str;

void handleNewMessages(int numNewMessages)
{
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    if (verify_user(chat_id) == false){
      bot.sendMessage(chat_id, "You are not authorised to use this bot.", "");
      return;
    }
    
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";

    if (text == "/text")
    {
      bot.sendMessage(chat_id, "Please enter a message within " + (String) CHAR_LIMIT + " characters.", "");
      while(bot.getUpdates(bot.last_message_received + 1) == 0){}// while waiting for message, do nothing

      // ensures that message obtained is less than CHAR_LIMIT
      while (bot.messages[0].text.length() > CHAR_LIMIT){
        bot.sendMessage(chat_id,"Your message is too long! Please enter a new message.", "");
        while(bot.getUpdates(bot.last_message_received + 1) == 0){}// while waiting for message, do nothing
      }
      int messageToLCD = bot.getUpdates(bot.last_message_received + 1);
      Serial.print("messageToLCD: ");
      Serial.println(messageToLCD);
      Serial.print(bot.messages[0].text);
      
      lcd.clear();
      currently_displayed_string = bot.messages[0].text;
      last_msg_sent = time(nullptr);
//      sprintf(&last_msg_sent_str, "%02d/%02d/%04d, %02d:%02d ", day(last_msg_sent), month(last_msg_sent), year(last_msg_sent), hour(last_msg_sent), minute(last_msg_sent));
      lcd.print(bot.messages[0].text);
      bot.sendMessage(chat_id, "Your message is on display now!");
    }

    if(text == "/status")
    {
      String reply = "The message currently displayed on the LCD screen is: \n";
      reply += '"' + currently_displayed_string + '"' + "\n";
//      reply += "This message was sent at " + last_msg_sent_str + "\n";
      bot.sendMessage(chat_id,reply,"Markdown");
    }
    if (text == "/start")
    {
      String welcome = "Welcome to the IoT display bot, " + from_name + ".\n";
      welcome += "/text : to send a messsage to the LCD display\n";
      welcome += "/status : to see the message currently displayed on the LCD\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}

void setup()
{
  int status;

  // initialize LCD with number of columns and rows: 
  // hd44780 returns a status from begin() that can be used
  // to determine if initalization failed.
  // the actual status codes are defined in <hd44780.h>
  status = lcd.begin(LCD_COLS, LCD_ROWS);
  if(status) // non zero status means it was unsuccesful
  {
    // begin() failed so blink error code using the onboard LED if possible
    hd44780::fatalError(status); // does not return
  }
  lcd.lineWrap();
  
  Serial.begin(115200);
  Serial.println();

  // attempt to connect to Wifi network:
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Check NTP/Time, usually it is instantaneous and you can delete the code below.
  Serial.print("Retrieving time: ");
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
}

void loop()
{
  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}
