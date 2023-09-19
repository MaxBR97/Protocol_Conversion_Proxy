
#include <stdlib.h>
#include "../include/InputProtocol.h"

using namespace std;

void keyboardInput(std::string& keyboardOut, bool& flag, std::mutex& lock, StompProtocol& stomp, InputProtocol& inputProtocol){ // function for input thread
	while (flag) {
		std::string insertNote("insert input");
        const short bufsize = 1024;
        char buf[bufsize];
		std::strcpy(buf, "");
		std::string line;
		if(!isUsedForIntegration) {
			std::cout << insertNote << std::endl;
			std::cin.getline(buf, bufsize);
			line = buf;
		}
		else {
			systemObserver.setKeyboardOutput(insertNote);
			line = systemObserver.getKeyboardInput();
		}
		//std::string line(buf);
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
	isUsedForIntegration = false;
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
		   std::this_thread::__sleep_for(std::chrono::seconds(0),std::chrono::nanoseconds(100));
		
	}
	std::cout << "Exiting...\n" << std::endl;
	allowKeyboardInput = false; // might need to be locked
	keyboardThread.join();
    return 0;
}

int integrationMain(int argc, char *argv[]) {
	isUsedForIntegration = true;
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
		systemObserver.setFrameIn(reply);
		std::cout << "reply: \n ------------- \n" << reply <<"\n---------------\n" <<std::endl;
		}
		else
		   std::this_thread::__sleep_for(std::chrono::seconds(0),std::chrono::nanoseconds(100));
		
	}
	std::cout << "Exiting...\n" << std::endl;
	allowKeyboardInput = false; // might need to be locked
	keyboardThread.join();
    return 0;
}

void initializeIntegration() {
	std::thread networkListener(integrationMain,0,nullptr);
	networkListener.detach();
}