#pragma once

#include "winerror.h"
#include "comdef.h"
#include <stdexcept>

inline void CheckErrorWinCom(HRESULT result)
{
    if (!SUCCEEDED(result))
    {
        const _com_error err(result);
        const LPCSTR errorMessage = err.ErrorMessage();
        throw std::runtime_error(errorMessage);
    }
}