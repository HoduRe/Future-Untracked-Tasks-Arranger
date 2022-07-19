#ifndef TASKS_H_
#define TASKS_H_

#include <string>
#include <vector>

class Tasks {

public:

	Tasks();
	~Tasks();

	double taskID;
	int importance;
	int progressionState;
	int effort;

	bool started;
	bool completed;
	bool deadline;

	std::string name;
	std::string description;
	std::string initialDate;
	std::string finalDate;

	std::vector<Tasks> subtaskList;

};

double GenerateID();

#endif