//
// Created by 张振虎 on 2022/5/31.
//

#ifndef TIGERMUSIC_PLAYLISTWIDGET_H
#define TIGERMUSIC_PLAYLISTWIDGET_H

#include <QWidget>
#include <QListView>
#include <QPushButton>
#include <QLineEdit>
#include <QTableView>
#include <QUrl>
#include "playlistmodel.h"

QT_BEGIN_NAMESPACE
class QAbstractItemView;

class QLabel;

class QMediaPlayer;

class QModelIndex;

class QPushButton;

class QComboBox;

class QSlider;

class QStatusBar;

class QVideoWidget;

class QListView;

QT_END_NAMESPACE

class PlaylistWidget : public QWidget {
Q_OBJECT

public:
    explicit PlaylistWidget(QWidget *parent = nullptr);

    ~PlaylistWidget() = default;

    QTableView *playlistView();

    PlaylistModel *playlistModel();

public slots:

    void clearAll();

    void selectAll();

    void clearSelected();

    void addMedia(const QList<QUrl> &urls);

    void onDoubleClicked(const QModelIndex &index);

    void play(const QModelIndex &index);

    void play(const QUrl &url);

    void onSaveAllClicked();
    void onSaveSelectedClicked();
    void onDeletedSelectedClicked();
    void onLoadFile();
    void addDir(const QString & path);
    void onOpenFiles();
    virtual void initWidget();
    void onMenu(const QPoint &);
    void onActionRemove();
    void onActionDelete();
signals:

    void doubleClicked(const QUrl &url);

protected:

    virtual void buildConnections();


//    QListView *m_playlistView = nullptr;
    QTableView *m_playlistView = nullptr;
    QPushButton *btn_play_all = nullptr;
    QPushButton *btn_clear_selected = nullptr;
    QPushButton *btn_clear_all = nullptr;
    QPushButton *btn_delete_selected = nullptr;

    QPushButton *btn_select_all = nullptr;

    QPushButton *btn_save_all = nullptr;
    QPushButton *btn_save_selected = nullptr;
    QPushButton *btn_load_file = nullptr;


    QLineEdit *input_prefix = nullptr;
    PlaylistModel *m_playlistModel = nullptr;
    QMenu *m_right_menu = nullptr;

};


#endif //TIGERMUSIC_PLAYLISTWIDGET_H
