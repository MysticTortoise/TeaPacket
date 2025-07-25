#include "TeaPacket/TeaPacket.hpp"
#include "TeaPacket/Debug/Logging.hpp"
#include "TeaPacket/Graphics/Graphics.hpp"
#include "TeaPacket/Graphics/Display.hpp"
#include "TeaPacket/Graphics/Viewport.hpp"
#include "TeaPacket/Assets/AssetReader.hpp"
#include "TeaPacket/Assets/AssetStream.hpp"
#include "TeaPacket/Graphics/ShaderVariable.hpp"
#include "TeaPacket/Graphics/Shader.hpp"
#include "TeaPacket/Graphics/Texture.hpp"
#include "TeaPacket/Graphics/Model.hpp"
#include "TeaPacket/Assets/AssetReaders/TGAReader.hpp"

using namespace TeaPacket;
using namespace TeaPacket::Graphics;

constexpr float verts[] = {
    1, 1, 1, 0,
    1, -1, 1, 1,
    -1, -1, 0, 1,
    -1, 1, 0, 0
};

const unsigned char tex[] = {
    255, 0, 0, 255,
    0, 255, 0, 255,
    0, 0, 255, 255,
    255, 255, 0, 255,
    0, 255, 255, 255,
    255, 0, 255, 255,
    0, 0, 0, 255,
    255, 255, 255, 255,
    155, 155, 155, 255
};

int main()
{

    TeaPacket::Initialize();
    const std::vector colorAttrList = {ShaderVariableType(SHADER_VAR_TYPE_FLOAT,2), ShaderVariableType(SHADER_VAR_TYPE_FLOAT, 2)};

    Shader shader = Shader::CreateShaderFromFiles("shaders/textured.vert", "shaders/textured.frag", colorAttrList, {ShaderVariableType(SHADER_VAR_TYPE_TEXTURE, 1)});
    char vertBytes[sizeof(verts)];
    memcpy(vertBytes, verts, sizeof(verts));
    std::vector<char> vertVec;
    for (int i = 0; i < sizeof(vertBytes); i++)
    {
        vertVec.push_back(vertBytes[i]);
    }

    const std::vector<unsigned long> indices = {0, 1, 3, 1, 2, 3};
    Model model = Model::CreateModel(vertVec, indices, colorAttrList);
    Texture texture = Assets::ReadTGA("test.tga");
    ShaderVariableValue val;
    val.tex = &texture;
    shader.SetParameter(0, val);

    Assets::AssetStream stream("model.bin");

    int array[50];
    array[51] = 23;

    while(ShouldRun())
    {
        SetRenderTarget(Display::Displays.back());
        ClearScreen(Color4{20, 70, 127, 255});
        shader.UseShader();
        model.Render();
        FinishRenderTarget();

        Display::Displays.back().PresentDisplay();

        Tick();
    }
    TeaPacket::DeInitialize();
}