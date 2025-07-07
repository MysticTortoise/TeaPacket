#pragma once

#include <algorithm>
#include <stdexcept>
#include <array>
#include <string>

#define TP_VEC_ALIAS(name, index) \
T& name() { return _values[index]; static_assert(index <= i, "Attempt to use an alias that doesn't exist for this TeaPacket Vector type! Alias index: " #index "." ); }; \
const T& name() const { return _values[index]; static_assert(index <= i, "Attempt to use an alias that doesn't exist for this TeaPacket Vector type! Alias index: " #index "."); }; \

#define TP_VEC_MATH_OPERATOR(OP) \
template<unsigned char s> \
Vector<T,i>& operator OP##= (const Vector<T,s>& other) \
{ \
    static_assert(i >= s, "Left side Vector must have a larger or equal size to the Right side Vector when performing math operations."); \
    for(unsigned char j = 0; j < s; j++) \
    { \
        this[j] OP##= other[j]; \
    }\
    return *this; \
} \
template<unsigned char s> \
Vector<T,i> operator OP (const Vector<T,s>& other) const \
{ \
    static_assert(i >= s, "Left side Vector must have a larger or equal size to the Right side Vector when performing math operations."); \
    Vector<T,i> vec = this; \
    return vec OP##= other; \
}

/// Math related functions and classes for TeaPacket
namespace TeaPacket::Math
{
    /// A template class for all base Vector types.
    template<typename T, unsigned char i>
    class Vector
    {
    public:
        static_assert(std::is_arithmetic_v<T>, "Provided Vector type is not an arithmetic type.");

        TP_VEC_ALIAS(x,0)
        TP_VEC_ALIAS(y,1)
        TP_VEC_ALIAS(z,2)
        TP_VEC_ALIAS(w,3)

        TP_VEC_ALIAS(r,0)
        TP_VEC_ALIAS(g,1)
        TP_VEC_ALIAS(b,2)
        TP_VEC_ALIAS(a,3)

        TP_VEC_ALIAS(u,0)
        TP_VEC_ALIAS(v,1)

        /// Adds one vector to another.
        /** All vector math operations are simply the operations applied on all members of that vector. \n
         * For example, addition adds all members together, subtraction subtracts all the 2nd Vector's values
         * from the first Vector's, etc.**/
        TP_VEC_MATH_OPERATOR(+) ///< Adds two vectors together.
        /// Subtracts one vector from another.
        TP_VEC_MATH_OPERATOR(-) ///< Subtracts two vectors.
        /// Multiplies one vector with another.
        TP_VEC_MATH_OPERATOR(*) ///< Multiplies two vectors.
        /// Divides one vector from another.
        TP_VEC_MATH_OPERATOR(/) ///< Divides two vectors.
        /// Modulos one vector from another.
        TP_VEC_MATH_OPERATOR(%) ///< Modulos two vectors.

        /// Check if two Vectors of the same type have equal values.
        /// @param other THe Vector to compare to this one.
        /// @return Whether the two Vectors have equal values.
        bool operator==(Vector<T,i> const& other) const
        {
            for (unsigned char j = 0; j < i; j++)
            {
                if (other[j] != this[j])
                {
                    return false;
                }
            }
            return true;
        }

        /// Check if two Vectors of the same type do not have equal values.
        /// @param other The Vector to compare to this one.
        /// @return Whether the two Vectors aren't equal or not.
        bool operator!=(Vector<T,i> const& other) const
        {
            return !(this==other);
        }

        /// Convert a Vector of one type into another type.
        template<typename O, unsigned char s>
        operator Vector<O,s>()
        {
            Vector<O,s> vec;
            constexpr unsigned char minsize = (std::min)(s,i);
            for (unsigned char j = 0; j < minsize; j++)
            {
                vec[j] = this[j];
            }
        }

        /// Get a copy of the Vector as a plain std::array.
        std::array<T,i> GetAsArray() const
        {
            return std::array<T,i>(_values);
        }

        /// Convert the Vector to a string (for printing or serialization purposes)
        operator std::string() const
        {
            std::string str = "{";
            for (unsigned char j = 0; j < i; j++)
            {
                str += std::to_string(_values[j]) + ",";
            }
            str.back() = '}';
            return str;
        }

        /// Default initializer, setting all values to garbage data.
        Vector()
        {
            _values = {};
        }

        /// Initializer list, sets all values to the given values in the list.
        Vector(std::initializer_list<T> il)
        {
            std::copy(il.begin(), il.end(), _values);
        }

    private:
        /// The internal array that stores the values.
        T _values[i];

    };
}

#define TP_VECTOR_TYPE_GEN_SOLO(size, type, typeInitial) \
typedef TeaPacket::Math::Vector<type, size> Vector##size##typeInitial;

#define TP_VECTOR_TYPE_GEN(vecsize) \
TP_VECTOR_TYPE_GEN_SOLO(vecsize, float, ) \
TP_VECTOR_TYPE_GEN_SOLO(vecsize, float, f) \
TP_VECTOR_TYPE_GEN_SOLO(vecsize, double, d) \
\
TP_VECTOR_TYPE_GEN_SOLO(vecsize, signed char, sb) \
TP_VECTOR_TYPE_GEN_SOLO(vecsize, char, b) \
TP_VECTOR_TYPE_GEN_SOLO(vecsize, unsigned char, ub) \
TP_VECTOR_TYPE_GEN_SOLO(vecsize, short, s) \
TP_VECTOR_TYPE_GEN_SOLO(vecsize, signed short, ss) \
TP_VECTOR_TYPE_GEN_SOLO(vecsize, unsigned short, us) \
TP_VECTOR_TYPE_GEN_SOLO(vecsize, int, i) \
TP_VECTOR_TYPE_GEN_SOLO(vecsize, signed int, si) \
TP_VECTOR_TYPE_GEN_SOLO(vecsize, unsigned int, ui) \
TP_VECTOR_TYPE_GEN_SOLO(vecsize, long, l) \
TP_VECTOR_TYPE_GEN_SOLO(vecsize, signed long, sl) \
TP_VECTOR_TYPE_GEN_SOLO(vecsize, unsigned long, ul) \
TP_VECTOR_TYPE_GEN_SOLO(vecsize, long long, ll) \
TP_VECTOR_TYPE_GEN_SOLO(vecsize, signed long long, sll) \
TP_VECTOR_TYPE_GEN_SOLO(vecsize, unsigned long long, ull)
