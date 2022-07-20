#ifndef TASKS_H_
#define TASKS_H_

#include <string>
#include <vector>

#define NAME_BUFFER 75
#define DESCRIPTION_BUFFER 250

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

	char name[NAME_BUFFER];
	char description[DESCRIPTION_BUFFER];
	std::string initialDate;
	std::string finalDate;

	std::vector<Tasks> subtaskList;

};

double GenerateID();

#endif