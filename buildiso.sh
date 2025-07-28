#!/bin/bash

cmake --build build
mkisofs -o build/build.iso build
