#ifndef FUTA_H_
#define FUTA_H_

#include <vector>
#include "Tasks.h"

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


	void AddSeparator();
	void AddTabulation();
	void AddSpacedText(std::string text);
	std::string ConstructItemName(std::string itemName, std::string taskName, int id);

	bool FUTAmenu;
	bool drawNewTask;
	Tasks provisionalNewTask;
	std::vector<Tasks> taskList;

};

#endif