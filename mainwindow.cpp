#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QSplitter"
#include "QVBoxLayout"
#include "filebrowser/filebrowserwidget.h"


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->statusBar()->showMessage("ready");

    auto central = new QWidget(this);
    this->setCentralWidget(central);
    auto central_layout = new QVBoxLayout(central);

    this->setWindowTitle("简易播放器");
    this->setGeometry(300, 300, 600, 600);


    this->browser = new FileBrowserWidget(this);
    this->browser->initWidget();
//    this->playListView = new PlayListWidget(this);

    this->player = new Player(this);
    this->playListView = this->player->playListView();
    this->browser->load();

    connect(this->browser, &FileBrowserWidget::doubleClicked, this->player, &Player::play);
    connect(this->browser, &FileBrowserWidget::exportUrls, this->player, &Player::addToPlaylist);


    this->player->setFixedHeight(100);

//    this->info = new QLabel(this);

//    auto *left = new QVBoxLayout(this);
//    QVBoxLayout *right = new QVBoxLayout(this);

    auto *splitter1 = new QSplitter(Qt::Orientation::Horizontal);
//    QVBoxLayout *layout = new QVBoxLayout(central);

    splitter1->addWidget(this->browser);
    splitter1->addWidget(this->playListView);

    auto h_layout = new QHBoxLayout();
//    h_layout->addWidget(this->info);
    h_layout->addWidget(this->player);

    central_layout->addWidget(splitter1);
    central_layout->addLayout(h_layout);


    connect(this->player, &Player::playing, this, &MainWindow::onPlay);
//    connect(this->player, &Player::playing, this, &MainWindow::onPlay);


}

MainWindow::~MainWindow() {
    this->browser->persist();
    delete ui;
}

void MainWindow::onPlay(const QUrl &url) {
    this->statusBar()->showMessage(url.path());
}