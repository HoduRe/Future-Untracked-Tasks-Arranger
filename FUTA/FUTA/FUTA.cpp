#include "imgui.h"
#include "FUTA.h"

FUTA::FUTA() : FUTAmenu(true), drawNewTask(false), provisionalNewTask() {}

FUTA::~FUTA() {}

void FUTA::Update(int screenWidth, int screenHeight) {

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
	ImGui::Begin("FUTA options", &FUTAmenu, flags);
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(screenWidth, screenHeight));

	//////// Window content ------------------------------------------------------

	ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_DefaultOpen;

	if (ImGui::CollapsingHeader("FUTA preferences")) { DrawOptions(); }
	if (ImGui::CollapsingHeader("FUTA tasks", headerFlags)) { DrawTaskList(); }

	//////// ------------------------------------------------------

	ImGui::End();

}


#pragma region DRAW_OPTIONS

void FUTA::DrawOptions() {


	DrawAddNewTask();

}


void FUTA::DrawAddNewTask() {

	ImGui::Text("Create new task");
	ImGui::SameLine();

	if (ImGui::Button("+")) {

		drawNewTask = true;
		provisionalNewTask = Tasks();

	}

	if (drawNewTask) {

		DrawTask(provisionalNewTask);
		if (ImGui::Button("Add new task")) {

			taskList.push_back(provisionalNewTask);
			drawNewTask = false;

		}

	}

}


#pragma endregion


#pragma region DRAW_TASK_LIST

void FUTA::DrawTaskList() {

	for (int i = 0; i < taskList.size(); i++) { DrawTask(taskList[i]); }

}


void FUTA::DrawTask(Tasks& task) {

	AddSeparator();

	// Add indicator if there are dependencies (since we use string for name, we can have a list for that? Just ommit yourself in the listing, and order them by name)
	ImGui::Text(task.name.c_str());
	AddSpacedText("Started");
	ImGui::Checkbox(ConstructItemName("started", task.taskID).c_str(), &task.started);
	AddSpacedText("Completed");
	ImGui::Checkbox(ConstructItemName("completed", task.taskID).c_str(), &task.completed);
	AddSpacedText("\t");
	if (ImGui::Button("Delete task")) { /*TODO THIS*/ }

	AddTabulation();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.25);
	const char* progressionItems[] = { "No state", "In progress", "Waiting for others", "Postposed" };
	ImGui::Combo("Current Progression State", &task.progressionState, progressionItems, IM_ARRAYSIZE(progressionItems));

	AddTabulation();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.25);
	const char* effortItems[] = { "No type", "Mindless", "Minimal focus", "Maximum focus" };
	ImGui::Combo("Effort Type", &task.effort, effortItems, IM_ARRAYSIZE(effortItems));

	AddTabulation();
	int startDay = 18; int startMonth = 7; int startYear = 2022;
	int endDay = 25; int endMonth = 7; int endYear = 2022;
	
	ComposeFUTADate(task.initialDate, task.finalDate, startDay, startMonth, startYear, endDay, endMonth, endYear);
	ImGui::Text("Starting Date (D-M-Y)"); ImGui::SameLine(); ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.1);

	ImGui::InputInt(ConstructItemName("startDay", task.taskID).c_str(), &startDay); ImGui::SameLine(); ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.1);
	ImGui::InputInt(ConstructItemName("startMonth", task.taskID).c_str(), &startMonth); ImGui::SameLine(); ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.1);
	ImGui::InputInt(ConstructItemName("startYear", task.taskID).c_str(), &startYear); AddTabulation();
	
	ImGui::Text("Ending Date (D-M-Y)"); ImGui::SameLine(); ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.1);
	
	ImGui::InputInt(ConstructItemName("endDay", task.taskID).c_str(), &endDay); ImGui::SameLine(); ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.1);
	ImGui::InputInt(ConstructItemName("endMonth", task.taskID).c_str(), &endMonth); ImGui::SameLine(); ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.1);
	ImGui::InputInt(ConstructItemName("endYear", task.taskID).c_str(), &endYear); ImGui::SameLine();
	
	ComposeTaskDate(task.initialDate, task.finalDate, startDay, startMonth, startYear, endDay, endMonth, endYear);
	AddSpacedText("Deadline");
	ImGui::Checkbox(ConstructItemName("deadline", task.taskID).c_str(), &task.deadline);

	AddTabulation();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.1);
	ImGui::InputInt("Importance", &task.importance);

	AddSeparator();

}


void FUTA::ComposeFUTADate(std::string startDate, std::string endDate, int& startDay, int& startMonth, int& startYear, int& endDay, int& endMonth, int& endYear) {

	if(startDate.size() >= 8) {

		startDay = stoi(startDate.substr(0, 2));
		startMonth = stoi(startDate.substr(2, 2));
		startYear = stoi(startDate.substr(4, startDate.size() - 4));

	}

	if(endDate.size() >= 8) {

		endDay = stoi(endDate.substr(0, 2));
		endMonth = stoi(endDate.substr(2, 2));
		endYear = stoi(endDate.substr(4, startDate.size() - 4));

	}

}


void FUTA::ComposeTaskDate(std::string& startDate, std::string& endDate, int startDay, int startMonth, int startYear, int endDay, int endMonth, int endYear) {

	if (startDay < 10) { startDate = "0" + std::to_string(startDay); } else { startDate = std::to_string(startDay); }
	if (startMonth < 10) { startDate += "0" + std::to_string(startMonth); } else { startDate += std::to_string(startMonth); }
	if (startYear < 1000) { startDate += "000" + std::to_string(startYear); } else { startDate += std::to_string(startYear); }
	
	if (endDay < 10) { endDate = "0" + std::to_string(endDay); } else { endDate = std::to_string(endDay); }
	if (endMonth < 10) { endDate += "0" + std::to_string(endMonth); } else { endDate += std::to_string(endMonth); }
	if (endYear < 1000) { endDate += "000" + std::to_string(endYear); } else { endDate += std::to_string(endYear); }

}


std::string FUTA::ConstructItemName(std::string itemName, double id) { return "##" + itemName + std::to_string(id); }


void FUTA::AddSeparator() { ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine(); }
void FUTA::AddTabulation() { ImGui::NewLine(); ImGui::Text("\t"); ImGui::SameLine(); }
void FUTA::AddSpacedText(std::string text) { ImGui::SameLine(); ImGui::Text(("\t" + text).c_str()); ImGui::SameLine(); }

#pragma endregion

