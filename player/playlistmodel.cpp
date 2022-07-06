/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "playlistmodel.h"
#include "qmediaplaylist.h"
#include "m3uplaylistwriter.h"
#include <QFileInfo>
#include <QUrl>
#include <QFile>
#include <QMessageBox>

PlaylistModel::PlaylistModel(QObject *parent)
        : QAbstractTableModel(parent) {
    m_playlist.reset(new QMediaPlaylist);

    connect(m_playlist.data(), &QMediaPlaylist::mediaAboutToBeInserted, this, &PlaylistModel::beginInsertItems);
    connect(m_playlist.data(), &QMediaPlaylist::mediaInserted, this, &PlaylistModel::endInsertItems);
    connect(m_playlist.data(), &QMediaPlaylist::mediaAboutToBeRemoved, this, &PlaylistModel::beginRemoveItems);
    connect(m_playlist.data(), &QMediaPlaylist::mediaRemoved, this, &PlaylistModel::endRemoveItems);
    connect(m_playlist.data(), &QMediaPlaylist::mediaChanged, this, &PlaylistModel::changeItems);
    this->setHeaderData(0, Qt::Horizontal, tr("歌曲"), Qt::DisplayRole);
    this->setHeaderData(1, Qt::Horizontal, tr("路径"), Qt::DisplayRole);
//    this->set(1, Qt::Horizontal, tr("路径"),Qt::DisplayRole);
//    this->setHeaderData(2,Qt::Horizontal,tr("年龄"));


}

QVariant PlaylistModel::headerData(int section, Qt::Orientation orientation, int role) const {
//    qDebug() << section << orientation << role;

    if (orientation == Qt::Orientation::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) {
            return tr("歌曲");

        } else if (section == 1) {
            return tr("路径");
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}


PlaylistModel::~PlaylistModel() = default;

int PlaylistModel::rowCount(const QModelIndex &parent) const {
//    return  !parent.isValid() ? (int) m_data.count() : 0;
    return m_playlist && !parent.isValid() ? m_playlist->mediaCount() : 0;
}

int PlaylistModel::columnCount(const QModelIndex &parent) const {

    return !parent.isValid() ? ColumnCount : 0;
}

QModelIndex PlaylistModel::index(int row, int column, const QModelIndex &parent) const {
    return m_playlist && !parent.isValid()
           && row >= 0 && row < m_playlist->mediaCount()
           && column >= 0 && column < ColumnCount
           ? createIndex(row, column)
           : QModelIndex();
}

//QModelIndex PlaylistModel::parent(const QModelIndex &child) const {
//    Q_UNUSED(child);
//
//    return {};
//}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return {};


    if (index.column() == Title && role == Qt::CheckStateRole) {

        return this->getCheckStat(index.row()) == Qt::Checked ? Qt::Checked : Qt::Unchecked;
    };
    if (role != Qt::DisplayRole && role !=Qt::ToolTipRole) {
        return {};
    }

    QUrl location = m_playlist->media(index.row());
//    qDebug()<<" PlaylistModel::data  " << index << location;
    if (index.column() == PlaylistModel::Title) {
        return QFileInfo(location.path()).fileName();
    }
//    qDebug() << " PlaylistModel::data  5" << location.toLocalFile();

    if (index.column() == PlaylistModel::URL) {
//        qDebug() << " PlaylistModel::data  5" << location.toLocalFile();
        return location.path();
    }

//    return value;

    return {};
}

QMediaPlaylist *PlaylistModel::playlist() const {
    return m_playlist.data();
}

//QUrl

bool PlaylistModel::setData(const QModelIndex &index, const QVariant &value, int role) {
//    return true;
    if (!index.isValid())
        return false;

    if (role == Qt::CheckStateRole && index.column() == Title) {

//        m_check_state[index] = (value == Qt::Checked ? Qt::Checked : Qt::Unchecked);
        this->setCheckStat(index, (value == Qt::Checked ? Qt::Checked : Qt::Unchecked));
        return true;
    } else {
//        m_data[index] = value;
    }

//    Q_UNUSED(role);
//    m_data[index] = value;
    emit dataChanged(index, index);
    return true;
}

void PlaylistModel::beginInsertItems(int start, int end) {
//    m_data.clear();
    beginInsertRows(QModelIndex(), start, end);
}

void PlaylistModel::endInsertItems(int start, int end) {

    for (int i = start; i <= end; i++) {
        this->setCheckStat(i, Qt::CheckState::Unchecked);
    }

    endInsertRows();
}

void PlaylistModel::beginRemoveItems(int start, int end) {
//    m_data.clear();
    for (int i = start; i <= end; i++) {
        this->removeCheckStat(i);
//        this->m_check_state.remove(this->index(i, 0));
    }

    beginRemoveRows(QModelIndex(), start, end);
}

void PlaylistModel::endRemoveItems() {
//    endInsertRows();
    QAbstractTableModel::endRemoveRows();
}

void PlaylistModel::changeItems(int start, int end) {
//    m_data.clear();
    emit dataChanged(index(start, 0), index(end, ColumnCount));
}

Qt::ItemFlags PlaylistModel::flags(const QModelIndex &index) const {

    if (index.isValid() && index.column() == Column::Title)
        return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    else
        return QAbstractTableModel::flags(index);
}
//QVariant PlaylistModel::headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) {
//    if (orientation==Qt::Horizontal && role== Qt::DisplayRole){
//        return
//    }
//}

//bool PlaylistModel::insertRows(int row, int count, const QModelIndex &parent){
//    int previes_count = this->m_data.count();
//
//    beginInsertRows(parent,);
//    endInsertRows();
//}
bool PlaylistModel::isSelectedAll() {
    auto iter = m_check_state.begin();
    while (iter != m_check_state.end()) {
        if (iter.value() == Qt::CheckState::Unchecked) {
            return false;
        }
        iter++;
    }
    return true;
}

void PlaylistModel::clearAll() {
    this->m_playlist->clear();
}

void PlaylistModel::removeCheckStat(int row) {
    QUrl url = m_playlist->media(row);
    this->m_check_state.remove(url.path());
}

void PlaylistModel::setCheckStat(const QModelIndex &index, Qt::CheckState stat) {
//    this->setData(index, stat, Qt::CheckStateRole);
    this->setCheckStat(index.row(), stat);
}

void PlaylistModel::setCheckStat(int row, Qt::CheckState stat) {
    QUrl url = m_playlist->media(row);
    this->m_check_state[url.path()] = stat;
    auto index = this->index(row, 0);
    emit dataChanged(index, index);

}

Qt::CheckState PlaylistModel::getCheckStat(int row) const {
    QUrl url = m_playlist->media(row);
    return this->m_check_state[url.path()];
//    this->setData(this->index(row, 0), stat, Qt::CheckStateRole);
}


void PlaylistModel::selectAll() {

    auto stat = this->isSelectedAll() ? Qt::CheckState::Unchecked : Qt::CheckState::Checked;
    for (int i = 0; i < this->m_playlist->mediaCount(); i++) {
        this->setCheckStat(i, stat);

    }

}

void PlaylistModel::clearSelected() {
    auto iter = m_check_state.begin();
    QList<QString> pool;
    while (iter != m_check_state.end()) {
        if (iter.value() == Qt::CheckState::Checked) {
            pool.append(iter.key());
        }
        iter++;
    };
            foreach(auto row, pool) {
            this->m_playlist->removeMedia(row);
        }

}

static bool isPlaylist(const QUrl &url) // Check for ".m3u" playlists.
{
    if (!url.isLocalFile())
        return false;
    const QFileInfo fileInfo(url.toLocalFile());
    return fileInfo.exists() && !fileInfo.suffix().compare(QLatin1String("m3u"), Qt::CaseInsensitive);
}

void PlaylistModel::addMedia(const QList<QUrl> &urls) {

    const int previousMediaCount = m_playlist->mediaCount();
    for (auto &url: urls) {
        if (isPlaylist(url)) {

            m_playlist->load(url);
        } else {

            m_playlist->addMedia(url);
        }

    }
    if (m_playlist->mediaCount() > previousMediaCount) {
        auto index = this->index(previousMediaCount, 0);
//        qDebug()<<"addToPlaylist  3"  ;
//        if (m_playlistView)
//            m_playlistView->setCurrentIndex(index);

//        jump(index);
    }
//    qDebug()<<"addToPlaylist  end" ;
}

void PlaylistModel::addMedia(const QUrl &url) {

    const int previousMediaCount = m_playlist->mediaCount();
    if (isPlaylist(url)) {

        m_playlist->load(url);
    } else {

        m_playlist->addMedia(url);
    }


//    if (m_playlist->mediaCount() > previousMediaCount) {
//        auto index = this->index(previousMediaCount, 0);
//
//    }

}

QUrl PlaylistModel::getMedia(const QModelIndex &index) {
    return this->m_playlist->media(index.row());
}

void PlaylistModel::sort(int column, Qt::SortOrder order) {

    this->m_playlist->sort(column, order);
}

static QUrl removePrefix(const QUrl &url, const QString &prefix) {

    if (prefix.isEmpty()) {
        return url;
    }
    auto url_s = url.toString();
    if (url_s.startsWith(prefix, Qt::CaseInsensitive)) {
        url_s = url_s.mid(prefix.length());
    }
//    url_s = url_s.remove(prefix, Qt::CaseInsensitive);
    if (url_s.startsWith('/')) {
        url_s = url_s.mid(1);
    }
    return url_s;
}

void PlaylistModel::saveAll(const QString &location, const QString &prefix) {
    QList<QUrl> urls;

    for (int i = 0; i < this->m_playlist->mediaCount(); i++) {
        auto url = this->m_playlist->media(i);
        urls.append(removePrefix(url, prefix));
    };
    this->save(location, urls);
}

void PlaylistModel::saveCheckedUrls(const QString &location, const QString &prefix) {
    QList<QUrl> urls;

            foreach(auto url, this->getCheckedUrls()) {
            urls.append(removePrefix(url, prefix));
        }

    this->save(location, urls);

}

bool PlaylistModel::save(const QString &location, QList<QUrl> &urls) {

//    auto file_path = location;
//    if (!location.endsWith(".m3u") && !location.endsWith(".m3u8")) {
//        location = location + ".m3u8";
//    }
//    qDebug() << "PlaylistModel::save " << file_path << urls.count();
    QFile file(location);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//        d->error = AccessDeniedError;
//        d->errorString = tr("The file could not be accessed.");
        emit this->error(tr("文件无法打开.") + location);

//        qDebug() << "PlaylistModel::save " << "打开错误";

        return false;
    }

//    QIODevice()
    M3uPlaylistWriter writer(&file);
    for (const auto &entry: urls) {
        writer.writeItem(entry.toString());
//        qDebug()<<entry.toString().toUtf8();
//        file.write(entry.toString().toUtf8());
//        file.write("\n");
//        file.flush();
    }
//        writer.writeItem(entry);
    file.close();
    return true;
}

void PlaylistModel::readFile(const QUrl &url) {
//    if (!url.isLocalFile())
//        return;
//    const QFileInfo fileInfo(url.toLocalFile());
//    return fileInfo.exists() && !fileInfo.suffix().compare(QLatin1String("m3u"), Qt::CaseInsensitive);

    this->m_playlist->load(url);
}

void PlaylistModel::deleteSelected() {
    auto urls = this->getCheckedUrls();

            foreach(auto url, urls) {
            QFile::moveToTrash(url.path());
            this->m_playlist->removeMedia(url);
        }
}

void PlaylistModel::deleteMedia(int row) {
    auto url = this->m_playlist->media(row);
    QFile::moveToTrash(url.path());
    this->m_playlist->removeMedia(url);

}

QList<QUrl> PlaylistModel::getCheckedUrls() {
    auto iter = m_check_state.begin();
    QList<QUrl> urls;

    while (iter != m_check_state.end()) {
        if (iter.value() == Qt::CheckState::Checked) {
            urls.append(iter.key());


        }
        iter++;
    }
    return urls;
}

QList<QUrl> PlaylistModel::getAllUrls() {
    QList<QUrl> urls;
    for (int i = 0; i < this->m_playlist->mediaCount(); ++i) {
        urls.append(this->m_playlist->media(i));
    }
    return urls;
}

void PlaylistModel::removeMedia(int row) {
    this->m_playlist->removeMedia(row);
}