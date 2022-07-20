#include "imgui.h"
#include "FUTA.h"

#define FUTA_FILE_NAME "FUTAPlanning.txt"

void FUTA::SaveData() {

	pugi::xml_document doc;

	for (int taskIndex = 0; taskIndex < taskList.size(); taskIndex++) {
		SaveTaskData(taskList[taskIndex], doc.append_child(std::to_string(taskList[taskIndex].taskID).c_str()));
	}

	doc.save_file(FUTA_FILE_NAME);

}


void FUTA::LoadData() {

	pugi::xml_document doc;
	pugi::xml_parse_result futaFile = doc.load_file(FUTA_FILE_NAME);

	if (futaFile != NULL) {
		for (pugi::xml_node taskNode = doc.first_child(); taskNode; taskNode = taskNode.next_sibling()) { taskList.push_back(LoadTaskData(taskNode)); }
	}

}


void FUTA::SaveTaskData(Tasks& task, pugi::xml_node node) {

	node.append_attribute("name") = task.name.c_str();
	node.append_attribute("importance") = task.importance;
	node.append_attribute("progressionState") = task.progressionState;
	node.append_attribute("effort") = task.effort;
	node.append_attribute("started") = task.started;
	node.append_attribute("completed") = task.completed;
	node.append_attribute("deadline") = task.deadline;
	node.append_attribute("description") = task.description.c_str();
	node.append_attribute("initialDate") = task.initialDate.c_str();
	node.append_attribute("finalDate") = task.finalDate.c_str();

	for (int subtaskIndex = 0; subtaskIndex < task.subtaskList.size(); subtaskIndex++) {

		Tasks& subtask = task.subtaskList[subtaskIndex];
		pugi::xml_node subtaskNode = node.append_child(std::to_string(subtask.taskID).c_str());
		SaveTaskData(subtask, subtaskNode);

	}

}


Tasks FUTA::LoadTaskData(pugi::xml_node node) {

	Tasks newTask;
	newTask.taskID = std::stod((std::string)node.name());
	newTask.importance = node.attribute("importance").as_int();
	newTask.progressionState = node.attribute("progressionState").as_int();
	newTask.effort = node.attribute("effort").as_int();
	newTask.started = node.attribute("started").as_bool();
	newTask.completed = node.attribute("completed").as_bool();
	newTask.deadline = node.attribute("deadline").as_bool();
	newTask.description = node.attribute("description").as_string();
	newTask.initialDate = node.attribute("initialDate").as_string();
	newTask.finalDate = node.attribute("finalDate").as_string();

	for (pugi::xml_node subtaskNode = node.first_child(); subtaskNode; subtaskNode = subtaskNode.next_sibling()) { newTask.subtaskList.push_back(LoadTaskData(subtaskNode)); }

	return newTask;

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


void FUTA::ComposeTaskDate(std::string& startDate, std::string& endDate, int startDay, int startMonth, int startYear, int endDay, int endMonth, int endYear) {

	if (startDay < 10) { startDate = "0" + std::to_string(startDay); }
	else { startDate = std::to_string(startDay); }
	if (startMonth < 10) { startDate += "0" + std::to_string(startMonth); }
	else { startDate += std::to_string(startMonth); }
	if (startYear < 1000) { startDate += "000" + std::to_string(startYear); }
	else { startDate += std::to_string(startYear); }

	if (endDay < 10) { endDate = "0" + std::to_string(endDay); }
	else { endDate = std::to_string(endDay); }
	if (endMonth < 10) { endDate += "0" + std::to_string(endMonth); }
	else { endDate += std::to_string(endMonth); }
	if (endYear < 1000) { endDate += "000" + std::to_string(endYear); }
	else { endDate += std::to_string(endYear); }

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
