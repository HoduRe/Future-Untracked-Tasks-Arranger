#include "imgui.h"
#include "FUTA.h"

FUTA::FUTA() : FUTAmenu(true), drawNewTask(false), provisionalNewTask(), toDeleteID(0), screenWidth(0), screenHeight(0) {}

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

	if (ImGui::CollapsingHeader("FUTA preferences")) { DrawOptions(); }
	if (ImGui::CollapsingHeader("FUTA tasks", headerFlags)) { DrawTaskList(); }

	//////// ------------------------------------------------------

	DeleteTask();
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

		DrawTask(provisionalNewTask, true);
		if (ImGui::Button("Add new task")) {

			taskList.push_back(provisionalNewTask);
			drawNewTask = false;

		}

	}

}


#pragma endregion


#pragma region DRAW_TASK_LIST

void FUTA::DrawTaskList() { for (int i = 0; i < taskList.size(); i++) { DrawTask(taskList[i]); } }


void FUTA::DrawTask(Tasks& task, bool isNewTask) {

	AddSeparator();

	ImGui::NewLine();
	DrawBasicTaskData(task, nullptr, isNewTask);
	ImGui::NewLine();
	DrawProgressState(task);
	DrawEffortType(task);
	DrawDates(task);

	AddTabulation();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.1);
	ImGui::InputInt(("Importance" + ConstructItemName("importance", task.taskID)).c_str(), &task.importance);

	AddTabulation();
	if (ImGui::TreeNode(("Task description" + ConstructItemName("taskDescription", task.taskID)).c_str())) {

		AddTabulation();
		ImGuiInputTextFlags descriptionFlags = ImGuiInputTextFlags_NoHorizontalScroll;
		ImVec2 descriptionBoxSize = ImVec2(ImGui::GetWindowWidth() * 0.8, ImGui::GetWindowHeight() * 0.1);
		ImGui::InputTextMultiline(ConstructItemName("description", task.taskID).c_str(), (char*)task.description.c_str(), task.description.capacity(), descriptionBoxSize, descriptionFlags);
		ImGui::TreePop();

	}

	DrawSubtasks(task);

	AddSeparator();

}


void FUTA::DrawBasicTaskData(Tasks& task, Tasks* parentTask, bool isNewTask) {

	ImGui::SameLine(); ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.5);
	ImGui::InputText(ConstructItemName("name", task.taskID).c_str(), (char*)task.name.c_str(), task.name.capacity());
	AddSpacedText("Started");
	DrawColoredButton(task.started, ConstructItemName("started", task.taskID));
	AddSpacedText("Completed");
	DrawColoredButton(task.completed, ConstructItemName("completed", task.taskID));
	AddSpacedText("\t");

	if (isNewTask == false && DrawDeletePopUp(task.taskID)) {

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

	AddTabulation();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.25);
	const char* effortItems[] = { "No type", "Mindless", "Minimal focus", "Maximum focus" };
	ImGui::Combo(("Effort Type" + ConstructItemName("effortType", task.taskID)).c_str(), &task.effort, effortItems, IM_ARRAYSIZE(effortItems));

}


void FUTA::DrawDates(Tasks& task) {

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

#pragma endregion

