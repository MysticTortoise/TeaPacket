#include "TeaPacket/Graphics/Model.hpp"
#include "TeaPacket/Graphics/ShaderVariable.hpp"

using namespace TeaPacket::Graphics;

Model Model::CreateModel(const std::vector<char>& vertexData, const std::vector<unsigned long>& indices, const std::vector<ShaderVariableType>& vertexAttributes)
{
    Model model;
    model.Initialize(vertexData, indices, vertexAttributes);
    return model;
}

void Model::Initialize(const std::vector<char>& vertexData, const std::vector<unsigned long>& indices, const std::vector<ShaderVariableType>& vertexAttributes)
{
    Pl_Initialize(vertexData, indices, vertexAttributes);
}

void Model::Render()
{
    Pl_Render();
}
