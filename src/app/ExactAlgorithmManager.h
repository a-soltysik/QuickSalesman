#pragma once

#include "TspManager.h"

namespace qs
{

class ExactAlgorithmManager : public TspManager
{
public:
    auto menu() -> void override;
};

}
