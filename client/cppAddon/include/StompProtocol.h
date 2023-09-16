#ifndef StompProtocol_h
#define StompProtocol_h

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include "../include/event.h"
#include "../include/StompFrame.h"
#include "../include/ConnectionHandler.h"

// TODO: implement the STOMP protocol
class StompProtocol
{
private:
   ConnectionHandler* handler;
   bool shouldTerminate;
   std::vector<StompFrame> sentMessages;
   std::vector<StompFrame> recievedMessages;
   std::map<std::pair<std::string, std::string>, std::vector<Event>> gameUpdates;
   std::unordered_map<std::string, int> subscriptions;
   static int recieptIdCounter;
   static int subscriptionIdCounter;
   bool loggedIn;
   short port = 7776;
   bool disconnectFrameSent;
   int generateRecieptID();
   int generateSubscriptionID();
   std::vector<std::string> getSubscribedDestinations();
public:
    StompProtocol(ConnectionHandler& handler);
    StompProtocol(const StompProtocol& stompProtocol);
    StompProtocol& operator=(const StompProtocol& stompProtocol);
    void sendConnectMessage(std::string accept_version, std::string host, std::string username, std::string passcode); // creates a StompFrame, converts it to string form (you can use StompFrame.toString()), and sends it using: handler->sendFrameAscii(...).
    void sendSendMessage(std::string destination, std::string body, bool reciept);
    void sendSubscribeMessage(std::string destination, bool reciept);
    void sendUnsubscribeMessage(std::string destination, bool reciept);
    void sendDisconnectMessage(bool reciept);
    void processReply(const std::string& message); // Write to file/to RAM (if message is of command "MESSAGE"), or disconnect if error.
    bool isShouldTerminate(); 
    bool isLoggedIn();
    void setPort(short p);
    std::vector<Event> getGameUpdatesFrom(std::string user,std::string destination);
    ~StompProtocol();
};

#endif