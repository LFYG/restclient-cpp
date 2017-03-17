# REST client for C++
[![MIT license](https://img.shields.io/badge/license-MIT-blue.svg)](http://opensource.org/licenses/MIT)


## About
This is a simple REST client for C++. It wraps winnet for http/https requests.

## Usage
restclient-cpp provides two ways of interacting with REST endpoints. There is
a simple one, which doesn't need you to configure an object to interact with
an API. However the simple way doesn't provide a lot of configuration options
either. So if you need more than just a simple HTTP call, you will probably
want to check out the advanced usage.

### Simple Usage
The simple API is just some static methods modeled after the most common HTTP
verbs:

```cpp
#include "restclient-cpp/restclient.h"

RestClient::Response r = RestClient::get("http://url.com")
RestClient::Response r = RestClient::post("http://url.com/post", "text/json", "{\"foo\": \"bla\"}")
RestClient::Response r = RestClient::put("http://url.com/put", "text/json", "{\"foo\": \"bla\"}")
RestClient::Response r = RestClient::del("http://url.com/delete")
RestClient::Response r = RestClient::head("http://url.com")
```

The response is of type RestClient::Response and has
three attributes:

```cpp
RestClient::Response.code // HTTP response code
RestClient::Response.body // HTTP response body
RestClient::Response.headers // HTTP response headers,The type is map
RestClient::Response.cookies // HTTP response cookies,The type is string
RestClient::Response.cookie // HTTP response cookies,The type is map
RestClient::Response.get_header //It can be used to get the specified header value
RestClient::Response.get_cookie //It can be used to get the specified cookie value
```

### Advanced Usage
However if you want more sophisticated features,
timeouts or request headers, there is also a different, more configurable way.

#### GET simple
```cpp
RestClient::Request request;

request.timeout 		= 3000;
request.followRedirects 	= false;
request.headers["User-Agent"]	= "Mozilla/5.0";
request.headers["Cookies"]	= "name=value;";

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

request.timeout 		= 3000;
request.followRedirects 	= false;
request.headers["User-Agent"] 	= "Mozilla/5.0";
request.headers["Cookies"] 	= "name=value;";

RestClient::Response response 	= RestClient::post("http://www.baidu.com/post.php", 
                                  "text/json", "{\"foo\": \"bla\"}",&request);

printf("%s \n", response.body.c_str());
printf("%d \n", response.code);
printf("%s \n", response.cookies.c_str());
printf("%s \n", response.headers["Content-Type"].c_str());
printf("%s \n", response.headers["Content-Length"].c_str());
```
### TODO
1.  http proxy
2.  upload file
3.  authentication
