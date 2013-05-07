#!/bin/bash
cd "$( dirname "$0" )"
rm -r "doc/html"
doxygen
cd doc/html
rm *.map *.md5 index.qhp

# image copying is done with Doxyfile IMAGE_PATH 
# cp ../../other/doc-image-generator/images/* ./
# cp ../*.png ./

