#!/bin/bash
montage $(ls -v *.ppm) -tile 8x6 -geometry +0+0 raytraced_qs8x6a0.75.png
