#include "GLGEGraphic/GLGEGraphic.h"

void debug(void*)
{
    printf("Tick\n");
}

int main()
{
    Window win("Hello World!", uivec2(600,600));

    RenderPipeline pipe({
        {"Init Frame", RenderPipelineStage{
            .type = GLGE_RENDER_PIPELINE_STAGE_START_WINDOW_FRAME,
            .data{.windowFrameStart{.window = &win}}
        }},
        {"End Frame", RenderPipelineStage{
            .type = GLGE_RENDER_PIPELINE_STAGE_END_WINDOW_FRAME,
            .data{.windowFrameEnd{.window = &win}}
        }},
        {"Debug", RenderPipelineStage{
            .type = GLGE_RENDER_PIPELINE_STAGE_CUSTOM,
            .data{.customStage{.custom_func = debug,.userData=nullptr}}
        }}
    });

    pipe.record();

    while (!win.isClosingRequested()) {
        glge_Graphic_MainTick();
        pipe.play();
    }
}