#ifndef StompFrame_h 
#define StompFrame_h 

#include <stdlib.h>
//#include <compare>
#include <initializer_list>
#include <bits/stdc++.h>
using namespace std;

class StompFrame
{
public:
   enum class Command { ERROR , RECIEPT ,MESSAGE , CONNECTED , CONNECT, SEND , SUBSCRIBE, UNSUBSCRIBE, DISCONNECT };
   enum class Header { ACCEPT_VERSION, DESTINATION, HOST, ID, MESSAGE_ID, RECIEPT_ID, RECIEPT, SUBSCRIPTION, VERSION, MESSAGE,LOGIN,PASSCODE };
   StompFrame (Command c, unordered_map <Header,std::string> umap, std::string body);
   StompFrame ( const std::string& message);
   Command getCommand();
   std::string getHeaderValue(Header header); // takes header and returns it's value 
   std::string getBody();

   std::string toString(); // converts Frame to a string according to the protocol.
   static std::string commandToString(Command com);
   static std::string headerToString (Header head);
   static Command stringToCommand (std:: string com);
   static Header stringToHeader ( std::string head);
   static std::string intToString(int num);
private:
  StompFrame::Command command;
  unordered_map<Header, std::string> umap;
  std::string body;

};

#endif 