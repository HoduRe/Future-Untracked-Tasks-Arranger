#ifndef FUTA_H_
#define FUTA_H_

#include <vector>

class Tasks;

class FUTA {

public:

	FUTA();
	~FUTA();

	void Update(int screenWidth, int screenHeight);

private:

	void DrawOptions();

	void DrawAddNewTask();


	void DrawTaskList();

	void DrawTask(Tasks& task);


	bool FUTAmenu;
	bool drawNewTask;
	Tasks* provisionalNewTask;
	std::vector<Tasks> taskList;

};

#endif