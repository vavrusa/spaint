#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include "canvasmgr.h"
#include "networkservice.h"
#include "networkserver.h"

class MainWindow : public QMainWindow
{
   Q_OBJECT

   public:
   MainWindow(QWidget* parent = 0);
   ~MainWindow();

   bool observe(CanvasMgr* cm);
   bool observe(NetworkService* net);

   protected:
   void saveSettings();
   void loadSettings();
   void closeEvent(QCloseEvent* event);

   private slots:
   void about();
   void renderCanvas();
   void showServerState(NetworkServer::state state, const QString& msg);
   void promptClientConnection();

   private:
   struct Private;
   Private* d;

   QMenuBar* createMenuBar();
};

#endif // MAINWINDOW_H
