/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file introstate.cpp
*
*/

#include <stdio.h>
#include <glib2d/glib2d.h>
#include "benchmarkstate.h"
#include "globals.h"
#include <random>

CBenchMarkState CBenchMarkState::m_BenchMarkState;

void CBenchMarkState::init(Application *app)
{
	m_app = app;

	m_app->m_currentState = BENCHMARK;

	ServiceLocator::getStateService()->setState(BENCHMARK);

	io = ImGui::GetIO();

	TRACE(T_INF, "Init function for BenchMarkState finished");
}

void CBenchMarkState::cleanup()
{
}

void CBenchMarkState::pause()
{
}

void CBenchMarkState::resume()
{
}

void CBenchMarkState::handleEvents()
{
}

void CBenchMarkState::update(float tpf)
{
	float speed = 8;

	if (!ImGui::GetIO().WantCaptureMouse)
	{
		if (glfwGetKey(gWin, GLFW_KEY_UP) == GLFW_PRESS)
		{

		}

		if (glfwGetKey(gWin, GLFW_KEY_DOWN) == GLFW_PRESS)
		{

		}

		if (glfwGetKey(gWin, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{

		}

		if (glfwGetKey(gWin, GLFW_KEY_LEFT) == GLFW_PRESS)
		{

		}
	}
}

void CBenchMarkState::draw()
{
	int height, width;

	std::default_random_engine generator;
	std::uniform_real_distribution<float> x(64, 1896);
	std::uniform_real_distribution<float> y(60, 1140);
	std::uniform_real_distribution<float> c(50, 255);
	std::uniform_real_distribution<float> a(0, 255);

	RenderProvider *rp = ServiceLocator::getRenderService();

	rp->getWindowSize(&width, &height);

	rp->g2dBeginRects(ServiceLocator::getTextureService()->GetTextureFromFileSystem("resources/ball16x16.png")); //consoleimages/psvita - real.png"));
	{
		rp->g2dSetCoordMode(G2D_CENTER);

		for (int i = 0; i < maxSprites; i++)
		{
			rp->g2dSetCoordXY(x(generator), y(generator));

			rp->g2dSetColor(ServiceLocator::getHelpService()->convertRGBtoColor(c(generator)*sin((float)1),
																				c(generator)*sin((float)1),
																				c(generator)*sin((float)1)));
			if (rotate) 
			{
				rp->g2dSetRotation(rot);
			}
			
			if (scale)
			{
				rp->g2dSetScale(scl, scl);
			}

			rp->g2dSetAlpha(a(generator)*sin((float)1));

			rp->g2dAdd();
		}
	}

	rp->g2dEnd();

	

	rot += rotSpeed;

	bool is = true;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(3.f, 2.f));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.112, .127, .155, 85.0));

	ImGui::Begin("Benchmarking Options", &is);
	{
		if (ImGui::CollapsingHeader("Rendering Method"))
		{
			ImGui::RadioButton("VERETX ARRAY", &renderMethod, 0);

			ImGui::RadioButton("VBO", &renderMethod, 1);

			ImGui::RadioButton("PM VBO", &renderMethod, 2);
		}

		if (ImGui::CollapsingHeader("Sprites & Attributes"))
		{
			ImGui::SliderInt("Max Sprites", &maxSprites, 100, 20000);

			ImGui::Checkbox("Rotate", &rotate);

			ImGui::DragFloat("Rotation Speed", &rotSpeed, 0.1);

			ImGui::Checkbox("Scale", &scale);

			ImGui::SliderFloat("Scale Factor", &scl, 1, 20);
		}

		ImGui::End();

		ImGui::PopStyleVar(2);

		ImGui::PopStyleColor(1);
	}

}