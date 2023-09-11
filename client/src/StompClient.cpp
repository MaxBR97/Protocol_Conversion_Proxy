
#include <stdlib.h>
//#include "../include/ConnectionHandler.h"
//#include "../include/StompProtocol.h"
//#include "../include/StompFrame.h"
#include "../include/InputProtocol.h"

using namespace std;


void keyboardInput(std::string& keyboardOut, bool& flag, std::mutex& lock, StompProtocol& stomp, InputProtocol& inputProtocol){ // function for input thread
	while (flag) {
        const short bufsize = 1024;
        char buf[bufsize];
		std::strcpy(buf, "");
		std::cout << "insert input" << std::endl;
        std::cin.getline(buf, bufsize);
		std::string line(buf);
		if(line.length() > 0)
		{
		   std::lock_guard<std::mutex> guard(lock);
		   keyboardOut.clear();
		   keyboardOut.assign(line);
		   if(keyboardOut.compare("") !=0 && inputProtocol.process(keyboardOut))
		   {
              inputProtocol.execute(stomp);
			  std::this_thread::__sleep_for(std::chrono::seconds(0),std::chrono::nanoseconds(200)); //wont hurt
		   }
		}
	}
}

int main(int argc, char *argv[]) {
	/* if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " insert host & port!" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);
    
    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }*/
	ConnectionHandler connectionHandler("", 0);
	bool allowKeyboardInput = true;
	std::mutex lock; // lock context everytime before accessing userInput.
	std::string userInput("");
	StompProtocol stomp(connectionHandler);
	InputProtocol inputProtocol;
	std::thread keyboardThread(keyboardInput, std::ref(userInput), std::ref(allowKeyboardInput), std::ref(lock), std::ref(stomp), std::ref(inputProtocol)); 
	std::string inputBufferAsString;
	std::string reply = "";
	


    while ( !stomp.isLoggedIn() || !stomp.isShouldTerminate()) {
        if(stomp.isLoggedIn())
		{
	    reply.clear();
		connectionHandler.getFrameAscii(reply, '\0');
		stomp.processReply(reply);
		std::cout << "reply: \n ------------- \n" << reply <<"\n---------------\n" <<std::endl;
		}
		else
		   std::this_thread::__sleep_for(std::chrono::seconds(0),std::chrono::nanoseconds(1));
		
	}
	std::cout << "Exiting...\n" << std::endl;
	allowKeyboardInput = false; // might need to be locked
	keyboardThread.join();
    return 0;
}