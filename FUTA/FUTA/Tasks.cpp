#include "Tasks.h"
#include <time.h>

Subtasks::Subtasks() : started(false), completed(false), progressionState(0), effort(0) {}

Subtasks::~Subtasks() {}

Tasks::Tasks() : importance(0), progressionState(0), effort(0), started(false), completed(false), deadline(false),
name("NewTask"), description(""), initialDate(""), finalDate(""), subtaskList() {

	time_t t = time(NULL); // 1. Is this a seed thing. 2. Fix the time conversions
	struct tm tm = *localtime(&t);
	double dayTimeComposition = (float)tm.tm_sec / (60.0 * 60.0 * 24.0) + (float)tm.tm_min / (60.0 * 24.0) + (float)tm.tm_hour / 24.0;
	std::string dataID = std::to_string(tm.tm_yday) + std::to_string(tm.tm_year - 100);
	taskID = std::stof(dataID.c_str()) + dayTimeComposition;

}

Tasks::~Tasks() {}


