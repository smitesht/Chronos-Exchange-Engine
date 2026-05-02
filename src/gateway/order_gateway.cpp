#include "../../include/gateway/order_gateway.h"
#include <iostream>
#include <string>

void OrderGateway::start(uint16_t port)
{
    boost::asio::io_context io;
    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), port));
    //std::cout << "Server started on port " << port << std::endl;

    while (true) 
    {
        tcp::socket socket(io);
        acceptor.accept(socket);

        //std::cout << "Client connected" << std::endl;

        handleClient(std::move(socket));
    }
}

void OrderGateway::handleClient(tcp::socket socket)
{
    try
    {
        Logger::instance().info("Client Connected");

        boost::asio::streambuf  buffer;

        while(true)
        {
            boost::system::error_code ec;
            boost::asio::read_until(socket, buffer, '\n', ec);

            if(ec) break;

            std::istream is(&buffer);
            std::string line;
            std::getline(is,line);
            Logger::instance().info("Received: " + line);

            try
            {
                OrderRequest req = m_parser.parse(line);

                MatchResult matchResult = m_matchEngine.process(req);
                std::string response = m_parser.serialize(matchResult);
                Logger::instance().info("Sending: " + response);
                boost::asio::write(socket, boost::asio::buffer(response));

            }
            catch(const std::exception& e)
            {
                std::string errorMsg = std::string("ERROR ") + e.what() + "\n";
                boost::asio::write(socket, boost::asio::buffer(errorMsg));

                Logger::instance().error(e.what());
            }
            


            /*
            std::cout << "Received: " << line << std::endl;
            

            std::cout << "[INFO] Parsed Order: "
              << req.orderId << std::endl;


            /*std::cout << "Parsed Order: "
              << req.orderId << " "
              << req.price << " "
              << req.quantity << std::endl;*/
        }
    }
    catch(const std::exception& e)
    {
        Logger::instance().error(e.what());
        //std::cerr << e.what() << '\n';
        //std::string errorMsg = std::string("ERROR ") + e.what() + "\n";
        //boost::asio::write(socket, boost::asio::buffer(errorMsg));
        //std::cout << "[ERROR] " << e.what() << std::endl;
    }

    Logger::instance().info("Client disconnected");
}