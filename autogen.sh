#!/usr/bin/env bash
autoreconf -iv
#./configure --prefix=$HOME/.local CXXFLAGS='-ggdb -std=gnu++14'
./configure --prefix=$HOME/.local CXXFLAGS='-ggdb'
