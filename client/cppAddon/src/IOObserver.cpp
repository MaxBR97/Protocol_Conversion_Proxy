#include <stdlib.h>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <iostream>

using namespace std;


class IOObserver {
	public:
	//blocking function
	std::string getKeyboardInput() {
		std::string ans;
		std::unique_lock<std::mutex> guard(keyboardInLock);
		if(in.length() == 0) {
			guard.unlock();
			while(true) {
				std::this_thread::__sleep_for(std::chrono::seconds(sleepTimeSec),std::chrono::nanoseconds(sleepTimeNano));
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
		std::unique_lock<std::mutex> guard(keyboardInLock);
		in = str;
	}
	std::string getKeyboardOutput(){
		std::string ans;
		std::unique_lock<std::mutex> guard(keyboardOutLock);
		if(out.length() == 0) {
			guard.unlock();
			while(true) {
				//cout<<"proxyOutput beat"<<endl;
				std::this_thread::__sleep_for(std::chrono::seconds(sleepTimeSec),std::chrono::nanoseconds(sleepTimeNano));
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
		std::unique_lock<std::mutex> guard(keyboardOutLock);
		out = str;
	}
	std::string getFrameIn(){
		std::unique_lock<std::mutex> guard(frameInLock);
		if(frameIn.empty()) {
			guard.unlock();
			while(frameIn.empty()) {
				std::this_thread::__sleep_for(std::chrono::seconds(sleepTimeSec),std::chrono::nanoseconds(sleepTimeNano));
			}
			guard.lock();
		}
		std::string ans = frameIn.front();
        frameIn.pop();
		return ans;
	}
	void setFrameIn(std::string str){
		std::unique_lock<std::mutex> guard(frameInLock);
		frameIn.push(str);
	}
	std::string getFrameOut(){
		std::unique_lock<std::mutex> guard(frameOutLock);
		if(frameOut.empty()) {
			guard.unlock();
			while(frameOut.empty()) {
				//cout<<"getFrameOut beat"<<endl;
				std::this_thread::__sleep_for(std::chrono::seconds(sleepTimeSec),std::chrono::nanoseconds(sleepTimeNano));
			}
			guard.lock();
		}
        std::string ans = frameOut.front();
        frameOut.pop();
		return ans;

	}
	void setFrameOut(std::string str) {
		std::unique_lock<std::mutex> guard(frameOutLock);
		frameOut.push(str);
	}

	void setSleepDelays(int secs, int nano) {
		sleepTimeNano = nano;
		sleepTimeSec = secs;
	}

	private:
	std::string in;
	std::string out;
	std::queue<std::string> frameIn;
	std::queue<std::string> frameOut;
	std::mutex keyboardInLock;
	std::mutex keyboardOutLock;
	std::mutex frameInLock;
	std::mutex frameOutLock;
	int sleepTimeNano = 9000;
	int sleepTimeSec = 1;
};

static IOObserver systemObserver;
static bool isUsedForIntegration;
