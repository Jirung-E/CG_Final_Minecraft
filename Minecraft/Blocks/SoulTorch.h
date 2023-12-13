#pragma once

#include "../../Objects/Block.h"


class SoulTorch : public Block {
public:
    SoulTorch(const std::string& id);

private:
    void initModel();
};
