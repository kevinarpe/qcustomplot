#!/bin/bash
cd "$( dirname "$0" )"
rm -r documentation/html
mkdir documentation/html

# copy doc images to documentation/html directory. Doxyfile's IMAGE_PATH includes them from there:
cp other/doc-image-generator/images/* documentation/html/
cp doc/images/* documentation/html/

doxygen
cd documentation/html
rm *.map *.md5 index.qhp

