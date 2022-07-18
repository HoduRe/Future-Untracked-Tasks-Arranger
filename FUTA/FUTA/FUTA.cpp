#include "imgui.h"
#include "FUTA.h"
#include "Tasks.h"

FUTA::FUTA() : FUTAmenu(true), drawNewTask(false), provisionalNewTask(nullptr) {}

FUTA::~FUTA() {}

void FUTA::Update(int screenWidth, int screenHeight) {

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
	ImGui::Begin("FUTA options", &FUTAmenu, flags);
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(screenWidth, screenHeight));

	//////// Window content ------------------------------------------------------

	ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_DefaultOpen;

	if (ImGui::CollapsingHeader("FUTA options")) { DrawOptions(); }
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
		Tasks newTask;
		provisionalNewTask = &newTask;

	}

	if (drawNewTask && provisionalNewTask != nullptr) {

		DrawTask(*provisionalNewTask);
		if (ImGui::Button("Add new task")) {

			taskList.push_back(*provisionalNewTask);
			drawNewTask = false;
			provisionalNewTask = nullptr;

		}

	}

	else if (drawNewTask) { ImGui::Text("Error adding new task. Press '+' to fix it.\n"); }

}


#pragma endregion


#pragma region DRAW_TASK_LIST

void FUTA::DrawTaskList() {

}


void FUTA::DrawTask(Tasks& task) {

}


#pragma endregion

