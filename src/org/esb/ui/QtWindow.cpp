/* 
 * File:   QtWindow.cpp
 * Author: HoelscJ
 *
 * Created on 6. Dezember 2010, 18:22
 */

#include <cstdlib>
#include <QtGui/QtGui>

#include <QtGui/QApplication>
#include <QtGui/QTextEdit>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  QApplication app(argc, argv);
  QTextEdit textEdit;
  textEdit.show();
  return app.exec();
  return 0;
}

