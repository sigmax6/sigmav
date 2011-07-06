#GENERITE MOC_FILE FOR SIMPLE QT PROJECT BY SIGMAX6

#!/bin/bash

#USER INPUT HERE

File=hello   #The filename of .ui
FrameName=hello   #The name of frame in the .ui
ExeFileName=hello #The name of dst exec file

#####################################
#DO NOT! EDIT THE LINES BELOW
######################################
#Delete the old files
rm -f $File.h $File.cpp moc_$File.cpp main.cpp $ExeFileName.pro

#From the .ui to .h and .cpp
uic -o $File.h $File.ui
uic -o $File.cpp -impl $File.h $File.ui
moc -o moc_$File.cpp $File.h

#Create main.cpp
echo "#include \"$File.h\"" > main.cpp    #create main.cpp and include the File.h just made
echo "#include <qapplication.h>" >> main.cpp
echo "#include <qtopia/qpeapplication.h>" >> main.cpp
echo "QTOPIA_ADD_APPICATION(\"$ExeFileName\",$FrameName)" >> main.cpp   
echo "QTOPIA_MAIN" >> main.cpp

#Progen .pro File
progen -o $ExeFileName.pro
