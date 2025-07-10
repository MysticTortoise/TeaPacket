#pragma once
#include "Platform/Graphics/PlatformModel.hpp"

#include <vector>


namespace TeaPacket::Graphics
{

    struct ShaderVariableType;

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
        void Initialize(const std::vector<char>& vertexData, const std::vector<unsigned long>& indices, const std::vector<ShaderVariableType>& vertexAttributes);
        /// Render the model using the currently bound Shader.
        void Render();
    private:
        void Pl_Initialize(const std::vector<char>& vertexData, const std::vector<unsigned long>& indices, const std::vector<ShaderVariableType>& vertexAttributes);
        void Pl_Render();
    public:
        /// Create and initialize a model.
        static Model CreateModel(const std::vector<char>& vertexData, const std::vector<unsigned long>& indices, const std::vector<ShaderVariableType>& vertexAttributes);
    private:
    };

}