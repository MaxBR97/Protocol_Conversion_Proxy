// #ifndef IOObserver
// #define IOObserver
#include <stdlib.h>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <iostream>
#include <condition_variable>

using namespace std;


class IOObserver {
	public:
	//blocking function
	std::string getKeyboardInput() {
		std::string ans;
		std::unique_lock<std::mutex> guard(keyboardInLock);
		waitOnKeyBoardIn.wait(guard, [this]{return (!in.empty());});
		ans = in.front();
		in.pop();
		return ans;
	}
	void setKeyboardInput(std::string str){
		{
			std::unique_lock<std::mutex> guard(keyboardInLock);
			in.push(str);
		}
		waitOnKeyBoardIn.notify_all();
	}
	std::string getKeyboardOutput(){
		std::string ans;
		std::unique_lock<std::mutex> guard(keyboardOutLock);
		waitOnKeyBoardOut.wait(guard, [this]{return (!out.empty());});
		ans = out.front();
		out.pop();
		return ans;

	}
	void setKeyboardOutput(std::string str){
		{
			std::unique_lock<std::mutex> guard(keyboardOutLock);
			out.push(str);
		}
		waitOnKeyBoardOut.notify_all();
	}
	std::string getFrameIn(){
		std::unique_lock<std::mutex> guard(frameInLock);
		waitOnFrameIn.wait(guard, [this]{return (!frameIn.empty());});
		std::string ans = frameIn.front();
        frameIn.pop();
		return ans;
	}
	void setFrameIn(std::string str){
		{
		std::unique_lock<std::mutex> guard(frameInLock);
		frameIn.push(str);
		}
		waitOnFrameIn.notify_all();
	}
	std::string getFrameOut(){
		std::unique_lock<std::mutex> guard(frameOutLock);
		waitOnFrameOut.wait(guard, [this] { return (!frameOut.empty());});
        std::string ans = frameOut.front();
        frameOut.pop();
		return ans;
	}
	void setFrameOut(std::string str) {
		{
			std::unique_lock<std::mutex> guard(frameOutLock);
			frameOut.push(str);
		}
		waitOnFrameOut.notify_all();
	}

	void setSleepDelays(int secs, int nano) {
		cout<<"DEPRECATED";
		sleepTimeNano = nano;
		sleepTimeSec = secs;
	}

	int getSleepDelayInNano() {
		cout<<"DEPRECATED";
		return sleepTimeNano;
	}

	private:
	std::queue<std::string> in;
	std::queue<std::string> out;
	std::queue<std::string> frameIn;
	std::queue<std::string> frameOut;
	std::mutex keyboardInLock;
	std::mutex keyboardOutLock;
	std::mutex frameInLock;
	std::mutex frameOutLock;
	std::condition_variable waitOnKeyBoardIn;
	std::condition_variable waitOnKeyBoardOut;
	std::condition_variable waitOnFrameIn;
	std::condition_variable waitOnFrameOut;
	int sleepTimeNano = 9000;
	int sleepTimeSec = 1;
};

static IOObserver systemObserver;
static bool isUsedForIntegration;

