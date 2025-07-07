#include "TeaPacket/Graphics/Model.hpp"

using namespace TeaPacket::Graphics;

Model Model::CreateModel(const std::vector<char>& vertexData, const std::vector<unsigned long>& indices, const std::vector<VertexAttribute>& vertexAttributes)
{
    Model model;
    model.Initialize(vertexData, indices, vertexAttributes);
    return model;
}

void Model::Initialize(const std::vector<char>& vertexData, const std::vector<unsigned long>& indices, const std::vector<VertexAttribute>& vertexAttributes)
{
    Pl_Initialize(vertexData, indices, vertexAttributes);
}

void Model::Render()
{
    Pl_Render();
}
