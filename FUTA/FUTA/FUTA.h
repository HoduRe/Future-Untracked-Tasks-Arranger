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
	std::string ConstructItemName(std::string itemName, double id);
	void ComposeFUTADate(std::string startDate, std::string endDate, int& startDay, int& startMonth, int& startYear, int& endDay, int& endMonth, int& endYear);
	void ComposeTaskDate(std::string& startDate, std::string& endDate, int startDay, int startMonth, int startYear, int endDay, int endMonth, int endYear);


	void AddSeparator();
	void AddTabulation();
	void AddSpacedText(std::string text);

	bool FUTAmenu;
	bool drawNewTask;
	Tasks provisionalNewTask;
	std::vector<Tasks> taskList;

};

#endif