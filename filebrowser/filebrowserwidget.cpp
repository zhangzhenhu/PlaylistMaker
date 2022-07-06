//
// Created by 张振虎 on 2022/6/5.
//

#include "filebrowserwidget.h"
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>

FileBrowserWidget::FileBrowserWidget(QWidget *parent)
        : PlaylistWidget(parent) {

}

void FileBrowserWidget::initWidget() {
//    qDebug() << "FileBrowserWidget::initWidget() ";


    auto v_layout = new QVBoxLayout(this);
    auto h1 = new QHBoxLayout();

    auto h2 = new QHBoxLayout();

//    btn_play_all = new QPushButton("播放全部");
    this->btn_open = new QPushButton(tr("浏览文件"));
    this->btn_select_all = new QPushButton("全选/反选");
    btn_clear_selected = new QPushButton("清除选中");
    btn_clear_all = new QPushButton("清除全部");
    btn_clear_selected->setToolTip(tr("把歌曲从列表中移除"));
    btn_clear_all->setToolTip(tr("把歌曲从列表中移除"));

    btn_delete_selected = new QPushButton("删除选中文件");
    btn_delete_selected->setToolTip(tr("把歌曲文件从电脑上删除到回收站"));

    this->btn_export_all = new QPushButton(tr("导入全部"), this);
    this->btn_export_selected = new QPushButton(tr("导入选中"), this);
//    btn_save_selected = new QPushButton("导出选中");
//    btn_save_all = new QPushButton("导出全部");
//    input_prefix = new QLineEdit(this);
//    input_prefix->setPlaceholderText("需要去除的路径前缀");
//    input_prefix->setToolTip("");

//    btn_save_all->setToolTip(tr("保存全部歌曲到播放列表中"));

    h1->addWidget(btn_open);
    h1->addWidget(btn_export_selected);
    h1->addWidget(btn_export_all);


    h2->addWidget(btn_select_all);
    h2->addWidget(btn_clear_selected);
    h2->addWidget(btn_clear_all);
    h2->addWidget(btn_delete_selected);


    v_layout->addLayout(h1);
    v_layout->addLayout(h2);
    v_layout->addWidget(m_playlistView);


    // 右键菜单
    m_playlistView->setContextMenuPolicy(Qt::CustomContextMenu);//必须设置
    m_right_menu = new QMenu(m_playlistView);

    auto action_add = new QAction("添加到右侧播放列表", m_right_menu);
    auto action_remove = new QAction("清除此歌曲", m_right_menu);
    auto action_delete = new QAction("从电脑删除此歌曲文件", m_right_menu);

    action_add->setVisible(true);
    action_delete->setVisible(true);
    action_remove->setVisible(true);
    m_right_menu->addAction(action_add);
    m_right_menu->addAction(action_remove);
    m_right_menu->addAction(action_delete);
    connect(action_add, SIGNAL(triggered()), this, SLOT(onActionAdd()));
    connect(action_remove, SIGNAL(triggered()), this, SLOT(onActionRemove()));
    connect(action_delete, SIGNAL(triggered()), this, SLOT(onActionDelete()));


    this->buildConnections();

}

void FileBrowserWidget::buildConnections() {
    connect(this->btn_select_all, &QPushButton::clicked, this, &PlaylistWidget::selectAll);
    connect(this->btn_clear_selected, &QPushButton::clicked, this, &PlaylistWidget::clearSelected);
    connect(this->btn_clear_all, &QPushButton::clicked, this, &PlaylistWidget::clearAll);
//    connect(this->btn_clear_s,&QPushButton::clicked,this,&PlaylistWidget::clearAll);
    connect(this->m_playlistView, &QTableView::doubleClicked, this, &PlaylistWidget::onDoubleClicked);
    connect(this->btn_delete_selected, &QPushButton::clicked, this, &PlaylistWidget::onDeletedSelectedClicked);
    connect(this->btn_open, &QPushButton::clicked, this, &PlaylistWidget::onOpenFiles);

    connect(this->btn_export_selected, &QPushButton::clicked, this, &FileBrowserWidget::onExportChecked);
    connect(this->btn_export_all, &QPushButton::clicked, this, &FileBrowserWidget::onExportAll);

    // 右键菜单
    connect(m_playlistView, &QTableView::customContextMenuRequested, this, &PlaylistWidget::onMenu);

}


void FileBrowserWidget::onExportChecked() {
    emit this->exportUrls(this->m_playlistModel->getCheckedUrls());
}

void FileBrowserWidget::onExportAll() {
//    QList<QUrl> urls;
//            foreach(auto u,urls){
//            medias.append(QUrl(u));
//        }

    emit this->exportUrls(this->m_playlistModel->getAllUrls());

}

void FileBrowserWidget::persist() {
    int i = 0;
//    this->m_playlistModel


//    QString file;
//    foreach(file,files){
//        this->add(file);
//    }
}

void FileBrowserWidget::load() {
//    QFile hw = QFile("./filebrowser.txt");
//
//    if (!hw.exists() || !hw.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        return;
//    }
//    QTextStream in(&hw);
//    while (!in.atEnd()) {
//        QString line = in.readLine();      //整行读取
////        qDebug() << line;
//        this->add(line.trimmed());
//    }

}

void FileBrowserWidget::onActionAdd() {
    QModelIndexList indexes = m_playlistView->selectionModel()->selectedIndexes();
    if (indexes.count() == 0) {
        //select nothing
        return;
    }
    QList<QUrl> urls;
            foreach(auto index, indexes) {
            urls.append(this->m_playlistModel->getMedia(index));
        }

    emit this->exportUrls(urls);

}
