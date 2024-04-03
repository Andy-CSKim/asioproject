//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2024 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::tcp;

class session
  : public std::enable_shared_from_this<session>
{
public:
  session(tcp::socket socket)
    : socket_(std::move(socket))
  {
  }

  void start()
  {
    std::cout<< "start" << std::endl;
    do_read();
  }

private:
  int nCount = 0;
  // std::atomic<bool> bStop;
  bool bStop = false;

  int nValue = 0;

  void do_loop() {
    std::cout<< "do_loop" << std::endl;
    while (!bStop) {
      nCount++;
      // if (nCount % 1000000 == 0) {
      //   std::cout << "nCount(loop): " << nCount << std::endl;
      // }
      // bStop.load();
    }
    std::cout<< "== do_loop end ==" << std::endl;
  }

  void do_read()
  {
    bStop = false;
    // bStop.store(false);
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t length)
        // [&](boost::system::error_code ec, std::size_t length)
        {
          if (!ec)
          {
            // andy : print data
            // std::cout << "rxed data(" << length<< "): "<< data_ << std::endl;
            bStop = true;

            // bStop.store(true);
            // (Watch out) cout is not thread safe, so it may be interupted by other thread
            std::cout << "read_some(" << nCount << "): " << data_ << std::endl;

            // conver to upper case
            for (int i = 0; i < length; i++) {
              data_[i] = std::toupper(data_[i]);
            }

            do_write(length);
          }
        });
    
    // [this] () { }
    // boost::thread t(&test_thread);

    // boost::session s;
    // s.do_loop();  --> boost::session::do_loop(session &s);

    boost::thread t(&session::do_loop, this);
    t.detach();
    //t.join();   // what's the difference between join and detach?

    // print bStop and nCount
    std::cout << " ==== do_read end ===="<< std::endl;
    
  }

  void do_write(std::size_t length)
  {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        // [&](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            // andy : print data
            // std::cout << "txed data(" << length<< "): "<< data_ << std::endl;
            std::cout << "txed data: "<< data_ << std::endl;
            do_read();
          }
        });
  }

  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
};

class server
{
public:
  server(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
  {
    do_accept();
  }

private:
  void do_accept()
  {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {
            std::make_shared<session>(std::move(socket))->start();
          }

          std::cout << "do_accept" << std::endl;
          do_accept();
        });
  }

  tcp::acceptor acceptor_;
};

void test_thread() {
  int nCount = 0;
  while (true) {
    nCount++;
    if (nCount % 1000000 == 0) {
      std::cout << "nCount(thread): " << nCount << std::endl;
    }
  }
}

int main(int argc, char* argv[])
{
  // boost::thread t(&test_thread);
  // t.join();

  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: async_tcp_echo_server <port>\n";
      return 1;
    }

    boost::asio::io_context io_context;

    // andy
    std::cout << "before server" << std::endl;
    server s(io_context, std::atoi(argv[1]));

    io_context.run();    // andy
    std::cout << "after server" << std::endl;

  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}


// class test {
//   void do_loop() {}
// }


// class test t;

// t.do_loop();  // --> test::do_loop(&t);
// thread(&test::do_loop, &t);

// void do_loop2() {}
// thread(do_loop2);

