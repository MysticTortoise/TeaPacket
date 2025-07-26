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

constexpr float verts2[] = {
    -0.35625f, 1, 1, 0,
    -0.35625f, 0.85694444445f, 1, 1,
    -1, 0.85694444445f, 0, 1,
    -1, 1, 0, 0
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

    char vertBytes2[sizeof(verts2)];
    memcpy(vertBytes2, verts2, sizeof(verts2));
    std::vector<char> vertVec2;
    for (int i = 0; i < sizeof(vertBytes2); i++)
    {
        vertVec2.push_back(vertBytes2[i]);
    }

    const std::vector<unsigned long> indices = {0, 1, 3, 1, 2, 3};
    Model model = Model::CreateModel(vertVec, indices, colorAttrList);
    Model model2 = Model::CreateModel(vertVec2, indices, colorAttrList);

#define texAmnt 5

    Texture textures[texAmnt] = {
        Assets::ReadTGA("images/uncompressed32.tga"),
        Assets::ReadTGA("images/uncompressed24.tga"),
        Assets::ReadTGA("images/uncompressed16.tga"),
        Assets::ReadTGA("images/uncompressedcolormapped.tga"),
        Assets::ReadTGA("images/uncompressedbw.tga")
    };

    Texture nums[texAmnt];
    for (unsigned char i = 0; i < texAmnt; i++)
    {
        nums[i] = Assets::ReadTGA("nums/" + std::to_string(i) + ".tga");
    }


    Assets::AssetStream stream("model.bin");

    while(ShouldRun())
    {
        static unsigned short time = 1000;
        static unsigned char index = -1;
        ++time;
        if (time > 200)
        {
            index++;
            index %= std::size(textures);
            time = 0;
        }


        SetRenderTarget(Display::Displays.back());
        ClearScreen(Color4{255, 0, 255, 255});

        ShaderVariableValue val{};
        val.tex = &nums[index];
        shader.SetParameter(0, val);
        shader.UseShader();
        model2.Render();
        val.tex = &textures[index];
        shader.SetParameter(0, val);
        shader.UseShader();
        model.Render();
        FinishRenderTarget();

        Display::Displays.back().PresentDisplay();

        Tick();
    }
    TeaPacket::DeInitialize();
}