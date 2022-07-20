#ifndef FUTA_H_
#define FUTA_H_

#include <vector>
#include "Tasks.h"

class FUTA {

public:

	FUTA();
	~FUTA();

	void Update(int _screenWidth, int _screenHeight);

private:

	void DrawOptions();

	void DrawAddNewTask();


	void DrawTaskList();

	void DrawTask(Tasks& task, bool isNewTask = false);
	void DrawBasicTaskData(Tasks& task, Tasks* parentTask = nullptr, bool isNewTask = false);
	void DrawDates(Tasks& task);
	void DrawSubtasks(Tasks& task);
	bool DrawDeletePopUp(double taskID);

	void DrawColoredButton(bool& condition, std::string buttonName);
	std::string ConstructItemName(std::string itemName, double id);
	void ComposeFUTADate(std::string startDate, std::string endDate, int& startDay, int& startMonth, int& startYear, int& endDay, int& endMonth, int& endYear);
	void ComposeTaskDate(std::string& startDate, std::string& endDate, int startDay, int startMonth, int startYear, int endDay, int endMonth, int endYear);

	void AddSeparator();
	void AddTabulation();
	void AddSpacedText(std::string text);

	void DeleteTask();

	int screenWidth, screenHeight;
	double toDeleteID;
	bool FUTAmenu;
	bool drawNewTask;
	Tasks provisionalNewTask;
	std::vector<Tasks> taskList;

};

#endif