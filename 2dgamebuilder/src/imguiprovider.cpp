#include <imgui.h>
#include "servicelocator.h"
#include "globals.h"
#include <GLFW/glfw3.h>

#ifdef _MSC_VER
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif

static double g_Time = 0.0f;
static GLuint g_FontTexture = 0;

static size_t       ImFormatStringV(char* buf, size_t buf_size, const char* fmt, va_list args);
static char*        ImStrdup(const char *str);

#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR)/sizeof(*_ARR)))

static inline int ImClamp(int v, int mn, int mx) { return (v < mn) ? mn : (v > mx) ? mx : v; }

static inline float ImClamp(float v, float mn, float mx) { return (v < mn) ? mn : (v > mx) ? mx : v; }

static inline ImVec2 ImClamp(const ImVec2& f, const ImVec2& mn, ImVec2 mx) { return ImVec2(ImClamp(f.x, mn.x, mx.x), ImClamp(f.y, mn.y, mx.y)); }

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

static void ImImpl_RenderDrawLists(ImDrawData* draw_data)
{
	// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
	ImGuiIO& io = ImGui::GetIO();
	int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
	int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
	if (fb_width == 0 || fb_height == 0)
		return;
	draw_data->ScaleClipRects(io.DisplayFramebufferScale);

	// We are using the OpenGL fixed pipeline to make the example code simpler to read!
	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, vertex/texcoord/color pointers.
	GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
	GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnable(GL_TEXTURE_2D);
	//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context

	// Setup viewport, orthographic projection matrix
	glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, +1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Render command lists
#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;
		const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;
		glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + OFFSETOF(ImDrawVert, pos)));
		glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + OFFSETOF(ImDrawVert, uv)));
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + OFFSETOF(ImDrawVert, col)));

		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
				glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer);
			}
			idx_buffer += pcmd->ElemCount;
		}
	}
#undef OFFSETOF

	// Restore modified state
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindTexture(GL_TEXTURE_2D, (GLuint)last_texture);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
	glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
	glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
	glDepthMask(GL_TRUE);

}

void ImGui_ImplGlfw_RenderDrawLists(ImDrawData* draw_data)
{
	// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
	ImGuiIO& io = ImGui::GetIO();
	int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
	int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
	if (fb_width == 0 || fb_height == 0)
		return;
	draw_data->ScaleClipRects(io.DisplayFramebufferScale);

	// We are using the OpenGL fixed pipeline to make the example code simpler to read!
	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, vertex/texcoord/color pointers.
	GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
	GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnable(GL_TEXTURE_2D);
	//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context

	// Setup viewport, orthographic projection matrix
	glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, +1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Render command lists
#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;
		const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;
		glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + OFFSETOF(ImDrawVert, pos)));
		glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + OFFSETOF(ImDrawVert, uv)));
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + OFFSETOF(ImDrawVert, col)));

		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
				glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer);
			}
			idx_buffer += pcmd->ElemCount;
		}
	}
#undef OFFSETOF

	// Restore modified state
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindTexture(GL_TEXTURE_2D, (GLuint)last_texture);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
	glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
	glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
	glDepthMask(GL_TRUE);

}

static const char* ImImpl_GetClipboardTextFn()
{
	return glfwGetClipboardString(gWin);
}

static void ImImpl_SetClipboardTextFn(const char* text)
{
	glfwSetClipboardString(gWin, text);
}

static const char* ImGui_ImplGlfw_GetClipboardText(void* user_data)
{
	return glfwGetClipboardString((GLFWwindow*)user_data);
}

static void ImGui_ImplGlfw_SetClipboardText(void* user_data, const char* text)
{
	glfwSetClipboardString((GLFWwindow*)user_data, text);
}

IMGUIProvider::IMGUIProvider()
{
	this->init();

	TRACE(T_INF, "Initialized");
}

IMGUIProvider::~IMGUIProvider()
{
	free(mainToolBar);
}

void IMGUIProvider::Shutdown()
{
    this->InvalidateDeviceObjects();

    ImGui::Shutdown();
}

void IMGUIProvider::NewFrame()
{
	//glfwGetWindowUserPointer(gWin, );

    if (!g_FontTexture)
        this->CreateDeviceObjects();

    ImGuiIO& io = ImGui::GetIO();

    // Setup display size (every frame to accommodate for window resizing)
    
	int w, h;
    int display_w, display_h;

    glfwGetWindowSize(gWin, &w, &h);
    glfwGetFramebufferSize(gWin, &display_w, &display_h);
    io.DisplaySize = ImVec2((float)display_w, (float)display_h);
	io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);

	// Setup time step
    double current_time =  glfwGetTime();
    io.DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f/60.0f);
    g_Time = current_time;

    // Setup inputs
    // (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
    if (glfwGetWindowAttrib(gWin, GLFW_FOCUSED))
    {
    	double mouse_x, mouse_y;
    	glfwGetCursorPos(gWin, &mouse_x, &mouse_y);
    	mouse_x *= (float)display_w / w;                        // Convert mouse coordinates to pixels
    	mouse_y *= (float)display_h / h;
    	io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);   // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
    }
    else
    {
    	io.MousePos = ImVec2(-1,-1);
    }
   
    for (int i = 0; i < 3; i++)
    {
        io.MouseDown[i] = g_MouseWasPressed[i] || glfwGetMouseButton(gWin, i) != 0;    // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
        g_MouseWasPressed[i] = false;
    }

    // Hide OS mouse cursor if ImGui is drawing it
    glfwSetInputMode(gWin, GLFW_CURSOR, io.MouseDrawCursor ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);

    // Start the frame
    ImGui::NewFrame();
}

bool IMGUIProvider::init()
{

	textureAtlas = new TextureAtlas("resources/icons.xml"); // load texture atlas that contains all icons used by our application

	// create main toolbar and add it to the imgui comonent service provider
	mainToolBar = new MainToolBar(textureAtlas,"MainToolBar"); 
	mainToolBar->isVisible = true; 
	this->addImGuiComponent(mainToolBar);

	// create the edit toolbar and add it to the imgui component service provider
	editBar = new EditBar(textureAtlas, "EditBar");
	this->addImGuiComponent(editBar);
	
	// create the sprite selector component and it to the imgui component service porvider
	spriteSelector = new SpriteSelector(textureAtlas, "SpriteSelector");
	spriteSelector->isVisible = false; 
	this->addImGuiComponent(spriteSelector);

	// create the layers toolbar and add it to imgui component service provider
	layerBar = new LayerBar(textureAtlas, "LayerBar");
	this->addImGuiComponent(layerBar);

	// create the game toolbar and add it to imgui component service provider
	gameBar = new GameBar(textureAtlas, "GameBar");
	this->addImGuiComponent(gameBar);

	

	loggerWindow = new LoggerWindow(textureAtlas, "LoggerWindow"); // initialize the logger window
	this->addImGuiComponent(loggerWindow);

	gridPropertiesWindow = new GridPropertiesWindow(textureAtlas, "GridProperties"); // initialize grid properties window
	this->addImGuiComponent(gridPropertiesWindow);
	
	preferencesWindow = new PreferencesWindow(textureAtlas,"PreferencesWindow"); // initialize the editor preferences window
	this->addImGuiComponent(preferencesWindow);

	fileDialogWindow = new FileDialogWindow(textureAtlas,"FileDialog"); // initialize file dialog window
	this->addImGuiComponent(fileDialogWindow);

	renderingStatsWindow = new RenderingStatsWindow(textureAtlas,"RenderingStats"); // initialize rendering stats window
	this->addImGuiComponent(renderingStatsWindow);

	cameraPropertiesWindow = new CameraPropertiesWindow(textureAtlas, "CameraProperties");
	this->addImGuiComponent(cameraPropertiesWindow);

	layerPropertiesWindow = new LayerPropertiesWindow(textureAtlas, "LayerProperties");
	this->addImGuiComponent(layerPropertiesWindow);

	screenShotWindow = new ScreenShotWindow(textureAtlas, "ScreenShotWindow");
	this->addImGuiComponent(screenShotWindow);

	colorPickerWindow = new ColorPickerWindow(textureAtlas, "ColorPickerWindow");
	this->addImGuiComponent(colorPickerWindow);
	
	spritePropertiesWindow = new SpritePropertiesWindow(textureAtlas, "SpritePropertiesWindow");
	this->addImGuiComponent(spritePropertiesWindow);

	benchMarkingWindow = new BenchmarkingWindow(textureAtlas, "BenchMarkingWindow");
	this->addImGuiComponent(benchMarkingWindow);



	ImGuiIO& io = ImGui::GetIO();
    
	//my_font0 = io.Fonts->AddFontDefault();
	//my_font0 = io.Fonts->AddFontFromFileTTF("extra_fonts/clacon.ttf", 16.0f);
	//my_font1 = io.Fonts->AddFontFromFileTTF("extra_fonts/DroidSans.ttf", 16.0f);
	//my_font2 = io.Fonts->AddFontFromFileTTF("extra_fonts/Perfect DOS VGA 437.ttf", 14.0f);
	my_font2 = io.Fonts->AddFontFromFileTTF("extra_fonts/Inconsolata-Regular.ttf", 14.0f);
	TRACE(T_INF, "IMGUI Loaded in memmory font clacon.ttf");

	//my_font2 = io.Fonts->AddFontFromFileTTF("extra_fonts/Karla-Regular.ttf", 16.0f);
	//my_font3 = io.Fonts->AddFontFromFileTTF("extra_fonts/ProggyClean.ttf", 13.0f); 	my_font3->DisplayOffset.y += 1;
	
	//my_font4 = io.Fonts->AddFontFromFileTTF("extra_fonts/ProggyTiny.ttf", 10.0f); 	my_font4->DisplayOffset.y += 1;

	io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;                 // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
    io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

    //io.RenderDrawListsFn = ImImpl_RenderDrawLists;
    //io.SetClipboardTextFn = ImImpl_SetClipboardTextFn;
    //io.GetClipboardTextFn = ImImpl_GetClipboardTextFn;
	
	io.RenderDrawListsFn = ImGui_ImplGlfw_RenderDrawLists;      // Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.
	io.SetClipboardTextFn = ImGui_ImplGlfw_SetClipboardText;
	io.GetClipboardTextFn = ImGui_ImplGlfw_GetClipboardText;
	io.ClipboardUserData = gWin;

#ifdef _MSC_VER
    io.ImeWindowHandle = glfwGetWin32Window(gWin);
#endif
	this->showHints = true;
	return true;
}

bool IMGUIProvider::CreateDeviceObjects()
{
	// Build texture atlas
	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

															  // Upload texture to graphics system
	GLint last_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	glGenTextures(1, &g_FontTexture);
	glBindTexture(GL_TEXTURE_2D, g_FontTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	// Store our identifier
	io.Fonts->TexID = (void *)(intptr_t)g_FontTexture;

	// Restore state
	glBindTexture(GL_TEXTURE_2D, last_texture);

	return true;
}

void IMGUIProvider::InvalidateDeviceObjects()
{
    if (g_FontTexture)
    {
        glDeleteTextures(1, &g_FontTexture);
        ImGui::GetIO().Fonts->TexID = 0;
        g_FontTexture = 0;
    }
}

void IMGUIProvider::draw()
{
	static bool show_Image = false;
	static bool opened = true;
	static bool show_test_window = true;
	static bool show_another_window = false;
	
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(3.f,2.f));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.112,.127,.155,85.0));

	if (show_Image)
	{
		showImage(&show_Image);
	}


	if (ServiceLocator::getConfigurationService()->pEditor_fps)
	{
		this->renderingStatsWindow->run(&ServiceLocator::getConfigurationService()->pEditor_fps);
	}

	/*
	 * Show sample windows
	 */

	// Show another simple window, this time using an explicit Begin/End pair
	if (show_another_window)
	{
		ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Another Window", &show_another_window);
		ImGui::Text("Hello");
		ImGui::End();
	}

	// Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if (show_test_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowUserGuide();
		ImGui::ShowTestWindow(&show_test_window);
	}

	//ServiceLocator::getToolBarService()->render();
	
	this->renderImGuiComponents();

	ImGui::PopStyleVar(2);

	ImGui::PopStyleColor(1);

	ImGui::Render();
	
}

void IMGUIProvider::showImage(bool* opened)
{
	static float uv_h = 512.0;
	static float uv_w = 512.0;
	static float uv_x = 0.0;
	static float uv_y = 0.0;
	static float uv_x1 = 1.0;
	static float uv_y1 = 1.0;

	if (!ImGui::Begin("Image", opened, ImVec2(0, 0), 50.0f)) //, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::End();
		return;
	}

	ImGui::Image((ImTextureID *)ServiceLocator::getTextureService()->GetTexture("media/icons.png")->id,
		ImVec2(uv_w, uv_h), // size
		ImVec2(uv_x, uv_y),
		ImVec2(uv_x1, uv_y1),
		ImColor(255, 255, 255, 255),
		ImColor(255, 255, 255, 128));

		ImVec2 tex_screen_pos = ImGui::GetCursorScreenPos();
		float tex_w = ServiceLocator::getTextureService()->GetTexture("media/icons.png")->tw;
		float tex_h = ServiceLocator::getTextureService()->GetTexture("media/icons.png")->th;

		ImVec2 uv0;
		ImVec2 uv1;

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			float focus_sz = 32.0f;
			
			float focus_x = ImClamp(ImGui::GetMousePos().x - tex_screen_pos.x - focus_sz * 0.5f, 0.0f, tex_w - focus_sz);
			float focus_y = ImClamp(ImGui::GetMousePos().y - tex_screen_pos.y - focus_sz * 0.5f, 0.0f, tex_h - focus_sz);
			
			uv0 = ImVec2((focus_x) / tex_w, (focus_y) / tex_h);
			uv1 = ImVec2((focus_x + focus_sz) / tex_w, (focus_y + focus_sz) / tex_h);

			ImGui::Text("uv0: (%.2f, %.2f)", uv0.x, uv0.y);
			ImGui::Text("uv1: (%.2f, %.2f)", uv1.x, uv1.y);
			ImGui::Image((ImTextureID *)ServiceLocator::getTextureService()->GetTexture("media/icons.png")->id, ImVec2(32, 32), uv0, uv1, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
			ImGui::EndTooltip();
		}

	ImGui::Separator();

	uv0 = ImVec2(2 / tex_w, 2 / tex_h);
	uv1 = ImVec2((2 + 34) / tex_w, (2 + 34) / tex_h);

	ImGui::ImageButton((ImTextureID *)ServiceLocator::getTextureService()->GetTexture("media/icons.png")->id, ImVec2(32, 32), uv0, uv1, 0, ImColor(0, 0, 0, 0), ImColor(255, 255, 255, 255));

	ImGui::SameLine();

	uv0 = ImVec2(38 / tex_w, 2 / tex_h);
	uv1 = ImVec2((38 + 34) / tex_w, (2 + 34) / tex_h);

	ImGui::ImageButton((ImTextureID *)ServiceLocator::getTextureService()->GetTexture("media/icons.png")->id, ImVec2(32, 32), uv0, uv1, 0, ImColor(0, 0, 0, 0), ImColor(255, 255, 255, 255));

	ImGui::SameLine();

	uv0 = ImVec2(38 / tex_w, 254 / tex_h);
	uv1 = ImVec2((38 + 34) / tex_w, (254 + 34) / tex_h);

	ImGui::ImageButton((ImTextureID *)ServiceLocator::getTextureService()->GetTexture("media/icons.png")->id, ImVec2(32, 32), uv0, uv1, 0, ImColor(0, 0, 0, 0), ImColor(255, 255, 255, 255));

	ImGui::Separator();

	const char* listbox_items[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon" };
	static int listbox_item_current = 1;
	ImGui::ListBox("listbox\n(single select)", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 10);

	ImGui::Separator();

	ImGui::DragFloat("uv_w", &uv_w, .01);
	ImGui::DragFloat("uv_h", &uv_h, .01);
	ImGui::DragFloat("uv_x", &uv_x, .01);
	ImGui::DragFloat("uv_y", &uv_y, .01);
	ImGui::DragFloat("uv_x1", &uv_x1, .01);
	ImGui::DragFloat("uv_y1", &uv_y1, .01);

	ImGui::End();

}

void IMGUIProvider::addImGuiComponent(ImGUIComponent *toolBar)
{
	// add toolbar to the list of toolbars
	this->imGuiComponents.insert((std::pair<string, ImGUIComponent*>(toolBar->getName(), toolBar)));
}

void IMGUIProvider::renderImGuiComponents()
{
	// render all toolbars on screen if they are visible

	map<string, ImGUIComponent*>::iterator it;

	for (it = imGuiComponents.begin(); it != imGuiComponents.end(); it++)
	{
		if ((it)->second->isVisible)
		{
			(it)->second->run(&(it)->second->isVisible);
		}
	}
}

ImGUIComponent* IMGUIProvider::getToolBar(string toolBarName)
{
	return (this->imGuiComponents.at(toolBarName));
}