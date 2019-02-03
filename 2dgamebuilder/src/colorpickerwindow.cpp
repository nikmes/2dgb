#include "colorpickerwindow.h"
#include <imgui/imgui.h>
#include "servicelocator.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h> // ImSaturate



ColorPickerWindow::ColorPickerWindow(TextureAtlas *textureAtlas, string name) :ImGUIComponent(textureAtlas, name)
{
	addIcons();
}

ColorPickerWindow::~ColorPickerWindow()
{

}

void ColorPickerWindow::addIcons()
{

}

void ColorPickerWindow::run(bool* opened)
{
	static float col[4] = { 0.0,0.0,0.0,0.0 };

	if (!ImGui::Begin(m_name.c_str(), opened))
	{
		ImGui::End();
		return;
	}
	this->ColorPicker4(col,true);

	ImGui::End();
}

bool ColorPickerWindow::ColorPicker4(float* col, bool show_alpha)
{
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	const float  SPACING = ImGui::GetStyle().ItemInnerSpacing.x;

	// Setup
	ImVec2 picker_pos = ImGui::GetCursorScreenPos();
	ImVec2 sv_picker_size = ImVec2(200.0f, 200.0f);     // Saturation/Value picking box
	float bars_width = 20.0f;                           // Width of Hue/Alpha picking bars
	float bar0_pos_x = picker_pos.x + sv_picker_size.x + SPACING;
	float bar1_pos_x = bar0_pos_x + bars_width + SPACING;

	float H, S, V;
	ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], H, S, V);

	// Color matrix logic
	bool value_changed = false, hsv_changed = false;
	ImGui::InvisibleButton("saturation_value_selector", sv_picker_size);
	if (ImGui::IsItemActive())
	{
		S = ImSaturate((io.MousePos.x - picker_pos.x) / (sv_picker_size.x - 1));
		V = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size.y - 1));
		value_changed = hsv_changed = true;
	}

	// Hue bar logic
	ImGui::SetCursorScreenPos(ImVec2(bar0_pos_x, picker_pos.y));
	ImGui::InvisibleButton("hue_selector", ImVec2(bars_width, sv_picker_size.y));
	if (ImGui::IsItemActive())
	{
		H = ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size.y - 1));
		value_changed = hsv_changed = true;
	}

	// Alpha bar logic
	if (show_alpha)
	{
		ImGui::SetCursorScreenPos(ImVec2(bar1_pos_x, picker_pos.y));
		ImGui::InvisibleButton("alpha_selector", ImVec2(bars_width, sv_picker_size.y));
		if (ImGui::IsItemActive())
		{
			col[3] = ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size.y - 1));
			value_changed = true;
		}
	}

	// Convert back to RGB
	if (hsv_changed)
		ImGui::ColorConvertHSVtoRGB(H >= 1.0f ? H - 10 * 1e-6f : H, S > 0.0f ? S : 10 * 1e-6f, V > 0.0f ? V : 1e-6f, col[0], col[1], col[2]);

	// R,G,B or H,S,V color editor
	ImGui::PushItemWidth((show_alpha ? SPACING + bars_width : 0) + sv_picker_size.x + SPACING + bars_width - 2 * ImGui::GetStyle().FramePadding.x);
	value_changed |= show_alpha ? ImGui::ColorEdit4("##edit", col) : ImGui::ColorEdit3("##edit", col);
	ImGui::PopItemWidth();

	// Try to cancel hue wrap (after ColorEdit), if any
	if (value_changed)
	{
		float new_H, new_S, new_V;
		ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], new_H, new_S, new_V);
		if (new_H <= 0 && H > 0)
		{
			if (new_V <= 0 && V != new_V)
				ImGui::ColorConvertHSVtoRGB(H, S, new_V <= 0 ? V * 0.5f : new_V, col[0], col[1], col[2]);
			else if (new_S <= 0)
				ImGui::ColorConvertHSVtoRGB(H, new_S <= 0 ? S * 0.5f : new_S, new_V, col[0], col[1], col[2]);
		}
	}

	// Render hue bar
	ImU32 hue_colors[] = { IMCOL32(255,0,0,255), IMCOL32(255,255,0,255), IMCOL32(0,255,0,255), IMCOL32(0,255,255,255), IMCOL32(0,0,255,255), IMCOL32(255,0,255,255), IMCOL32(255,0,0,255) };
	for (int i = 0; i < 6; ++i)
	{
		draw_list->AddRectFilledMultiColor(
			ImVec2(bar0_pos_x, picker_pos.y + i * (sv_picker_size.y / 6)),
			ImVec2(bar0_pos_x + bars_width, picker_pos.y + (i + 1) * (sv_picker_size.y / 6)),
			hue_colors[i], hue_colors[i], hue_colors[i + 1], hue_colors[i + 1]);
	}
	float bar0_line_y = (float)(int)(picker_pos.y + H * sv_picker_size.y + 0.5f);
	draw_list->AddLine(ImVec2(bar0_pos_x - 2, bar0_line_y), ImVec2(bar0_pos_x + bars_width + 2, bar0_line_y), IMCOL32_WHITE);

	// Render alpha bar
	if (show_alpha)
	{
		float alpha = ImSaturate(col[3]);
		float bar1_line_y = (float)(int)(picker_pos.y + alpha * sv_picker_size.y + 0.5f);
		draw_list->AddRectFilledMultiColor(ImVec2(bar1_pos_x, picker_pos.y), ImVec2(bar1_pos_x + bars_width, picker_pos.y + sv_picker_size.y), IMCOL32_BLACK, IMCOL32_BLACK, IMCOL32_WHITE, IMCOL32_WHITE);
		draw_list->AddLine(ImVec2(bar1_pos_x - 2, bar1_line_y), ImVec2(bar1_pos_x + bars_width + 2, bar1_line_y), IMCOL32((int)(255 - alpha), 255, 255, 255));
	}

	// Render color matrix
	ImVec4 hue_color_f(1, 1, 1, 1);
	ImGui::ColorConvertHSVtoRGB(H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
	ImU32 hue_color32 = ImGui::ColorConvertFloat4ToU32(hue_color_f);
	draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + sv_picker_size, IMCOL32_WHITE, hue_color32, hue_color32, IMCOL32_WHITE);
	draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + sv_picker_size, IMCOL32_BLACK_TRANS, IMCOL32_BLACK_TRANS, IMCOL32_BLACK, IMCOL32_BLACK);

	// Render cross-hair
	const float CROSSHAIR_SIZE = 7.0f;
	ImVec2 p((float)(int)(picker_pos.x + S * sv_picker_size.x + 0.5f), (float)(int)(picker_pos.y + (1 - V) * sv_picker_size.y + 0.5f));
	draw_list->AddLine(ImVec2(p.x - CROSSHAIR_SIZE, p.y), ImVec2(p.x - 2, p.y), IMCOL32_WHITE);
	draw_list->AddLine(ImVec2(p.x + CROSSHAIR_SIZE, p.y), ImVec2(p.x + 2, p.y), IMCOL32_WHITE);
	draw_list->AddLine(ImVec2(p.x, p.y + CROSSHAIR_SIZE), ImVec2(p.x, p.y + 2), IMCOL32_WHITE);
	draw_list->AddLine(ImVec2(p.x, p.y - CROSSHAIR_SIZE), ImVec2(p.x, p.y - 2), IMCOL32_WHITE);

	return value_changed;
}

bool ColorPickerWindow::ColorPicker3(float col[3])
{
	return ColorPicker4(col, false);
}