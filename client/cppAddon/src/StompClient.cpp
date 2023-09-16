
#include <stdlib.h>
#include "../include/InputProtocol.h"

using namespace std;

class IOObserver {
	public:
	//blocking function
	std::string getKeyboardInput() {
		std::string ans;
		std::unique_lock<std::mutex> guard(keyboardLock);
		if(in.length() == 0) {
			guard.unlock();
			while(true) {
				std::this_thread::__sleep_for(std::chrono::seconds(0),std::chrono::nanoseconds(3000));
				if(in.length() != 0)
					break;
			}
			guard.lock();
		}
		ans = in;
		in = "";
		return in;
	}
	void setKeyboardInput(std::string str){
		std::unique_lock<std::mutex> guard(keyboardLock);
		in = str;
	}
	std::string getKeyboardOutput(){
		std::string ans;
		std::unique_lock<std::mutex> guard(keyboardLock);
		if(out.length() == 0) {
			guard.unlock();
			while(true) {
				std::this_thread::__sleep_for(std::chrono::seconds(0),std::chrono::nanoseconds(3000));
				if(out.length() != 0)
					break;
			}
			guard.lock();
		}
		ans = out;
		out = "";
		return ans;

	}
	void setKeyboardOutput(std::string str){
		std::unique_lock<std::mutex> guard(keyboardLock);
		out = str;
	}
	std::string getFrameIn(){

	}
	void setFrameIn(std::string str){

	}
	std::string getFrameOut(){

	}
	void setFrameOut(std::string str) {

	}

	private:
	std::string in;
	std::string out;
	std::string frameIn;
	std::string frameOut;
	std::mutex keyboardLock;
	std::mutex connectionLock;
};

static IOObserver systemObserver;
static bool isUsedForIntegration;


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