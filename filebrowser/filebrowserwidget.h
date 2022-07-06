//
// Created by 张振虎 on 2022/6/5.
//

#ifndef TIGERMUSIC_FILEBROWSERWIDGET_H
#define TIGERMUSIC_FILEBROWSERWIDGET_H

#include "../player/playlistwidget.h"
#include <QPushButton>

class FileBrowserWidget : public PlaylistWidget{
Q_OBJECT
public:
    explicit FileBrowserWidget(QWidget *parent = nullptr);

    ~FileBrowserWidget() = default;
    void initWidget() override ;
public slots:

    void onExportChecked();

    void onExportAll();
    void  persist();
    void load();

    void onActionAdd();

signals:

    void exportUrls(const QList<QUrl> &);
    void playUrl(const QUrl &);


protected:
    void buildConnections() override;


    QPushButton * btn_open= nullptr;
    QPushButton * btn_export_selected= nullptr;
    QPushButton * btn_export_all= nullptr;


};


#endif //TIGERMUSIC_FILEBROWSERWIDGET_H
