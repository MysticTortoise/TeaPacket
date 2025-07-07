#pragma once
#include "Platform/Graphics/PlatformModel.hpp"

#include <vector>

namespace TeaPacket::Graphics
{
    enum VertexAttributeType
    {
        VERTEX_ATTR_TYPE_FLOAT,
        VERTEX_ATTR_TYPE_INT,
        VERTEX_ATTR_TYPE_UINT
    };

    struct VertexAttribute
    {
        VertexAttributeType type;
        unsigned char amount;

        VertexAttribute(VertexAttributeType type, unsigned char amount) : type(type), amount(amount){}
    };

    class Model
    {
    public:
        PlatformModel platformModel;

        void Initialize(const std::vector<char>& vertexData, const std::vector<unsigned long>& indices, const std::vector<VertexAttribute>& vertexAttributes);
        void Render();
    private:
        void Pl_Initialize(const std::vector<char>& vertexData, const std::vector<unsigned long>& indices, const std::vector<VertexAttribute>& vertexAttributes);
        void Pl_Render();
    public:
        static Model CreateModel(const std::vector<char>& vertexData, const std::vector<unsigned long>& indices, const std::vector<VertexAttribute>& vertexAttributes);
    private:
    };

}