#include "GLGEGraphic/GLGEGraphic.h"

int main()
{
    Window win("Hello World!", uivec2(600,600));
    win.setVSync(GLGE_VSYNC_ON);

    RenderPipeline pipe({}, &win);

    pipe.record();

    while (!win.isClosingRequested()) {
        static uivec3 color = uivec3(0,0,0);

        glge_Graphic_MainTick();
        pipe.play();
        win.setClearColor(vec4(color.r, color.g, color.b, 255.f) / 255.f);
        color.r = (color.r + 1) % 256;
        color.g = (color.g + 3) % 256;
        color.b = (color.b + 5) % 256;
        pipe.record();
    }
}