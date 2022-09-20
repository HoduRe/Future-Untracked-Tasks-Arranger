#include "imgui.h"
#include "FUTA.h"
#include <time.h>

FUTAFilter::FUTAFilter() : orderType(0), onlyDeadlines(false), onlyProgressible(false), onlyNonProgressible(false), onlyStarted(false), onlyNonCompleted(true), onlyWoke(false),
onlyDeprived(false), onlyPained(false), onlyWasted(false), onlyOperative(false), allWoke(true), allDeprived(true), allPained(true), allWasted(true), allOperative(true), allNoneType(true),
onlyNonRecurrent(false), onlyOnTheGym(false) {}

FUTA::FUTA() : FUTAmenu(true), toDeleteID(0), screenWidth(0), screenHeight(0), filterOptions(), userFilterOptions(), reorderVector(false), openAllTasks(false), closeAllTasks(false),
taskSearcher() {

	struct tm tm;
	time_t t = time(NULL);
	localtime_s(&tm, &t);
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
	if (ImGui::CollapsingHeader("FUTA started task")) { DrawStartedTasks(); }
	if (ImGui::CollapsingHeader("FUTA tasks", headerFlags)) { DrawTaskList(); }

	//////// ------------------------------------------------------

	if (reorderVector) { ReorderTaskVector(); }
	openAllTasks = false;
	closeAllTasks = false;

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
	ImGui::Checkbox("Non-progressible", &filterOptions.onlyNonProgressible); AddSpacedText("");
	ImGui::Checkbox("Non-recurrent", &filterOptions.onlyNonRecurrent); AddSpacedText("");
	ImGui::Checkbox("On the gym", &filterOptions.onlyOnTheGym); AddSpacedText("");
	ImGui::Checkbox("Started##Filter", &filterOptions.onlyStarted); AddSpacedText("");
	ImGui::Checkbox("Non-Completed", &filterOptions.onlyNonCompleted);

	AddTabulation();
	ImGui::Text("Select filtering 'all x' options:"); AddSpacedText("");
	ImGui::Checkbox("Waking up##AllX", &filterOptions.allWoke); AddSpacedText("");
	ImGui::Checkbox("Sleep deprived##AllX", &filterOptions.allDeprived); AddSpacedText("");
	ImGui::Checkbox("Physical pain##AllX", &filterOptions.allPained); AddSpacedText("");
	ImGui::Checkbox("Mentally wasted##AllX", &filterOptions.allWasted); AddSpacedText("");
	ImGui::Checkbox("Operative##AllX", &filterOptions.allOperative); AddSpacedText("");
	ImGui::Checkbox("No type##AllX", &filterOptions.allNoneType);

	AddTabulation();
	ImGui::Text("Select filtering 'exclude x' options:"); AddSpacedText("");
	ImGui::Checkbox("Waking up##ExcludeX", &filterOptions.onlyWoke); AddSpacedText("");
	ImGui::Checkbox("Sleep deprived##ExcludeX", &filterOptions.onlyDeprived); AddSpacedText("");
	ImGui::Checkbox("Physical pain##ExcludeX", &filterOptions.onlyPained); AddSpacedText("");
	ImGui::Checkbox("Mentally wasted##ExcludeX", &filterOptions.onlyWasted); AddSpacedText("");
	ImGui::Checkbox("Operative##ExcludeX", &filterOptions.onlyOperative);

	AddTabulation();
	ImGui::SetNextItemWidth(screenWidth * 0.3);
	ImGui::InputText("Task Name Searcher", (char*)taskSearcher, IM_ARRAYSIZE(taskSearcher)); ImGui::NewLine();



}

#pragma endregion


#pragma region DRAW_TASK_LIST

void FUTA::DrawStartedTasks() { DrawCurrentlyStartedTasks(); }

void FUTA::DrawCurrentlyStartedTasks() {

	std::vector<Tasks> wokeVec;
	std::vector<Tasks> deprivedVec;
	std::vector<Tasks> painedVec;
	std::vector<Tasks> wastedVec;
	std::vector<Tasks> operativeVec;

	for (int i = 0; i < taskList.size(); i++) {

		if (taskList[i].started) {

			if (taskList[i].woke) { wokeVec.push_back(taskList[i]); }
			if (taskList[i].deprived) { deprivedVec.push_back(taskList[i]); }
			if (taskList[i].pained) { painedVec.push_back(taskList[i]); }
			if (taskList[i].wasted) { wastedVec.push_back(taskList[i]); }
			if (taskList[i].operative) { operativeVec.push_back(taskList[i]); }

		}

	}

	ImGui::NewLine();
	AddSpacedText("Started tasks ordered by stress:");
	AddSeparator(); AddSpacedText(""); AddSpacedText("Waking up:");
	for (int i = 0; i < wokeVec.size(); i++) { AddTabulation(); AddSpacedText(""); AddSpacedText(wokeVec[i].name); }
	AddSeparator();	AddSpacedText(""); AddSpacedText("Sleep deprived:");
	for (int i = 0; i < deprivedVec.size(); i++) { AddTabulation(); AddSpacedText(""); AddSpacedText(deprivedVec[i].name); }
	AddSeparator(); AddSpacedText(""); AddSpacedText("Physically pained:");
	for (int i = 0; i < painedVec.size(); i++) { AddTabulation(); AddSpacedText(""); AddSpacedText(painedVec[i].name); }
	AddSeparator(); AddSpacedText(""); AddSpacedText("Mentally wasted:");
	for (int i = 0; i < wastedVec.size(); i++) { AddTabulation(); AddSpacedText(""); AddSpacedText(wastedVec[i].name); }
	AddSeparator(); AddSpacedText(""); AddSpacedText("Operative:");
	for (int i = 0; i < operativeVec.size(); i++) { AddTabulation(); AddSpacedText(""); AddSpacedText(operativeVec[i].name); }
	AddSeparator();

}

#pragma endregion


#pragma region DRAW_TASK_LIST

void FUTA::DrawTaskList() {

	ImGui::Text("Create new task"); ImGui::SameLine();
	if (ImGui::Button("+")) {

		taskList.emplace(taskList.begin(), Tasks());
		filterOptions = FUTAFilter();

	}

	AddSpacedText("");
	if (ImGui::Button("Open all tasks")) { openAllTasks = true; } AddSpacedText("");
	if (ImGui::Button("Close all tasks")) { closeAllTasks = true; }

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
		if (taskSearcher[0] != NULL) {
			std::string taskName = taskList[i].name;
			if (taskName.find(taskSearcher) == std::string::npos) { draw = false; }
		}
		if (filterOptions.onlyDeadlines && taskList[i].deadline == false) { draw = false; }
		if (filterOptions.onlyProgressible && (taskList[i].progressionState == 0 || taskList[i].progressionState == 1) == false) { draw = false; }
		if (filterOptions.onlyNonProgressible && (taskList[i].progressionState < 2)) { draw = false; }
		if (filterOptions.onlyNonRecurrent && (taskList[i].progressionState == 5)) { draw = false; }
		if (filterOptions.onlyOnTheGym && taskList[i].onTheGym == false) { draw = false; }
		if (filterOptions.onlyStarted && taskList[i].started == false) { draw = false; }
		if (filterOptions.onlyNonCompleted && taskList[i].completed) { draw = false; }
		if ((filterOptions.allWoke == false || taskList[i].woke == false) && (filterOptions.allDeprived == false || taskList[i].deprived == false) &&
			(filterOptions.allPained == false || taskList[i].pained == false) && (filterOptions.allWasted == false || taskList[i].wasted == false) &&
			(filterOptions.allOperative == false || taskList[i].operative == false) &&
			(filterOptions.allNoneType == false || (taskList[i].woke || taskList[i].deprived || taskList[i].pained || taskList[i].wasted || taskList[i].operative))) {
			draw = false;
		}
		// Yikes
		if (filterOptions.onlyWoke && taskList[i].woke) { draw = false; }
		if (filterOptions.onlyDeprived && taskList[i].deprived) { draw = false; }
		if (filterOptions.onlyPained && taskList[i].pained) { draw = false; }
		if (filterOptions.onlyWasted && taskList[i].wasted) { draw = false; }
		if (filterOptions.onlyOperative && taskList[i].operative) { draw = false; }
		if (draw) { DrawTask(taskList[i]); }

	}

}


void FUTA::DrawTask(Tasks& task) {

	AddSeparator();

	ImGui::NewLine();
	DrawBasicTaskData(task, nullptr);
	ImGui::NewLine();

	if (openAllTasks) { ImGui::SetNextItemOpen(openAllTasks); }
	if (closeAllTasks) { ImGui::SetNextItemOpen(!closeAllTasks); }
	if (ImGui::TreeNode(("Task data" + ConstructItemName("taskData", task.taskID)).c_str())) {

		DrawProgressState(task);

		AddSpacedText("");
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.08);
		ImGui::InputInt(("Importance" + ConstructItemName("importance", task.taskID)).c_str(), &task.importance);
		AddSpacedText("");
		ImGui::Checkbox(("On the gym" + ConstructItemName("onTheGym", task.taskID)).c_str(), &task.onTheGym);

		DrawStressType(task);
		DrawDates(task);

		AddTabulation();
		ImGui::Checkbox(("" + ConstructItemName("taskDescriptionCheckbox", task.taskID)).c_str(), &task.textOpen); ImGui::SameLine();
		if (task.textOpen) { ImGui::SetNextItemOpen(true); }
		if (ImGui::TreeNode(("Task description" + ConstructItemName("taskDescription", task.taskID)).c_str())) {

			AddTabulation();
			ImGuiInputTextFlags descriptionFlags = ImGuiInputTextFlags_NoHorizontalScroll;
			ImVec2 descriptionBoxSize = ImVec2(ImGui::GetWindowWidth() * 0.8, ImGui::GetWindowHeight() * 0.1);
			ImGui::InputTextMultiline(ConstructItemName("description", task.taskID).c_str(), (char*)task.description, IM_ARRAYSIZE(task.description), descriptionBoxSize, descriptionFlags);
			ImGui::TreePop();

		}

		if (task.subtaskList.size() > 0) {

			float totalCompleteTasks = 0;
			for (int progressIt = 0; progressIt < task.subtaskList.size(); progressIt++) { if (task.subtaskList[progressIt].completed) { totalCompleteTasks++; } }
			AddTabulation(); AddSpacedText("\t\tSubtask progress"); ImGui::SetNextItemWidth(screenWidth * 0.5);
			ImGui::ProgressBar(totalCompleteTasks / task.subtaskList.size(), ImVec2(0.0f, 0.0f));

		}

		DrawSubtasks(task);

		ImGui::TreePop();

	}

}


void FUTA::DrawBasicTaskData(Tasks& task, Tasks* parentTask) {

	ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_EnterReturnsTrue;
	ImGui::SameLine(); ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.5);
	if (ImGui::InputText(ConstructItemName("name", task.taskID).c_str(), (char*)task.name, IM_ARRAYSIZE(task.name), inputTextFlags)) { reorderVector = true; }
	if (ImGui::IsItemDeactivatedAfterEdit()) { reorderVector = true; }
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
	const char* progressionItems[] = { "No state", "In progress", "Waiting for another task", "Waiting for other people", "Postposed", "Recurrent" };
	ImGui::Combo(("Current Progression State" + ConstructItemName("progressState", task.taskID)).c_str(), &task.progressionState, progressionItems, IM_ARRAYSIZE(progressionItems));

}


void FUTA::DrawStressType(Tasks& task) {

	AddTabulation();
	ImGui::Text("Stress state:");
	AddSpacedText("");

	ImGui::Checkbox(("Waking up" + ConstructItemName("woke", task.taskID)).c_str(), &task.woke);
	AddSpacedText("");
	ImGui::Checkbox(("Sleep deprived" + ConstructItemName("sleepDeprived", task.taskID)).c_str(), &task.deprived);
	AddSpacedText("");
	ImGui::Checkbox(("Physic pain" + ConstructItemName("physicPain", task.taskID)).c_str(), &task.pained);
	AddSpacedText("");
	ImGui::Checkbox(("Mentally wasted" + ConstructItemName("mentallyWasted", task.taskID)).c_str(), &task.wasted);
	AddSpacedText("");
	ImGui::Checkbox(("Operative" + ConstructItemName("operative", task.taskID)).c_str(), &task.operative);

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

	ImGui::Checkbox(("" + ConstructItemName("defaultsubtaskOpenCheckbox", task.taskID)).c_str(), &task.subtasksOpen); ImGui::SameLine();
	if (task.subtasksOpen) { ImGui::SetNextItemOpen(true); }
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
