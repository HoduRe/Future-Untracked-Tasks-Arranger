#include "imgui.h"
#include "FUTA.h"
#include <algorithm>

#define FUTA_DATA_FILE_NAME "FUTAPlanning.txt"
#define FUTA_FILTER_FILE_NAME "FUTAFilter.txt"

void FUTA::SaveData() {

	pugi::xml_document doc;

	for (int taskIndex = 0; taskIndex < taskList.size(); taskIndex++) {
		SaveTaskData(taskList[taskIndex], doc.append_child(("Task" + std::to_string(taskIndex)).c_str()));
	}

	doc.save_file(FUTA_DATA_FILE_NAME);

	SaveFilterData();

}


void FUTA::LoadData() {

	pugi::xml_document doc;
	pugi::xml_parse_result futaFile = doc.load_file(FUTA_DATA_FILE_NAME);

	if (futaFile != NULL) {
		for (pugi::xml_node taskNode = doc.first_child(); taskNode; taskNode = taskNode.next_sibling()) { taskList.push_back(LoadTaskData(taskNode)); }
	}

	LoadFilterData();
	ReorderTaskVector();

}


void FUTA::SaveTaskData(Tasks& task, pugi::xml_node node) {

	node.append_attribute("taskID") = std::to_string(task.taskID).c_str();
	node.append_attribute("name") = task.name;
	node.append_attribute("importance") = task.importance;
	node.append_attribute("progressionState") = task.progressionState;
	node.append_attribute("started") = task.started;
	node.append_attribute("completed") = task.completed;
	node.append_attribute("deadline") = task.deadline;
	node.append_attribute("textOpen") = task.textOpen;
	node.append_attribute("subtasksOpen") = task.subtasksOpen;
	node.append_attribute("description") = task.description;
	node.append_attribute("initialDate") = task.initialDate.c_str();
	node.append_attribute("finalDate") = task.finalDate.c_str();
	node.append_attribute("onTheGym") = task.onTheGym;
	node.append_attribute("woke") = task.woke;
	node.append_attribute("deprived") = task.deprived;
	node.append_attribute("pained") = task.pained;
	node.append_attribute("wasted") = task.wasted;
	node.append_attribute("operative") = task.operative;

	for (int subtaskIndex = 0; subtaskIndex < task.subtaskList.size(); subtaskIndex++) {

		Tasks& subtask = task.subtaskList[subtaskIndex];
		pugi::xml_node subtaskNode = node.append_child(("Subtask" + std::to_string(subtaskIndex)).c_str());
		SaveTaskData(subtask, subtaskNode);

	}

}


Tasks FUTA::LoadTaskData(pugi::xml_node node) {

	Tasks newTask;
	newTask.taskID = std::stod((std::string)node.attribute("taskID").as_string());
	newTask.importance = node.attribute("importance").as_int();
	newTask.progressionState = node.attribute("progressionState").as_int();
	newTask.started = node.attribute("started").as_bool();
	newTask.completed = node.attribute("completed").as_bool();
	newTask.deadline = node.attribute("deadline").as_bool();
	newTask.textOpen = node.attribute("textOpen").as_bool();
	newTask.subtasksOpen = node.attribute("subtasksOpen").as_bool();
	newTask.initialDate = node.attribute("initialDate").as_string();
	newTask.finalDate = node.attribute("finalDate").as_string();
	newTask.onTheGym = node.attribute("onTheGym").as_bool();
	newTask.woke = node.attribute("woke").as_bool();
	newTask.deprived = node.attribute("deprived").as_bool();
	newTask.pained = node.attribute("pained").as_bool();
	newTask.wasted = node.attribute("wasted").as_bool();
	newTask.operative = node.attribute("operative").as_bool();

	char* nameBuffer = (char*)node.attribute("name").as_string();
	for (int stringIndex = 0; stringIndex < NAME_BUFFER; stringIndex++) { newTask.name[stringIndex] = nameBuffer[stringIndex]; }

	char* descriptionBuffer = (char*)node.attribute("description").as_string();
	for (int stringIndex = 0; stringIndex < DESCRIPTION_BUFFER; stringIndex++) { newTask.description[stringIndex] = descriptionBuffer[stringIndex]; }

	for (pugi::xml_node subtaskNode = node.first_child(); subtaskNode; subtaskNode = subtaskNode.next_sibling()) { newTask.subtaskList.push_back(LoadTaskData(subtaskNode)); }

	return newTask;

}


void FUTA::SaveFilterData() {

	pugi::xml_document doc;

	pugi::xml_node filterNode = doc.append_child("FilterOptions");

	filterNode.append_attribute("orderType") = userFilterOptions.orderType;
	filterNode.append_attribute("onlyDeadlines") = userFilterOptions.onlyDeadlines;
	filterNode.append_attribute("onlyProgressible") = userFilterOptions.onlyProgressible;
	filterNode.append_attribute("onlyNonProgressible") = userFilterOptions.onlyNonProgressible;
	filterNode.append_attribute("onlyStarted") = userFilterOptions.onlyStarted;
	filterNode.append_attribute("onlyNonCompleted") = userFilterOptions.onlyNonCompleted;
	filterNode.append_attribute("onlyWoke") = userFilterOptions.onlyWoke;
	filterNode.append_attribute("onlyDeprived") = userFilterOptions.onlyDeprived;
	filterNode.append_attribute("onlyPained") = userFilterOptions.onlyPained;
	filterNode.append_attribute("onlyWasted") = userFilterOptions.onlyWasted;
	filterNode.append_attribute("onlyOperative") = userFilterOptions.onlyOperative;
	filterNode.append_attribute("allWoke") = userFilterOptions.allWoke;
	filterNode.append_attribute("allDeprived") = userFilterOptions.allDeprived;
	filterNode.append_attribute("allPained") = userFilterOptions.allPained;
	filterNode.append_attribute("allWasted") = userFilterOptions.allWasted;
	filterNode.append_attribute("allOperative") = userFilterOptions.allOperative;
	filterNode.append_attribute("allNoneType") = userFilterOptions.allNoneType;
	filterNode.append_attribute("onlyNonRecurrent") = userFilterOptions.onlyNonRecurrent;
	filterNode.append_attribute("onlyOnTheGym") = userFilterOptions.onlyOnTheGym;

	doc.save_file(FUTA_FILTER_FILE_NAME);

}


void FUTA::LoadFilterData() {

	pugi::xml_document doc;
	pugi::xml_parse_result futaFile = doc.load_file(FUTA_FILTER_FILE_NAME);

	if (futaFile != NULL) {

		pugi::xml_node filterNode = doc.first_child();
		filterOptions.orderType = filterNode.attribute("orderType").as_int();
		filterOptions.onlyDeadlines = filterNode.attribute("onlyDeadlines").as_bool();
		filterOptions.onlyProgressible = filterNode.attribute("onlyProgressible").as_bool();
		filterOptions.onlyNonProgressible = filterNode.attribute("onlyNonProgressible").as_bool();
		filterOptions.onlyStarted = filterNode.attribute("onlyStarted").as_bool();
		filterOptions.onlyNonCompleted = filterNode.attribute("onlyNonCompleted").as_bool();
		filterOptions.onlyWoke = filterNode.attribute("onlyWoke").as_bool();
		filterOptions.onlyDeprived = filterNode.attribute("onlyDeprived").as_bool();
		filterOptions.onlyPained = filterNode.attribute("onlyPained").as_bool();
		filterOptions.onlyWasted = filterNode.attribute("onlyWasted").as_bool();
		filterOptions.onlyOperative = filterNode.attribute("onlyOperative").as_bool();
		filterOptions.allWoke = filterNode.attribute("allWoke").as_bool();
		filterOptions.allDeprived = filterNode.attribute("allDeprived").as_bool();
		filterOptions.allPained = filterNode.attribute("allPained").as_bool();
		filterOptions.allWasted = filterNode.attribute("allWasted").as_bool();
		filterOptions.allOperative = filterNode.attribute("allOperative").as_bool();
		filterOptions.allNoneType = filterNode.attribute("allNoneType").as_bool();
		filterOptions.onlyNonRecurrent = filterNode.attribute("onlyNonRecurrent").as_bool();
		filterOptions.onlyOnTheGym = filterNode.attribute("onlyOnTheGym").as_bool();

	}

}


void FUTA::DeleteTask() {

	if (toDeleteID != 0) {

		for (int i = 0; i < taskList.size(); i++) {

			if (taskList[i].taskID == toDeleteID) {

				taskList.erase(taskList.begin() + i);
				i = taskList.size();

			}

		}
	}

	toDeleteID = 0;

}


void FUTA::ComposeFUTADate(std::string startDate, std::string endDate, int& startDay, int& startMonth, int& startYear, int& endDay, int& endMonth, int& endYear) {

	if (startDate.size() >= 8) {

		startDay = stoi(startDate.substr(0, 2));
		startMonth = stoi(startDate.substr(2, 2));
		startYear = stoi(startDate.substr(4, startDate.size() - 4));

	}

	if (endDate.size() >= 8) {

		endDay = stoi(endDate.substr(0, 2));
		endMonth = stoi(endDate.substr(2, 2));
		endYear = stoi(endDate.substr(4, startDate.size() - 4));

	}

}


void ComposeDateSet(std::string& date, int day, int month, int year) {

	if (day < 0) { day = 0; }
	if (month < 0) { month = 0; }
	if (year < 0) { year = 0; }

	if (day > 31) { day = 31; }
	if (month > 12) { month = 12; }
	if (year > 9999) { year = 9999; }

	if (day < 10) { date = "0" + std::to_string(day); }
	else { date = std::to_string(day); }
	if (month < 10) { date += "0" + std::to_string(month); }
	else { date += std::to_string(month); }
	if (year < 1000) { date += "000" + std::to_string(year); }
	else { date += std::to_string(year); }

}

void FUTA::ComposeTaskDate(std::string& startDate, std::string& endDate, int startDay, int startMonth, int startYear, int endDay, int endMonth, int endYear) {

	ComposeDateSet(startDate, startDay, startMonth, startYear);
	ComposeDateSet(endDate, endDay, endMonth, endYear);

}


std::string FUTA::ConstructItemName(std::string itemName, double id) { return "##" + itemName + std::to_string(id); }


void FUTA::DrawColoredButton(bool& condition, std::string buttonName) {

	if (condition) {

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(0.0f, 1.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(0.5f, 1.0f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0.5f, 1.0f, 0.5f));

	}

	else if (!condition) {

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(1.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(1.0f, 0.5f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(1.0f, 0.5f, 0.5f));

	}

	if (ImGui::Button(buttonName.c_str(), ImVec2(20, 20))) { condition = !condition; }
	ImGui::PopStyleColor(3);

}


void FUTA::AddSeparator() { ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine(); }
void FUTA::AddTabulation() { ImGui::NewLine(); ImGui::Text("\t"); ImGui::SameLine(); }
void FUTA::AddSpacedText(std::string text) { ImGui::SameLine(); ImGui::Text(("\t" + text).c_str()); ImGui::SameLine(); }


bool ReorderByName(Tasks& taskA, Tasks& taskB) {

	for (int i = 0; i < NAME_BUFFER; i++) {

		char auxA = taskA.name[i];
		char auxB = taskB.name[i];

		if (auxA >= 97 && auxA <= 122) { auxA -= 32; }
		if (auxB >= 97 && auxB <= 122) { auxB -= 32; }
		if (auxA != auxB) { return auxA < auxB; }

	}

	return true;
}

std::string ComposeReverseDate(std::string& originalString) {

	std::string auxString;

	if (originalString.size() > 4) {

		auxString = originalString.substr(4, originalString.size() - 4);
		auxString += originalString.substr(2, 2);
		auxString += originalString.substr(0, 2);

	}

	return auxString;
}

bool ReorderByStartingDate(Tasks& taskA, Tasks& taskB) {

	std::string auxA = ComposeReverseDate(taskA.initialDate);
	std::string auxB = ComposeReverseDate(taskB.initialDate);

	if (auxA.size() == 0) { auxA = "20220000"; }
	if (auxB.size() == 0) { auxB = "20220000"; }

	// Ensure dates that start by the 0-0-currentYear don't get shown first:
	if (auxA.size() > 7 && auxA.compare(4, 1, "0") == 0 && auxA.compare(5, 1, "0") == 0 && auxA.compare(6, 1, "0") == 0 && auxA.compare(7, 1, "0") == 0) { return false; }
	if (auxB.size() > 7 && auxB.compare(4, 1, "0") == 0 && auxB.compare(5, 1, "0") == 0 && auxB.compare(6, 1, "0") == 0 && auxB.compare(7, 1, "0") == 0) { return true; }

	return std::stoi(auxA) < std::stoi(auxB);

}


bool ReorderByImportance(Tasks& taskA, Tasks& taskB) { return taskA.importance > taskB.importance; }


void FUTA::ReorderTaskVector() {

	switch (filterOptions.orderType) {

	case 0: std::sort(taskList.begin(), taskList.end(), ReorderByName); break;
	case 1: std::sort(taskList.begin(), taskList.end(), ReorderByStartingDate); break;
	case 2: std::sort(taskList.begin(), taskList.end(), ReorderByImportance); break;

	default: break;
	}

	reorderVector = false;

}


