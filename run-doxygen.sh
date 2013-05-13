#!/bin/bash
cd "$( dirname "$0" )"
rm -r doc/html
mkdir doc/html

# copy doc images to doc/html directory. Doxyfile's IMAGE_PATH includes them from there:
cp other/doc-image-generator/images/* doc/html/
cp doc/images/* doc/html/

doxygen
cd doc/html
rm *.map *.md5 index.qhp

