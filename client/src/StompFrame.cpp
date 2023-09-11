#include "../include/StompFrame.h"

   StompFrame::StompFrame (StompFrame::Command c, unordered_map <Header,std::string> umap, std::string body) : command(c), umap(umap), body(body)
   {
   }
   StompFrame::StompFrame (const std::string& message) : command(), umap(), body("")
   {
    std::istringstream ss(message);
    std::string word;
    std::getline(ss,word,'\n');
    command = stringToCommand(word);
    std::getline(ss,word,'\n');
    while(word.length() > 0)
    {
       int start = 0;
       int middle = word.find(':');
       int end = word.length();
       std::string header = word.substr(start, middle);
       middle = middle + 1;
       std::string value = word.substr(middle,end);
       umap.insert({stringToHeader(header),value});

       std::getline(ss,word,'\n');
    }
    bool flag = true;
    while(std::getline(ss,word,'\n'))
    {
        if(flag){
        body += word;
        flag = false;
        }
        else
        {
            body += "\n";
            body += word;
        }
    }
   }

   StompFrame::Command StompFrame::getCommand()
   {
      return this->command;
   }

   std::string StompFrame::getHeaderValue(Header head)
   {
       if (umap.find(head) == umap.end())
          return NULL; // header doesnt exists
       else 
       {
          auto it = umap.find(head);
          return it->second;// returns value for key 'head'
       }
         
   }

   std::string StompFrame::getBody()
   {
      return body;
   }

   std::string StompFrame::toString()
   {
       std::string ans ("");
       ans.append(commandToString(command));
       ans.append("\n");
       for(std::pair<Header,std::string> element : umap)
       {
         ans.append(headerToString(element.first));
         ans.append(":");
         ans.append(element.second);
         ans.append("\n");
       }
       ans.append("\n");
       ans.append(body);

       return ans;
   }

   std::string StompFrame::commandToString(StompFrame::Command com)
   {
      if(com == StompFrame::Command::CONNECT)
          return "CONNECT";
      else if(com == StompFrame::Command::ERROR)
          return "ERROR";
       else if(com == StompFrame::Command::CONNECTED)
          return "CONNECTED";
       else if(com == StompFrame::Command::RECIEPT)
          return "RECEIPT";
       else if(com == StompFrame::Command::MESSAGE)
          return "MESSAGE";
       else if(com == StompFrame::Command::DISCONNECT)
          return "DISCONNECT";
       else if(com == StompFrame::Command::SEND)
          return "SEND";
       else if(com == StompFrame::Command::SUBSCRIBE)
          return "SUBSCRIBE";
       else if(com == StompFrame::Command::UNSUBSCRIBE)
          return "UNSUBSCRIBE";
      return "";
   }

   StompFrame::Command StompFrame::stringToCommand(std::string com)
   {
      if(com.compare("CONNECT") == 0)
          return StompFrame::Command::CONNECT;
      else if(com .compare("ERROR") == 0)
          return StompFrame::Command::ERROR;
       else if(com.compare("CONNECTED") == 0)
          return StompFrame::Command::CONNECTED;
       else if(com.compare("RECEIPT") == 0)
          return StompFrame::Command::RECIEPT;
       else if(com .compare("MESSAGE") == 0)
          return StompFrame::Command::MESSAGE;
       else if(com.compare("DISCONNECT") == 0)
          return StompFrame::Command::DISCONNECT;
       else if(com.compare("SEND") == 0)
          return StompFrame::Command::SEND;
       else if(com.compare("SUBSCRIBE") == 0)
          return StompFrame::Command::SUBSCRIBE;
       else if(com.compare("UNSUBSCRIBE") == 0)
          return StompFrame::Command::UNSUBSCRIBE;
      cout<<"DIDNT RECOGNIZE STOMP COMMAND! (shouldnt reach this line)"<<endl;
      return StompFrame::Command::ERROR;
   }

   std::string StompFrame::headerToString(StompFrame::Header head)
   {
       if(head == StompFrame::Header::ACCEPT_VERSION)
          return "accept-version";
       else if(head == StompFrame::Header::DESTINATION)
          return "destination";
      else if(head == StompFrame::Header::HOST)
          return "host";
      else if(head == StompFrame::Header::ID)
          return "id";
      else if(head == StompFrame::Header::LOGIN)
          return "login";
      else if(head == StompFrame::Header::MESSAGE)
          return "message";
      else if(head == StompFrame::Header::MESSAGE_ID)
          return "message_id";
      else if(head == StompFrame::Header::PASSCODE)
          return "passcode";
      else if(head == StompFrame::Header::RECIEPT)
          return "receipt";
      else if(head == StompFrame::Header::RECIEPT_ID)
          return "receipt_id";
      else if(head == StompFrame::Header::SUBSCRIPTION)
          return "subscription";
      else if(head == StompFrame::Header::VERSION)
          return "version";
      return "";
   }

   StompFrame::Header StompFrame::stringToHeader(std::string head)
   {
       if(head.compare("accept-version") == 0)
          return StompFrame::Header::ACCEPT_VERSION;
       else if(head.compare("destination") == 0)
          return StompFrame::Header::DESTINATION;
      else if(head.compare("host") == 0)
          return StompFrame::Header::HOST;
      else if(head.compare("id") == 0)
          return StompFrame::Header::ID;
      else if(head.compare("login")==0)
          return StompFrame::Header::LOGIN;
      else if(head.compare("message")==0)
          return StompFrame::Header::MESSAGE;
      else if(head.compare("message-id")==0)
          return  StompFrame::Header::MESSAGE_ID;
      else if(head .compare("passcode") == 0)
          return StompFrame::Header::PASSCODE;
      else if(head.compare("receipt") == 0)
          return StompFrame::Header::RECIEPT;
      else if(head .compare("receipt-id")==0)
          return StompFrame::Header::RECIEPT_ID;
      else if(head.compare("subscription") == 0 )
          return StompFrame::Header::SUBSCRIPTION;
      else if(head .compare("version") == 0)
          return StompFrame::Header::VERSION;
      cout<<"DIDNT RECOGNIZE STOMP HEADER! (shouldnt reach this line)"<<endl;
      return StompFrame::Header::DESTINATION;
   }

   std::string StompFrame::intToString(int num)
   {
     stringstream ss;
     ss << num;
     string str = ss.str();
     return str;
   }



