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
	ImGui::Checkbox(ConstructItemName("started", task.name, task.taskID).c_str(), &task.started);
	AddSpacedText("Completed");
	ImGui::Checkbox(ConstructItemName("completed", task.name, task.taskID).c_str(), &task.completed);
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
	AddSpacedText("Deadline");
	ImGui::Checkbox("##deadline", &task.deadline);

	AddTabulation();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.1);
	ImGui::InputInt("Importance", &task.importance);

	AddSeparator();

}


void FUTA::AddSeparator() { ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine(); }
void FUTA::AddTabulation() { ImGui::NewLine(); ImGui::Text("\t"); ImGui::SameLine(); }
void FUTA::AddSpacedText(std::string text) { ImGui::SameLine(); ImGui::Text(("\t" + text).c_str()); ImGui::SameLine(); }
std::string FUTA::ConstructItemName(std::string itemName, std::string taskName, int id) { return "##" + itemName + taskName + std::to_string(id); }

#pragma endregion

