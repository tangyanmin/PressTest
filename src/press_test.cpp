
#ifndef HTTPPOST_H_
#define HTTPPOST_H_

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;
using std::string;

const char *cmd_dblist = "{\"cmd\": \"dblist\"}\r\n";
const char *cmd_dbclear = "{\"cmd\": \"dbclear\", \"args\": [\"test100\"]}\r\n";
const char *cmd_dbdel = "{\"cmd\": \"dbdel\", \"args\": [\"test100\"]}\r\n";
const char *cmd_dbnew = "{\"cmd\": \"dbnew\", \"args\": [\"test100\", {\"opq_model\": \"10000\", \"max_size\": 100000}]}\r\n";

char *cmd_hset = "{\"cmd\":\"hset\",\"args\":[\"test100\",\"67c2c60c852711e6b7204ccc6a23ab42\",\"7qpXQktOAACAAAAAtkXlvTckJzwi3/q88tFSvjlyjj2LAr89dLyyveCdob1gTaO9oWb5vPj+c77FnYg99kA6vvEVkj2ROKK8AJJDvTT/ALwiWN89ErAePppTiL0xa6A9EowwPr6S8LwaV7G8fNO5vJ7Vo70IYtE9U5ImvjHjzz04tVe9O6jyu4BTFz60bSu+Pp/wu37yDT0bklS90tf8PbKyxT3Zxwy+UIiGPUgTqr3WC0Q9twI9PRwGJb1BKPE9KCFcPU93Bb1qDaC8CuHxO6SGCb2RmKK9ZKVXvQWwu7xmGPO9Y2opvt7kBj2maYy8sb9VPfvGVj2YQjM9ur4SvVDwj72PMLG9/scaPU24sby9ag29+dAXPfcIKr22nDw+JKxWPSIeOj6vWle+zqXWvKPYmjw1Gdi9qFqHvElYtD2Roqw9czA9vbyKsT0RcTS8Ib3CuxIoBb6c/3i9mj3gPT8ddb20Mwq+5QuQPezr57ycFJy9WvMIPenXOT0xkyS7loS7vWI8pTxvRok9/4L8vev5Eb3Hiiu9KrgpPcbAOT6j+Aq+BpraPMZeq70oIKS94gajPTzn0DxUWJu9dmJ2PSqKwb39uw0+Rh9JPV7oWL2M2Jw7lHYavdLgMT5InHe98xdHvW3Tzr0Jt7M8b2+oPcERgj2aa6E95hiVPdaZkbvq5t88B8oyvUqkBr4=\"]}\r\n";
char *cmd_search = "{\"cmd\":\"search\",\"args\":[\"test100\",\"7qpXQktOAACAAAAAtkXlvTckJzwi3/q88tFSvjlyjj2LAr89dLyyveCdob1gTaO9oWb5vPj+c77FnYg99kA6vvEVkj2ROKK8AJJDvTT/ALwiWN89ErAePppTiL0xa6A9EowwPr6S8LwaV7G8fNO5vJ7Vo70IYtE9U5ImvjHjzz04tVe9O6jyu4BTFz60bSu+Pp/wu37yDT0bklS90tf8PbKyxT3Zxwy+UIiGPUgTqr3WC0Q9twI9PRwGJb1BKPE9KCFcPU93Bb1qDaC8CuHxO6SGCb2RmKK9ZKVXvQWwu7xmGPO9Y2opvt7kBj2maYy8sb9VPfvGVj2YQjM9ur4SvVDwj72PMLG9/scaPU24sby9ag29+dAXPfcIKr22nDw+JKxWPSIeOj6vWle+zqXWvKPYmjw1Gdi9qFqHvElYtD2Roqw9czA9vbyKsT0RcTS8Ib3CuxIoBb6c/3i9mj3gPT8ddb20Mwq+5QuQPezr57ycFJy9WvMIPenXOT0xkyS7loS7vWI8pTxvRok9/4L8vev5Eb3Hiiu9KrgpPcbAOT6j+Aq+BpraPMZeq70oIKS94gajPTzn0DxUWJu9dmJ2PSqKwb39uw0+Rh9JPV7oWL2M2Jw7lHYavdLgMT5InHe98xdHvW3Tzr0Jt7M8b2+oPcERgj2aa6E95hiVPdaZkbvq5t88B8oyvUqkBr4=\",10,{\"payload\":true,\"minscore\":0.0}]}\r\n";


//{"cmd": "dblist"}
//{"cmd": "dbdel", "args": ["pkgtest"]}
//{"cmd": "dbfsync", "args": ["pkgtest"]}
//{"cmd": "dbnew", "args": ["pkgtest", {"opq_model": "10000", "max_size": 100000}]}
//{"cmd":"dbnew","args":["pkgtest",{"max_size":1000000,"opq_model":"10000"}]}
//{"cmd": "dbclear", "args": ["pkgtest"]}
//
//{"cmd":"hdel","args":["pkgtest","d5ff976e99ca11e68cce4ccc6a4922b7"]}
//{"cmd":"hget","args":["pkgtest","d5ff976e99ca11e68cce4ccc6a4922b7"]}
//{"cmd":"hgetbatch","args":["pkgtest","d5e93e4299ca11e68cce4ccc6a4922b7"]}
//{"cmd":"hgetbatch","args":["pkgtest",["d5e93e4299ca11e68cce4ccc6a4922b7","d5f203f699ca11e68cce4ccc6a4922b7"]]}
//{"cmd":"hgetbatchidx","args":["pkgtest",1,10]}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define MAXLINE 10240

int sendCMD(const string& host, int port, int counter, const string& data, string& reponse_data)
{
  struct sockaddr_in pin;
  int sock_fd;
  char buf[MAXLINE];
  int n;

  signal(SIGPIPE,SIG_IGN);

  bzero(&pin, sizeof(pin));
  pin.sin_family = AF_INET;
  inet_pton(AF_INET, host.c_str(), &pin.sin_addr);
  pin.sin_port = htons(port);

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  //fcntl(sock_fd,F_SETFL,O_NONBLOCK);

  n=connect(sock_fd, (struct sockaddr *)&pin, sizeof(struct sockaddr_in));
  if (-1 == n)
  {
     perror("call connect");
     exit(1);
  }
  while(counter-- > 0)
  {
    write(sock_fd, data.c_str(), data.length());
    n=read(sock_fd, buf, MAXLINE);
    if (0 == n)
      printf("the othere side has been closed.\n");
    else
      printf("receive from server:%s\n",buf);
  }
  close(sock_fd);
  return 0;
}

int sendCMD(const string& host, const string& port, int counter, const string& data, string& reponse_data)
{
  try
  {
    boost::asio::io_service io_service;
    boost::system::error_code error;
    //如果io_service存在复用的情况
    if(io_service.stopped())
      io_service.reset();

    tcp::socket socket(io_service);
    tcp::endpoint endpoint(boost::asio::ip::address_v4::from_string(host), 8022);
    socket.connect(endpoint, error);

    // 这里要判断一下, 否则没有连上会通过.
    if (error)
        throw boost::system::system_error(error);

    char buf[10240] = {0};
    while((--counter)> 0)
    {	usleep(1000);
        // buf要注意控制大小。
        socket.write_some(boost::asio::buffer(data.data(), data.length()), error);
        cout<<"send len:"<< data.length() <<" data:" << data.data() <<std::endl;
        memset(buf, 0, sizeof(buf));
        size_t len = socket.read_some(boost::asio::buffer(buf), error);
        cout<<"recv len:"<< len <<" data:" << buf <<std::endl;

        // 这是也要判断一下, 否则服务端运行断开, 这里会出现死循环.
        if (error == boost::asio::error::eof)
            break; // Connection closed cleanly by peer.
        else if (error)
            throw boost::system::system_error(error); // Some other error.
    }
  }
  catch(std::exception& e)
  {
    reponse_data = e.what();
      return -4;
  }
  return 0;
}

int TEST_dbnew(const string& host, const string& port)
{
	cout<<"-------------dbnew-----------------"<<std::endl;
	int dbnum = 2;
	string reponse_data;
	sendCMD(host, port, dbnum, string(cmd_dbnew), reponse_data);
}

int TEST_dbclear(const string& host, const string& port)
{
	cout<<"-------------dbclear-----------------"<<std::endl;
	int dbnum = 2;
	string reponse_data;
	sendCMD(host, port, dbnum, string(cmd_dbclear), reponse_data);
}

int TEST_dblist(const string& host, const string& port)
{
	cout<<"-------------dblist-----------------"<<std::endl;
	int dbnum = 2;
	string reponse_data;
	sendCMD(host, port, dbnum, string(cmd_dblist), reponse_data);
}

int TEST_dbdel(const string& host, const string& port)
{
	cout<<"-------------dbdel-----------------"<<std::endl;
	int dbnum = 2;
	string reponse_data;
	sendCMD(host, port, dbnum, string(cmd_dbdel), reponse_data);
}

int TEST_search(const string& host, const string& port)
{
	cout<<"-------------search-----------------"<<std::endl;
	int dbnum = 10;
	string reponse_data;
	sendCMD(host, port, dbnum, string(cmd_search), reponse_data);
}

int TEST_hset(const string& host, const string& port)
{
	cout<<"-------------hset-----------------"<<std::endl;
	int dbnum = 2;
	string reponse_data;
	sendCMD(host, port, dbnum, string(cmd_hset), reponse_data);
    return 0;
}



int main(int argc, char* argv[])
//int test()
{
  string host = "127.0.0.1";
  string port = "8022";
  //string port = "9999";

//  int ret = 0;
//  string reponse_data;
//  sendCMD(host, 8022, 10, string(cmd_dblist), reponse_data);

  TEST_dblist(host, port);
  TEST_dbnew(host, port);
  TEST_dblist(host, port);
//  TEST_dbclear(host, port);
//  TEST_dblist(host, port);
  TEST_hset(host, port);
  TEST_search(host, port);
  TEST_dbdel(host, port);
  TEST_dblist(host, port);


  return 0;
}


#endif /* HTTPPOST_H_ */
