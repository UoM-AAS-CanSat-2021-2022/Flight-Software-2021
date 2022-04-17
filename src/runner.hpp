#pragma once

#include <cstdint>
#include <functional>
#include <vector>

struct Task {
	std::uint32_t last_run_ms;
	std::uint32_t delay_ms;
	std::function<void(void)> job;

	Task(std::uint32_t, std::uint32_t, std::function<void(void)>);
};

class Runner {
	std::vector<Task> _tasks;

public:
	void schedule_task(std::function<void(void)>);
	void schedule_task(std::uint32_t, std::function<void(void)>);
	void run();
};
