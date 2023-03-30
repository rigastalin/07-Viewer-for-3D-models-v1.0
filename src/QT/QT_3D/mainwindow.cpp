#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifdef __cplusplus
extern "C" {
#include "../../afins.h"
#include "../../parse.h"
}
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->labelQuote->setText("«Так не бывает в природе: овцы целы и волки сыты. "
        "Если у одного на счастье подкова, значит, другой отбросил копыта.»");
    ui->labelWho->setText(" - ВОЛК");

    settings = new QSettings("QT", "3D_Viewer", this);
    connect(ui->buttonSettingsSava, SIGNAL(clicked()), this, SLOT(saveSettings()));
    loadSettings();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::saveSettings() {
    settings->setValue("color_back", *ui->openGLWidget->color_back);
    settings->setValue("color_face", *ui->openGLWidget->color_face);
    settings->setValue("color_ver", *ui->openGLWidget->color_ver);
    settings->setValue("flagParallel", ui->openGLWidget->flagParallel);
    settings->setValue("flagDot", ui->openGLWidget->flagDot);
    settings->setValue("f_ver", ui->openGLWidget->f_ver);
    settings->setValue("f_sphere", ui->openGLWidget->f_sphere);
    settings->setValue("index", ui->openGLWidget->index);
    settings->setValue("lineWidth", ui->openGLWidget->geometries->lineWidth);
    settings->setValue("size_of_ver", ui->openGLWidget->geometries->size_of_ver);
    settings->setValue("filepath", ui->openGLWidget->geometries->filepath);
    settings->setValue("vCount", ui->openGLWidget->geometries->vCount);
    settings->setValue("fCount", ui->openGLWidget->geometries->fCount);
}

void MainWindow::loadSettings() {
    ui->openGLWidget->loadflag = 1;
    colorBack = new QColor();
    *colorBack = settings->value("color_back").toByteArray().constData();
    ui->openGLWidget->color_back = colorBack;
    lineColor = new QColor();
    *lineColor = settings->value("color_face").toByteArray().constData();
    verColor = new QColor();
    ui->openGLWidget->color_face = lineColor;
    *verColor = settings->value("color_ver").toByteArray().constData();
    ui->openGLWidget->color_ver = verColor;
    int f = settings->value("flagParallel").toInt();
    ui->openGLWidget->flagParallel = f;
    ui->comboBox_proection->setCurrentIndex(f);
    f = settings->value("flagDot").toInt();
    ui->openGLWidget->flagDot = f;
    ui->comboBox_strip->setCurrentIndex(f);
    f = settings->value("f_ver").toInt();
    ui->openGLWidget->f_ver = f;
    f = settings->value("f_sphere").toInt();
    ui->openGLWidget->f_sphere = f;
    f = settings->value("index").toInt();
    ui->comboBox_vertex->setCurrentIndex(f);
    f = settings->value("lineWidth").toInt();
    ui->openGLWidget->lineWidth = f;
    ui->spinLineChange->setValue(f);
    f = settings->value("size_of_ver").toInt();
    ui->openGLWidget->size_of_ver = f;
    ui->spinVertexChange->setValue(f);

    file = settings->value("filepath").toByteArray().constData();
    ui->openGLWidget->filepath = file;
    QList<QString> name = file.split('/');
    ui->labelFileName->setText("ИМЯ ФАЙЛА: " + name.last());

    vCount1 = settings->value("vCount").toByteArray().constData();
    ui->labelVertexName->setText("ВЕРШИНЫ: " + vCount1);
    fCount1 = settings->value("fCount").toByteArray().constData();
    ui->labelFacetsName->setText("РЕБРА: " + fCount1);
}

void MainWindow::deletePush() {
    if (flag == 1) {
        start->deleteLater();
        flag = 0;
    }
    if (flag1 == 1) {
        butJPG->deleteLater();
        butBMP->deleteLater();
        flag1 = 0;
    }
}

// <------------ INFO: NAME, VERTEX, FACETS ---------------->
void MainWindow::on_buttonChoseFile_clicked() {
    deletePush();
    ui->openGLWidget->loadflag = 0;
    int size = 0;
    QList<QString> f = ui->labelFileName->text().split(" ");
    QString s = f.last();
    ui->labelFileName->setText("ИМЯ ФАЙЛА: ");
    filename = QFileDialog::getOpenFileName(this, "/", "*.obj");
    QList<QString> name;
    name = filename.split('/');
    if (!name.last().isEmpty()) {
        s = name.last();
    } else {
        size = 1;
    }

    ui->labelFileName->setText("ИМЯ ФАЙЛА: " + s);

    QFile file1(filename);
    if (!file1.exists()) {
        return;
    } else {
        ui->openGLWidget->geometries->filepath = filename;
        if (size != 1) {
            ui->openGLWidget->geometries->parsingObj();
            ui->openGLWidget->geometries->drawing(&ui->openGLWidget->m_program);

            ui->labelVertexName->setText("ВЕРШИНЫ: " + ui->openGLWidget->geometries->vCount);
            ui->labelFacetsName->setText("РЕБРА: " + ui->openGLWidget->geometries->fCount);

            ui->spinXMove->setValue(0.0);
            ui->spinYMove->setValue(0.0);
            ui->spinZMove->setValue(0.0);

            ui->spinXRotation->setValue(0.0);
            ui->spinYRotation->setValue(0.0);
            ui->spinZRotation->setValue(0.0);

            ui->sliderScale->setValue(10);
            ui->spinScale->setValue(0.1);

            ui->sliderXMove->setValue(0);
            ui->sliderYMove->setValue(0);
            ui->sliderZMove->setValue(0);

            ui->sliderXRotation->setValue(0);
            ui->sliderYRotation->setValue(0);
            ui->sliderZRotation->setValue(0);

            on_buttonScale_clicked();
        }
    }
    
}

// <---------------- SCALING ------------------->
void MainWindow::on_buttonScale_clicked() {
    deletePush();
    double val = (double)ui->spinScale->value();
    ui->openGLWidget->geometries->scaling(val);
    ui->openGLWidget->update();
}

void MainWindow::on_sliderScale_valueChanged(int value) {
    deletePush();
    float valueScale = (float)value / 10;
    ui->spinScale->setValue(valueScale);
    ui->openGLWidget->geometries->scaling(valueScale);
    ui->openGLWidget->update();
}

// <---------------- MOVING ------------------->
void MainWindow::on_buttonMove_clicked() {
    deletePush();
    double valX = (double)ui->spinXMove->value();
    double valY = (double)ui->spinYMove->value();
    double valZ = (double)ui->spinZMove->value();
    ui->openGLWidget->geometries->movingX(valX);
    ui->openGLWidget->geometries->movingY(valY);
    ui->openGLWidget->geometries->movingZ(valZ);
    ui->openGLWidget->updating();
}

void MainWindow::on_sliderXMove_valueChanged(int value) {
    deletePush();
    float moveXScale = (float)value / 100;
    ui->spinXMove->setValue(moveXScale);
    ui->openGLWidget->geometries->movingX(moveXScale);
    ui->openGLWidget->updating();
}

void MainWindow::on_sliderYMove_valueChanged(int value) {
    deletePush();
    float moveYScale = (float)value / 100;
    ui->spinYMove->setValue(moveYScale);
    ui->openGLWidget->geometries->movingY(moveYScale);
    ui->openGLWidget->updating();
}

void MainWindow::on_sliderZMove_valueChanged(int value) {
    deletePush();
    float moveZScale = (float)value / 100;
    ui->spinZMove->setValue(moveZScale);
    ui->openGLWidget->geometries->movingZ(moveZScale);
    ui->openGLWidget->updating();
}

// <---------------- ROTATION ------------------->
void MainWindow::on_buttonRotate_clicked() {
    deletePush();
    double valX = (double)ui->spinXRotation->value();
    double valY = (double)ui->spinYRotation->value();
    double valZ = (double)ui->spinZRotation->value();
    ui->openGLWidget->geometries->rotatingX(valX);
    ui->openGLWidget->geometries->rotatingY(valY);
    ui->openGLWidget->geometries->rotatingZ(valZ);
    ui->openGLWidget->updating();
}

void MainWindow::on_sliderXRotation_valueChanged(int value) {
    deletePush();
    float rotationXScale = (float)value / 100;
    ui->spinXRotation->setValue(rotationXScale);
    ui->openGLWidget->geometries->rotatingX(rotationXScale);
    ui->openGLWidget->update();
}

void MainWindow::on_sliderYRotation_valueChanged(int value) {
    deletePush();
    float rotationYScale = (float)value / 100;
    ui->spinYRotation->setValue(rotationYScale);
    ui->openGLWidget->geometries->rotatingY(rotationYScale);
    ui->openGLWidget->update();
}

void MainWindow::on_sliderZRotation_valueChanged(int value) {
    deletePush();
    float rotationZScale = (float)value / 100;
    ui->spinZRotation->setValue(rotationZScale);
    ui->openGLWidget->geometries->rotatingZ(rotationZScale);
    ui->openGLWidget->update();
}


// <---------------- COLOR CHANGE ------------------->
void MainWindow::on_buttonColorChangeFace_clicked() {
    deletePush();
    QColor newColor = QColorDialog::getColor();
        if (newColor.isValid()) {
            *lineColor = newColor;
            *ui->openGLWidget->color_face = newColor;
            ui->openGLWidget->updating();
        }
}


void MainWindow::on_buttonColorChangeVertex_clicked() {
    deletePush();
    QColor newColor = QColorDialog::getColor(*ui->openGLWidget->color_ver);
    if (newColor.isValid()) {
        *ui->openGLWidget->color_ver = newColor;
        ui->openGLWidget->update();
    }
}

void MainWindow::on_buttonColorChangeBack_clicked() {
    deletePush();
    QColor newColor = QColorDialog::getColor(*ui->openGLWidget->color_back);
    if (newColor.isValid()) {
        *ui->openGLWidget->color_back = newColor;
        ui->openGLWidget->update();
    }
}

// <---------------- OUR HOUSE ------------------->
void MainWindow::on_buttonOurHouse_clicked() {
    deletePush();
    QString link  = "https://21-school.ru/";
    QDesktopServices::openUrl(QUrl(link));
}


// <---------------- STYLE CHANGE ------------------->
void MainWindow::on_buttonVertexChange_clicked() {
    deletePush();
    ui->openGLWidget->geometries->size_of_ver = ui->spinVertexChange->value();
    ui->openGLWidget->updating();
}

void MainWindow::on_comboBox_vertex_currentIndexChanged(int index) {
    deletePush();
    ui->openGLWidget->index = index;
    if (index == 2) {
        ui->openGLWidget->f_ver = 1;
        ui->openGLWidget->f_sphere = 0;
    } else {
        ui->openGLWidget->f_ver = index;
        ui->openGLWidget->f_sphere = index;
    }
}

void MainWindow::on_comboBox_strip_currentIndexChanged(int index) {
    deletePush();
    ui->openGLWidget->flagDot = index;
}

void MainWindow::on_buttonLineChange_clicked() {
    deletePush();
    ui->openGLWidget->geometries->lineWidth = ui->spinLineChange->value();
    ui->openGLWidget->updating();
}

void MainWindow::on_comboBox_proection_currentIndexChanged(int index) {
    deletePush();
    ui->openGLWidget->flagParallel = index;
    ui->openGLWidget->resizeGL(4, 3);
    ui->openGLWidget->updating();
}

void MainWindow::on_buttonScreenshot_clicked() {
    if (flag == 1) {
        start->deleteLater();
        flag = 0;
    }
    if (flag1 == 0) {
        butBMP =  new QPushButton(".BMP", this);
        QRect geobutBMP;
        geobutBMP.setX(770);
        geobutBMP.setY(624);
        geobutBMP.setWidth(95);
        geobutBMP.setHeight(130);
        butBMP->setStyleSheet("QPushButton {font: bold; font-size: 20px;"
              "background-color: #5767CD; color: white; border: 1px solid #F3D712}"
                              "QPushButton:pressed {background-color: #7489C3}");
        butBMP->setGeometry(geobutBMP);
        butBMP->show();

        butJPG =  new QPushButton(".JPG", this);
        QRect geobutJPG;
        geobutJPG.setX(860);
        geobutJPG.setY(624);
        geobutJPG.setWidth(93);
        geobutJPG.setHeight(130);
        butJPG->setGeometry(geobutJPG);
        butJPG->setStyleSheet("QPushButton {font: bold; font-size: 20px;"
              "background-color: #5767CD; color: white; ""border: 1px solid #F3D712}"
                              "QPushButton:pressed {background-color: #7489C3}");
        butJPG->show();
        flag1 = 1;
    }
    connect(butBMP, SIGNAL(clicked()), this, SLOT(buttonBMP()));
    connect(butJPG, SIGNAL(clicked()), this, SLOT(buttonJPG()));
}

void MainWindow::buttonBMP() {
    presentTime = time.currentTime().toString();
    presentDate = date.currentDate().toString();
    QString screenFile = QFileDialog::getExistingDirectory();
    ui->openGLWidget->grabFramebuffer().save(screenFile + "/" + presentDate +
                                             presentTime + ".BMP", NULL, 100);
    if (flag1 == 1) {
        butBMP->deleteLater();
        butJPG->deleteLater();
        flag1 = 0;
    }
}

void MainWindow::buttonJPG() {
    presentTime = time.currentTime().toString();
    presentDate = date.currentDate().toString();
    QString screenFile = QFileDialog::getExistingDirectory();
    ui->openGLWidget->grabFramebuffer().save(screenFile + "/" + presentDate +
                                             presentTime + ".JPG", NULL, 100);
    if (flag1 == 1) {
        butBMP->deleteLater();
        butJPG->deleteLater();
        flag1 = 0;
    }
}

void MainWindow::on_buttonGif_clicked() {
    if (flag1 == 1) {
        butJPG->deleteLater();
        butBMP->deleteLater();
        flag1 = 0;
    }
    if (flag == 0) {
        start =  new QPushButton("RECORDING", this);
        QRect geobutStart;
        geobutStart.setX(150);
        geobutStart.setY(60);
        geobutStart.setWidth(480);
        geobutStart.setHeight(40);
        start->setStyleSheet("QPushButton {font: bold; font-size: 20px;"
                  "background-color: rgba(255,255,255,100); color: blue; }"
                        "QPushButton:pressed {background-color: white}");
        start->setGeometry(geobutStart);
        start->show();
        flag = 1;
    }
    connect(start, SIGNAL (clicked()),this, SLOT (startGif()));
}

void MainWindow::startGif() {
    start->deleteLater();
    gif = new QGifImage;
    img = new QImage;
    if (flag == 1) {
        stop = new QPushButton("STOP", this);
        QRect geobutStop;
        geobutStop.setX(150);
        geobutStop.setY(60);
        geobutStop.setWidth(480);
        geobutStop.setHeight(40);
        stop->setStyleSheet("QPushButton { font: bold; font-size: 20px;"
                    "background-color: rgba(255,255,255,100); color: blue; }"
                         "QPushButton:pressed {background-color: white}");
        stop->setGeometry(geobutStop);
        stop->show();
        timer = new QTimer(this);
        flag = 0;
        connect(timer, SIGNAL(timeout()), this, SLOT(recordingGif()));
        timer->start(1000);
        connect(stop, SIGNAL (clicked()),this, SLOT (saveGif()));
    }
}

void MainWindow::recordingGif() {
    *img = ui->openGLWidget->grabFramebuffer();
    gif->addFrame(*img);
}

void MainWindow::saveGif() {
    stop->deleteLater();
    timer->stop();
    presentTime = time.currentTime().toString();
    presentDate = date.currentDate().toString();
    QString gifFile = QFileDialog::getExistingDirectory();
    gif->save(gifFile + "/" + presentDate + " " + presentTime + ".GIF");
}

void MainWindow::on_originSettings_clicked() {
    deletePush();
    ui->openGLWidget->loadflag = 0;
    ui->openGLWidget->color_ver = new QColor(12, 26, 114, 0);
    ui->openGLWidget->color_back = new QColor(247, 224, 57, 0);
    ui->openGLWidget->color_face = new QColor(65, 83, 197, 255);
    ui->openGLWidget->geometries->filepath = "";
    ui->openGLWidget->f_ver = 0;
    ui->openGLWidget->geometries->f_ver = 0;
    ui->openGLWidget->f_sphere = 0;
    ui->openGLWidget->geometries->f_sphere = 0;
    ui->openGLWidget->index = 0;
    ui->openGLWidget->flagParallel = 0;
    ui->openGLWidget->geometries->flagParallel = 0;
    ui->openGLWidget->flagDot = 0;
    ui->openGLWidget->geometries->flagDot = 0;
    ui->openGLWidget->geometries->lineWidth = 1;
    ui->openGLWidget->geometries->size_of_ver = 1;
    ui->openGLWidget->geometries->vertexCount = 0;
    ui->openGLWidget->geometries->facetsCount = 0;
    ui->openGLWidget->geometries->vCount = "";
    ui->openGLWidget->geometries->fCount = "";

    ui->spinXMove->setValue(0.0);
    ui->spinYMove->setValue(0.0);
    ui->spinZMove->setValue(0.0);

    ui->spinXRotation->setValue(0.0);
    ui->spinYRotation->setValue(0.0);
    ui->spinZRotation->setValue(0.0);

    ui->sliderXMove->setValue(0);
    ui->sliderYMove->setValue(0);
    ui->sliderZMove->setValue(0);

    ui->sliderXRotation->setValue(0);
    ui->sliderYRotation->setValue(0);
    ui->sliderZRotation->setValue(0);

    ui->sliderScale->setValue(10);
    ui->spinScale->setValue(0.1);

    ui->openGLWidget->updating();
    saveSettings();
    loadSettings();
}
