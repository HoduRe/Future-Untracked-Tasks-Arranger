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

	bool started;
	bool completed;
	bool deadline;
	bool textOpen;
	bool subtasksOpen;
	bool onTheGym;
	bool woke;
	bool deprived;
	bool pained;
	bool wasted;
	bool operative;

	char name[NAME_BUFFER];
	char description[DESCRIPTION_BUFFER];
	std::string initialDate;
	std::string finalDate;

	std::vector<Tasks> subtaskList;

};

double GenerateID();

#endif