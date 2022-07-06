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

#include "player.h"

#include "playercontrols.h"
#include "playlistmodel.h"
#include "qmediaplaylist.h"
//#include "videowidget.h"

#include <QMediaMetaData>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QAudioOutput>
#include <QMediaFormat>
#include <QtWidgets>

Player::Player(QWidget *parent)
    : QWidget(parent)
{
//! [create-objs]
    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);
//! [create-objs]


//    m_playlistModel = new PlaylistModel(this);
//    m_playlist = m_playlistModel->playlist();
//! [2]
//    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, this, &Player::playlistPositionChanged);//**

    // player layout
    QBoxLayout *layout = new QVBoxLayout(this);

    // display
//    QBoxLayout *displayLayout = new QHBoxLayout;
//    displayLayout->addWidget(m_videoWidget, 2);
//#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    m_playlistWidget = new PlaylistWidget(this);
    m_playlistWidget->initWidget();

    m_playlistView = m_playlistWidget->playlistView();
    m_playlistModel = m_playlistWidget->playlistModel();
    m_playlist = m_playlistModel->playlist();

//    m_playlistView->setModel(m_playlistModel); //dfdsf
//    m_playlistView->setCurrentIndex(m_playlistModel->index(m_playlist->currentIndex(), 0));//dfdf
    connect(m_playlistView, &QAbstractItemView::activated, this, &Player::jump);

    // duration slider and label
    QHBoxLayout *hLayout = new QHBoxLayout;

    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setRange(0, m_player->duration());
    connect(m_slider, &QSlider::sliderMoved, this, &Player::seek);
    hLayout->addWidget(m_slider);

    m_labelDuration = new QLabel();
    m_labelDuration->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    hLayout->addWidget(m_labelDuration);
    layout->addLayout(hLayout);

    // controls
    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setContentsMargins(0, 0, 0, 0);

//    QPushButton *openButton = new QPushButton(tr("Open"), this);
//    connect(openButton, &QPushButton::clicked, this, &Player::open);
//    controlLayout->addWidget(openButton);
//    controlLayout->addStretch(1);

    m_controls = new PlayerControls();
    m_controls->setState(m_player->playbackState());
    m_controls->setVolume(m_audioOutput->volume());
    m_controls->setMuted(m_controls->isMuted());
    m_controls->setVolume(30);


//    connect(m_audioOutput, &QAudioOutput::mutedChanged, controls, &PlayerControls::setMuted);

    controlLayout->addWidget(m_controls);
    controlLayout->addStretch(1);



    layout->addLayout(controlLayout);

#if defined(Q_OS_QNX)
    // On QNX, the main window doesn't have a title bar (or any other decorations).
    // Create a status bar for the status information instead.
    m_statusLabel = new QLabel;
    m_statusBar = new QStatusBar;
    m_statusBar->addPermanentWidget(m_statusLabel);
    m_statusBar->setSizeGripEnabled(false); // Without mouse grabbing, it doesn't work very well.
    layout->addWidget(m_statusBar);
#endif

    setLayout(layout);

    if (!isPlayerAvailable()) {
        QMessageBox::warning(this, tr("Service not available"),
                             tr("The QMediaPlayer object does not have a valid service.\n"\
                                "Please check the media service plugins are installed."));

        m_controls->setEnabled(false);
        if (m_playlistView)
            m_playlistView->setEnabled(false);
//        openButton->setEnabled(false);
//        m_fullScreenButton->setEnabled(false);
    }

//    metaDataChanged();

    this->buildConnections();
}

void Player::buildConnections(){
    connect(m_player, &QMediaPlayer::durationChanged, this, &Player::durationChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &Player::positionChanged);
//    connect(m_player, QOverload<>::of(&QMediaPlayer::metaDataChanged), this, &Player::metaDataChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &Player::statusChanged);
    connect(m_player, &QMediaPlayer::bufferProgressChanged, this, &Player::bufferingProgress);
    connect(m_player, &QMediaPlayer::hasVideoChanged, this, &Player::videoAvailableChanged);
    connect(m_player, &QMediaPlayer::errorChanged, this, &Player::displayErrorMessage);
    connect(m_player, &QMediaPlayer::tracksChanged, this, &Player::tracksChanged);

    connect(m_controls, &PlayerControls::play, m_player, &QMediaPlayer::play);
    connect(m_controls, &PlayerControls::pause, m_player, &QMediaPlayer::pause);
    connect(m_controls, &PlayerControls::stop, m_player, &QMediaPlayer::stop);

    connect(m_controls, &PlayerControls::next, m_playlist, &QMediaPlaylist::next);
    connect(m_controls, &PlayerControls::previous, this, &Player::previousClicked);
    connect(m_controls, &PlayerControls::changeVolume, m_audioOutput, &QAudioOutput::setVolume);
    connect(m_controls, &PlayerControls::changeMuting, m_audioOutput, &QAudioOutput::setMuted);

    connect(m_controls, &PlayerControls::changeRate, m_player, &QMediaPlayer::setPlaybackRate);
//    connect(controls, &PlayerControls::stop, m_videoWidget, QOverload<>::of(&QVideoWidget::update));

    connect(m_player, &QMediaPlayer::playbackStateChanged, m_controls, &PlayerControls::setState);
    connect(m_audioOutput, &QAudioOutput::volumeChanged, m_controls, &PlayerControls::setVolume);

    // 双击播放列表中的歌曲 进行播放
    connect(this->m_playlistWidget,&PlaylistWidget::doubleClicked,this,&Player::play);

}

bool Player::isPlayerAvailable() const
{
    return m_player->isAvailable();
}

void Player::open()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Files"));
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted)
        addToPlaylist(fileDialog.selectedUrls());
}

static bool isPlaylist(const QUrl &url) // Check for ".m3u" playlists.
{
    if (!url.isLocalFile())
        return false;
    const QFileInfo fileInfo(url.toLocalFile());
    return fileInfo.exists() && !fileInfo.suffix().compare(QLatin1String("m3u"), Qt::CaseInsensitive);
}

void Player::addToPlaylist(const QList<QUrl> &urls)
{
    this->m_playlistWidget->addMedia(urls);
}
//void Player::addToPlaylist(const QList<QUrl> &urls)
//{
//    QList<QUrl> medias;
//    foreach(auto u,urls){
//            medias.append(QUrl(u));
//    }
//    this->m_playlistWidget->addMedia(medias);
//}


void Player::durationChanged(qint64 duration)
{
    m_duration = duration / 1000;
    m_slider->setMaximum(duration);
}

void Player::positionChanged(qint64 progress)
{
    if (!m_slider->isSliderDown())
        m_slider->setValue(progress);

    updateDurationInfo(progress / 1000);
}

//void Player::metaDataChanged()
//{
//    auto metaData = m_player->metaData();
//    setTrackInfo(QString("%1 - %2")
//            .arg(metaData.value(QMediaMetaData::AlbumArtist).toString())
//            .arg(metaData.value(QMediaMetaData::Title).toString()));

//#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
//    for (int i = 0; i < QMediaMetaData::NumMetaData; i++) {
//        if (QLineEdit* field = qobject_cast<QLineEdit*>(m_metaDataFields[i]))
//            field->clear();
//        else if (QLabel* label = qobject_cast<QLabel*>(m_metaDataFields[i]))
//            label->clear();
//        m_metaDataFields[i]->setDisabled(true);
//        m_metaDataLabels[i]->setDisabled(true);
//    }

//    for (auto &key : metaData.keys()) {
//        int i = int(key);
//        if (key == QMediaMetaData::CoverArtImage) {
//            QVariant v = metaData.value(key);
//            if (QLabel *cover = qobject_cast<QLabel*>(m_metaDataFields[key])) {
//                QImage coverImage = v.value<QImage>();
//                cover->setPixmap(QPixmap::fromImage(coverImage));
//            }
//        } else if (key == QMediaMetaData::ThumbnailImage) {
//            QVariant v = metaData.value(key);
//            if (QLabel *thumbnail = qobject_cast<QLabel*>(m_metaDataFields[key])) {
//                QImage thumbnailImage = v.value<QImage>();
//                thumbnail->setPixmap(QPixmap::fromImage(thumbnailImage));
//            }
//        } else if (QLineEdit *field = qobject_cast<QLineEdit*>(m_metaDataFields[key])) {
//            QString stringValue = metaData.stringValue(key);
//            field->setText(stringValue);
//        }
//        m_metaDataFields[i]->setDisabled(false);
//        m_metaDataLabels[i]->setDisabled(false);
//    }
//#endif
//}

//QString Player::trackName(const QMediaMetaData &metaData, int index)
//{
//    QString name;
//    QString title = metaData.stringValue(QMediaMetaData::Title);
//    QLocale::Language lang = metaData.value(QMediaMetaData::Language).value<QLocale::Language>();

//    if (title.isEmpty()) {
//        if (lang == QLocale::Language::AnyLanguage)
//            name = tr("Track %1").arg(index+1);
//        else
//            name = QLocale::languageToString(lang);
//    } else {
//        if (lang == QLocale::Language::AnyLanguage)
//            name = title;
//        else
//            name = QString("%1 - [%2]").arg(title).arg(QLocale::languageToString(lang));
//    }
//    return name;
//}

void Player::tracksChanged()
{
//    m_audioTracks->clear();
//    m_videoTracks->clear();
//    m_subtitleTracks->clear();

//    const auto audioTracks = m_player->audioTracks();
//    m_audioTracks->addItem(QString::fromUtf8("No audio"), -1);
//    for (int i = 0; i < audioTracks.size(); ++i)
//        m_audioTracks->addItem(trackName(audioTracks.at(i), i), i);
//    m_audioTracks->setCurrentIndex(m_player->activeAudioTrack() + 1);

//    const auto videoTracks = m_player->videoTracks();
//    m_videoTracks->addItem(QString::fromUtf8("No video"), -1);
//    for (int i = 0; i < videoTracks.size(); ++i)
//        m_videoTracks->addItem(trackName(videoTracks.at(i), i), i);
//    m_videoTracks->setCurrentIndex(m_player->activeVideoTrack() + 1);

//    m_subtitleTracks->addItem(QString::fromUtf8("No subtitles"), -1);
//    const auto subtitleTracks = m_player->subtitleTracks();
//    for (int i = 0; i < subtitleTracks.size(); ++i)
//        m_subtitleTracks->addItem(trackName(subtitleTracks.at(i), i), i);
//    m_subtitleTracks->setCurrentIndex(m_player->activeSubtitleTrack() + 1);
}

void Player::previousClicked()
{
    // Go to previous track if we are within the first 5 seconds of playback
    // Otherwise, seek to the beginning.
    if (m_player->position() <= 5000) {
        m_playlist->previous();
    } else {
        m_player->setPosition(0);
    }
}

void Player::jump(const QModelIndex &index)
{
    if (index.isValid()) {
        m_playlist->setCurrentIndex(index.row());
    }
}

void Player::playlistPositionChanged(int currentItem)
{
    if (m_playlistView)
        m_playlistView->setCurrentIndex(m_playlistModel->index(currentItem, 0));
    m_player->setSource(m_playlist->currentMedia());
}

void Player::seek(int mseconds)
{
    m_player->setPosition(mseconds);
}

void Player::statusChanged(QMediaPlayer::MediaStatus status)
{
    handleCursor(status);

    // handle status message
    switch (status) {
    case QMediaPlayer::NoMedia:
    case QMediaPlayer::LoadedMedia:
        setStatusInfo(QString());
        break;
    case QMediaPlayer::LoadingMedia:
        setStatusInfo(tr("Loading..."));
        break;
    case QMediaPlayer::BufferingMedia:
    case QMediaPlayer::BufferedMedia:
        setStatusInfo(tr("Buffering %1%").arg(qRound(m_player->bufferProgress()*100.)));
        break;
    case QMediaPlayer::StalledMedia:
        setStatusInfo(tr("Stalled %1%").arg(qRound(m_player->bufferProgress()*100.)));
        break;
    case QMediaPlayer::EndOfMedia:
        QApplication::alert(this);
        m_playlist->next();
        break;
    case QMediaPlayer::InvalidMedia:
        displayErrorMessage();
        break;
    }
}

void Player::handleCursor(QMediaPlayer::MediaStatus status)
{
#ifndef QT_NO_CURSOR
    if (status == QMediaPlayer::LoadingMedia ||
        status == QMediaPlayer::BufferingMedia ||
        status == QMediaPlayer::StalledMedia)
        setCursor(QCursor(Qt::BusyCursor));
    else
        unsetCursor();
#endif
}

void Player::bufferingProgress(float progress)
{
    if (m_player->mediaStatus() == QMediaPlayer::StalledMedia)
        setStatusInfo(tr("Stalled %1%").arg(qRound(progress*100.)));
    else
        setStatusInfo(tr("Buffering %1%").arg(qRound(progress*100.)));
}

void Player::videoAvailableChanged(bool available)
{
//    if (!available) {
//        disconnect(m_fullScreenButton, &QPushButton::clicked, m_videoWidget, &QVideoWidget::setFullScreen);
//        disconnect(m_videoWidget, &QVideoWidget::fullScreenChanged, m_fullScreenButton, &QPushButton::setChecked);
//        m_videoWidget->setFullScreen(false);
//    } else {
//        connect(m_fullScreenButton, &QPushButton::clicked, m_videoWidget, &QVideoWidget::setFullScreen);
//        connect(m_videoWidget, &QVideoWidget::fullScreenChanged, m_fullScreenButton, &QPushButton::setChecked);

//        if (m_fullScreenButton->isChecked())
//            m_videoWidget->setFullScreen(true);
//    }
}

//void Player::selectAudioStream()
//{
//    int stream = m_audioTracks->currentData().toInt();
//    m_player->setActiveAudioTrack(stream);
//}

//void Player::selectVideoStream()
//{
//    int stream = m_videoTracks->currentData().toInt();
//    m_player->setActiveVideoTrack(stream);
//}

//void Player::selectSubtitleStream()
//{
//    int stream = m_subtitleTracks->currentData().toInt();
//    m_player->setActiveSubtitleTrack(stream);
//}

//void Player::setTrackInfo(const QString &info)
//{
//    m_trackInfo = info;

//    if (m_statusBar) {
//        m_statusBar->showMessage(m_trackInfo);
//        m_statusLabel->setText(m_statusInfo);
//    } else {
//        if (!m_statusInfo.isEmpty())
//            setWindowTitle(QString("%1 | %2").arg(m_trackInfo).arg(m_statusInfo));
//        else
//            setWindowTitle(m_trackInfo);
//    }
//}

void Player::setStatusInfo(const QString &info)
{
    m_statusInfo = info;

    if (m_statusBar) {
//        m_statusBar->showMessage(m_trackInfo);
        m_statusLabel->setText(m_statusInfo);
    } else {
//        if (!m_statusInfo.isEmpty())
//            setWindowTitle(QString("%1 | %2").arg(m_trackInfo).arg(m_statusInfo));
//        else
//            setWindowTitle(m_trackInfo);
    }
}

void Player::displayErrorMessage()
{
    if (m_player->error() == QMediaPlayer::NoError)
        return;
    setStatusInfo(m_player->errorString());
}

void Player::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || m_duration) {
        QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60,
            currentInfo % 60, (currentInfo * 1000) % 1000);
        QTime totalTime((m_duration / 3600) % 60, (m_duration / 60) % 60,
            m_duration % 60, (m_duration * 1000) % 1000);
        QString format = "mm:ss";
        if (m_duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    m_labelDuration->setText(tStr);
}

void Player::audioOutputChanged(int index)
{
//    auto device = m_audioOutputCombo->itemData(index).value<QAudioDevice>();
//    m_player->audioOutput()->setDevice(device);
}
PlaylistWidget *Player::playListView(){
    return this->m_playlistWidget;
}

void Player::play(const QUrl &url) {
//    qDebug()<<url.toLocalFile();
//    qDebug()<<"Player::play " << url;
    this->m_player->setSource(url);
    this->m_player->play();
    emit this->playing(url);
}
