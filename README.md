# REST client for C++
[![Build Status](https://travis-ci.org/LowBoyTeam/restclient-cpp.svg?branch=master)](https://travis-ci.org/LowBoyTeam/restclient-cpp)
[![Coverage Status](https://coveralls.io/repos/LowBoyTeam/restclient-cpp/badge.svg?branch=master&service=github)](https://coveralls.io/github/LowBoyTeam/restclient-cpp?branch=master)
[![MIT license](https://img.shields.io/badge/license-MIT-blue.svg)](http://opensource.org/licenses/MIT)


## About
This is a simple REST client for C++. It wraps winnet for HTTP requests.

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

The response is of type [RestClient::Response][restclient_response] and has
three attributes:

```cpp
RestClient::Response.code // HTTP response code
RestClient::Response.body // HTTP response body
RestClient::Response.headers // HTTP response headers
```

### Advanced Usage
However if you want more sophisticated features like connection reuse,
timeouts or authentication, there is also a different, more configurable way.
