#include "Tasks.h"
#include <time.h>

Subtasks::Subtasks() : started(false), completed(false), progressionState(0), effort(0) {}

Subtasks::~Subtasks() {}

Tasks::Tasks() : importance(0), initialDate(-1), finalDate(-1), progressionState(0), effort(0), started(false), completed(false), deadline(false),
name("NewTask"), description(""), subtaskList() {

	time_t t = time(NULL); // 1. Is this a seed thing. 2. Fix the time conversions
	struct tm tm = *localtime(&t);
	printf("now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	taskID = 0;

}

Tasks::~Tasks() {}


