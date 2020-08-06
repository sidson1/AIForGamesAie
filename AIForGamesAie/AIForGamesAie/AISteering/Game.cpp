#include "Game.h"
#include "Application.h"
#include "GameStateManager.h"
#include "DemoState.h"
#include "FadeTransition.h"
#include "LevelEditorState.h"
#include "cimgui_impl_raylib.h"
#include "Numbers.h"
#include "rlgl.h"

Game::Game(int gameWidth, int gameHeight, int gameZoom) : Application::Application(gameWidth,gameHeight,gameZoom)
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
}

Game::~Game()
{

}

void Game::Create()
{
	//Change to switch from editor to player ya hear?
	if (true)
	{
		m_gameStateManager->SetState("Editor", new LevelEditorState(this));
		m_gameStateManager->PushState("Editor", new FadeTransition(this, "Editor", 0.25f));
	}


	//ImGui
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplRaylib_Init();

	//Build Texture atlas
	int width;
	int height;

	ImGuiContext* ctx;
	ImGuiIO& io = ImGui::GetIO();
	ImGui::GetIO().DisplaySize = {(float)(GetGameWidth() * m_gameZoom),(float)(GetGameHeight() * m_gameZoom)};

	unsigned char* pixels;
	io.Fonts->AddFontDefault();
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, NULL);
	
	unsigned int texID = rlLoadTexture(pixels,width,height,PixelFormat::UNCOMPRESSED_R8G8B8A8,1);

	io.Fonts->TexID = (ImTextureID)texID;

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowBorderSize, -1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_ChildBorderSize, -1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_PopupBorderSize, -1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FrameBorderSize, -1.0f);
	

	
	//ImGui
}

void Game::StartDraw()
{
	ImGui_ImplRaylib_NewFrame(m_gameZoom);
	ImGui_ImplRaylib_ProcessEvent();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(0);
}

void Game::EndDraw()
{
	ImGui::Render();
	m_drawData = ImGui::GetDrawData();
	raylib_render_cimgui(m_drawData);
}
