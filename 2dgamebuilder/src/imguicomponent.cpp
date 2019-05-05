#include "imguicomponent.h"

ImGUIComponent::~ImGUIComponent()
{

}

ImGUIComponent::ImGUIComponent()
{

}

ImGUIComponent::ImGUIComponent(TextureAtlas *textureAtlas, string toolBarName)
{
	isVisible = false;

	m_name = toolBarName;

	m_textureAtlas = textureAtlas;

	texId = (ImTextureID*)m_textureAtlas->getTexture()->id;

	tex = m_textureAtlas->getTexture();
}

string ImGUIComponent::getName()
{
	return m_name;
}

void ImGUIComponent::switchVisibility()
{
	if (isVisible)
	{
		isVisible = false;
	}
	else
	{
		isVisible = true;
	}
}