#pragma once
#include <functional>

class TimedCall {
	TimedCall(std::function<int(void)> f, uint32_t time);
	void Update();
	bool IsFinished() { return isFinished_; }

private:
	std::function<int(void)> f_;
	uint32_t time_;
	bool isFinished_ = false;


};
