#include "TeaPacket/Graphics/Model.hpp"

#include <d3d11.h>
#include <unordered_map>

#include "WindowsGraphics.hpp"
#include "Error/Win32ErrorHandling.hpp"
#include "TeaPacket/Debug/Logging.hpp"

using namespace TeaPacket::Graphics;

static std::unordered_map<VertexAttributeType, size_t> AttributeSizes = {
    {VERTEX_ATTR_TYPE_FLOAT, sizeof(float)},
    {VERTEX_ATTR_TYPE_INT, sizeof(int)},
    {VERTEX_ATTR_TYPE_UINT, sizeof(unsigned int)}
};


void Model::Pl_Initialize(const std::vector<char>& vertexData, const std::vector<unsigned long>& indices, const std::vector<VertexAttribute>& vertexAttributes)
{
    size_t vertexSize = 0;
    for (const VertexAttribute& attribute : vertexAttributes)
    {
        vertexSize += AttributeSizes.at(attribute.type) * attribute.amount;
    }

    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA d3dVertexData, d3dIndexData;

    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = static_cast<unsigned int>(vertexData.size());
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    d3dVertexData.pSysMem = vertexData.data();
    d3dVertexData.SysMemPitch = 0;
    d3dVertexData.SysMemSlicePitch = 0;

    CheckErrorWinCom(
        device->CreateBuffer(&vertexBufferDesc, &d3dVertexData, platformModel.vertexBuffer.GetAddressOf())
    );

    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = static_cast<unsigned int>(sizeof(unsigned long) * indices.size());
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    d3dIndexData.pSysMem = indices.data();
    d3dIndexData.SysMemPitch = 0;
    d3dIndexData.SysMemSlicePitch = 0;

    CheckErrorWinCom(
        device->CreateBuffer(&indexBufferDesc, &d3dIndexData, platformModel.indexBuffer.GetAddressOf())
    );

    platformModel.vertexSize = vertexSize;
    platformModel.indexCount = indices.size();
}

void Model::Pl_Render()
{
    const unsigned int stride = static_cast<unsigned int>(platformModel.vertexSize);
    const unsigned int offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, platformModel.vertexBuffer.GetAddressOf(), &stride, &offset);
    deviceContext->IASetIndexBuffer(platformModel.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    deviceContext->DrawIndexed(static_cast<unsigned int>(platformModel.indexCount), 0, 0);
}
