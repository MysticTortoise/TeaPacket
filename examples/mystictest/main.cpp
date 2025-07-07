#include "TeaPacket/TeaPacket.hpp"
#include "TeaPacket/Graphics/Graphics.hpp"
#include "TeaPacket/Graphics/Display.hpp"
#include "TeaPacket/Graphics/Viewport.hpp"
#include "TeaPacket/Files/AssetReader.hpp"
#include "TeaPacket/Debug/Logging.hpp"
#include "TeaPacket/Graphics/Shader.hpp"

using namespace TeaPacket;
using namespace TeaPacket::Graphics;

int main()
{

    TeaPacket::Initialize();
    Viewport viewport = Viewport::CreateViewport(1280, 720);

    const std::vector colorAttrList = {VertexAttribute(VERTEX_ATTR_TYPE_FLOAT,2), VertexAttribute(VERTEX_ATTR_TYPE_FLOAT, 4)};

    Shader shader = Shader::CreateShaderFromFiles("shaders/color.vert", "shaders/color.frag", colorAttrList);
    const std::vector<char> verts = Files::ReadAllBytesFromAsset("testmodelverts");
    const std::vector<unsigned long> indices = {0, 1, 2};
    Model model = Model::CreateModel(verts, indices, colorAttrList);

    while(ShouldRun())
    {
        BeginRenderToViewport(viewport);
        ClearScreen(Color4{0, 255, 0, 255});
        shader.UseShader();
        model.Render();
        FinishRenderToViewport();

        RenderViewportToDisplay(Display::Displays.back(), viewport);

        Tick();
    }
    TeaPacket::DeInitialize();
}