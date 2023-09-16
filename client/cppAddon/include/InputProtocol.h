#ifndef InputProtocol_h
#define InputProtocol_h

#include <unordered_map>
#include "../include/StompProtocol.h"

using namespace std;


class InputProtocol {

public:
   enum Command { LOGIN, JOIN, EXIT, REPORT, SUMMARY,LOGOUT};
   enum Header { HOST,PORT,USERNAME,PASSWORD, GAME_NAME, FILE};

   const std::string VERSION = "1.2";
   
   InputProtocol();
   bool process(std::string message); // sets Command and Headers according to 'message'. Returns false if message doesn't comply with the protocol structure.
   void execute(StompProtocol& stompProtocol); // executes command using stomp protocol. for example, if COMMAND == LOGIN, stompProtocol.sendConnectMessage(...) is called
   void SetUsername(std::string s);

private:
   Command command;
   unordered_map<Header, std::string> umap;
   bool setCommand(std::string word);
   std::string myUsername;
};
#endif