#include "imgui.h"
#include "FUTA.h"
#include <time.h>

FUTAFilter::FUTAFilter() : orderType(0), onlyDeadlines(false), onlyProgressible(false), onlyStarted(false), onlyNonCompleted(true), onlyMindless(false),
onlyMinimalFocus(false), onlyMaximumFocus(false) {}

FUTA::FUTA() : FUTAmenu(true), toDeleteID(0), screenWidth(0), screenHeight(0), filterOptions(), userFilterOptions(), reorderVector(false) {

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	currentYear = tm.tm_year + 1900;

}

FUTA::~FUTA() {}

void FUTA::Update(int _screenWidth, int _screenHeight) {

	screenWidth = _screenWidth;
	screenHeight = _screenHeight;

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
	ImGui::Begin("FUTA options", &FUTAmenu, flags);
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(screenWidth, screenHeight));

	//////// Window content ------------------------------------------------------

	ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_DefaultOpen;

	if (ImGui::CollapsingHeader("FUTA preferences", headerFlags)) { DrawOptions(); }
	if (ImGui::CollapsingHeader("FUTA tasks", headerFlags)) { DrawTaskList(); }

	//////// ------------------------------------------------------

	if (reorderVector) { ReorderTaskVector(); }

	DeleteTask();
	ImGui::End();

}


#pragma region DRAW_OPTIONS

void FUTA::DrawOptions() {

	AddTabulation();
	ImGui::Text("Select ordering options:");
	AddSpacedText(""); AddSpacedText(""); AddSpacedText(""); AddSpacedText(""); AddSpacedText("");

	if (ImGui::Button("Save current options as default")) {

		userFilterOptions = filterOptions;
		SaveFilterData();

	}
	ImGui::SameLine(); if (ImGui::Button("Restore filtering options")) { filterOptions = FUTAFilter(); }

	AddTabulation(); AddSpacedText("");
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.25);
	const char* filterItems[] = { "By name", "By starting date", "By importance" };
	if (ImGui::Combo("Task order", &filterOptions.orderType, filterItems, IM_ARRAYSIZE(filterItems))) { reorderVector = true; }
	ImGui::SameLine();
	if (ImGui::Button("Rearrange again")) { reorderVector = true; }


	AddTabulation();
	ImGui::Text("Select filtering 'only x' options:"); AddSpacedText("");
	ImGui::Checkbox("Deadlines", &filterOptions.onlyDeadlines); AddSpacedText("");
	ImGui::Checkbox("Progressible", &filterOptions.onlyProgressible); AddSpacedText("");
	ImGui::Checkbox("Started##Filter", &filterOptions.onlyStarted); AddSpacedText("");
	ImGui::Checkbox("Non-Completed", &filterOptions.onlyNonCompleted);

	AddTabulation();
	ImGui::Text("Select filtering 'exclude x' options:"); AddSpacedText("");
	ImGui::Checkbox("Mindless focus", &filterOptions.onlyMindless); AddSpacedText("");
	ImGui::Checkbox("Minimal focus", &filterOptions.onlyMinimalFocus); AddSpacedText("");
	ImGui::Checkbox("Maximum focus", &filterOptions.onlyMaximumFocus); ImGui::NewLine();

}

#pragma endregion


#pragma region DRAW_TASK_LIST

void FUTA::DrawTaskList() {

	ImGui::Text("Create new task"); ImGui::SameLine();
	if (ImGui::Button("+")) {
		
		taskList.emplace(taskList.begin(), Tasks());
		filterOptions = FUTAFilter();
	
	}

	ImGui::SameLine(); AddSpacedText("");
	if (DrawAllDeletePopUp()) {
		for (int i = 0; i < taskList.size();) {
			if (taskList[i].completed) { taskList.erase(taskList.begin() + i); }
			else { i++; }
		}
	}

	ImGui::SameLine();

	for (int i = 0; i < taskList.size(); i++) {

		bool draw = true;
		if (filterOptions.onlyDeadlines && taskList[i].deadline == false) { draw = false; }
		if (filterOptions.onlyProgressible && (taskList[i].progressionState == 0 || taskList[i].progressionState == 1) == false) { draw = false; }
		if (filterOptions.onlyStarted && taskList[i].started == false) { draw = false; }
		if (filterOptions.onlyNonCompleted && taskList[i].completed) { draw = false; }
		if (filterOptions.onlyMindless && taskList[i].effort == 1) { draw = false; }
		if (filterOptions.onlyMinimalFocus && taskList[i].effort == 2) { draw = false; }
		if (filterOptions.onlyMaximumFocus && taskList[i].effort == 3) { draw = false; }
		if (draw) { DrawTask(taskList[i]); }

	}

}


void FUTA::DrawTask(Tasks& task) {

	AddSeparator();

	ImGui::NewLine();
	DrawBasicTaskData(task, nullptr);
	ImGui::NewLine();

	if (ImGui::TreeNode(("Task data" + ConstructItemName("taskData", task.taskID)).c_str())) {

		DrawProgressState(task);
		DrawEffortType(task);

		AddSpacedText("");
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.08);
		ImGui::InputInt(("Importance" + ConstructItemName("importance", task.taskID)).c_str(), &task.importance);

		DrawDates(task);

		AddTabulation();
		if (ImGui::TreeNode(("Task description" + ConstructItemName("taskDescription", task.taskID)).c_str())) {

			AddTabulation();
			ImGuiInputTextFlags descriptionFlags = ImGuiInputTextFlags_NoHorizontalScroll;
			ImVec2 descriptionBoxSize = ImVec2(ImGui::GetWindowWidth() * 0.8, ImGui::GetWindowHeight() * 0.1);
			ImGui::InputTextMultiline(ConstructItemName("description", task.taskID).c_str(), (char*)task.description, IM_ARRAYSIZE(task.description), descriptionBoxSize, descriptionFlags);
			ImGui::TreePop();

		}

		DrawSubtasks(task);

		ImGui::TreePop();

	}

}


void FUTA::DrawBasicTaskData(Tasks& task, Tasks* parentTask) {

	ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_EnterReturnsTrue;
	ImGui::SameLine(); ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.5);
	ImGui::InputText(ConstructItemName("name", task.taskID).c_str(), (char*)task.name, IM_ARRAYSIZE(task.name), inputTextFlags);
	AddSpacedText("Started");
	DrawColoredButton(task.started, ConstructItemName("started", task.taskID));
	AddSpacedText("Completed");
	DrawColoredButton(task.completed, ConstructItemName("completed", task.taskID));
	AddSpacedText("\t");

	if (DrawDeletePopUp(task.taskID)) {

		if (parentTask == nullptr) { toDeleteID = task.taskID; return; }

		for (int i = 0; i < parentTask->subtaskList.size(); i++) {

			if (parentTask->subtaskList[i].taskID == task.taskID) {

				parentTask->subtaskList.erase(parentTask->subtaskList.begin() + i);
				i = parentTask->subtaskList.size();

			}

		}

	}

}


void FUTA::DrawProgressState(Tasks& task) {

	AddTabulation();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.25);
	const char* progressionItems[] = { "No state", "In progress", "Waiting for another task", "Waiting for other people", "Postposed" };
	ImGui::Combo(("Current Progression State" + ConstructItemName("progressState", task.taskID)).c_str(), &task.progressionState, progressionItems, IM_ARRAYSIZE(progressionItems));

}


void FUTA::DrawEffortType(Tasks& task) {

	AddSpacedText("");
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.15);
	const char* effortItems[] = { "No type", "Mindless", "Minimal focus", "Maximum focus" };
	ImGui::Combo(("Effort Type" + ConstructItemName("effortType", task.taskID)).c_str(), &task.effort, effortItems, IM_ARRAYSIZE(effortItems));

}


void FUTA::DrawDates(Tasks& task) {

	AddTabulation();
	int startDay = 0; int startMonth = 0; int startYear = currentYear;
	int endDay = 0; int endMonth = 0; int endYear = currentYear;

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

}


void FUTA::DrawSubtasks(Tasks& task) {

	AddTabulation();

	if (ImGui::TreeNode(("Subtasks" + ConstructItemName("subtasks", task.taskID)).c_str())) {

		AddTabulation();
		if (ImGui::Button(("Add Subtasks" + ConstructItemName("addSubtasks", task.taskID)).c_str())) { task.subtaskList.push_back(Tasks()); }

		for (int i = 0; i < task.subtaskList.size(); i++) {

			Tasks& newTask = task.subtaskList[i];

			AddTabulation();
			DrawBasicTaskData(newTask, &task);
			DrawProgressState(newTask);
			ImGui::NewLine();

		}

		ImGui::TreePop();

	}

}


bool FUTA::DrawDeletePopUp(double taskID) {

	int selectedOption = -1;
	const char* options[] = { "Yes", "No" };

	std::string nameID = ConstructItemName("taskDelete", taskID);

	if (ImGui::Button(("Delete task" + nameID).c_str())) { ImGui::OpenPopup(("Deletion menu" + nameID).c_str()); }
	if (ImGui::BeginPopup(("Deletion menu" + nameID).c_str())) {

		ImGui::Text("Are you sure?");
		ImGui::Separator();
		for (int i = 0; i < IM_ARRAYSIZE(options); i++) { if (ImGui::Selectable(options[i])) { selectedOption = i; } }
		ImGui::EndPopup();

	}

	if (selectedOption == 0) { return true; }
	return false;

}

bool FUTA::DrawAllDeletePopUp() {

	int selectedOption = -1;
	const char* options[] = { "Yes", "No" };

	if (ImGui::Button("Delete all completed tasks")) { ImGui::OpenPopup("All deletion menu"); }
	if (ImGui::BeginPopup("All deletion menu")) {

		ImGui::Text("Are you sure?");
		ImGui::Separator();
		for (int i = 0; i < IM_ARRAYSIZE(options); i++) { if (ImGui::Selectable(options[i])) { selectedOption = i; } }
		ImGui::EndPopup();

	}

	if (selectedOption == 0) { return true; }
	return false;

}

#pragma endregion

