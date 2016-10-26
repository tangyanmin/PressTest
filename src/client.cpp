/****************************************************************************
 * Copyright (C), 2016, Sensetime Tech. Co., Ltd.
 * file name  : client.cpp
 * version    : 1.0
 * create date: 2016-10-19 16:06:49 +0800
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
//void test_asio_synclient()

//int main()
int client()
{
    typedef boost::asio::io_service IoService;
    // 该命名空间下有几个常用类: accetpt, resolver, endpoint, socket
    typedef boost::asio::ip::tcp TCP;

    try
    {
        IoService ios;
        boost::system::error_code error;

        // 1. 用解释器的方法, 常用来解析域名, 如
        // query("www.163.com","80"), 也可以 query("www.163.com","telnet")
        // echo          7/tcp
        // ftp           21/tcp                 # File Transfer Protocol (Control)
        // telnet        23/tcp                 # Virtual Terminal Protocol
        // smtp          25/tcp                 # Simple Mail Transfer Protocol
        // time          37/tcp  timeserver     # Time
//      TCP::resolver resolver(ios);
//      TCP::resolver::query query("127.0.0.1", "10000");
//      TCP::resolver::iterator endpoint_iterator = resolver.resolve(query);
//      TCP::resolver::iterator end;
//      TCP::socket socket(ios);
//      socket.connect(*endpoint_iterator, error);

        // 2. 用简便的方法
        TCP::socket socket(ios);
        //TCP::endpoint endpoint(boost::asio::ip::address_v4::from_string("127.0.0.1"), 10000);
        TCP::endpoint endpoint(boost::asio::ip::address_v4::from_string("127.0.0.1"), 9999);
        socket.connect(endpoint, error);

        // 这里要判断一下, 否则没有连上会通过.
        if (error)
            throw boost::system::system_error(error);

        while(true)
        {
            // boost::array<char, 128> buf;
            std::vector<char> buf(128);
            size_t len = socket.read_some(boost::asio::buffer(buf), error);
            cout<<len<<std::endl;
            cout<<buf.data()<<std::endl;

            // 这是也要判断一下, 否则服务端运行断开, 这里会出现死循环.
            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            // buf要注意控制大小。
            socket.write_some(boost::asio::buffer(buf, len), error);
        }
    }
    catch (std::exception& e)
    {
        //PRINT_DEBUG(e.what());
    	cout<<e.what()<<std::endl;
    }
    return 1;
}
