/**************************************************************************************************/
// Project: uTLGBotLib
// File: main.cpp
// Description: Project main file
// Created on: 19 mar. 2019
// Last modified date: 21 mar. 2019
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
    uint8_t num_received_messages = Bot.getUpdate();
    uint8_t i;
    while(num_received_messages > 0)
    {
        i = num_received_messages;

        Serial.printf("Received message.\n");
        Serial.printf("Chat ID: %d\n", Bot.received_msg[i].chat_id);
        Serial.printf("Chat Title: %s\n", Bot.received_msg[i].chat_title);
        Serial.printf("Chat Alias: %s\n", Bot.received_msg[i].alias);
        Serial.printf("Sent date (UNIX epoch time): %d\n", Bot.received_msg[i].date);
        Serial.printf("From user: %s\n", Bot.received_msg[i].from_user);
        Serial.printf("Text:\n%s\n", Bot.received_msg[i].text);
        Serial.println("-----------------------------------------");

        num_received_messages = num_received_messages - 1;
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
