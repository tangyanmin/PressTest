/****************************************************************************
 * Copyright (C), 2016, Sensetime Tech. Co., Ltd.
 * file name  : server.cpp
 * version    : 1.0
 * create date: 2016-10-19 16:13:07 +0800
 * creator    : tangyanmin@sensetime.com
 * description: TODO
 * modify log : {who} {when} {what}
 ***************************************************************************/

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using std::string;
using namespace std;

namespace
{
    std::string make_daytime_string()
    {
        using namespace std; // For time_t, time and ctime;
        time_t now = time(0);
        return ctime(&now);
    }
}

void test_asio_synserver()
{
    typedef boost::asio::io_service IoService;
    typedef boost::asio::ip::tcp TCP;

    try
    {
        IoService ios;
        /*
            *以下构造函数相当于以下步骤
            * basic_socket_acceptor<tcp> acceptor(io_service);
            * acceptor.open(tcp::v4());
            * acceptor.bind(tcp::endpoint(13));
            * acceptor.listen(0);//default
        */
        /*
            tcp::endpoint(端点)由以下三个部分组成
            1. IP地址(address, address_v4, address_v6)
            2. 端口号
            3. 协议版本
        */
        TCP::acceptor acceptor(ios, TCP::endpoint(TCP::v4(), 9999));
        for (;;)
        {
            TCP::socket socket(ios);
            acceptor.accept(socket);
            std::string message = make_daytime_string();

            boost::system::error_code error;
            char buf[1024] = {0};
            memset(buf, 0, sizeof(buf));
            size_t len = socket.read_some(boost::asio::buffer(buf), error);
            cout<<"recv len:"<< len <<" data:" << buf <<std::endl;

            message += "   ";
            message += buf;
            boost::system::error_code ignored_error;
            boost::asio::write(socket,
                               boost::asio::buffer(message),
                               boost::asio::transfer_all(),
                               ignored_error);
            //PRINT_DEBUG(message);
            cout<<message<<std::endl;
        }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

int main(void)
{
	test_asio_synserver();
	return 1;
}

