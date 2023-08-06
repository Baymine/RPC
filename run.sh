#!/bin/bash
./bin/test_http_server ./conf/test_http_server.xml
# ./bin/test_tinypb_server ./conf/test_tinypb_server.xml


# -X: 表示请求命令，后面指定对应的HTTP方法，可以忽略，因为GET方法就是默认的curl方法
# curl -X GET 'http://127.0.0.1:20000/query?id=10000'