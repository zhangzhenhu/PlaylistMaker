//
// Created by 张振虎 on 2022/5/31.
//

#include "playlistwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QUrl>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>

PlaylistWidget::PlaylistWidget(QWidget *parent)
        : QWidget(parent) {
    m_playlistView = new QTableView(this);

    m_playlistModel = new PlaylistModel(this);

    m_playlistView->setModel(m_playlistModel);

    m_playlistView->resizeColumnToContents(0);
    m_playlistView->setColumnWidth(0, 200);
    m_playlistView->horizontalHeader()->setStretchLastSection(true);
    m_playlistView->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选中模式为选中行
    m_playlistView->setSelectionMode(QAbstractItemView::SingleSelection);//设置选中单个
    m_playlistView->setSortingEnabled(true); // 允许排序




//    m_playlistView->setRowHeight()
//    this->initWidget();
}

void PlaylistWidget::initWidget() {
//    qDebug() << "PlaylistWidget::initWidget() ";


    auto v_layout = new QVBoxLayout(this);
    auto h1 = new QHBoxLayout();

    auto h2 = new QHBoxLayout();

//    btn_play_all = new QPushButton("播放全部");
    btn_select_all = new QPushButton("全选/反选");
    btn_clear_selected = new QPushButton("清除选中");
    btn_clear_all = new QPushButton("清除全部");
    btn_clear_selected->setToolTip(tr("把歌曲从列表中移除"));
    btn_clear_all->setToolTip(tr("把歌曲从列表中移除"));

    btn_delete_selected = new QPushButton("删除选中文件");
    btn_delete_selected->setToolTip(tr("把歌曲文件从电脑上删除到回收站"));

    btn_save_selected = new QPushButton("导出选中");

    btn_save_all = new QPushButton("导出全部");
    btn_save_all->setToolTip(tr("保存全部歌曲到播放列表中"));

    btn_load_file = new QPushButton("读取播放列表");
    btn_load_file->setToolTip(tr("读取已经存在的播放列表文件(.m3u,.m3u8)"));


    input_prefix = new QLineEdit(this);
    input_prefix->setPlaceholderText("需要去除的路径前缀");
    input_prefix->setToolTip("");


    h2->addWidget(btn_select_all);
    h2->addWidget(btn_clear_selected);
    h2->addWidget(btn_clear_all);
    h2->addWidget(btn_delete_selected);

    h1->addWidget(input_prefix);
    h1->addWidget(btn_save_selected);
    h1->addWidget(btn_save_all);
    h1->addWidget(btn_load_file);

    v_layout->addLayout(h1);
    v_layout->addLayout(h2);
    v_layout->addWidget(m_playlistView);

    // 右键菜单
    m_playlistView->setContextMenuPolicy(Qt::CustomContextMenu);//必须设置
    m_right_menu = new QMenu(m_playlistView);
    auto action_remove = new QAction("清除此歌曲", m_right_menu);
    auto action_delete = new QAction("从电脑删除此歌曲文件", m_right_menu);

//    auto action_add = new QAction("添加到右侧播放列表",tableview);
    action_delete->setVisible(true);
    action_remove->setVisible(true);
    m_right_menu->addAction(action_remove);
    m_right_menu->addAction(action_delete);
    connect(action_remove, SIGNAL(triggered()), this, SLOT(onActionRemove()));
    connect(action_delete, SIGNAL(triggered()), this, SLOT(onActionDelete()));


    this->buildConnections();

}

void PlaylistWidget::buildConnections() {
    connect(this->btn_select_all, &QPushButton::clicked, this, &PlaylistWidget::selectAll);
    connect(this->btn_clear_selected, &QPushButton::clicked, this, &PlaylistWidget::clearSelected);
    connect(this->btn_clear_all, &QPushButton::clicked, this, &PlaylistWidget::clearAll);
//    connect(this->btn_clear_s,&QPushButton::clicked,this,&PlaylistWidget::clearAll);
    connect(this->m_playlistView, &QTableView::doubleClicked, this, &PlaylistWidget::onDoubleClicked);
    connect(this->btn_save_all, &QPushButton::clicked, this, &PlaylistWidget::onSaveAllClicked);
    connect(this->btn_save_selected, &QPushButton::clicked, this, &PlaylistWidget::onSaveSelectedClicked);
    connect(this->btn_delete_selected, &QPushButton::clicked, this, &PlaylistWidget::onDeletedSelectedClicked);
    connect(this->btn_load_file, &QPushButton::clicked, this, &PlaylistWidget::onLoadFile);

    // 右键菜单

    connect(m_playlistView, &QTableView::customContextMenuRequested, this, &PlaylistWidget::onMenu);


}

QTableView *PlaylistWidget::playlistView() {
    return m_playlistView;
}

PlaylistModel *PlaylistWidget::playlistModel() {
    return this->m_playlistModel;
}

void PlaylistWidget::clearAll() {
    this->m_playlistModel->clearAll();
}

void PlaylistWidget::selectAll() {
    this->m_playlistModel->selectAll();
}

void PlaylistWidget::clearSelected() {
    this->m_playlistModel->clearSelected();
}


void PlaylistWidget::addMedia(const QList<QUrl> &urls) {
    this->m_playlistModel->addMedia(urls);

}

void PlaylistWidget::play(const QModelIndex &index) {
//    this.
}

void PlaylistWidget::play(const QUrl &url) {
}

void PlaylistWidget::onDoubleClicked(const QModelIndex &index) {

    auto url = this->m_playlistModel->getMedia(index);
//    qDebug()<<"PlaylistWidget::onDoubleClicked " << index << url;
    emit this->doubleClicked(url);

}

void PlaylistWidget::onSaveAllClicked() {
    QString filePath = QFileDialog::getSaveFileName(
            this, QString::fromLocal8Bit("保存文件"),
            "",
            tr("播放列表文件(*.m3u *.m3u8);;"));


    QString prefix = this->input_prefix->text();
    this->m_playlistModel->saveAll(filePath, prefix);

}

void PlaylistWidget::onSaveSelectedClicked() {
    QString filePath = QFileDialog::getSaveFileName(
            this, QString::fromLocal8Bit("保存文件"),
            "",
            tr("播放列表文件(*.m3u *.m3u8);;"));

    QString prefix = this->input_prefix->text();
    this->m_playlistModel->saveCheckedUrls(filePath, prefix);


}

void PlaylistWidget::onDeletedSelectedClicked() {
    auto result = QMessageBox::warning(this,
                                       tr("文件删除警告"),
                                       tr("此操作会把歌曲文件从电脑上删除，你确定吗？"),
                                       QMessageBox::Ok | QMessageBox::Cancel,
                                       QMessageBox::Cancel);
    if (result != QMessageBox::Ok) {
        return;
    }
    this->m_playlistModel->deleteSelected();
//    qDebug() << "PlaylistWidget::onDeletedSelectedClicked" ;

}

void PlaylistWidget::addDir(const QString &path) {
    QDir dir(path);
    if (!dir.exists())
        return;
//    QDir dir(directory);
    //设置文件过滤器
    QStringList nameFilters;
    //设置文件过滤格式
    nameFilters << "*.mp3";
    nameFilters << "*.wav";
    nameFilters << "*.flac";
    nameFilters << "*.ape";
    dir.setNameFilters(nameFilters);
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);    //过滤后只留下文件夹、文件以及隐藏文件，不包括当前文件夹和上级文件夹
    dir.setSorting(QDir::DirsFirst);    //优先排列文件夹

            foreach(QFileInfo info, dir.entryInfoList()) {
            if (info.isFile()) {
                this->m_playlistModel->addMedia(info.absoluteFilePath());

            } else {
                //do what you wanna do
                addDir(info.filePath());
            }
        }
}

void PlaylistWidget::onOpenFiles() {

    QString directory = QFileDialog::getExistingDirectory(NULL, "选取文件夹", ".");  // 起始路径

    //    for file_path, name in self.get_files(directory):
    //        self.add_row(file_path, file_name=name)
//    QDir dir(directory);
//    //设置文件过滤器
//    QStringList nameFilters;
//    //设置文件过滤格式
//    nameFilters << "*.mp3";
//    nameFilters << "*.wav";
//    nameFilters << "*.flac";
//    nameFilters << "*.ape";
//
//    //将过滤后的文件名称存入到files列表中
//    QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
//    QString file;
//            foreach(file, files) {
//            this->add(file);
//        }
    this->addDir(directory);
}

/*
 * 读取播放列表文件 (.m3u,.m3u8)
 */
void PlaylistWidget::onLoadFile() {
//    QString runPath = QCoreApplication::applicationDirPath();//获取项目的根路径
//    QString file_name = QFileDialog::getOpenFileName(this,QStringLiteral("选择文件"),runPath,"Text Files(*.txt)",nullptr,QFileDialog::DontResolveSymlinks);
    auto urls = QFileDialog::getOpenFileUrls(this,
                                             "选取文件夹",
                                             QUrl(),// 起始路径
                                             "播放列表(*.m3u *.m3u8)",
                                             nullptr,
                                             QFileDialog::DontResolveSymlinks
    );
            foreach(auto url, urls) {
            this->m_playlistModel->readFile(url);
        }

}

/*
 * 右键菜单
 */
void PlaylistWidget::onMenu(const QPoint &p) {
    QModelIndex index = m_playlistView->indexAt(p);//获取鼠标点击位置项的索引
//    qDebug() << "PlaylistWidget::onMenu(const QPoint &p)" <<index.isValid()<< index;
    if (index.isValid())//数据项是否有效，空白处点击无菜单
    {
//        QItemSelectionModel *selections = m_playlistView->selectionModel();//获取当前的选择模型
//        auto selected = selections->selectedIndexes();//返回当前选择的模型索引
//        if(selected.count() ==1) //选择单个项目时的右键菜单显示Action1
//        {
//            Action1->setVisible(true);
//        }
//        else   //如果选中多个项目，则右键菜单显示Action2
//        {
//            Action2->setVisible(true);
//        }
        m_right_menu->exec(QCursor::pos());//数据项有效才显示菜单
    }
}

/*
 * 右键菜单-清除
 */
void PlaylistWidget::onActionRemove() {
    QModelIndexList indexes = m_playlistView->selectionModel()->selectedIndexes();
    if (indexes.count() == 0) {
        //select nothing
        return;
    }
            foreach(auto index, indexes) {
            this->m_playlistModel->removeMedia(index.row());
        }

}

/*
 * 右键菜单-删除文件
 */
void PlaylistWidget::onActionDelete() {
    QModelIndexList indexes = m_playlistView->selectionModel()->selectedIndexes();
    if (indexes.count() == 0) {
        //select nothing
        return;
    }
    auto result = QMessageBox::warning(this,
                                       tr("文件删除警告"),
                                       tr("此操作会把歌曲文件从电脑上删除，你确定吗？"),
                                       QMessageBox::Ok | QMessageBox::Cancel,
                                       QMessageBox::Cancel);
    if (result != QMessageBox::Ok) {
        return;
    }
            foreach(auto index, indexes) {
            this->m_playlistModel->deleteMedia(index.row());
        }
}