#!/bin/bash
cd "$( dirname "$0" )"
rm -r documentation/html
mkdir documentation/html

# let doxygen generate qhp index and html hierarchy from source files:
doxygen

# do post-processing on documentation images in html hierarchy:
echo Post-processing documentation images...
sh documentation/postprocess-doc-images.sh

# generate qhc file from qhp index for QtAssistant integration:
echo Building qhc...
qhelpgenerator documentation/html/index.qhp -o documentation/qthelp/qcustomplot.qch

# cleanup html hierarchy:
rm documentation/html/index.qhp
rm documentation/html/*.map
rm documentation/html/*.md5

