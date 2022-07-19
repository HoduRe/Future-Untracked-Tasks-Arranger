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

void FUTA::DrawTaskList() { for (int i = 0; i < taskList.size(); i++) { DrawTask(taskList[i]); } }


void FUTA::DrawTask(Tasks& task) {

	AddSeparator();

	DrawBasicTaskData(task);

	AddTabulation();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.25);
	const char* progressionItems[] = { "No state", "In progress", "Waiting for others", "Postposed" };
	ImGui::Combo("Current Progression State", &task.progressionState, progressionItems, IM_ARRAYSIZE(progressionItems));

	AddTabulation();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.25);
	const char* effortItems[] = { "No type", "Mindless", "Minimal focus", "Maximum focus" };
	ImGui::Combo("Effort Type", &task.effort, effortItems, IM_ARRAYSIZE(effortItems));

	DrawDates(task);

	AddTabulation();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.1);
	ImGui::InputInt("Importance", &task.importance);

	AddTabulation();
	if (ImGui::TreeNode("Task description")) {

		AddTabulation();
		ImGuiInputTextFlags descriptionFlags = ImGuiInputTextFlags_NoHorizontalScroll;
		ImVec2 descriptionBoxSize = ImVec2(ImGui::GetWindowWidth() * 0.8, ImGui::GetWindowHeight() * 0.1);
		ImGui::InputTextMultiline(ConstructItemName("description", task.taskID).c_str(), (char*)task.description.c_str(), task.description.capacity(), descriptionBoxSize, descriptionFlags);
		ImGui::TreePop();

	}

	DrawSubtasks(task);

	AddSeparator();

}


void FUTA::DrawBasicTaskData(Tasks& task) {

	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.5);
	ImGui::InputText(ConstructItemName("name", task.taskID).c_str(), (char*)task.name.c_str(), task.name.capacity());
	AddSpacedText("Started");
	ImGui::Checkbox(ConstructItemName("started", task.taskID).c_str(), &task.started);
	AddSpacedText("Completed");
	ImGui::Checkbox(ConstructItemName("completed", task.taskID).c_str(), &task.completed);
	AddSpacedText("\t");
	if (ImGui::Button("Delete task")) { /*TODO THIS: send a message of you sure, bruh*/ }
	// Add indicator if there are dependencies (since we use string for name, we can have a list for that? Just ommit yourself in the listing, and order them by name)

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


void FUTA::DrawSubtasks(Tasks& tasks) {

	AddTabulation();

	if (ImGui::TreeNode("Subtasks")) {

		AddTabulation();
		if (ImGui::Button("Add Subtask")) { tasks.subtaskList.push_back(Tasks()); }

		for (int i = 0; i < tasks.subtaskList.size(); i++) {

			Tasks& newTask = tasks.subtaskList[i];

			AddTabulation();
			DrawBasicTaskData(newTask);

			AddTabulation();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.25);
			const char* progressionItems[] = { "No state", "In progress", "Waiting for others", "Postposed" };
			ImGui::Combo("Current Progression State", &newTask.progressionState, progressionItems, IM_ARRAYSIZE(progressionItems));
			ImGui::NewLine();

		}

		ImGui::TreePop();

	}

}


#pragma endregion

