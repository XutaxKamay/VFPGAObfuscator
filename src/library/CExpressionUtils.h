#ifndef VFPGA_OBFUSCATOR_LIBRARY_C_EXPRESSION_UTILS_H
#define VFPGA_OBFUSCATOR_LIBRARY_C_EXPRESSION_UTILS_H

#include "StandardHeaders.h"

namespace VFPGAObfuscatorLibrary
{

    class CExpressionUtils
    {
      public:
        template <typename VECTOR_ELEMENT_T = std::byte,
                  std::size_t TMP_SIZE_T    = 0x1000000>
        struct TemporaryArray
        {
            std::array<VECTOR_ELEMENT_T, TMP_SIZE_T> array;
            std::size_t real_size;

            template <std::size_t REAL_SIZE_T>
            static constexpr auto ToRealSizedArray(const auto& array)
            {
                std::array<VECTOR_ELEMENT_T, REAL_SIZE_T>
                  real_sized_array {};

                for (std::size_t i = 0; i < REAL_SIZE_T; i++)
                {
                    real_sized_array[i] = array[i];
                }

                return real_sized_array;
            }
        };

        template <typename VECTOR_ELEMENT_T = std::byte,
                  std::size_t TMP_SIZE_T    = 0x1000000>
        static constexpr auto VectorToTmpArray(
          const std::vector<VECTOR_ELEMENT_T>& vector);
    };

}

template <typename VECTOR_ELEMENT_T, std::size_t TMP_SIZE_T>
constexpr auto VFPGAObfuscatorLibrary::CExpressionUtils::VectorToTmpArray(
  const std::vector<VECTOR_ELEMENT_T>& vector)
{
    TemporaryArray<VECTOR_ELEMENT_T, TMP_SIZE_T> tmp {};

    if (vector.size() <= tmp.array.size())
    {
        for (std::size_t i = 0; i < vector.size(); i++)
        {
            tmp.array[i] = vector[i];
        }

        tmp.real_size = vector.size();
    }

    return tmp;
}

#endif
