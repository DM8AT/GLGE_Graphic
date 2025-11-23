/**
 * @file main.cpp
 * @author DM8AT
 * @brief the main entry point for the GLGE example
 * @version 0.1
 * @date 2025-11-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//add GLGE
#include "GLGEGraphic/GLGEGraphic.h"
#include "GLGE_Core/GLGECore.h"
//add ImGui
#include "imgui/imgui.h"
#include "GLGE_ImGuiImpl.h"

//add GLGE examples
#include "Example/All.h"

/**
 * @brief store which example was selected
 */
enum class SelectedExample {
    //no example was selected
    NONE,
    //user requested to just close the application
    QUIT,
    //the first person example was selected
    FIRST_PERSON
};

/**
 * @brief a function that handles the ImGui drawing for the selection window
 * 
 * @param _example a pointer to the example to select
 */
static void imgui_Draw(void* _example) {
    //extract a pointer to the example
    SelectedExample* example = (SelectedExample*)_example;
    //create the ImGui window
    ImGui::SetNextWindowPos(ImVec2{0,0});
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Select Example", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

    //write some information text
    ImGui::Text("Please select one of the below examples to run");

    //handle the first person example
    if (ImGui::Button("First Person example")) 
    {*example = SelectedExample::FIRST_PERSON;}
    //handle a quit option
    if (ImGui::Button("Quit"))
    {*example = SelectedExample::QUIT;}

    //finish the window
    ImGui::End();
}

/**
 * @brief a function that is run to handle the selection of the example to run
 * 
 * @return SelectedExample the example to run
 */
static SelectedExample runSelector() {
    //set the settings correctly
    WindowSettings settings = GLGE_WINDOW_SETTINGS_DEFAULT;
    settings.resizable = false;
    //create the window for the selector
    Window win("GLGE Example - Choose which example to run", uivec2(600, 600), settings);
    //use VSync (high FPS are not needed)
    win.setVSync(GLGE_VSYNC_ON);
    //store the window as a render target
    RenderTarget window(&win, GLGE_WINDOW);

    //initialize the ImGui extension
    glge_ImGui_init(win);

    //store which example was selected
    SelectedExample example = SelectedExample::NONE;

    //store the render pipeline to use for rendering
    RenderPipeline pipe{{{
        "Start frame", 
        RenderPipelineStage{
            .type = GLGE_RENDER_PIPELINE_STAGE_CUSTOM,
            .data{.customStage{
                .custom_func = glge_ImGui_FrameStart,
                .userData = &window
            }}
        }
        }, {
        "Draw ImGui",
        RenderPipelineStage{
            .type = GLGE_RENDER_PIPELINE_STAGE_CUSTOM,
            .data{.customStage{
                .custom_func = imgui_Draw,
                .userData = (void*)&example
            }}
        }
        }, {
        "Finish frame",
        RenderPipelineStage{
            .type = GLGE_RENDER_PIPELINE_STAGE_CUSTOM,
            .data{.customStage{
                .custom_func = glge_ImGui_FrameEnd,
                .userData = &win
            }}
        }}
    }, &win};

    //record the pipeline
    pipe.record();

    //main loop for the selector
    while (!win.isClosingRequested()) {
        //each tick, run a main tick (gather events, update data) and run the render pipeline (render everything)
        glge_Graphic_MainTick();
        pipe.play();

        //if the example was set, stop
        if (example != SelectedExample::NONE)
        {win.requestClosing();}
    }

    //finish the ImGui extension
    glge_ImGui_Shutdown();

    //finally, return the selected example
    return example;
}

int main(void) {
    //select the example to run
    SelectedExample example = runSelector();

    //handle according to the example
    switch (example)
    {
    //handle the first person example
    case SelectedExample::FIRST_PERSON: {
            //create an instance of the first person example and run it
            FirstPersonExample example;
            example.run();
        }
        break;
    //none is handled the same as quit
    case SelectedExample::NONE:
    //quitting just means doing nothing
    case SelectedExample::QUIT:
        break;
    
    default:
        //how did we get here?
        GLGE_ABORT("Unknown example type");
        break;
    }
}