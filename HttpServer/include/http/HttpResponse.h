#pragma once

#include <muduo/net/TcpServer.h>

namespace http
{

class HttpResponse 
{
public:
    enum HttpStatusCode
    {
        //标准的http状态码
        kUnknown,
        k200Ok = 200,
        k204NoContent = 204,
        k301MovedPermanently = 301,
        k400BadRequest = 400,
        k401Unauthorized = 401,
        k403Forbidden = 403,
        k404NotFound = 404,
        k409Conflict = 409,
        k500InternalServerError = 500,
    };

    HttpResponse(bool close = true)
        : statusCode_(kUnknown)
        , closeConnection_(close)
    {}

    void setVersion(std::string version)
    { httpVersion_ = version; }
    void setStatusCode(HttpStatusCode code) //设置http状态码
    { statusCode_ = code; }

    HttpStatusCode getStatusCode() const //获取当前状态码
    { return statusCode_; }

    void setStatusMessage(const std::string message)
    { statusMessage_ = message; }

    void setCloseConnection(bool on)
    { closeConnection_ = on; }

    bool closeConnection() const
    { return closeConnection_; }
    
    void setContentType(const std::string& contentType) //设置内容类型
    { addHeader("Content-Type", contentType); }

    void setContentLength(uint64_t length)
    { addHeader("Content-Length", std::to_string(length)); }

    void addHeader(const std::string& key, const std::string& value)  //添加http头部
    { headers_[key] = value; }
    
    void setBody(const std::string& body)
    { 
        body_ = body;
        // body_ += "\0";
    }

    void setStatusLine(const std::string& version,
                         HttpStatusCode statusCode,
                         const std::string& statusMessage);   //设置状态行（版本，状态码和状态消息）

    void setErrorHeader(){}

    void appendToBuffer(muduo::net::Buffer* outputBuf) const;  
    //将整个相应数据追加到输出缓冲区，这是将相应转换为HTTP协议格式的关键方法。
    /*
    首先构造状态行（HTTP版本、状态码和状态消息）
    根据连接策略添加Connection头部（close或Keep-Alive）
    添加所有自定义头部
    最后添加响应体内容
    */ 
private:
    std::string                        httpVersion_; 
    HttpStatusCode                     statusCode_;
    std::string                        statusMessage_;
    bool                               closeConnection_;
    std::map<std::string, std::string> headers_;
    std::string                        body_;
    bool                               isFile_;
};
//http响应表示类，用于构建和管理服务器向客户端返回的http相应数据，是http服务器实现响应生成的组件。
} // namespace http