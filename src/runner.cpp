#include "runner.hpp"
#include "Arduino.h"

Task::Task(std::uint32_t _last_run_ms, std::uint32_t _delay_ms, job_t _job)
	: last_run_ms(_last_run_ms)
	, delay_ms(_delay_ms)
	, job(_job) { }

OneShot::OneShot(std::uint32_t _run_after_ms, job_t _job)
	: run_after_ms(_run_after_ms)
	, job(_job) { }

bool OneShot::operator>(const OneShot& other) const {
	return this->run_after_ms > other.run_after_ms;
}

void Runner::schedule_task(job_t job) {
	_tasks.emplace_back(0, 0, job);
}

void Runner::schedule_task(std::uint32_t delay_ms, job_t job) {
	_tasks.emplace_back(0, delay_ms, job);
}

void Runner::run_after(std::uint32_t run_after_ms, job_t job) {
	_one_shots.emplace(millis() + run_after_ms, job);
}

void Runner::run() {
	while (!_one_shots.empty()) {
		const auto& top = _one_shots.top();

		if (top.run_after_ms < millis()) {
			top.job();
			_one_shots.pop();
		} else {
			break;
		}
	}

	for (auto& task : _tasks) {
		const auto now = millis();
		if (now - task.last_run_ms > task.delay_ms) {
			task.last_run_ms = now;
			task.job();
		}
	}
}
