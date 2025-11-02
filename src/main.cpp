#include "GLGEGraphic/GLGEGraphic.h"
#include "GLGE_Core/GLGECore.h"

#include <fstream>

class AssetFile : public BaseAsset
{
public:

    AssetFile(const std::string_view& _path)
     : path(_path)
    {}

    virtual void load() noexcept override {
        updateLoadState(ASSET_STATE_LOADING);

        //open the requested file
        std::ifstream f(path.data(), std::ifstream::ate);
        //sanity check the load
        if (!f.is_open()) {
            updateLoadState(ASSET_STATE_FAILED);
            return;
        }

        //load the file's contents
        size_t len = f.tellg();
        f.seekg(std::ifstream::beg);
        contents.resize(len);
        f.read(contents.data(), len);

        //loaded
        updateLoadState(ASSET_STATE_LOADED);
    }

    /**
     * @brief Get the Contents of the file
     * 
     * @return const std::string& a constant reference to the 
     */
    inline const std::string& getContents() const noexcept {return contents;}

protected:

    //store the path to the file
    std::string_view path;
    //store the contents of the file
    std::string contents;

};

int main()
{
    String p = MeshAsset::import("assets/mesh/Suzane.fbx");
    AssetHandle mesh = AssetManager::create<MeshAsset>(p);
    
    MeshAsset* m = AssetManager::getAsset<MeshAsset>(mesh);
    Mesh& m_mesh = m->mesh();

    Window win("Hello World!", uivec2(600,600));
    win.setVSync(GLGE_VSYNC_ON);

    AssetHandle tex = AssetManager::create<TextureAsset>("assets/textures/qwantani_sunrise_puresky_8k.hdr", true, GLGE_TEXTURE_RGBA_H);

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

    glge_Shader_Compile();

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