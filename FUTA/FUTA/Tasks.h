#ifndef TASKS_H_
#define TASKS_H_

#include <string>
#include <vector>

class Subtasks {

public:

	Subtasks();
	~Subtasks();

	bool started;
	bool completed;

	int progressionState;
	int effort;

};


class Tasks {

public:

	Tasks();
	~Tasks();

	int taskID;
	int importance;
	int initialDate;
	int finalDate;
	int progressionState;
	int effort;

	bool started;
	bool completed;
	bool deadline;

	std::string name;
	std::string description;

	std::vector<Subtasks> subtaskList;

};

#endif