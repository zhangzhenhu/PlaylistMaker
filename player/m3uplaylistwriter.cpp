//
// Created by 张振虎 on 2022/6/4.
//

#include "m3uplaylistwriter.h"
#include <QDebug>

M3uPlaylistWriter::M3uPlaylistWriter(QIODevice *device)
        : m_device(device), m_textStream(new QTextStream(m_device)) {

    *m_textStream << "#EXTM3U8" << "\r\n";
//    *m_textStream << "#EXT-X-VERSION:3" << Qt::endl;
};

M3uPlaylistWriter::~M3uPlaylistWriter() {
    delete m_textStream;
};

bool M3uPlaylistWriter::writeItem(const QString &item) {
//    qDebug() << "M3uPlaylistWriter::writeItem " << item;

    *m_textStream << "#EXTINF:" << "\r\n";
    *m_textStream << item.toUtf8() << "\r\n";
    return true;
}

