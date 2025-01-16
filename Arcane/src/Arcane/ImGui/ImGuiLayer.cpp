#include <arcpch.h>

#include "Arcane/ImGui/ImGuiLayer.h"
#include "Arcane/Core/Application.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Arcane
{

	ImGuiLayer::ImGuiLayer() :
		Layer("ImGuiLayer")
	{
        m_BlockEvents = true;
	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()
	{
        ARC_PROFILE_FUNCTION();
        IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();

#ifndef ARC_DEBUG
        io.IniFilename = NULL;
#endif

        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow()->GetNativeWindow());

        ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void ImGuiLayer::OnDetach()
	{
        ARC_PROFILE_FUNCTION();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}

    void ImGuiLayer::OnImGuiRender()
    {
        ARC_PROFILE_FUNCTION();
#ifdef ARC_DEBUG
        ImGui::Begin("Renderer Info");
        ImGui::Text("Vendor: %s",   glGetString(GL_VENDOR));
        ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
        GLint majVer = 0, minVer = 0, profile = 0;
        glGetIntegerv(GL_MAJOR_VERSION, &majVer);
        glGetIntegerv(GL_MINOR_VERSION, &minVer);
        glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

        std::string contextProfile;
        if (profile & GL_CONTEXT_CORE_PROFILE_BIT)
            contextProfile = "Core";

        if (profile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
            contextProfile = "Compat";


        ImGui::Text("OpenGL: v%d.%d %s", majVer, minVer, contextProfile.c_str());
        ImGui::End();
#endif
    }

    void ImGuiLayer::OnEvent(Event& event)
    {
        if (!m_BlockEvents) return;

        ImGuiIO& io = ImGui::GetIO();
        event.Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
        event.Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
    }

    void ImGuiLayer::Begin()
    {
        ARC_PROFILE_FUNCTION();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End()
    {
        ARC_PROFILE_FUNCTION();
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float) app.GetWindow()->GetWidth(), (float) app.GetWindow()->GetHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_context = glfwGetCurrentContext();

            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_context);
        }
    }

}

