#pragma once
#include <string>
#include "../common/order_request.h"
#include "../engine/matching_engine.h"

class Parser
{
public:
    OrderRequest parse(const std::string& line);
    std::string serialize(const MatchResult& result);
};