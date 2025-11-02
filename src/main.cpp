#include "GLGEGraphic/GLGEGraphic.h"
#include "GLGE_Core/GLGECore.h"

int main()
{
    String p = MeshAsset::import("assets/mesh/Suzane.fbx");
    AssetHandle mesh = AssetManager::create<MeshAsset>(p);
    
    MeshAsset* m = AssetManager::getAsset<MeshAsset>(mesh);
    Mesh& m_mesh = m->mesh();

    Window win("Hello World!", uivec2(600,600));
    win.setVSync(GLGE_VSYNC_ON);

    AssetHandle tex = AssetManager::create<TextureAsset>("assets/textures/cubeTexture.png", false, GLGE_TEXTURE_RGB);

    const char* vertexShader = R"(
#version 450 core

layout (location = 0) in vec3 v_pos;

void main() {
    gl_Position = vec4(v_pos, 1);
})";

    const char* fragmentShader = R"(
#version 450 core

layout (location = 0) out vec4 FragColor;

void main() {
    FragColor = vec4(1,0,0,1);
})";

    Shader shader = {
        ShaderStage{
            .sourceCode = vertexShader,
            .srcType = SHADER_SOURCE_GLSL,
            .stage = SHADER_STAGE_VERTEX
        },
        ShaderStage{
            .sourceCode = fragmentShader,
            .srcType = SHADER_SOURCE_GLSL,
            .stage = SHADER_STAGE_FRAGMENT
        }
    };

    AssetManager::waitForLoad(tex);
    Texture* textures[] = {AssetManager::getAsset<TextureAsset>(tex)->getTexture()};
    Material mat(&shader, textures, sizeof(textures)/sizeof(*textures));

    RenderPipeline pipe({}, &win);
    
    glge_Shader_Compile();
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