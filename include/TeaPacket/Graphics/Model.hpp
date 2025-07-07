#pragma once
#include "Platform/Graphics/PlatformModel.hpp"

#include <vector>

namespace TeaPacket::Graphics
{
    /// Type for Vertex Attributes.
    enum VertexAttributeType
    {
        VERTEX_ATTR_TYPE_FLOAT,
        VERTEX_ATTR_TYPE_INT, /// Signed Integer
        VERTEX_ATTR_TYPE_UINT
    };

    /// A Vertex Attribute.
    /** These structs inform TeaPacket how to interpret mesh data. \n
     *  For each type of data a mesh can have, it should have a VertexAttribute. \n
     *  One VertexAttribute corresponds to an "in" variable in a GLSL shader. \n
     *  Order matters. The first VertexAttribute on a mesh/shader will be tied to the in value at location 0, the second
     *  VertexAttribute will be tied to the in value at location 1, etc. \n
     *
     *  For example, a mesh with UV data should
     *  have one VertexAttribute with type "float" and size 3 for the position of its vertices, and a second VertexAttribute
     *  with type "float" and size 2 for UV data.
     ***/
    struct VertexAttribute
    {
        /// The type of the attribute (float, int, etc.)
        VertexAttributeType type;
        /// The size of the attribute. A singular value is 1, while vectors are 2+.
        unsigned char amount;

        VertexAttribute(VertexAttributeType type, unsigned char amount) : type(type), amount(amount){}
    };

    /// Represents a piece of render-able geometry.
    class Model
    {
    public:
        /// Platform data.
        PlatformModel platformModel;

        /// Initialize the model with geometry data
        ///
        /// @param vertexData A chunk of bytes representing each vertex. vertexAttributes is used to interpret that data.
        /// @param indices The indices for this mesh. Indices connect the vertices to create faces.
        /// @param vertexAttributes A list of VertexAttributes that inform TeaPacket how the model should be interpreted.
        void Initialize(const std::vector<char>& vertexData, const std::vector<unsigned long>& indices, const std::vector<VertexAttribute>& vertexAttributes);
        /// Render the model using the currently bound Shader.
        void Render();
    private:
        void Pl_Initialize(const std::vector<char>& vertexData, const std::vector<unsigned long>& indices, const std::vector<VertexAttribute>& vertexAttributes);
        void Pl_Render();
    public:
        /// Create and initialize a model.
        static Model CreateModel(const std::vector<char>& vertexData, const std::vector<unsigned long>& indices, const std::vector<VertexAttribute>& vertexAttributes);
    private:
    };

}