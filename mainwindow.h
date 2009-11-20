#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include "canvasmgr.h"

class MainWindow : public QMainWindow
{
   Q_OBJECT

   public:
   MainWindow(QWidget* parent = 0);
   ~MainWindow();

   bool observe(CanvasMgr* cm);

   public slots:
   void init();

   signals:
   void initialized();

   protected:
   void saveSettings();
   void loadSettings();
   void closeEvent(QCloseEvent *event);

   private:
   struct Private;
   Private* d;

   QMenuBar* createMenuBar();
};

#endif // MAINWINDOW_H
