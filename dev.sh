#!/bin/bash

tar xvf install.tgz
make shaders GLSLC=install/bin/glslc

tail -f /dev/null