#include "../include/ConnectionHandler.h"
#include "../include/StompProtocol.h"
#include "../include/event.h"
#include <stdlib.h>
#include "../include/StompFrame.h"
#include <unordered_map>

int StompProtocol::recieptIdCounter = 0;
int StompProtocol::subscriptionIdCounter = 0;

    StompProtocol::StompProtocol(ConnectionHandler& handler) : handler(&handler),
     shouldTerminate(false),
     sentMessages(), recievedMessages(), gameUpdates(), subscriptions(), loggedIn(false) , disconnectFrameSent(false)
    {
     this->handler = &handler;
     shouldTerminate = false;
     recieptIdCounter = 0;
     subscriptionIdCounter = 0;
    }

    StompProtocol::StompProtocol(const StompProtocol& stomp) : handler(stomp.handler), shouldTerminate(stomp.shouldTerminate),
    sentMessages(), recievedMessages(), gameUpdates(), subscriptions(stomp.subscriptions), loggedIn(false) , disconnectFrameSent(false)
    {
        recieptIdCounter = 0;
        subscriptionIdCounter = 0;
    }

    StompProtocol& StompProtocol::operator=(const StompProtocol& stomp)
    {
        handler = stomp.handler;
        shouldTerminate = false;
        recieptIdCounter = 0;
        subscriptionIdCounter = 0;

        return (*this);
    }

    bool StompProtocol::isLoggedIn()
    {
        return loggedIn;
    }

    int StompProtocol::generateRecieptID()
    {
        recieptIdCounter +=1;
        return recieptIdCounter;
    }

    int StompProtocol::generateSubscriptionID()
    {
        subscriptionIdCounter +=1;
        return subscriptionIdCounter;
    }

    std::vector<std::string> StompProtocol::getSubscribedDestinations()
    {
       std::vector<std::string> dest;
       for(auto it=subscriptions.begin();it!=subscriptions.end();it++) {
           dest.push_back(it->first);
       }
       return dest;
    }
    void StompProtocol::sendConnectMessage(std::string accept_version, std::string host, std::string login, std::string passcode)
    {
        
        unordered_map<StompFrame::Header, std::string> headers;
        headers[StompFrame::Header::ACCEPT_VERSION] = accept_version;
        headers[StompFrame::Header::HOST] = host;
        headers[StompFrame::Header::LOGIN] = login;
        headers[StompFrame::Header::PASSCODE] = passcode;

        (*handler) = ConnectionHandler(host, port);
        if (!handler->connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        }
        else
           loggedIn = true;
        StompFrame frame (StompFrame::Command::CONNECT, headers, "");
        handler->sendFrameAscii(frame.toString(), '\0');
        sentMessages.push_back(frame);
    } // creates a StompFrame, converts it to string form (you can use StompFrame.toString()), and sends it using: handler->sendFrameAscii(...).

    void StompProtocol::sendSendMessage(std::string destination, std::string body, bool reciept)
    {
        unordered_map<StompFrame::Header, std::string> headers;
        headers[StompFrame::Header::DESTINATION] = destination;
        if(reciept)
           headers[StompFrame::Header::RECIEPT] = StompFrame::intToString(generateRecieptID());

        StompFrame frame (StompFrame::Command::SEND, headers, body);
        handler->sendFrameAscii(frame.toString(), '\0');
        
       /* Event recievedEvent = Event(body);
        std::string sentBy = recievedEvent.getSender();
        std::pair <std::string, std::string> pa;
        pa.first = sentBy;
        pa.second = destination;
        //if(gameUpdates.count(sentBy) == 0)
        //   gameUpdates.insert({sentBy, std::vector<Event>()});
        gameUpdates[pa].push_back(recievedEvent);
        //cout<<"PUSHE EVENT:\n " << (gameUpdates[sentBy].end() -1)->get_team_a_name() << "_" << recievedEvent.get_team_b_name() <<endl;
*/
        sentMessages.push_back(frame);
    }

    void StompProtocol::sendSubscribeMessage(std::string destination, bool reciept)
    {
        int subId  = generateSubscriptionID();
        unordered_map<StompFrame::Header, std::string> headers;
        headers[StompFrame::Header::DESTINATION] = destination;
        headers[StompFrame::Header::ID] = StompFrame::intToString(subId);
        if(reciept)
           headers[StompFrame::Header::RECIEPT] = StompFrame::intToString(generateRecieptID());
        subscriptions[destination] = subId;
        StompFrame frame (StompFrame::Command::SUBSCRIBE, headers, "");
        handler->sendFrameAscii(frame.toString(), '\0');
        sentMessages.push_back(frame);
    }

    void StompProtocol::sendUnsubscribeMessage(std::string destination, bool reciept)
    {
        unordered_map<StompFrame::Header, std::string> headers;
        int subscriptionId = 0;
        std::vector<std::string> allSubs = getSubscribedDestinations();
        for (auto it = subscriptions.begin(); it != subscriptions.end(); ++it) {
           if((it->first).compare(destination) == 0)
           {
               subscriptionId = it->second;
           }
        }
        headers[StompFrame::Header::ID] = StompFrame::intToString(subscriptionId);
        if(reciept)
           headers[StompFrame::Header::RECIEPT] = StompFrame::intToString(generateRecieptID());

        StompFrame frame (StompFrame::Command::UNSUBSCRIBE, headers, "");
        handler->sendFrameAscii(frame.toString(), '\0');
        sentMessages.push_back(frame);
    }

    void StompProtocol::sendDisconnectMessage(bool reciept)
    {
        unordered_map<StompFrame::Header, std::string> headers;
        headers[StompFrame::Header::RECIEPT] = StompFrame::intToString(generateRecieptID());

        StompFrame frame(StompFrame::Command::DISCONNECT, headers, "");
        handler->sendFrameAscii(frame.toString(), '\0');
        disconnectFrameSent = true;
        sentMessages.push_back(frame);
    }

    void StompProtocol::processReply(const std::string& message)
    {
        StompFrame frame(message);
        if(frame.getCommand() == StompFrame::Command::MESSAGE)
        {
           Event recievedEvent = Event(frame.getBody());      
           std::string sentBy = recievedEvent.getSender();
           std::string destination = frame.getHeaderValue(StompFrame::Header::DESTINATION);
           //cout<< "EVENT: \n" << recievedEvent.toString() << "\n SENT BY: \n" <<sentBy << "\n DESTINATION: \n" << destination << endl;
           std::pair<std::string, std::string> pa;
           pa.first = sentBy;
           pa.second = destination;
           gameUpdates[pa].push_back(recievedEvent);
        }
        else if(frame.getCommand() == StompFrame::Command::RECIEPT)
        {
            if(disconnectFrameSent)
            {
                disconnectFrameSent = false;
                loggedIn = false;
                shouldTerminate = true;
                std::cout << "disconnected gracefully!"<<endl;
            }
        }
        else if (frame.getCommand() == StompFrame::Command::ERROR)
        {
            shouldTerminate = true;
            loggedIn = false;
            std::cout << "Error frame recieved, terminating connection" << endl;
        }
    } 


    bool StompProtocol::isShouldTerminate()
    {
        return this->shouldTerminate;
    }

    std::vector<Event> StompProtocol::getGameUpdatesFrom(std::string user,std::string destination)
    {
        std::pair<std::string,std::string> pa;
        pa.first = user;
        pa.second = destination;
        return this->gameUpdates[pa];
    }
    void StompProtocol::setPort(short p)
    {
        port = p;
    }

    StompProtocol::~StompProtocol()
    {
        std::cout << "Destructor of StompProtocol called, does nothing." << std::endl;
    }