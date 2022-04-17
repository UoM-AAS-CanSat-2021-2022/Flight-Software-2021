#include "runner.hpp"
#include "Arduino.h"

Task::Task(std::uint32_t _last_run_ms, std::uint32_t _delay_ms, std::function<void(void)> _job)
	: last_run_ms(_last_run_ms)
	, delay_ms(_delay_ms)
	, job(_job) { }

void Runner::schedule_task(std::function<void(void)> job) {
	_tasks.emplace_back(0, 0, job);
}

void Runner::schedule_task(std::uint32_t delay_ms, std::function<void(void)> job) {
	_tasks.emplace_back(0, delay_ms, job);
}

void Runner::run() {
	for (auto& task : _tasks) {
		const auto now = millis();
		if (now - task.last_run_ms > task.delay_ms) {
			task.last_run_ms = now;
			task.job();
		}
	}
}
