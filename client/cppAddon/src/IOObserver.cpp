#include <stdlib.h>
#include <string>
#include <thread>
#include <mutex>
#include <queue>


class IOObserver {
	public:
	//blocking function
	std::string getKeyboardInput() {
		std::string ans;
		std::unique_lock<std::mutex> guard(keyboardInLock);
		if(in.length() == 0) {
			guard.unlock();
			while(true) {
				std::this_thread::__sleep_for(std::chrono::seconds(0),std::chrono::nanoseconds(sleepTime));
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
				std::this_thread::__sleep_for(std::chrono::seconds(0),std::chrono::nanoseconds(sleepTime));
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
				std::this_thread::__sleep_for(std::chrono::seconds(0),std::chrono::nanoseconds(sleepTime));
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
				std::this_thread::__sleep_for(std::chrono::seconds(0),std::chrono::nanoseconds(sleepTime));
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

	private:
	std::string in;
	std::string out;
	std::queue<std::string> frameIn;
	std::queue<std::string> frameOut;
	std::mutex keyboardInLock;
	std::mutex keyboardOutLock;
	std::mutex frameInLock;
	std::mutex frameOutLock;
	const int sleepTime = 9000;
};

static IOObserver systemObserver;
static bool isUsedForIntegration;
