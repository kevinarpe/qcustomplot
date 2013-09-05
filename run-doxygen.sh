#!/bin/bash
cd "$( dirname "$0" )"
rm -r documentation/html
mkdir documentation/html

doxygen
cd documentation/html
rm *.map *.md5 index.qhp

