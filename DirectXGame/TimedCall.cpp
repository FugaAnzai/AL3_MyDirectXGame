#include "TimedCall.h"

TimedCall::TimedCall(std::function<int(void)> f, uint32_t time) {
	
	f_ = f;
	time_ = time;

}

void TimedCall::Update() {

	if (isFinished_) {
		return;	
	}

	if (time_ <= 0) {
		isFinished_ = true;
		f_();
	}

}
