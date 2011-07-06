TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

CONFIG += thread

SOURCES	+= main.cpp

FORMS	= mymenu.ui

IMAGES	= 图片/1.jpg \
	图片/2.jpg \
	图片/3.jpg \
	图片/4.jpg \
	图片/5.jpg \
	图片/6.jpg \
	图片/7.jpg

DBFILE	= menu.db
unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}


TARGET = menu


