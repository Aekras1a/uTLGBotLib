/**************************************************************************************************/
// Project: uTLGBotLib
// File: main.cpp
// Description: Project main file
// Created on: 19 mar. 2019
// Last modified date: 19 apr. 2019
// Version: 0.0.1
/**************************************************************************************************/

#ifdef ARDUINO

/**************************************************************************************************/

/* Libraries */

// Standard C/C++ libraries
#include <string.h>

// Device libraries (Arduino ESP32 Core)
#include <Arduino.h>
#include <WiFi.h>

// Custom libraries
#include "utlgbotlib.h"

/**************************************************************************************************/

// WiFi Parameters
#define WIFI_SSID "mynet1234"
#define WIFI_PASS "password1234"
#define MAX_CONN_FAIL 50
#define MAX_LENGTH_WIFI_SSID 31
#define MAX_LENGTH_WIFI_PASS 63

// Telegram Bot Token (Get from Botfather)
#define TLG_TOKEN "XXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

/**************************************************************************************************/

/* Functions Prototypes */

void wifi_init_stat(void);
bool wifi_handle_connection(void);

/**************************************************************************************************/

/* Globals */

// Create Bot object
uTLGBot Bot(TLG_TOKEN);

/**************************************************************************************************/

/* Main Function */

void setup(void)
{
    Serial.begin(115200);
    
    // Initialize WiFi station connection
    wifi_init_stat();
}

void loop()
{
    // Check if WiFi is connected
    if(!wifi_handle_connection())
    {
        // Wait 100ms and check again
        delay(100);
        return;
    }
    
    /*
    // Test connection and disconnection
    Serial.printf("Connection: %d\n", Bot.is_connected());
    Bot.connect();
    Serial.printf("Connection: %d\n", Bot.is_connected());
    Bot.disconnect();
    Serial.printf("Connection: %d\n", Bot.is_connected());
    
    // Test Bot getMe command
    Bot.getMe();

    // Test Bot sendMessage command
    Bot.sendMessage(-244141233, "Hello world");
    Bot.sendMessage(-244141233, "<b>HTML Parse-response Test</b>", "HTML", false, false, 1046);
    */

    // Test Bot getUpdate command and receive messages
    while(Bot.getUpdates())
    {
        Serial.println("-----------------------------------------");
        Serial.println("Received message.");

        Serial.printf("  From chat ID: %d\n", Bot.received_msg.chat.id);
        Serial.printf("  From chat type: %s\n", Bot.received_msg.chat.type);
        Serial.printf("  From chat alias: %s\n", Bot.received_msg.chat.username);
        Serial.printf("  From chat name: %s %s\n", Bot.received_msg.chat.first_name, 
            Bot.received_msg.chat.last_name);
        Serial.printf("  From chat title: %s\n", Bot.received_msg.chat.title);
        if(Bot.received_msg.chat.all_members_are_administrators)
            Serial.println("  From chat where all members are admins.");
        else
            Serial.println("  From chat where not all members are admins.");
        
        Serial.printf("  From user ID: %d\n", Bot.received_msg.from.id);
        Serial.printf("  From user alias: %s\n", Bot.received_msg.from.username);
        Serial.printf("  From user name: %s %s\n", Bot.received_msg.from.first_name, 
            Bot.received_msg.from.last_name);
        Serial.printf("  From user with language code: %s\n", Bot.received_msg.from.language_code);
        if(Bot.received_msg.from.is_bot)
            Serial.println("  From user that is a Bot.");
        else
            Serial.println("  From user that is not a Bot.");
        
        Serial.printf("  Message ID: %d\n", Bot.received_msg.message_id);
        Serial.printf("  Message sent date (UNIX epoch time): %ul\n", Bot.received_msg.date);
        Serial.printf("  Text: %s\n", Bot.received_msg.text);
        Serial.printf("-----------------------------------------\n");
    }
    
    // Wait 1 min for next iteration
    delay(60000);
}

/**************************************************************************************************/

/* Functions */

// Init WiFi interface
void wifi_init_stat(void)
{
    Serial.println("Initializing TCP-IP adapter...");
    Serial.print("Wifi connecting to SSID: ");
    Serial.println(WIFI_SSID);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    printf("TCP-IP adapter successfuly initialized.\n");
}

/**************************************************************************************************/

/* WiFi Change Event Handler */

bool wifi_handle_connection(void)
{
    static bool wifi_connected = false;

    // Device is not connected
    if(WiFi.status() != WL_CONNECTED)
    {
        // Was connected
        if(wifi_connected)
        {
            Serial.println("WiFi disconnected.");
            wifi_connected = false;
        }

        return false;
    }
    // Device connected
    else
    {
        // Wasn't connected
        if(!wifi_connected)
        {
            Serial.println("");
            Serial.println("WiFi connected");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());

            wifi_connected = true;
        }

        return true;
    }
}

/**************************************************************************************************/

#endif
