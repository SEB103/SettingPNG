/*========================================================================================
    PROJECT Setting PNG
==========================================================================================
    Copyright (c) 2021. Sergey Burkin.
    GNU GENERAL PUBLIC LICENSE Version 2, June 1991.

    SUBSYSTEM:    SettingPNG.exe Application
    FILE:         main.cpp
    AUTHOR:       Sergey Burkin

    OVERVIEW
    ========
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
==========================================================================================*/
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
  QCoreApplication::setApplicationName("SettingPNG");
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}
