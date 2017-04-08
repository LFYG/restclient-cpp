# [REST client for C++](https://lowboyteam.github.io/restclient-cpp/)
[![MIT license](https://img.shields.io/badge/license-MIT-blue.svg)](http://opensource.org/licenses/MIT)


## 关于
这是一个用于C++的简单便捷的REST Client组件,它包装了windows (winhttp/winnet) api !  
支持代理,http BasicAuth 认证

## 环境
C++11

## 用法
restclient-cpp提供了两种与REST端点交互的方式.  
有一个简单的,不需要你配置,所以如果你需要的不仅仅是一个简单的HTTP调用,  
你可能会想查看高级用法。还提供了一个编码转换类,  [textconv_helper](https://github.com/LowBoyTeam/restclient-cpp/blob/master/restclient-cpp/textconv_helper.hpp "Title")  
使用方法:
```cpp
//response utf8
std::wstring body = textconv_helper::A2W_(response.body.c_str(), CP_UTF8);
wprintf(L"body = %s \n", body.c_str());
```
默认使用的是Winnet Api,切换WinHttp Api
```cpp
#define _USE_WIHTTP_INTERFACE
```
### 简单使用

```cpp
#include "restclient-cpp/restclient.h"

RestClient::Response r = RestClient::get("http://url.com")
RestClient::Response r = RestClient::post("http://url.com/post", "text/json", "{\"foo\": \"bla\"}")
RestClient::Response r = RestClient::put("http://url.com/put", "text/json", "{\"foo\": \"bla\"}")
RestClient::Response r = RestClient::del("http://url.com/delete")
RestClient::Response r = RestClient::head("http://url.com")
```

响应类型为RestClient :: Response
属性：

```cpp
RestClient::Response.code // HTTP response code
RestClient::Response.body // HTTP response body
RestClient::Response.headers // HTTP response headers,The type is map
RestClient::Response.cookies // HTTP response cookies,The type is string
RestClient::Response.cookie // HTTP response cookies,The type is map
RestClient::Response.get_header //It can be used to get the specified header value
RestClient::Response.get_cookie //It can be used to get the specified cookie value
```

### 高级用法

#### GET simple
```cpp
RestClient::Request request;

request.timeout     	        = 3000;
request.followRedirects     	= false;
request.headers["User-Agent"]	= "Mozilla/5.0";
request.headers["Cookies"]	  = "name=value;";

RestClient::Response response	= RestClient::get("http://www.baidu.com", &request);

printf("%s \n", response.body.c_str());
printf("%d \n", response.code);
printf("%s \n", response.cookies.c_str());
printf("%s %s \n", response.Cookie["BAIDUID"].c_str(), response.get_cookie("BAIDUID").c_str());
printf("%s %s \n", response.headers["Location"].c_str(), response.get_header("Location").c_str());
printf("%s %s \n", response.headers["Content-Type"].c_str(), response.get_header("Content-Type").c_str());
printf("%s %s \n", response.headers["Content-Length"].c_str(), response.get_header("Content-Length").c_str());
```

#### POST simple
```cpp
RestClient::Request request;

request.timeout	= 3000;
request.followRedirects	= false;
request.headers["User-Agent"]	= "Mozilla/5.0";
request.headers["Cookies"] = "name=value;";

RestClient::Response response = RestClient::post("http://www.baidu.com/post.php",
"text/json", "{\"foo\": \"bla\"}",&request);

printf("%s \n", response.body.c_str());
printf("%d \n", response.code);
printf("%s \n", response.cookies.c_str());
printf("%s \n", response.headers["Content-Type"].c_str());
printf("%s \n", response.headers["Content-Length"].c_str());
```

#### Proxy simple

```cpp
RestClient::Request request;

request.proxy.proxy = "http=115.29.2.139:80";
request.proxy.username = "";
request.proxy.password = "";

//request.basicAuth.username = "your username";
//request.basicAuth.password = "your password";

request.timeout	= 3000;
request.followRedirects	= false;
request.headers["User-Agent"]	= "Mozilla/5.0";
request.headers["Cookie"] = "name=value;";

RestClient::Response response	= RestClient::get("http://ipip.yy.com/get_ip_info.php", &request);

//response utf8
std::wstring body = textconv_helper::A2W_(response.body.c_str(), CP_UTF8);
wprintf(L"body = %s \n", body.c_str());

printf("code = %d \n", response.code);
printf("cookie = %s \n", response.cookies.c_str());
printf("%s %s \n", response.Cookie["BAIDUID"].c_str(), response.get_cookie("BAIDUID").c_str());
printf("%s %s \n", response.headers["Location"].c_str(), response.get_header("Location").c_str());
printf("%s %s \n", response.headers["Content-Type"].c_str(), response.get_header("Content-Type").c_str());
printf("%s %s \n", response.headers["Content-Length"].c_str(), response.get_header("Content-Length").c_str());

```

### TODO
1.  upload file
