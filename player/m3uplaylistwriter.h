//
// Created by 张振虎 on 2022/6/4.
//

#ifndef TIGERMUSIC_M3UPLAYLISTWRITER_H
#define TIGERMUSIC_M3UPLAYLISTWRITER_H

#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QUrl>

class M3uPlaylistWriter {
public:
    explicit M3uPlaylistWriter(QIODevice *device);

    ~M3uPlaylistWriter();

    bool writeItem( const QString &item);

private:
    QIODevice *m_device;
    QTextStream *m_textStream;
};


#endif //TIGERMUSIC_M3UPLAYLISTWRITER_H
