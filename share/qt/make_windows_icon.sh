#!/bin/bash
# create multiresolution windows icon
ICON_SRC=../../src/qt/res/icons/bitsend.png
ICON_DST=../../src/qt/res/icons/bitsend.ico
convert ${ICON_SRC} -resize 16x16 bitsend-16.png
convert ${ICON_SRC} -resize 32x32 bitsend-32.png
convert ${ICON_SRC} -resize 48x48 bitsend-48.png
convert bitsend-16.png bitsend-32.png bitsend-48.png ${ICON_DST}

