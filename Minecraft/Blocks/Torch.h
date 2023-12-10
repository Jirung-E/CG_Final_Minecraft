#pragma once

#include "../../Objects/Block.h"


class Torch : public Block {
public:
    Torch(const std::string& id);

private:
    void initModel();
};
