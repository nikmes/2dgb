/**
* \date    14/02/2015
* \author  Nicholas Messaritis
*
* \file    glfwvideoprovider.cpp
*
* Implementation of the glfw3 video service provider
*
*/

#include <stdio.h>
#include <imgui/imgui.h>
#include "loggerwindow.h"
#include "servicelocator.h"


static size_t       ImFormatStringV(char* buf, size_t buf_size, const char* fmt, va_list args);
static char*        ImStrdup(const char *str);

#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR)/sizeof(*_ARR)))

static size_t ImFormatStringV(char* buf, size_t buf_size, const char* fmt, va_list args)
{
	int w = vsnprintf(buf, buf_size, fmt, args);
	buf[buf_size - 1] = 0;
	return (w == -1) ? buf_size : (size_t)w;
}

static char* ImStrdup(const char *str)
{
	char *buff = (char*)ImGui::MemAlloc(strlen(str) + 1);
	IM_ASSERT(buff);
	strcpy(buff, str);
	return buff;
}

LoggerWindow::LoggerWindow(TextureAtlas *textureAtlas, string name) : ImGUIComponent(textureAtlas, name)
{
	addIcons();
 }

LoggerWindow::~LoggerWindow()
{

}

void LoggerWindow::run(bool* opened)
{

	ImVec4 imGreen(0.0f, 1.0f, 0.0f, 1.0f);

	static bool showOptions = false;

	ImGui::SetNextWindowSize(ImVec2(520, 800), ImGuiSetCond_FirstUseEver);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ServiceLocator::getConfigurationService()->pLog_color);
	
	ImGui::Begin("Application Traces", opened, ImVec2(0, 0), 1.0f, ImGuiWindowFlags_NoScrollbar);// | ImGuiWindowFlags_MenuBar);
	
	if (ImGui::Button("File Log"))
	{

	}
	ImGui::Separator();

//	if (ImGui::BeginMenuBar())
//	{
//		if (ImGui::BeginMenu("Menu"))
//		{
			//ShowExampleMenuFile();
//			ImGui::EndMenu();
//		}
//		if (ImGui::BeginMenu("Examples"))
//		{
//			ImGui::MenuItem("Metrics", NULL, &showOptions);
//			ImGui::EndMenu();
//		}
//		ImGui::EndMenuBar();
//	}

	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetTextLineHeightWithSpacing() * 1.5),false, ImGuiWindowFlags_HorizontalScrollbar);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

	for (size_t i = 0; i < items.size(); i++)
	{
		const char* item = items[i];

		ImVec4 col = imGreen; // A better implement may store a type per-item. For the sample let's just parse the text.
		
		if (strstr(item, "ERR"))
		{
			col = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
		}
		else if (strstr(item, "DEB"))

		{
			col = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
		}

		ImGui::TextColored(col, item);
	}

	if (scrollToBottom)
		ImGui::SetScrollPosHere();

	scrollToBottom = false;

	ImGui::PopStyleVar();

	ImGui::EndChild();

	ImGui::Separator();


	if (ImGui::Button("Clear Log"))
	{
		this->clearLog();
	}

	ImGui::SameLine();

	if (ImGui::Button("Save Log"))
	{

	}

	ImGui::SameLine();

	if (ImGui::Button("Log Options"))
	{
		showOptions = true;
	}

	ImGui::End();
	
	if (showOptions) this->showOptions(&showOptions);

	ImGui::PopStyleColor(1);
	
}

void LoggerWindow::showOptions(bool *opened)
{
	XMLConfigurationProvider *confProvider = ServiceLocator::getConfigurationService();

	ImVec4 imGreen(0.0f, 1.0f, 0.0f, 1.0f);

	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiSetCond_FirstUseEver);

	if (!ImGui::Begin(m_name.c_str(), opened, ImVec2(0, 0), 1.0f))
	{
		ImGui::End();
		return;
	}

	ImGui::TextColored(imGreen, "Logger");
	ImGui::Checkbox("[ACT] Active", &ServiceLocator::getLogService()->m_active);
	ImGui::Checkbox("[FIL] Log To File", &confProvider->pLog_logtofile);
	ImGui::Separator();
	ImGui::TextColored(imGreen, "Level");
	ImGui::Checkbox("[INF] Informative", &confProvider->pLog_loginformationmsgs);
	ImGui::Checkbox("[DEB] Debuging", &confProvider->pLog_logdebugingmsgs);
	ImGui::Checkbox("[WAR] Warnings", &confProvider->pLog_logwarningmsgs);
	ImGui::Checkbox("[ERR] Errors", &confProvider->pLog_logerrormsgs);
	ImGui::Checkbox("[PRO] Problems", &confProvider->pLog_logproblemmsgs);
	ImGui::Separator();
	ImGui::TextColored(imGreen, "Options");
	ImGui::Checkbox("[DAT] Date", &confProvider->pLog_logdate);
	ImGui::Checkbox("[TIM] Time", &confProvider->pLog_logtime);
	ImGui::Checkbox("[LIN] Source File Line", &confProvider->pLog_logline);
	ImGui::Checkbox("[NAM] Source File Name", &confProvider->pLog_logfile);
	ImGui::Checkbox("[FUN] Function/Method Name", &confProvider->pLog_logfunc);
	ImGui::Separator();
	ImGui::ColorEdit4("Win Color ", (float*)&ServiceLocator::getConfigurationService()->pLog_color); 
	ImGui::End();

	if (confProvider->pLog_loginformationmsgs)
		ServiceLocator::getLogService()->setTraceFlag(T_INF);
	else
		ServiceLocator::getLogService()->clearTraceFlag(T_INF);

	if (confProvider->pLog_logdebugingmsgs)
		ServiceLocator::getLogService()->setTraceFlag(T_DEB);
	else
		ServiceLocator::getLogService()->clearTraceFlag(T_DEB);

	if (confProvider->pLog_logwarningmsgs)
		ServiceLocator::getLogService()->setTraceFlag(T_WAR);
	else
		ServiceLocator::getLogService()->clearTraceFlag(T_WAR);

	if (confProvider->pLog_logerrormsgs)
		ServiceLocator::getLogService()->setTraceFlag(T_ERR);
	else
		ServiceLocator::getLogService()->clearTraceFlag(T_ERR);

	if (confProvider->pLog_logproblemmsgs)
		ServiceLocator::getLogService()->setTraceFlag(T_PRO);
	else
		ServiceLocator::getLogService()->clearTraceFlag(T_PRO);
}

void LoggerWindow::addLog(const char* fmt, ...)
{
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	ImFormatStringV(buf, IM_ARRAYSIZE(buf), fmt, args);
	va_end(args);
	items.push_back(ImStrdup(buf));
	scrollToBottom = true;
}

void LoggerWindow::clearLog()
{
	for (size_t i = 0; i < items.size(); i++)
	{
		ImGui::MemFree(items[i]);
	}
	items.clear();
	scrollToBottom = true;
}

void LoggerWindow::addIcons()
{
	
}