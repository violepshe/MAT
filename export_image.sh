#!/bin/bash
#This script calls an application called QTI2GRF and receives arguments, 
#the expected output is an image with .tiff extension 


by_disp() {
V:/aeapps/siesta_lite/prodbig/QTI2GRF.exe << EOF
HIDE
QTGR ORDB
$1
QTGR LCAS $2
USE TYPE ELEM SHEL
USE TYPE NODE
CONT FROM $3
CONT NLEV 9
DISP ONLY
DISP SCAL 0.25
USE EWIN
COPY TIF
QUIT 
EOF
}

by_fill() {
V:/aeapps/siesta_lite/prodbig/QTI2GRF.exe << EOF
HIDE
QTGR ORDB
$1
QTGR LCAS $2
USE TYPE ELEM SHEL
FILL FROM $3
CONT NLEV 9
DISP SCAL 0.25
USE EWIN
COPY TIF
QUIT 
EOF
}

by_fill_gnums() {
V:/aeapps/siesta_lite/prodbig/QTI2GRF.exe << EOF
HIDE
QTGR ORDB
$1
QTGR LCAS $2
FILL FROM GNUM
DISP SCAL 0.25
DISP OVER
USE EWIN
USE MWIN 0.12
COPY TIF
QUIT 
EOF
}

if [ "$5" == "disp"]; then
    by_disp $1 $3 $4
fi

if [ "$5" == "fill"]; then
    by_fill $1 $3 $4
fi

if [ "$5" == "gnums"]; then
    by_fill_gnums $1 $3
fi
