#!/usr/bin/env bash

DIR=$1
FILE=$2

g++ -g -std=c++11 -o $FILE -I/opt/libraries/nghttp2/1.14.1/include -L/opt/libraries/nghttp2/1.14.1/lib ../$DIR/$FILE.cpp -lnghttp2_asio -lboost_system -lssl -lcrypto -lpthread
