#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include "nlohmann/json.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include "Basket.h"
#include <optional>
#include <vector>
#include <algorithm>
#include <atomic>
namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
using std::vector;
using std::find;
void Register(const std::string& name, const std::string& id, const std::string& address, int port){
    net::io_context ioc;
    tcp::resolver resolver(ioc);
    beast::tcp_stream stream(ioc);
    auto const results = resolver.resolve("consul", "8500");
    stream.connect(results);
    std::string body = "{"
        "\"Name\":\"" + name + "\","
        "\"ID\":\"" + id + "\","
        "\"Address\":\"" + address + "\","
        "\"Port\":" + std::to_string(port) + ","
        "\"Check\":{"
            "\"HTTP\":\"http://" + address + ":" + std::to_string(port) + "/health\","
            "\"Interval\":\"10s\""
        "}"
    "}";
    http::request<http::string_body> req{http::verb::put, "/v1/agent/service/register", 11};
    req.set(http::field::host, "consul");
    req.set(http::field::content_type, "application/json");
    req.body() = body;
    req.prepare_payload();
    http::write(stream, req);
    beast::flat_buffer buffer;
    http::response<http::string_body> res;
    http::read(stream, buffer, res);
    stream.socket().shutdown(tcp::socket::shutdown_both);
}
http::response<http::string_body> handle_request(http::request<http::string_body> const& req){
    //get by id
    if(req.target() == "/health") {
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::content_type, "text/plain");
        res.result(http::status::ok);
        res.body() = "OK";
        res.prepare_payload();
        return res;
    }
    if(req.method() == http::verb::get && req.target().starts_with("/basket/")){
        std::string target = std::string(req.target());
        target = target.substr(0, target.find('?'));
        if(!target.empty() && target.back() == '/') target.pop_back();
        std::string userId = target.substr(target.find_last_of('/') + 1);
        nlohmann::json json_response = {{"message", Basket::GetBasketById(userId)},
                                        {"URL", "http://www.localhost:9256/basket/"}};
        http::response<http::string_body> res{http::status::ok, req.version()};

        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        //the response back
        res.body() = json_response.dump();
        res.prepare_payload();
        return res;
    //get all
    }else if(req.method() == http::verb::get && req.target() == "/basket"){
        std::cout<<"reseved the request to get all items"<<std::endl;
        auto basket = Basket::GetBasket();
        std::cout<<"got all the items"<<std::endl;
        for(auto it = basket.begin(); it != basket.end(); ++it){
            auto id = it.key();
            auto& basket = it.value();
        }
        nlohmann::json json_response = {{"basket", basket},
                                        {"URL", "http://www.localhost:9256/basket/"}};
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        //the response back
        res.body() = json_response.dump();
        res.prepare_payload();
        std::cout<<"sending back all the items"<<std::endl;
        return res;
    //for delete by id
    }else if(req.method() == http::verb::delete_ && req.target().starts_with("/basket/")){
        std::string target = std::string(req.target());
        target = target.substr(0, target.find('?'));
        if(!target.empty() && target.back() == '/') target.pop_back();
        std::string id = target.substr(target.find_last_of('/') + 1);
        Basket::DeleteItem(id);
        nlohmann::json json_response = {{"message", "Item deleted"},
                                        {"URL", "http://www.localhost:9256/basket/" + id}};
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        //the response back
        res.body() = json_response.dump();
        res.prepare_payload();
        return res;
    }else if(req.method() == http::verb::delete_ && req.target() == "/basket"){
        Basket::DeleteAllItems();
        nlohmann::json json_response = {{"message", "Basket Cleared"},
                                        {"URL", "http://www.localhost:9256/basket"}};
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        //the response back
        res.body() = json_response.dump();
        res.prepare_payload();
        return res;
    }
    //defalt response for an unsupported method
    return http::response<http::string_body>{http::status::bad_request, req.version()};
}
//this is for http server conections
class Session : public std::enable_shared_from_this<Session>{
    tcp::socket socket_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
    public:
        explicit Session(tcp::socket socket) : socket_(std::move(socket)){}
        void run(){do_read();}
    private: 
        void do_read(){
            auto self(shared_from_this());
            http::async_read(socket_,buffer_,req_, [this, self](beast::error_code ec, std::size_t){
                if(!ec){
                    do_write(handle_request(req_));
                }
            });
        }
        void do_write(http::response<http::string_body> res){
            auto self(shared_from_this());
            auto sp = std::make_shared<http::response<http::string_body>>(std::move(res));
            http::async_write(socket_, *sp, [this,self,sp](beast::error_code ec, std::size_t){
                socket_.shutdown(tcp::socket::shutdown_send, ec);
            });
        }
};
class Listener : public std::enable_shared_from_this<Listener>{
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    public:
        Listener(net::io_context& ioc, tcp::endpoint endpoint) : ioc_(ioc), acceptor_(net::make_strand(ioc)){
            beast::error_code ec;
            acceptor_.open(endpoint.protocol(), ec);
            if(ec){
                std::cerr << "Open error: " << ec.message() << std::endl;
                return;
            }
            acceptor_.set_option(net::socket_base::reuse_address(true), ec);
            if(ec){
                std::cerr << "Set Option Error: " << ec.message() << std::endl;
                return;
            }
            acceptor_.bind(endpoint, ec);
            if(ec){
                std::cerr << "Binding Error: " << ec.message() << std::endl;
                return;
            }
            acceptor_.listen(net::socket_base::max_listen_connections, ec);
            if(ec){
                std::cerr << "Listening Error: " << ec.message() << std::endl;
                return;
            }
            std::cout<<"accept loop started"<<std::endl;
            do_accept();
        }
    private:
        void do_accept(){
            acceptor_.async_accept(net::make_strand(ioc_), [this](beast::error_code ec, tcp::socket socket){
                if(!ec) std::make_shared<Session>(std::move(socket))->run();
                do_accept();
            });
        };
};
int main(){
    try{
        auto const address = net::ip::make_address("0.0.0.0");
        unsigned short port = 9256;
        net::io_context ioc{1};
        auto listener = std::make_shared<Listener>(ioc,tcp::endpoint{address,port});
        std::cout<<"Server running on 9256"<<std::endl;
        Basket basket;
        Register("basket-service", "basket-1", "basket", 9256);
        ioc.run();
        std::cerr<<"ioc loop exited"<<std::endl;
    }catch(const std::exception& e){
        std::cerr<<"Error: "<<e.what()<<std::endl;
    }
}