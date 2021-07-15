/*========================================================================================
    PROJECT Setting PNG
==========================================================================================
    Copyright (c) 2021. Sergey Burkin.
    GNU GENERAL PUBLIC LICENSE Version 2, June 1991.

    SUBSYSTEM:    SettingPNG.exe Application
    FILE:         mainwindow.cpp
    AUTHOR:       Sergey Burkin

    OVERVIEW
    ========
    Source file for definition of class "MainWindow".

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
==========================================================================================*/

#include <QDir>
#include <QFileDialog>
#include <QProgressBar>
#include <QSettings>
#include <QtGui>
#include <QtWidgets>

#include "mainwindow.h"
#include "ui_mainwindow.h"
namespace GL
{
} // namespace GL

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  m_progress = new QProgressBar(this);
  m_progress->setVisible(false);

  ui->pushButtonUndo->setEnabled(false);

  connect(ui->pushButton_SaveSettings, &QPushButton::clicked, this, &MainWindow::pushButton_SaveSettings_clicked);
  connect(ui->pushButton_FindPng, &QPushButton::clicked, this, &MainWindow::pushButton_FindPng_clicked);
  connect(ui->pushButton_FixPng, &QPushButton::clicked, this, &MainWindow::pushButton_FixPng_clicked);
  connect(ui->pushButtonOpen, &QPushButton::clicked, this, &MainWindow::pushButtonOpen_clicked);
  connect(ui->pushButtonSave, &QPushButton::clicked, this, &MainWindow::pushButtonSave_clicked);
  connect(ui->pushButtonClose, &QPushButton::clicked, this, &MainWindow::pushButtonClose_clicked);
  connect(ui->pushButtonUndo, &QPushButton::clicked, this, &MainWindow::pushButtonUndo_clicked);
  connect(ui->pushButtonOpenFiles, &QPushButton::clicked, this, &MainWindow::pushButtonOpenFiles_clicked);
  connect(ui->comboBoxColorSpace, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentTextChanged), this,
          &MainWindow::comboBoxColorSpace_currentTextChanged);

  QString iniFileName = QString("%1.ini").arg(APPLICATION_NAME);
  QString iniFilePath = QDir::toNativeSeparators(QApplication::applicationDirPath() + "/../ini/" + iniFileName);

  m_settings = new QSettings(iniFilePath, QSettings::IniFormat, this); // INI

  //READ QSettings DATA:
  m_openDirectory = readSettings(m_openDirectory, "openDirectory", SETTINGS_MAIN).toString();
  ui->lineEditOpen->setText(m_openDirectory);

  m_saveDirectory = readSettings(m_saveDirectory, "saveDirectory", SETTINGS_MAIN).toString();

  if(m_saveDirectory.isEmpty())
    m_saveDirectory = m_openDirectory + "/fixed";

  ui->lineEditSave->setText(m_saveDirectory);

  m_colorSpace = readSettings(m_colorSpace, "colorSpace", SETTINGS_MAIN).toString();
  ui->comboBoxColorSpace->setCurrentText(m_colorSpace);
} // MainWindow::MainWindow(...)
// ----------------------------------------------------------------------

MainWindow::~MainWindow()
{
  delete ui;
} // MainWindow::~MainWindow()
// ----------------------------------------------------------------------

const QSettings* MainWindow::Settings() const
{
  const QSettings* const pointer = m_settings;
  return pointer;
} // const QSettings* MainWindow::Settings() const
// ----------------------------------------------------------------------

void MainWindow::pushButtonSave_clicked()
{
  QString startDir = (m_saveDirectory.isEmpty()) ? "/home" : m_saveDirectory;
  m_saveDirectory  = QFileDialog::getExistingDirectory(this, tr("Save Directory"), startDir,
                                                      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  ui->lineEditSave->setText(m_saveDirectory);
} // void MainWindow::pushButtonSave_clicked()
// ----------------------------------------------------------------------

void MainWindow::pushButtonOpen_clicked()
{
  QString startDir = (m_openDirectory.isEmpty()) ? "/home" : m_openDirectory;
  m_openDirectory  = QFileDialog::getExistingDirectory(this, tr("Open Directory"), startDir,
                                                      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  ui->lineEditOpen->setText(m_openDirectory);
  m_saveDirectory = m_openDirectory + "/fixed";
  ui->lineEditSave->setText(m_saveDirectory);
} // void MainWindow::pushButtonOpen_clicked()
// ----------------------------------------------------------------------

void MainWindow::pushButton_FindPng_clicked()
{
  QApplication::processEvents();
  QDir dir(m_openDirectory);
  QString mask("*.png");

  QStringList list = dir.entryList(mask.split(" "), QDir::Files);

  m_model.setStringList(list);
  ui->listViewNames->setModel(&m_model);
  QItemSelectionModel selection(&m_model);
  ui->listViewNames->setSelectionModel(&selection);

  QString count = QString::number(m_model.stringList().count());
  statusBar()->showMessage(tr("In catalog found: ") + count + tr(" .png files"));
} // void MainWindow::pushButton_FindPng_clicked()
// ----------------------------------------------------------------------

void MainWindow::pushButton_FixPng_clicked()
{
  statusBar()->addPermanentWidget(m_progress);
  m_progress->setRange(0, m_model.stringList().count());
  m_progress->setMinimumWidth(200);
  m_progress->setAlignment(Qt::AlignCenter);
  m_progress->setVisible(true);
  qApp->processEvents();

  QDir dir(m_saveDirectory);

  if(!dir.exists())
    dir.mkdir(m_saveDirectory);

  for(int counter = 0; counter < m_model.stringList().count(); ++counter)
  {
    QImage img;
    img.load(m_openDirectory + "/" + m_model.stringList().at(counter));
    img.setColorSpace(QColorSpace::DisplayP3);
    QFile file(m_saveDirectory + "/" + m_model.stringList().at(counter));
    file.open(QIODevice::WriteOnly);
    img.save(&file, "PNG");
    if(counter % 10 == 0)
      m_progress->setValue(counter);
    qApp->processEvents();
  }

  m_progress->setVisible(false);
  statusBar()->showMessage(tr("Conversion completed successfully! "));
} // void MainWindow::pushButton_FixPng_clicked()
// ----------------------------------------------------------------------

void MainWindow::pushButton_SaveSettings_clicked()
{
  writeSettings(m_openDirectory, "openDirectory", SETTINGS_MAIN);
  qDebug() << "openDirectory:  " << m_openDirectory;

  writeSettings(m_saveDirectory, "saveDirectory", SETTINGS_MAIN);
  qDebug() << "saveDirectory:  " << m_saveDirectory;

  writeSettings(m_colorSpace, "colorSpace", SETTINGS_MAIN);
  qDebug() << "colorSpace:  " << m_colorSpace;
} //void MainWindow::pushButton_SaveSettings_clicked()
// ----------------------------------------------------------------------

void MainWindow::writeSettings(const QVariant& value, const QString& name, const QString& group, const QString& subdir)
{
  if(m_settings != nullptr)
  {
    m_settings->beginGroup(group);
    QString subdirectory = "/";
    if(subdir != "")
      subdirectory = QString("/%1/").arg(subdir);
    m_settings->setValue(subdirectory + name, value);
    m_settings->endGroup();
  }
} //void MainWindow::writeSettings(..)
// ----------------------------------------------------------------------

QVariant MainWindow::readSettings(const QVariant& defaultValue, const QString& name, const QString& group, const QString& subdir)
{
  QVariant result {};
  if(m_settings != nullptr)
  {
    m_settings->beginGroup(group);
    QString subdirectory = "/";
    if(subdir != "")
      subdirectory = QString("/%1/").arg(subdir);
    result = m_settings->value(subdirectory + name, defaultValue);
    m_settings->endGroup();
  }
  return result;
} // QVariant MainWindow::readSettings(...)

// ----------------------------------------------------------------------

void MainWindow::setOpenDirectory(const QString& openDirectory)
{
  m_openDirectory = openDirectory;
} // void MainWindow::setOpenDirectory(const QString& openDirectory)
// ----------------------------------------------------------------------

void MainWindow::comboBoxColorSpace_currentTextChanged(const QString& arg1)
{
  m_colorSpace = arg1;
} // void MainWindow::comboBoxColorSpace_currentTextChanged(const QString& arg1)
// ----------------------------------------------------------------------

void MainWindow::pushButtonClose_clicked()
{
  this->close();
} // void MainWindow::pushButtonClose_clicked()
// ----------------------------------------------------------------------

void MainWindow::pushButtonOpenFiles_clicked()
{
  QApplication::processEvents();
  QString startDir = (m_openDirectory.isEmpty()) ? "/home" : m_openDirectory;
  QStringList list = QFileDialog::getOpenFileNames(this, "Select one or more files to open", startDir, "Images (*.png)");

  if(list.isEmpty())
    return;

  QFileInfo tempFile(list.at(0));
  m_openDirectory = tempFile.path();
  m_saveDirectory = m_openDirectory + "/fixed";
  ui->lineEditOpen->setText(m_openDirectory);
  ui->lineEditSave->setText(m_saveDirectory);

  QStringList names;
  for(QString& file : list)
  {
    QFileInfo temp(file);
    names.append(temp.fileName());
  }

  m_model.setStringList(names);
  ui->listViewNames->setModel(&m_model);
  QItemSelectionModel selection(&m_model);
  ui->listViewNames->setSelectionModel(&selection);

  QString count = QString::number(m_model.stringList().count());
  statusBar()->showMessage(tr("Selected : ") + count + tr(" .png files"));
} // void MainWindow::pushButtonOpenFiles_clicked()
// ----------------------------------------------------------------------

void MainWindow::pushButtonUndo_clicked()
{
  // TODO MainWindow::pushButtonUndo_clicked()
}
// ----------------------------------------------------------------------
