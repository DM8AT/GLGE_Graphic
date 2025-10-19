#include "GLGEGraphic/GLGEGraphic.h"

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
        }}
    });

    while (!win.isClosingRequested()) {
        glge_Graphic_MainTick();
    }
}