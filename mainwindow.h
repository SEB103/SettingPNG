#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/*========================================================================================
    PROJECT Setting PNG
==========================================================================================
    Copyright (c) 2021. Sergey Burkin.
    GNU GENERAL PUBLIC LICENSE Version 2, June 1991.

    SUBSYSTEM:    SettingPNG.exe Application
    FILE:         Mainwindow .h
    AUTHOR:       Sergey Burkin

    OVERVIEW
    ========
    Header file for definition of class "MainWindow".

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
==========================================================================================*/

#include <QMainWindow>
#include <QStringListModel>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
class QProgressBar;
class QSettings;
class QDir;
class QStringListModel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();
  const QSettings* Settings() const;

  QString openDirectory() const { return m_openDirectory; };
  void setOpenDirectory(const QString& openDirectory);

private slots:

  void pushButtonSave_clicked();
  void pushButtonOpen_clicked();
  void pushButton_FindPng_clicked();
  void pushButton_FixPng_clicked();
  void pushButton_SaveSettings_clicked();
  void comboBoxColorSpace_currentTextChanged(const QString& arg1);
  void pushButtonClose_clicked();
  void pushButtonOpenFiles_clicked();
  void pushButtonUndo_clicked();

private:
  Ui::MainWindow* ui;
  const QString APPLICATION_NAME = "SettingPNG";
  const QString SETTINGS_MAIN    = "/Settings/main";

  QSettings* m_settings;
  QStringListModel m_model;

  void writeSettings(const QVariant& value, const QString& name, const QString& group, const QString& subdir = "");
  QVariant readSettings(const QVariant& defaultValue, const QString& name, const QString& group, const QString& subdir = "");
  QString m_openDirectory {""};
  QString m_saveDirectory {""};
  QString m_colorSpace {"QColorSpace::DisplayP3"};
  QProgressBar* m_progress;
};
#endif // MAINWINDOW_H
