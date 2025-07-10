#pragma once


namespace TeaPacket::Graphics
{

    class Texture;

    /// Type for Vertex Attributes.
    enum ShaderVariableBaseType : unsigned char
    {
        SHADER_VAR_TYPE_FLOAT,  ///< Single-precision float
        SHADER_VAR_TYPE_INT,    ///< Signed Integer
        SHADER_VAR_TYPE_UINT,    ///< Unsigned Integer
        SHADER_VAR_TYPE_TEXTURE, ///< Texture/Sampler.
    };

    union ShaderVariableValue
    {
        float val_float;
        signed int val_int;
        unsigned int val_uint;
        Texture* val_tex;
    };

    /// Represents a variable within a shader.
    /** Informs TeaPacket on various shader properties \n
     *  These structs can correspond to a number of different things based on context. They are as follows:
     *      - The inputs of a shader
     *          - By extension, these structs are also used to interpret mesh data.
     *      - A shader uniform
     *
     *  As an example of this struct's use, a ShaderVariable with type SHADER_VAR_TYPE_FLOAT and amount 4 is a vec4 in GLSL (or a float4 in HLSL)
     ***/
    struct ShaderVariableType
    {
        /// The type of the attribute (float, int, etc.)
        ShaderVariableBaseType type;
        /// The size of the attribute. A singular value is 1, while vectors are 2+.
        unsigned char amount;

        ShaderVariableType():type(static_cast<ShaderVariableBaseType>(0)), amount(0) {}
        ShaderVariableType(ShaderVariableBaseType type, unsigned char amount) : type(type), amount(amount){}
    };

    struct ShaderVariable
    {
        ShaderVariableType type;
        ShaderVariableValue value;

        ShaderVariable(){}
        ShaderVariable(ShaderVariableBaseType type, unsigned char amount) : type(ShaderVariableType(type, amount)){}
    };
}