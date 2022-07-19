#include "imgui.h"
#include "FUTA.h"

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


void FUTA::AddSeparator() { ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine(); }
void FUTA::AddTabulation() { ImGui::NewLine(); ImGui::Text("\t"); ImGui::SameLine(); }
void FUTA::AddSpacedText(std::string text) { ImGui::SameLine(); ImGui::Text(("\t" + text).c_str()); ImGui::SameLine(); }
