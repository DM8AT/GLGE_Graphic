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
    AssetManager::waitForLoad(mesh);
    
    MeshAsset* m = AssetManager::getAsset<MeshAsset>(mesh);
    Mesh& m_mesh = m->mesh();

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