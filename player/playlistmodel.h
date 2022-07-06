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

#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractItemModel>
#include <QScopedPointer>
#include <QUrl>

QT_BEGIN_NAMESPACE
class QMediaPlaylist;
QT_END_NAMESPACE

class PlaylistModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column
    {
        Title = 0,
        URL = 1,
        ColumnCount
    };

    explicit PlaylistModel(QObject *parent = nullptr);
    ~PlaylistModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent) const override;

    [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex &parent ={}) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role ) override;
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;

    [[nodiscard]] QMediaPlaylist *playlist() const;
    bool isSelectedAll();
    Qt::CheckState getCheckStat(int row) const;
    QUrl getMedia(const QModelIndex & index);
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder)  override;
    QList<QUrl> getCheckedUrls();
    QList<QUrl> getAllUrls();
    void deleteMedia(int row);
    void removeMedia(int row);

public slots:
    void clearAll();
    void selectAll();
    void clearSelected();
    void setCheckStat(const QModelIndex &index,Qt::CheckState);
    void setCheckStat(int row,Qt::CheckState);
//    void selected(const QModelIndex &index);
//    void selected( int row);

    void removeCheckStat(int row);
    void addMedia(const QList<QUrl> &urls);
    void addMedia(const QUrl &urls);
    void saveAll(const QString &location,const QString &prefix="");
    void saveCheckedUrls(const QString &location,const QString &prefix="");
    void readFile(const QUrl &file);
    bool save(const QString &location,QList<QUrl> &urls);
    void deleteSelected();
//    bool save(QIODevice *device,QList<QUrl> &urls);
signals:
    void error(QString msg);

private slots:
//    bool insertRows(int row, int count, const QModelIndex &parent) override;
    void beginInsertItems(int start, int end);
    void endInsertItems(int start, int end);
    void beginRemoveItems(int start, int end);
    void endRemoveItems();
    void changeItems(int start, int end);

private:
    QScopedPointer<QMediaPlaylist> m_playlist;
//    QMap<QModelIndex, QVariant> m_data;
    QMap<QString, Qt::CheckState> m_check_state;
};

#endif // PLAYLISTMODEL_H
