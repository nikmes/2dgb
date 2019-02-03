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
#include "preferenceswindow.h"
#include "servicelocator.h"

PreferencesWindow::PreferencesWindow(TextureAtlas *textureAtlas,string name) : ImGUIComponent(textureAtlas, name)
{
	m_textureAtlas = textureAtlas;
}

PreferencesWindow::~PreferencesWindow()
{

}

void PreferencesWindow::addIcons()
{

}

void PreferencesWindow::run(bool* opened)
{
	ImVec4 imGreen(0.0f, 1.0f, 0.0f, 1.0f);
	XMLConfigurationProvider *confProvider = ServiceLocator::getConfigurationService();

	ImGui::Begin("Editor Preferences", opened, ImVec2(0, 0), 1.0f);
	{
		if (ImGui::CollapsingHeader("Video Settings"))
		{
			ServiceLocator::getImguiComponentService()->colorPicker4((float*)&ServiceLocator::getConfigurationService()->pEditor_color, true);


		}
		if (ImGui::CollapsingHeader("OpenGL Settings"))
		{
			int prevSyncMode = confProvider->pOpenGlVsync;

			ImGui::TextColored(imGreen, "OpenGL Settings");
			ImGui::RadioButton("VSYNC 30 FPS", &confProvider->pOpenGlVsync,2);
			ImGui::RadioButton("VSYNC 60 FPS", &confProvider->pOpenGlVsync,1);
			ImGui::RadioButton("VSYNC NO", &confProvider->pOpenGlVsync,0);
			ImGui::Checkbox("Ambient Light", &confProvider->pLog_enabled);
			ImGui::Checkbox("GL_TEXTURE_MIN_FILTER", &confProvider->pLog_enabled);
			ImGui::Checkbox("GL_TEXTURE_MAG_FILTER", &confProvider->pLog_enabled);		

			if (prevSyncMode != confProvider->pOpenGlVsync)
			{
				int vSyncMode = confProvider->pOpenGlVsync;

				if (vSyncMode == 1)
				{
					glfwSwapInterval(1); // 1 - enable vsync 60
				}

				if (vSyncMode == 2)
				{
					glfwSwapInterval(2); // 2 - enable vsync 30 - disable vsync
				}

				if (vSyncMode == 0)
				{
					glfwSwapInterval(0); // 2 - disable vsync
				}
			}

			ImGui::Separator();
		}
		if (ImGui::CollapsingHeader("Logger Settings"))
		{
			ImGui::TextColored(imGreen, "Logger");
			ImGui::Checkbox("[ACT] Active", &confProvider->pLog_enabled);
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

		if (ImGui::CollapsingHeader("General"))
		{
		}
	}
	ImGui::End();
}