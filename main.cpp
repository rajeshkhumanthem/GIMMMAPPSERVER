#include <QCoreApplication>

#include "myfcmapplication.h"
int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  MyFcmApplication app;

  return a.exec();
}
