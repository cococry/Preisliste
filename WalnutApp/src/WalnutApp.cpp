#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"

struct Papier
{
	int32_t Umfang = 1;
	uint32_t Grammatur = 0;
	int32_t Auflage = 1;
	std::string Druck = "Kein Wert";
	std::string Format = "Kein Wert";
	std::string PapierArt = "Kein Wert";
	std::string Verarbeitung = "Kein Wert";

	static bool Complete(const Papier& papier)
	{
		return papier.Umfang != 0 && papier.Grammatur != 0 && papier.Auflage != 0
			&& papier.Druck != "Kein Wert" && papier.Format != "Kein Wert"
			&& papier.PapierArt != "Kein Wert" && papier.Verarbeitung != "Kein Wert";
	}
};
class OmniPrintLayer : public Walnut::Layer
{
public:
	virtual void OnAttach() override
	{
		mOpmniPrintLogo = std::make_shared<Walnut::Image>("assets/omniprint_logo.png");

		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}
	virtual void OnUIRender() override
	{
		ImGui::Begin("OmniPrint");
		ImGui::Text("Preisliste v0.1 Alpha");
		ImGui::Image(mOpmniPrintLogo->GetDescriptorSet(), { (float)mOpmniPrintLogo->GetWidth(), (float)mOpmniPrintLogo->GetHeight()});
		ImGui::End();

		ImGui::Begin("Auftrag");		
		if (mPapier.Auflage < 1)
			mPapier.Auflage = 1;

		ImGui::DragInt("Umfang", &mPapier.Umfang, 0.005f);
		if (mPapier.Umfang < 1)
			mPapier.Umfang = 1;

		DropDownMenuUInt("Grammatur (g/m2)", mPapier.Grammatur, mValidGrammaturen);
		DropDownMenuString("Druck", mPapier.Druck, mValidDrucks);
		DropDownMenuString("Format", mPapier.Format, mValidFormate);
		DropDownMenuString("Papier", mPapier.PapierArt, mValidPapiere);
		DropDownMenuString("Verarbeitung", mPapier.Verarbeitung, mValidVerarbeitungen);

		ImGui::DragInt("Auflage", &mPapier.Auflage, 0.005f);
		if (mPapier.Auflage < 1)
			mPapier.Auflage = 1;

		if (Papier::Complete(mPapier))
		{
			ImGui::Separator();
			if (ImGui::Button("Preis Berechnen"))
			{

			}
		}
		ImGui::End();
	}
private:
	void DropDownMenuUInt(const char* label, uint32_t& valueToChange, const std::vector<uint32_t>& values)
	{
		std::string grammaturStr = std::to_string(valueToChange);
		const char* grammaturCstr = grammaturStr.c_str();
		if (ImGui::BeginCombo(label, grammaturCstr))
		{
			for (int n = 0; n < values.size(); n++)
			{
				bool is_selected = (valueToChange == values[n]);
				std::string selectedStr = std::to_string(values[n]);
				if (ImGui::Selectable(selectedStr.c_str(), is_selected))
					valueToChange = values[n];
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}
	void DropDownMenuString(const char* label, std::string& valueToChange, const std::vector<std::string>& values)
	{
		std::string grammaturStr = valueToChange;
		const char* grammaturCstr = grammaturStr.c_str();
		if (ImGui::BeginCombo(label, grammaturCstr))
		{
			for (int n = 0; n < values.size(); n++)
			{
				bool is_selected = (valueToChange == values[n]);
				std::string selectedStr = values[n];
				if (ImGui::Selectable(selectedStr.c_str(), is_selected))
					valueToChange = values[n];
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}
	Papier mPapier;

	// Meta Data
	std::vector<uint32_t> mValidGrammaturen = { 60, 70, 80, 100, 120, 135, 150, 170, 200, 246, 250, 280, 300, 350, 400 };
	std::vector<std::string> mValidFormate = { "DINA7", "DINA6", "DINA5", "DINA4", "DIN3", "DINA2", "DINA1", "DINA0" };
	std::vector<std::string> mValidDrucks = { "1/0", "1/1", "4/0", "4/1", "4/4", "4/5", "5/0", "5/1", "5/4", "5/5", "6/0", "6/1", "6/4", "6/5", "6/6" };
	std::vector<std::string> mValidPapiere = { 
		"Colorcopy", "SoporSet", "PlanoDynamic", 
		"MultiArtSilk", "MunkenPure", "Munkenlynx", "FocusCard matt", 
		"FocusCard matt", "FocusCard satiniert", 
		"JacSpilt matt", "JacSplit glanz", "Ettiketten transparent", 
		"Synamps", "Synamps", "Synamps", "Digilable matt", 
		"Digilable glanz","Acetad seide sk" };
	std::vector<std::string> mValidVerarbeitungen = { "Schneiden", "Rillen", "Falzen",
		"Blockleimung" , "Wire-O Bindung", "Rückstichheftung",
		"Klebebindung", "Laminieren glzd", "Laminieren matt",
		"Zweifach Lochung", "Vierfach Lochung", "Sonder Lochung",
		"Perforfieren" };

	std::shared_ptr<Walnut::Image> mOpmniPrintLogo;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "OmniPrint Preisliste";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<OmniPrintLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}