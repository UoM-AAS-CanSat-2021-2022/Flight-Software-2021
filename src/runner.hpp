#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <queue>

using job_t = std::function<void(void)>;

struct Task {
	std::uint32_t last_run_ms;
	std::uint32_t delay_ms;
	job_t job;

	Task(std::uint32_t, std::uint32_t, job_t);
};

struct OneShot {
	std::uint32_t run_after_ms;
	job_t job;

	OneShot(std::uint32_t, job_t);

	bool operator>(const OneShot&) const;
};

class Runner {
	std::vector<Task> _tasks;
	std::priority_queue<OneShot, std::vector<OneShot>, std::greater<OneShot>> _one_shots;

public:
	void schedule_task(job_t);
	void schedule_task(std::uint32_t, job_t);
	void run_after(std::uint32_t, job_t);
	void run();
};
