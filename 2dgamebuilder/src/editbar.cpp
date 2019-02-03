#include "editbar.h"
#include <imgui/imgui.h>
#include "servicelocator.h"

EditBar::EditBar(TextureAtlas *textureAtlas, string name) :ImGUIComponent(textureAtlas, name)
{
	this->addIcons();
	ServiceLocator::getStateService()->setRollerOperation(RO_SCALE_ITEM);
}

EditBar::~EditBar()
{
}

void EditBar::run(bool *opened)
{
	ImVec2 uv0, uv1;

	vector<textureAtlasItem>::iterator it;
	vector<textureAtlasItem>::iterator it2;
	vector<textureAtlasItem>::iterator it3;

	static int drawOnClick = 1;
	static int rollerOperation = 0;
	static int alighOrientation = 0;

	ImGui::Begin(m_name.c_str(), opened, ImVec2((47 * icons.size()), 46), 0.70f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
	{
		it = icons.begin();

		textureAtlasItem currItem = *it;

		uv0 = ImVec2(currItem.x / (float)tex->w, currItem.y / (float)tex->h);
		uv1 = ImVec2((currItem.x + currItem.w) / (float)tex->w, (currItem.y + currItem.h) / (float)tex->h);

		ImGui::Image(texId, ImVec2(20, 20), uv0, uv1, ImColor(255, 255, 255, 255));

		for (it = icons.begin()+1; it != icons.end(); it++)
		{
			textureAtlasItem currItem = *it;
			uv0 = ImVec2(currItem.x / (float)tex->w, currItem.y / (float)tex->h);
			uv1 = ImVec2((currItem.x + currItem.w) / (float)tex->w, (currItem.y + currItem.h) / (float)tex->h);
			ImGui::PushID(currItem.id);

			ImGui::SameLine();

			if (ImGui::ImageButton(texId, ImVec2(40, 40), uv0, uv1, 0, ImColor(0, 0, 0, 0), ImColor(255, 255, 255, 255)))
			{
				if (currItem.name == "tilemode")
				{
					TRACE(T_DEB, "Enabled tiled mode editing. Set subState EST_DRAWTILED");
					ServiceLocator::getStateService()->setSubState(EST_DRAWTILED);

					if (drawOnClick)
					{
						ServiceLocator::getStateService()->setDrawType(DT_TILED_CLICKED);
					}
					else
					{
						ServiceLocator::getStateService()->setDrawType(DT_TILED_PRESSED);
					}
				}

				if (currItem.name == "freehandmode")
				{
					TRACE(T_DEB, "Enabled freehand mode editing. Set subState EST_DRAWFREEHAND");
					ServiceLocator::getStateService()->setSubState(EST_DRAWFREEHAND);
				}

				if (currItem.name == "delete")
				{
					TRACE(T_DEB, "Enabled delete mode. Set subState EST_DELETE");
					ServiceLocator::getStateService()->setSubState(EST_DELETE);
				}

				if (currItem.name == "move")
				{
					TRACE(T_DEB, "Enabled freehand mode editing. Set subState to EST_MOVEITEMS");
					ServiceLocator::getStateService()->setSubState(EST_MOVEITEMS);
				}

				// cursor keys movement type
				if (currItem.name == "move-camera")
				{
					TRACE(T_DEB, "Cursor keys will move the camera. Set moveType to MT_CAMERA");
					ServiceLocator::getStateService()->setMoveType(MT_CAMERA);
				}

				if (currItem.name == "move-player")
				{
					TRACE(T_DEB, "Cursor keys will move the player. Set moveType to MT_PLAYER");
					ServiceLocator::getStateService()->setMoveType(MT_PLAYER);
				}

				if (currItem.name == "move-layers")
				{
					TRACE(T_DEB, "Cursor keys will move the current layer. Set moveType to MT_LAYER");
					ServiceLocator::getStateService()->setMoveType(MT_LAYER);
				}

				if (currItem.name == "box2d")
				{
					ServiceLocator::getStateService()->setSubState(EST_BOX2D);
				}
					
				if (currItem.name == "square")
				{
					ServiceLocator::getStateService()->setSubState(EST_SQUARE);
				}
				if (currItem.name == "singleselect")
				{
					ServiceLocator::getStateService()->setSubState(EST_SINGLE_SELECT);
				}
			}
			
			if (currItem.name == "singleselect")
			{
				if (ImGui::BeginPopupContextItem("Align to direction"))
				{
					ImGui::Text("Align Operation:");

					for (it3 = alignIcons.begin(); it3 != alignIcons.end(); it3++)
					{
						textureAtlasItem currItem = *it3;

						uv0 = ImVec2(currItem.x / (float)tex->w, currItem.y / (float)tex->h);
						
						uv1 = ImVec2((currItem.x + currItem.w) / (float)tex->w, (currItem.y + currItem.h) / (float)tex->h);

						ImGui::Image(texId, ImVec2(28, 28), uv0, uv1, ImColor(255, 255, 255, 255), ImColor(0, 0, 0, 0));

						if (currItem.name == "align-center")
						{
							ImGui::SameLine();

							if (ImGui::RadioButton("Center", &alighOrientation, 0))
							{
								ServiceLocator::getStateService()->setSubState(EST_SINGLE_SELECT);
								TRACE(T_DEB, "Align to selected sprite center");
							}
						}

						if (currItem.name == "align-top")
						{
							ImGui::SameLine();
							if (ImGui::RadioButton("Top", &alighOrientation, 1))
							{
								ServiceLocator::getStateService()->setSubState(EST_SINGLE_SELECT);
								TRACE(T_DEB, "Mouse roller rotates current sprite");
							}
						}

						if (currItem.name == "align-bottom")
						{
							ImGui::SameLine();
							if (ImGui::RadioButton("Bottom", &alighOrientation, 2))
							{
								ServiceLocator::getStateService()->setSubState(EST_SINGLE_SELECT);
								TRACE(T_DEB, "Mouse roller adjust transparency of current sprite");
							}
						}

						if (currItem.name == "align-left")
						{
							ImGui::SameLine();
							if (ImGui::RadioButton("Left", &alighOrientation, 3))
							{
								ServiceLocator::getStateService()->setSubState(EST_SINGLE_SELECT);
								TRACE(T_DEB, "Mouse roller flips current sprite horizontaly");
							}
						}

						if (currItem.name == "align-right")
						{
							ImGui::SameLine();
							if (ImGui::RadioButton("Right", &alighOrientation, 4))
							{
								ServiceLocator::getStateService()->setSubState(EST_SINGLE_SELECT);
								TRACE(T_DEB, "Mouse roller flips current sprite verticaly");
							}
						}

					}

					ImGui::EndPopup();
				}
			}
			
			if (currItem.name == "tilemode")
			{
				if (ImGui::BeginPopupContextItem("item context menu"))
				{
					ImGui::Text("Draw when Mouse Button is");
					if (ImGui::RadioButton("Clicked", &drawOnClick, 1))
					{
						ServiceLocator::getStateService()->setDrawType(DT_TILED_CLICKED);
						ServiceLocator::getStateService()->setSubState(EST_DRAWTILED);
						TRACE(T_DEB, "Enabled tilemode editing. Set subState to EST_DRAWTILED_CLICKED");

					}

					if (ImGui::RadioButton("Pressed", &drawOnClick, 0))
					{
						ServiceLocator::getStateService()->setDrawType(DT_TILED_PRESSED);
						ServiceLocator::getStateService()->setSubState(EST_DRAWTILED);
						TRACE(T_DEB, "Enabled tilemode editing. Set subState to EST_DRAWTILED_PRESSED");
					}
					
					ImGui::EndPopup();
				}

			}
			
			if (currItem.name == "mouse-roller")
			{
				if (ImGui::BeginPopupContextItem("mouse roller context menu"))
				{
					ImGui::Text("Roller Operation:");
					
					for (it2 = rollerIcons.begin(); it2 != rollerIcons.end(); it2++)
					{
						textureAtlasItem currItem = *it2;
						uv0 = ImVec2(currItem.x / (float)tex->w, currItem.y / (float)tex->h);
						uv1 = ImVec2((currItem.x + currItem.w) / (float)tex->w, (currItem.y + currItem.h) / (float)tex->h);

						ImGui::Image(texId, ImVec2(28, 28), uv0, uv1, ImColor(255, 255, 255, 255), ImColor(0, 0, 0, 0));
						
						if (currItem.name == "scale")
						{
							ImGui::SameLine();
							if (ImGui::RadioButton("scale", &rollerOperation, 0))
							{
								ServiceLocator::getStateService()->setRollerOperation(RO_SCALE_ITEM);
								TRACE(T_DEB, "Mouse roller scales current sprite");
							}
						}

						if (currItem.name == "rotate")
						{
							ImGui::SameLine();
							if (ImGui::RadioButton("rotate", &rollerOperation, 1))
							{
								ServiceLocator::getStateService()->setRollerOperation(RO_ROTATE_ITEM);
								TRACE(T_DEB, "Mouse roller rotates current sprite");
							}
						}

						if (currItem.name == "transparency")
						{
							ImGui::SameLine();
							if (ImGui::RadioButton("transparency", &rollerOperation, 2))
							{
								ServiceLocator::getStateService()->setRollerOperation(RO_ADJUST_TRANSPARENCY);
								TRACE(T_DEB, "Mouse roller adjust transparency of current sprite");
							}
						}

						if (currItem.name == "fliphorizontal")
						{
							ImGui::SameLine();
							if (ImGui::RadioButton("fliphorizontal", &rollerOperation, 3))
							{
								ServiceLocator::getStateService()->setRollerOperation(RO_FLIP_HORIZONTALLY);
								TRACE(T_DEB, "Mouse roller flips current sprite horizontaly");
							}
						}

						if (currItem.name == "flipvertical")
						{
							ImGui::SameLine();
							if (ImGui::RadioButton("flipvertical", &rollerOperation, 4))
							{
								ServiceLocator::getStateService()->setRollerOperation(RO_FLIP_VERTICALLY);
								TRACE(T_DEB, "Mouse roller flips current sprite verticaly");
							}
						}

					}

					ImGui::EndPopup();
				}

			}

			// for every icon display hint if hint is set and hints are enabled

			if (ServiceLocator::getConfigurationService()->pEditor_hints == true)
			{
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip(currItem.hint.c_str());
				}
			}

			ImGui::PopID();
		}
	}

	ImGui::End();
}

void EditBar::addIcons()
{
	// populate the toolbar with icons in the same order you want them to appear
	textureAtlasItem taItem;

	taItem = m_textureAtlas->getItem("editmode");
	taItem.hint = "";
	icons.push_back(taItem);
	
	// drawing modes
	taItem = m_textureAtlas->getItem("tilemode");
	taItem.hint = "Set drawing mode to tile mode";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("freehandmode");
	taItem.hint = "Set drawing mode to free hand mode";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("mouse-roller");
	taItem.hint = "Assign operation to mouse roller";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("hand");
	taItem.hint = "Enable dragging mode";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("singleselect");
	taItem.hint = "Enable single select mode";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("multipleselect");
	taItem.hint = "Enable multible select mode";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("move");
	taItem.hint = "Enable moving mode";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("delete");
	taItem.hint = "Enable selecte and delete sprites mode";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("properties");
	taItem.hint = "Show window with current sprite properties";
	icons.push_back(taItem);

	// what is moved using cursor keys
	taItem = m_textureAtlas->getItem("move-camera");
	taItem.hint = "Cursor keys move the camera";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("move-layers");
	taItem.hint = "Cursor keys move the current layer";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("move-player");
	taItem.hint = "Cursor keys move the player";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("box2d");
	taItem.hint = "Draw Box2D collision lines";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("square");
	taItem.hint = "Draw filled square";
	icons.push_back(taItem);

	// mouse roller operations
	taItem = m_textureAtlas->getItem("fliphorizontal");
	taItem.hint = "Set mouse roller operation to flip horizontaly the selected sprite";
	rollerIcons.push_back(taItem);

	taItem = m_textureAtlas->getItem("flipvertical");
	taItem.hint = "Set mouse roller operation to flip verticaly the selected sprite";
	rollerIcons.push_back(taItem);

	taItem = m_textureAtlas->getItem("rotate");
	taItem.hint = "Set mouse roller operation to rotate the selected sprite";
	rollerIcons.push_back(taItem);

	taItem = m_textureAtlas->getItem("scale");
	taItem.hint = "Set mouse roller operation to scale the selected sprite";
	rollerIcons.push_back(taItem);

	taItem = m_textureAtlas->getItem("transparency");
	taItem.hint = "Set mouse roller operation to adjust transparency of the selected sprite";
	rollerIcons.push_back(taItem);

	// align operations
	taItem = m_textureAtlas->getItem("align-center");
	taItem.hint = "Align Center";
	alignIcons.push_back(taItem);

	taItem = m_textureAtlas->getItem("align-top");
	taItem.hint = "Align Top";
	alignIcons.push_back(taItem);

	taItem = m_textureAtlas->getItem("align-bottom");
	taItem.hint = "Align Bottom";
	alignIcons.push_back(taItem);

	taItem = m_textureAtlas->getItem("align-left");
	taItem.hint = "Align Left";
	alignIcons.push_back(taItem);

	taItem = m_textureAtlas->getItem("align-right");
	taItem.hint = "Align Right";
	alignIcons.push_back(taItem);
}