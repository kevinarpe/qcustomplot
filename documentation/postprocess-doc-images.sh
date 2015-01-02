#!/bin/bash

#
# This script compresses documentation PNGs by reducing their palettes.
# The color count was chosen individually to achieve a good balance between
# image quality and file size.
#

cd "$( dirname "$0" )"

find html -name 'class*__inherit__graph.png' -exec mogrify -depth 8 -colors 16 +dither {} \;
find html -name 'inherit_graph_*.png' -exec mogrify -depth 8 -colors 16 +dither {} \;

mogrify -depth 8 -colors 256  +dither html/RelationOverview.png
mogrify -depth 8 -colors 256  +dither html/LayoutsystemSketch.png
mogrify -depth 8 -colors 256  +dither html/InheritanceOverview.png
mogrify -depth 8 -colors 64  +dither html/QCPItemPixmap.png
mogrify -depth 8 -colors 256 +dither html/QCPColorGradient-periodic.png
mogrify -depth 8 -colors 256 +dither html/QCPColorMap.png
mogrify -depth 8 -colors 256 +dither html/QCPColorGradient-levelcount.png
mogrify -depth 8 -colors 64  +dither html/QCPItemText.png
mogrify -depth 8 -colors 64  +dither html/AxisRectSpacingOverview.png
mogrify -depth 8 -colors 64  +dither html/QCPItemBracket-length.png
mogrify -depth 8 -colors 64  +dither html/QCPItemEllipse.png
mogrify -depth 8 -colors 256 +dither html/QCPColorMap-interpolate.png
mogrify -depth 8 -colors 64  +dither html/AxisNamesOverview.png
mogrify -depth 8 -colors 64  +dither html/QCPMarginGroup.png
mogrify -depth 8 -colors 64  +dither html/layoutsystem-multipleaxisrects.png
mogrify -depth 8 -colors 64  +dither html/QCPGraph.png
mogrify -depth 8 -colors 64  +dither html/QCPFinancial.png
mogrify -depth 8 -colors 64  +dither html/QCPCurve.png
mogrify -depth 8 -colors 64  +dither html/QCPLineEnding.png
mogrify -depth 8 -colors 64  +dither html/QCPItemRect.png
mogrify -depth 8 -colors 64  +dither html/layoutsystem-addingplottitle.png
mogrify -depth 8 -colors 256 +dither html/QCPColorScale.png
mogrify -depth 8 -colors 64  +dither html/QCPBarsGroup.png
mogrify -depth 8 -colors 64  +dither html/QCPItemCurve.png
mogrify -depth 8 -colors 64  +dither html/QCPColorMap-tightboundary.png
mogrify -depth 8 -colors 64  +dither html/QCPItemBracket.png
mogrify -depth 8 -colors 64  +dither html/QCPItemTracer.png
mogrify -depth 8 -colors 64  +dither html/QCPStatisticalBox.png
mogrify -depth 8 -colors 64  +dither html/QCPItemLine.png
mogrify -depth 8 -colors 64  +dither html/QCPItemStraightLine.png
mogrify -depth 8 -colors 64  +dither html/QCPBars.png

