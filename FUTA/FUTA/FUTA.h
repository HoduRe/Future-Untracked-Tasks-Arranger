#ifndef FUTA_H_
#define FUTA_H_

#include <vector>
#include "Tasks.h"
#include "PugiXml/src/pugixml.hpp"

struct FUTAFilter {

	FUTAFilter();

	int orderType;
	bool onlyDeadlines;
	bool onlyProgressible;
	bool onlyStarted;
	bool onlyNonCompleted;
	bool onlyNoType;
	bool onlyMindless;
	bool onlyMinimalFocus;
	bool onlyMaximumFocus;

};

class FUTA {

public:

	FUTA();
	~FUTA();

	void Update(int _screenWidth, int _screenHeight);
	void LoadData();
	void SaveData();

private:

	void DrawOptions();

	void DrawTaskList();
	void DrawTask(Tasks& task);
	void DrawBasicTaskData(Tasks& task, Tasks* parentTask = nullptr);
	void DrawProgressState(Tasks& task);
	void DrawEffortType(Tasks& task);
	void DrawDates(Tasks& task);
	void DrawSubtasks(Tasks& task);
	bool DrawDeletePopUp(double taskID);
	bool DrawAllDeletePopUp();

	void DrawColoredButton(bool& condition, std::string buttonName);
	std::string ConstructItemName(std::string itemName, double id);
	void ComposeFUTADate(std::string startDate, std::string endDate, int& startDay, int& startMonth, int& startYear, int& endDay, int& endMonth, int& endYear);
	void ComposeTaskDate(std::string& startDate, std::string& endDate, int startDay, int startMonth, int startYear, int endDay, int endMonth, int endYear);

	void AddSeparator();
	void AddTabulation();
	void AddSpacedText(std::string text);

	void SaveTaskData(Tasks& task, pugi::xml_node node);
	Tasks LoadTaskData(pugi::xml_node node);
	void SaveFilterData();
	void LoadFilterData();
	void DeleteTask();

	void ReorderTaskVector();

	int screenWidth, screenHeight, currentYear;
	double toDeleteID;
	bool FUTAmenu;

	bool reorderVector;

	std::vector<Tasks> taskList;
	FUTAFilter filterOptions;
	FUTAFilter userFilterOptions;

};

#endif