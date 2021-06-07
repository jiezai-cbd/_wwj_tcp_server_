#!/bin/sh

cd build \
&& cmake .. \
&& make \
&& make install \
&& cd .. \
&& cd test \
&& cd build \
&& cmake ..\
&& make \
&& ./test1

