#define GLFW_INCLUDE_NONE

#include "Ui.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

static void apply_style() {
    ImGuiStyle& st = ImGui::GetStyle();
    st.WindowRounding    = 10.0f;
    st.FrameRounding     = 8.0f;
    st.PopupRounding     = 10.0f;
    st.ScrollbarRounding = 12.0f;
    st.GrabRounding      = 8.0f;
    st.TabRounding       = 10.0f;

    st.WindowPadding = ImVec2(12, 12);
    st.FramePadding  = ImVec2(10, 6);
    st.ItemSpacing   = ImVec2(10, 8);

    ImVec4* c = st.Colors;
    c[ImGuiCol_WindowBg]         = ImVec4(0.07f, 0.07f, 0.08f, 0.96f);
    c[ImGuiCol_ChildBg]          = ImVec4(0.07f, 0.07f, 0.08f, 0.00f);
    c[ImGuiCol_PopupBg]          = ImVec4(0.07f, 0.07f, 0.08f, 0.98f);
    c[ImGuiCol_Border]           = ImVec4(0.20f, 0.20f, 0.22f, 0.40f);
    c[ImGuiCol_FrameBg]          = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
    c[ImGuiCol_FrameBgHovered]   = ImVec4(0.16f, 0.16f, 0.19f, 1.00f);
    c[ImGuiCol_FrameBgActive]    = ImVec4(0.18f, 0.18f, 0.21f, 1.00f);
    c[ImGuiCol_TitleBg]          = ImVec4(0.06f, 0.06f, 0.07f, 1.00f);
    c[ImGuiCol_TitleBgActive]    = ImVec4(0.06f, 0.06f, 0.07f, 1.00f);
    c[ImGuiCol_Button]           = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);
    c[ImGuiCol_ButtonHovered]    = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);
    c[ImGuiCol_ButtonActive]     = ImVec4(0.20f, 0.20f, 0.24f, 1.00f);
    c[ImGuiCol_Tab]              = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
    c[ImGuiCol_TabHovered]       = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);
    c[ImGuiCol_TabActive]        = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);
    c[ImGuiCol_SliderGrab]       = ImVec4(0.70f, 0.70f, 0.75f, 0.80f);
    c[ImGuiCol_SliderGrabActive] = ImVec4(0.85f, 0.85f, 0.90f, 0.90f);
}

void ui_init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();
    apply_style();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void ui_shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ui_begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

static const char* mode_name(ViewportMode m) {
    switch (m) {
        case ViewportMode::One:     return "1 panel";
        case ViewportMode::Two:     return "2 panels";
        case ViewportMode::Four:    return "4 panels";
    }
    return "1 panel";
}

void ui_draw(UiState& s) {
    ImGui::SetNextWindowPos(ImVec2(12, 12), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(330, 220), ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize;

    ImGui::Begin("Controls", nullptr, flags);

    if (ImGui::BeginCombo("Layout", mode_name(s.viewportMode))) {
        if (ImGui::Selectable("1 panel", s.viewportMode == ViewportMode::One))  s.viewportMode = ViewportMode::One;
        if (ImGui::Selectable("2 panels", s.viewportMode == ViewportMode::Two)) s.viewportMode = ViewportMode::Two;
        if (ImGui::Selectable("4 panels", s.viewportMode == ViewportMode::Four))s.viewportMode = ViewportMode::Four;
        ImGui::EndCombo();
    }

    ImGui::Separator();

    ImGui::Checkbox("Play", &s.playing);
    ImGui::SliderFloat("Alpha", &s.alpha, 0.0f, 1.0f, "%.2f");
    ImGui::SliderFloat("Min", &s.vmin, -1.0f, 2.0f, "%.3f");
    ImGui::SliderFloat("Max", &s.vmax, -1.0f, 2.0f, "%.3f");

    ImGui::End();
}

void ui_end() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
