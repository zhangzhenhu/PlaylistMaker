#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "depreated/filebrowser.h"
//#include "depreated/_playlistwidget.h"
#include "player/player.h"
#include "player/playlistwidget.h"
#include "filebrowser/filebrowserwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onPlay(const QUrl&url);
private:
    Ui::MainWindow *ui=nullptr;
    FileBrowserWidget * browser=nullptr;
//    PlayListWidget * playlist=nullptr;
    Player *player=nullptr;
    PlaylistWidget * playListView=nullptr;
    QLabel *info = nullptr;

};
#endif // MAINWINDOW_H
