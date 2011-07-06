#!/bin/bash

# External output may be "VGA" or "VGA-0" or "DVI-0" or "TMDS-1"
EXTERNAL_OUTPUT="HDMI2"
INTERNAL_OUTPUT="LVDS1"
# External location may be "--left-of" or "--right-of" or "--above" or "--below"
EXTERNAL_LOCATION="--right-of"
 
# Figure out which user and X11 display to work on
X_USER=$(ps au | grep startx | awk '{print $1}')
export DISPLAY=':0.0'
 
# Switch to X user if necessary
if [ "$X_USER" != "$USER" ]
then
	SU="su $X_USER -c"
else
	SU="sh -c"
fi
 
# Figure out current state
EXTERNAL_STATE=$($SU xrandr | sed -n "/$EXTERNAL_OUTPUT/,/$INTERNAL_OUTPUT/p" | grep '*')
 
if [ -z "$EXTERNAL_STATE" ]
then
	$SU "xrandr --output $EXTERNAL_OUTPUT $EXTERNAL_LOCATION $INTERNAL_OUTPUT --auto"
else
	$SU "xrandr --output $EXTERNAL_OUTPUT --off"
fi
 
