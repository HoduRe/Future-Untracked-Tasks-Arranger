#ifndef FUTA_H_
#define FUTA_H_

#include <vector>
#include "Tasks.h"
#include "PugiXml/src/pugixml.hpp"

class FUTA {

public:

	FUTA();
	~FUTA();

	void Update(int _screenWidth, int _screenHeight);
	void LoadData();
	void SaveData();

private:

	void DrawOptions();

	void DrawAddNewTask();


	void DrawTaskList();

	void DrawTask(Tasks& task, bool isNewTask = false);
	void DrawBasicTaskData(Tasks& task, Tasks* parentTask = nullptr, bool isNewTask = false);
	void DrawProgressState(Tasks& task);
	void DrawEffortType(Tasks& task);
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

	void SaveTaskData(Tasks& task, pugi::xml_node node);
	Tasks LoadTaskData(pugi::xml_node node);
	void DeleteTask();

	int screenWidth, screenHeight;
	double toDeleteID;
	bool FUTAmenu;
	bool drawNewTask;
	Tasks provisionalNewTask;
	std::vector<Tasks> taskList;

};

#endif