#pragma once
#include "boost/asio.hpp"
#include "../../include/utils/parser.h"
#include "../engine/matching_engine.h"
#include "../utils/logger.h"

using boost::asio::ip::tcp;

class OrderGateway
{
private:
    MatchingEngine m_matchEngine;
    Parser m_parser;
    void handleClient(tcp::socket socket);

public:
    void start(uint16_t port);
};