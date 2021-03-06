#include <QDesktopWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QSettings>

#include "ui_template_window.h"
#include "../headers/furniture.hpp"
#include "../headers/template_window.hpp"
#include "../headers/room.hpp"

TemplateWindow::TemplateWindow(QWidget *parent,
                               QList<QGraphicsItem*> roomList)
    : CenteredWindow(parent), ui(new Ui::TemplateWindow), m_roomList(roomList)
{
    ui->setupUi(this);

    setWindowCenter(1.25, 1.25);
    setWindowTitle("Home Planner 2D");

    /* Always start with the first catalog tab opened */
    ui->toolBox->setCurrentIndex(0);

    m_roomArea = 0;

    /* Creates and initializes the scene, then rooms */
    drawGraphicsScene();
    drawRooms();
}

TemplateWindow::~TemplateWindow() {
    delete ui;
}

void TemplateWindow::drawGraphicsScene()
{
    scene = new QGraphicsScene(this);
    /* screenWidth and screenHeight are inherited from CenteredWindow */
    scene->setSceneRect(0,0, screenWidth/1.5, screenHeight/1.5);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

    /* Initial 'zoom' */
    ui->graphicsView->scale(1.5, 1.5);
}

void TemplateWindow::drawRooms()
{
    /* If we chose 'Use Default Template', this list will be empty */
    if (m_roomList.isEmpty()) {
        setDefaultApartmentScheme();
    }

    /* Now this list contains either user-created rooms or default rooms.
     * First we need to remove select, focus and move flags. */
    for (auto room : m_roomList) {

        /* room is of type QGraphicsItem*, need cast to Room */
         Room *itemRoom = qgraphicsitem_cast<Room*>(room);
        /* Disable all flags (selection, focus and moving) */
         auto currentFlags = itemRoom->flags();
         itemRoom->setFlags(currentFlags & (~currentFlags));
         ui->graphicsView->scene()->addItem(itemRoom);

         /* While we are here, we can calculate room area */
         m_roomArea += itemRoom->getArea();
    }
    /* Draw doors on top of rooms */
    for (auto door : m_doorList) {
        ui->graphicsView->scene()->addItem(door);
    }
}

void TemplateWindow::setDefaultApartmentScheme()
{
    /* Rooms are added to m_roomList because they will be drawn later.
     * Reason for that -> they would be drawn over the doors. */

    /* Living room with kitchen:  6.5 x 4  */
    Room *living = new Room(6.5*33, 4*33, ":/img/furniture/floor/floor_light_3.jpg");
    living->setPos(225, 150);
    m_roomList.append(living);

    /* Hallway:  3.5 x 1  */
    Room *hallway = new Room(3.5*33, 1*33, ":/img/furniture/floor/tiles_light_grey.jpeg");
    hallway->setPos(440, 249);
    m_roomList.append(hallway);

    /* Bedroom 1:  3.5 x 3  */
    Room *bedroom1 = new Room(3.5*33, 3*33, ":/img/furniture/floor/floor_light_3.jpg");
    bedroom1->setPos(523, 150);
    m_roomList.append(bedroom1);

    /* Bedroom 2:  2.5 x 4  */
    Room *bedroom2 = new Room(2.5*33, 4*33, ":/img/furniture/floor/floor_light_3.jpg");
    bedroom2->setPos(556, 249);
    m_roomList.append(bedroom2);

    /* Bathroom: 2.5 x 3 */
    Room *bathroom = new Room(2.5*33, 3*33, ":/img/furniture/floor/tiles_white_1.jpg");
    bathroom->setPos(440, 150);
    m_roomList.append(bathroom);

    /* ADDING DOORS */
    /* Because we have put all default rooms in m_roomList, they will be drawn
     * right after this function finishes. Problem is the doors are added to the
     * scene before rooms and they will be drawn under them. Because of that,
     * we created a m_doorsList and put these door (Furniture) objects in that list
     * to be drawn later, after rooms. */

    /* Enter door */
    Furniture *d0 = new Furniture(":/img/furniture/doors/doors_5.png", 20, 30);
    d0->setPos(470, 259); d0->rotate(-90);
    m_doorList.append(d0);

    /* Hallway-living room door */
    Furniture *d1 = new Furniture(":/img/furniture/doors/doors_5.png", 20, 30);
    d1->setPos(421.5, 251); d1->rotate(180);
    m_doorList.append(d1);

    /* Hallway-bathroom door */
    Furniture *d2 = new Furniture(":/img/furniture/doors/doors_6.png", 20, 30);
    d2->setPos(447, 226.3); d2->rotate(-90);
    m_doorList.append(d2);

    /* Hallway-bedroom1 door */
    Furniture *d3 = new Furniture(":/img/furniture/doors/doors_3.png", 20, 30);
    d3->setPos(529, 226.3); d3->rotate(-90);
    m_doorList.append(d3);

    /* Hallway-bedroom2 door */
    Furniture *d4 = new Furniture(":/img/furniture/doors/doors_3.png", 20, 30);
    d4->setPos(553.5, 251);
    m_doorList.append(d4);
}


/* Furniture manipulation */
void TemplateWindow::on_btnMoveLeft_clicked()
{
    QList<QGraphicsItem*> selectedItems = ui->graphicsView->scene()->selectedItems();

    if (selectedItems.isEmpty())
        return;
    else {
        for (auto item : selectedItems) {
            Furniture *itemFurniture = qgraphicsitem_cast<Furniture*>(item);
            itemFurniture->move(-5, 0);
        }
    }
}

void TemplateWindow::on_btnMoveRight_clicked()
{
    QList<QGraphicsItem*> selectedItems = ui->graphicsView->scene()->selectedItems();
    if (selectedItems.isEmpty())
        return;
    else {
        for (auto item : selectedItems) {
            Furniture *itemFurniture = qgraphicsitem_cast<Furniture*>(item);
            itemFurniture->move(5, 0);
        }
    }
}

void TemplateWindow::on_btnRotateRight_clicked()
{
    QList<QGraphicsItem*> selectedItems = ui->graphicsView->scene()->selectedItems();
    if (selectedItems.isEmpty())
        return;
    else {
        for (auto item : selectedItems) {
            Furniture *itemFurniture = qgraphicsitem_cast<Furniture*>(item);
            itemFurniture->rotate(5);
        }
    }
}

void TemplateWindow::on_btnRotateLeft_clicked()
{
    QList<QGraphicsItem*> selectedItems = ui->graphicsView->scene()->selectedItems();
    if (selectedItems.isEmpty())
        return;
    else {
        for (auto item : selectedItems) {
            Furniture *itemFurniture = qgraphicsitem_cast<Furniture*>(item);
            itemFurniture->rotate(-5);
        }
    }
}

void TemplateWindow::on_btnFlip_clicked()
{
    QList<QGraphicsItem*> selectedItems = ui->graphicsView->scene()->selectedItems();
    if (selectedItems.isEmpty())
        return;
    else {
        for (auto item : selectedItems) {
            Furniture *itemFurniture = qgraphicsitem_cast<Furniture*>(item);
            itemFurniture->swapFlipped();
            itemFurniture->update();    // Immediately call paint() which redraws the furniture
        }
    } // if-else
}

void TemplateWindow::on_btnRotate90Right_clicked()
{
    QList<QGraphicsItem*> selectedItems = ui->graphicsView->scene()->selectedItems();
    if (selectedItems.isEmpty())
        return;
    else {
        for (auto item : selectedItems) {
            Furniture *itemFurniture = qgraphicsitem_cast<Furniture*>(item);
            itemFurniture->rotate(90);
        }
    }
}

void TemplateWindow::on_btnRotate90Left_clicked()
{
    QList<QGraphicsItem*> selectedItems = ui->graphicsView->scene()->selectedItems();
    if (selectedItems.isEmpty())
        return;
    else {
        for (auto item : selectedItems) {
            Furniture *itemFurniture = qgraphicsitem_cast<Furniture*>(item);
            itemFurniture->rotate(-90);
        }
    }
}

void TemplateWindow::on_btnDeleteItem_clicked()
{
    QList<QGraphicsItem*> selectedItems = ui->graphicsView->scene()->selectedItems();
    if (selectedItems.isEmpty())
        return;
    else {
        for (auto item : selectedItems) {
            Furniture *itemFurniture = qgraphicsitem_cast<Furniture*>(item);
            delete itemFurniture;
        }
    }
}

/* Scene manipulation */

void TemplateWindow::on_btnRotateSceneRight_clicked() {
    ui->graphicsView->rotate(5);
}

void TemplateWindow::on_btnRotateSceneLeft_clicked() {
    ui->graphicsView->rotate(-5);
}

void TemplateWindow::on_btnCenterScene_clicked() {
    ui->graphicsView->centerOn(screenWidth/3, screenHeight/3);
}

void TemplateWindow::on_btnZoomIn_clicked() {
    ui->graphicsView->scale(1.1, 1.1);
}

void TemplateWindow::on_btnZoomOut_clicked() {
    ui->graphicsView->scale(0.9, 0.9);
}


void TemplateWindow::keyPressEvent(QKeyEvent *event)
{
    /* CONTROLS:
     * +   zooms in  the scene
     * -   zooms out the scene
     * C   centers the scene
     * Z   rotates the scene left
     * X   rotates the scene right
     */

    /* Rotation and translation are "bigger" if shift is pressed */
    bool shiftPressed = event->modifiers() & Qt::ShiftModifier;

    switch ( event->key() )
    {
        /* Zooming in/out */
        case Qt::Key_Plus:
            ui->btnZoomIn->click();
            break;
        case Qt::Key_Minus:
            ui->btnZoomOut->click();
            break;

        /* Centering */
        case Qt::Key_C:
            ui->btnCenterScene->click();
            break;

        /* Scene rotation */
        case Qt::Key_Z:
            ui->btnRotateSceneLeft->click();
            break;
        case Qt::Key_X:
            ui->btnRotateSceneRight->click();
            break;

        /* Furniture manipulation */
        case Qt::Key_Delete:
            ui->btnDeleteItem->click();
            break;

        case Qt::Key_F:
            ui->btnFlip->click();
            break;

        /* Furniture rotation */
        case Qt::Key_R:
            if (shiftPressed)
                ui->btnRotate90Right->click();
            else
                ui->btnRotateRight->click();
            break;
        case Qt::Key_E:
            if (shiftPressed)
                ui->btnRotate90Left->click();
            else
                ui->btnRotateLeft->click();
            break;
    }
}


/* Menu bar options */
void TemplateWindow::on_actionClear_All_triggered() {
    ui->graphicsView->scene()->clear();
}

void TemplateWindow::on_actionQuit_triggered() {
    TemplateWindow::close();
}

void TemplateWindow::on_SaveAsImage_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Scene As",
            "Screenshot.png", "PNG(*.png);; JPEG(*.jpg *.jpeg)");
    if (fileName.isEmpty())
        return;

//    QPixmap pixmap = QWidget::grab(ui->graphicsView->rect()); // Not deprecated
    QPixmap pixmap = QPixmap::grabWidget(ui->graphicsView);   // Deprecated, but works better
    pixmap.save(fileName);
}
void TemplateWindow::on_actionStatsInfo_triggered()
{
    QMessageBox::information(this, "Apartment info",
        "Rooms created: " + QString::number(Room::numberRooms) + "\n\n" +
        "Apartment size: " + QString::number(m_roomArea) + " m²\n\n" +
        "Used pieces of furniture: " + QString::number(Furniture::numberFurniture) + "\n"
    );
}

/* EXPORT */
void TemplateWindow::on_actionExportProject_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Choose where to export project",
                "", "Text (*.txt);; All Files (*)");

    if (fileName.isEmpty())
        return;

    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);

        /* Grab all items from the scene */
        QList<QGraphicsItem*> items = ui->graphicsView->scene()->items();
        out << items;
    }

    file.close();
}

/* IMPORT */
void TemplateWindow::on_actionImportProject_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Choose a project to import",
                "", "Text (*.txt);; All Files (*)");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);
        ui->graphicsView->scene()->clear();

        QList<QGraphicsItem*> inputItems;
//        in >> inputItems;

//        for (auto item : inputItems)
//            ui->graphicsView->scene()->addItem(item);
    }

    file.close();
    TemplateWindow::update();
}


void TemplateWindow::on_actionShortcuts_triggered()
{
    QMessageBox::information(this, "Shortcuts",
        "SHORTCUT \t\t ACTION \n\n"
        "CTRL + H \t\t Opens this window \n"
        "CTRL + L \t\t Clears everything from the scene \n"
        "CTRL + S \t\t Saves scene as image \n"
        "CTRL + Q \t\t Quits HomePlanner2D \n\n"

        "FURNITURE (must be selected): \n"
        "E   [E+SHIFT]"  "\t"   "Left rotate  [by 90] \n"
        "R   [R+SHIFT]"  "\t"   "Right rotate [by 90] \n"
        "Arrows [SHIFT]" "\t"   "Move [by 10px] \n"
        "DEL"           "\t\t"  "Delete selection \n\n"

        "SCENE: (when no items are selected) \n"
        "+/-"           "\t\t"  "Zoom in/out \n"
        "C"             "\t\t"  "Center scene \n"
        "Z   [Z+SHIFT]"  "\t"   "Left rotate  [by 90] \n"
        "X   [X+SHIFT]"  "\t"   "Right rotate  [by 90] \n"
    );
}


/* FURNITURE */

/* SOFAS - category 1 */

void TemplateWindow::on_btnSofa1_Black_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/sofa_1_black.png", 50, 30);
    scene->addItem(f);
}
void TemplateWindow::on_btnSofa1_White_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/sofa_1_white.png", 50, 30);
    scene->addItem(f);
}
void TemplateWindow::on_btnSofa1_Blue_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/sofa_1_light_blue.png", 50, 30);
    scene->addItem(f);
}
void TemplateWindow::on_btnSofa1_Purple_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/sofa_1_purple.png", 50, 30);
    scene->addItem(f);
}
void TemplateWindow::on_btnSofa1_Red_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/sofa_1_red.png", 50, 30);
    scene->addItem(f);
}
void TemplateWindow::on_btnSofa1_Beige_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/sofa_1_light_brown.png", 50, 30);
    scene->addItem(f);
}
void TemplateWindow::on_btnSofa1_Green_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/sofa_1_green.png", 50, 30);
    scene->addItem(f);
}

/* SOFAS - category 2 */

void TemplateWindow::on_btnSofa2_Grey_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/sofa_2_grey.png", 50, 30);
    scene->addItem(f);
}
void TemplateWindow::on_btnSofa2_White_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/sofa_2_white.png", 50, 30);
    scene->addItem(f);
}
void TemplateWindow::on_btnSofa2_Red_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/sofa_2_red.png", 50, 30);
    scene->addItem(f);
}
void TemplateWindow::on_btnSofa2_Yellow_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/sofa_2_yellow.png", 50, 30);
    scene->addItem(f);
}

/* CORNER SOFAS - category 1 */

void TemplateWindow::on_btnCornerSofa1_Black_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/corner_sofa_1_black.png", 60, 50);
    scene->addItem(f);
}
void TemplateWindow::on_btnCornerSofa1_Beige_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/corner_sofa_1_light.png", 60, 50);
    scene->addItem(f);
}
void TemplateWindow::on_btnCornerSofa1_White_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/corner_sofa_1_white.png", 60, 50);
    scene->addItem(f);
}
void TemplateWindow::on_btnCornerSofa1_Brown_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/corner_sofa_1_brown.png", 60, 50);
    scene->addItem(f);
}
void TemplateWindow::on_btnCornerSofa1_Red_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/corner_sofa_1_red.png", 60, 50);
    scene->addItem(f);
}
void TemplateWindow::on_btnCornerSofa1_Purple_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/corner_sofa_1_purple.png", 60, 50);
    scene->addItem(f);
}
void TemplateWindow::on_btnCornerSofa1_Blue_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/corner_sofa_1_blue.png", 60, 50);
    scene->addItem(f);
}
void TemplateWindow::on_btnCornerSofa1_Skyblue_clicked()
{
    Furniture *f = new Furniture(":/img/furniture/sofas/corner_sofa_1_skyblue.png", 60, 50);
    scene->addItem(f);
}
void TemplateWindow::on_btnCornerSofa1_Green_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/corner_sofa_1_green.png", 60, 50);
    scene->addItem(f);
}

/* CORNER SOFAS - category 2 */

void TemplateWindow::on_btnCornerSofa2_Beige_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/corner_sofa_2_yellow.png", 90, 55);
    scene->addItem(f);
}
void TemplateWindow::on_btnCornerSofa2_Purple_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/corner_sofa_2_purple.png", 90, 55);
    scene->addItem(f);
}
void TemplateWindow::on_btnCornerSofa2_Blue_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/corner_sofa_2_blue.png", 90, 55);
    scene->addItem(f);
}

/* CORNER SOFAS - category 3 */

void TemplateWindow::on_btnCornerSofa3_Black_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/corner_sofa_3_black.png", 90, 55);
    scene->addItem(f);
}
void TemplateWindow::on_btnCornerSofa3_White_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/corner_sofa_3_white.png", 90, 55);
    scene->addItem(f);
}
void TemplateWindow::on_btnCornerSofa3_Beige_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/corner_sofa_3_beige.png", 90, 55);
    scene->addItem(f);
}
void TemplateWindow::on_btnCornerSofa3_Red_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/corner_sofa_3_red.png", 90, 55);
    scene->addItem(f);
}
void TemplateWindow::on_btnCornerSofa3_Purple_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/corner_sofa_3_purple.png", 90, 55);
    scene->addItem(f);
}
void TemplateWindow::on_btnCornerSofa3_Blue_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/corner_sofa_3_blue.png", 90, 55);
    scene->addItem(f);
}

/* CORNER SOFAS - category 4 */

void TemplateWindow::on_btnCornerSofa4_Black_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/corner_sofa_4_black.png", 65, 55);
    scene->addItem(f);
}
void TemplateWindow::on_btnCornerSofa4_White_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/corner_sofa_4_white.png", 65, 55);
    scene->addItem(f);
}

/* BENCHES */

void TemplateWindow::on_btnBenchBamboo_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/sofa_3_bamboo.png", 45, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnBenchWooden_Light_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/sofa_3_wooden_light.png", 45, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnBenchWooden_Dark_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sofas/sofa_3_wooden_dark.png", 45, 25);
    scene->addItem(f);
}

/* ARMCHAIRS - category 1 */

void TemplateWindow::on_btnArmchair1_black_clicked() {
    Furniture *f = new Furniture(":/img/furniture/armchairs/armchair_1_black.png", 25, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnArmchair1_white_clicked() {
    Furniture *f = new Furniture(":/img/furniture/armchairs/armchair_1_white.png", 25, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnArmchair1_green_clicked() {
    Furniture *f = new Furniture(":/img/furniture/armchairs/armchair_1_green.png", 25, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnArmchair1_blue_clicked() {
    Furniture *f = new Furniture(":/img/furniture/armchairs/armchair_1_blue.png", 25, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnArmchair1_orange_clicked() {
    Furniture *f = new Furniture(":/img/furniture/armchairs/armchair_1_orange.png", 25, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnArmchair1_red_clicked() {
    Furniture *f = new Furniture(":/img/furniture/armchairs/armchair_1_red.png", 25, 25);
    scene->addItem(f);
}

/* ARMCHAIRS - category 2 */

void TemplateWindow::on_btnArmchair2_lightgrey_clicked() {
    Furniture *f = new Furniture(":/img/furniture/armchairs/armchair_2_lightgrey.png", 20, 23);
    scene->addItem(f);
}
void TemplateWindow::on_btnArmchair2_lightgreen_clicked()
{
    Furniture *f = new Furniture(":/img/furniture/armchairs/armchair_2_lightgreen.png", 20, 23);
    scene->addItem(f);
}

/* ARMCHAIRS - category 3 */

void TemplateWindow::on_btnArmchair3_black_clicked() {
    Furniture *f = new Furniture(":/img/furniture/armchairs/armchair_3_black.png", 25, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnArmchair3_red_clicked() {
    Furniture *f = new Furniture(":/img/furniture/armchairs/armchair_3_red.png", 25, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnArmchair3_purple_clicked() {
    Furniture *f = new Furniture(":/img/furniture/armchairs/armchair_3_purple.png", 25, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnArmchair3_blue_clicked() {
    Furniture *f = new Furniture(":/img/furniture/armchairs/armchair_3_blue.png", 25, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnArmchair3_green_clicked() {
    Furniture *f = new Furniture(":/img/furniture/armchairs/armchair_3_green.png", 25, 25);
    scene->addItem(f);
}

/* ARMCHAIRS - category 4 */

void TemplateWindow::on_btnArmchair4_purple_clicked() {
    Furniture *f = new Furniture(":/img/furniture/armchairs/armchair_4_purple.png", 22, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnArmchair4_blue_clicked() {
    Furniture *f = new Furniture(":/img/furniture/armchairs/armchair_4_blue.png", 22, 25);
    scene->addItem(f);
}

/* TABOURETS */

void TemplateWindow::on_btnTabouret_black_clicked() {
    Furniture *f = new Furniture(":/img/furniture/armchairs/tabouret_black.png", 15, 15);
    scene->addItem(f);
}
void TemplateWindow::on_btnTabouret_brown_clicked() {
    Furniture *f = new Furniture(":/img/furniture/armchairs/tabouret_brown.png", 15, 15);
    scene->addItem(f);
}
void TemplateWindow::on_btnTabouret_white_clicked() {
    Furniture *f = new Furniture(":/img/furniture/armchairs/tabouret_grey.png", 15, 15);
    scene->addItem(f);
}
void TemplateWindow::on_btnTabouret_blue_clicked() {
    Furniture *f = new Furniture(":/img/furniture/armchairs/tabouret_blue.png", 15, 15);
    scene->addItem(f);
}

/* TABLES - category 1 (dining) */

void TemplateWindow::on_btnTable1_dark_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/table_1_dark.png", 50, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnTable1_grey_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/table_1_grey.png", 50, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnTable1_light_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/table_1_light.png", 50, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnTable1_white_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/table_1_white.png", 50, 25);
    scene->addItem(f);
}

/* TABLES - category 2 (coffee) */

void TemplateWindow::on_btnTable2_darkblue_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/table_2_darkblue.png", 25, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnTable2_dark_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/table_2_dark.png", 25, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnTable2_light_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/table_2_light.png", 25, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnTable2_grey_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/table_2_grey.png", 25, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnTable2_white_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/table_2_white.png", 25, 25);
    scene->addItem(f);
}

/* TABLES - category 3 (round) */

void TemplateWindow::on_btnTable3_dark_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/table_3_round_dark_wood.png", 25, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnTable3_light_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/table_3_round_light_wood.png", 25, 25);
    scene->addItem(f);
}

/* TABLES - category 4 (planks) */

void TemplateWindow::on_btnTable4_dark_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/table_4_dark_wood.png", 40, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnTable4_light_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/table_4_light_wood.png", 40, 25);
    scene->addItem(f);
}

/* TABLES - category 5 (set 1) */

void TemplateWindow::on_btnTable5_dark_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/table_5_complete_dark.png", 50, 50);
    scene->addItem(f);
}
void TemplateWindow::on_btnTable5_blue_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/table_5_complete_blue.png", 50, 50);
    scene->addItem(f);
}
void TemplateWindow::on_btnTable5_brown_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/table_5_complete_brown.png", 50, 50);
    scene->addItem(f);
}

/* TABLES - category 6 (set 2) */

void TemplateWindow::on_btnTable6_dark_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/table_6_dark.png", 45, 35);
    scene->addItem(f);
}
void TemplateWindow::on_btnTable6_light_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/table_6_light.png", 45, 35);
    scene->addItem(f);
}

/* TABLES - category 7 (semi-rounded) */

void TemplateWindow::on_btnTable7_dark_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/table_7_dark.png", 50, 30);
    scene->addItem(f);
}
void TemplateWindow::on_btnTable7_light_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/table_7_light.png", 50, 30);
    scene->addItem(f);
}

/* TABLES - category 8 (glass) */

void TemplateWindow::on_btnTableGlass_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/glass_table.png", 35, 20);
    scene->addItem(f);
}

/* TABLES - category 9 (TV stands) */

void TemplateWindow::on_btnTVStand_brown_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/tv_stand_table_1_brown.png", 55, 13);
    scene->addItem(f);
}
void TemplateWindow::on_btnTVStand_darkgrey_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/tv_stand_table_1_darkgrey.png", 55, 13);
    scene->addItem(f);
}
void TemplateWindow::on_btnTVStand_lightgrey_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/tv_stand_table_1_lightgrey.png", 55, 13);
    scene->addItem(f);
}
void TemplateWindow::on_btnTVStand_dark_clicked() {
    Furniture *f = new Furniture(":/img/furniture/tables/tv_stand_table_2_dark.png", 55, 15);
    scene->addItem(f);
}

/* CHAIRS - category 1 (computer) */

void TemplateWindow::on_btnChair1_black_clicked() {
    Furniture *f = new Furniture(":/img/furniture/chairs/chair_1_black.png", 20, 20);
    scene->addItem(f);
}
void TemplateWindow::on_btnChair1_grey_clicked() {
    Furniture *f = new Furniture(":/img/furniture/chairs/chair_1_grey.png", 20, 20);
    scene->addItem(f);
}
void TemplateWindow::on_btnChair1_blue_clicked() {
    Furniture *f = new Furniture(":/img/furniture/chairs/chair_1_blue.png", 20, 20);
    scene->addItem(f);
}
void TemplateWindow::on_btnChair1_red_clicked() {
    Furniture *f = new Furniture(":/img/furniture/chairs/chair_1_red.png", 20, 20);
    scene->addItem(f);
}
void TemplateWindow::on_btnChair1_yellow_clicked() {
    Furniture *f = new Furniture(":/img/furniture/chairs/chair_1_yellow.png", 20, 20);
    scene->addItem(f);
}

/* CHAIRS - category 2 (normal) */

void TemplateWindow::on_btnChair2_light_clicked() {
    Furniture *f = new Furniture(":/img/furniture/chairs/chair_2_light.png", 15, 18);
    scene->addItem(f);
}
void TemplateWindow::on_btnChair2_dark_clicked() {
    Furniture *f = new Furniture(":/img/furniture/chairs/chair_2_dark.png", 15, 18);
    scene->addItem(f);
}

/* CHAIRS - category 3 (normal) */

void TemplateWindow::on_btnChair3_dark_clicked() {
    Furniture *f = new Furniture(":/img/furniture/chairs/chair_3_dark.png", 15, 18);
    scene->addItem(f);
}
void TemplateWindow::on_btnChair3_light_clicked() {
    Furniture *f = new Furniture(":/img/furniture/chairs/chair_3_light.png", 15, 18);
    scene->addItem(f);
}

/* CHAIRS - category 4 (stools) */

void TemplateWindow::on_btnStool_brown_clicked() {
    Furniture *f = new Furniture(":/img/furniture/chairs/stool_brown.png", 15, 15);
    scene->addItem(f);
}
void TemplateWindow::on_btnStool_light_clicked() {
    Furniture *f = new Furniture(":/img/furniture/chairs/stool_light_brown.png", 15, 15);
    scene->addItem(f);
}

/* CABINETS - category 1 */

void TemplateWindow::on_btnCabinet1_brown_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/cabinet_1_brown.png", 23, 18);
    scene->addItem(f);
}
void TemplateWindow::on_btnCabinet1_light_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/cabinet_1_light.png", 23, 18);
    scene->addItem(f);
}
void TemplateWindow::on_btnCabinet1_white_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/cabinet_1_white.png", 23, 18);
    scene->addItem(f);
}

/* CABINETS - category 2 */

void TemplateWindow::on_btnCabinet2_dark_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/cabinet_2_dark_brown.png", 20, 16);
    scene->addItem(f);
}
void TemplateWindow::on_btnCabinet2_brown_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/cabinet_2_brown.png", 20, 16);
    scene->addItem(f);
}

/* CABINETS - category 3 */

void TemplateWindow::on_btnCabinet3_dark_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/cabinet_3_dark.png", 17, 17);
    scene->addItem(f);
}
void TemplateWindow::on_btnCabinet3_light_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/cabinet_3_light.png", 17, 17);
    scene->addItem(f);
}

/* CABINETS - category 4 (night tables) */

void TemplateWindow::on_btnNightTable_darkblue_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/night_table_1_darkblue.png", 20, 15);
    scene->addItem(f);
}
void TemplateWindow::on_btnNightTable_normal_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/night_table_1_normal.png", 20, 15);
    scene->addItem(f);
}
void TemplateWindow::on_btnNightTable_white_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/night_table_1_white.png", 20, 15);
    scene->addItem(f);
}

/* WARDROBES - category 1 */

void TemplateWindow::on_btnWardrobe1_black_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/wardrobe_1_black.png", 45, 20);
    scene->addItem(f);
}
void TemplateWindow::on_btnWardrobe1_grey_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/wardrobe_1_grey.png", 45, 20);
    scene->addItem(f);
}
void TemplateWindow::on_btnWardrobe1_white_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/wardrobe_1_white.png", 45, 20);
    scene->addItem(f);
}
void TemplateWindow::on_btnWardrobe1_brown_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/wardrobe_1_brown.png", 45, 20);
    scene->addItem(f);
}
void TemplateWindow::on_btnWardrobe1_normal_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/wardrobe_1_normal.png", 45, 20);
    scene->addItem(f);
}
void TemplateWindow::on_btnWardrobe1_light_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/wardrobe_1_light.png", 45, 20);
    scene->addItem(f);
}

/* WARDROBES - category 2 */

void TemplateWindow::on_btnWardrobe2_black_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/wardrobe_2_black.png", 50, 20);
    scene->addItem(f);
}
void TemplateWindow::on_btnWardrobe2_grey_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/wardrobe_2_grey.png", 50, 20);
    scene->addItem(f);
}
void TemplateWindow::on_btnWardrobe2_white_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/wardrobe_2_white.png", 50, 20);
    scene->addItem(f);
}
void TemplateWindow::on_btnWardrobe2_dark_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/wardrobe_2_dark.png", 50, 20);
    scene->addItem(f);
}
void TemplateWindow::on_btnWardrobe2_normal_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/wardrobe_2_normal.png", 50, 20);
    scene->addItem(f);
}
void TemplateWindow::on_btnWardrobe2_light_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/wardrobe_2_light.png", 50, 20);
    scene->addItem(f);
}

/* WARDROBES - category 3 */

void TemplateWindow::on_btnWardrobe3_clicked() {
    Furniture *f = new Furniture(":/img/furniture/wardrobes & cabinets/wardrobe_3.png", 50, 20);
    scene->addItem(f);
}

/* KITCHEN EQUIPMENT */

void TemplateWindow::on_btnBottomCabinet1_clicked() {
    Furniture *f = new Furniture(":/img/furniture/kitchen/bottom_cabinet_1.png", 45, 45);
    scene->addItem(f);
}
void TemplateWindow::on_btnBottomCabinet2_clicked() {
    Furniture *f = new Furniture(":/img/furniture/kitchen/bottom_cabinet_2.png", 30, 24);
    scene->addItem(f);
}
void TemplateWindow::on_btnBottomCabinet3_clicked() {
    Furniture *f = new Furniture(":/img/furniture/kitchen/bottom_cabinet_3.png", 15, 23);
    scene->addItem(f);
}
void TemplateWindow::on_btnTopCabinet1_clicked() {
    Furniture *f = new Furniture(":/img/furniture/kitchen/top_cabinet_1.png", 45, 45);
    scene->addItem(f);
}
void TemplateWindow::on_btnTopCabinet2_clicked() {
    Furniture *f = new Furniture(":/img/furniture/kitchen/top_cabinet_2.png", 30, 17);
    scene->addItem(f);
}
void TemplateWindow::on_btnTopCabinet3_clicked() {
    Furniture *f = new Furniture(":/img/furniture/kitchen/top_cabinet_3.png", 15, 17);
    scene->addItem(f);
}
void TemplateWindow::on_btnStove_clicked() {
    Furniture *f = new Furniture(":/img/furniture/kitchen/stove.png", 24, 24);
    scene->addItem(f);
}

/* SINKS */

void TemplateWindow::on_btnSink1_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sinks/sink_1.png", 15, 15);
    scene->addItem(f);
}
void TemplateWindow::on_btnSink2_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sinks/sink_2.png", 20, 15);
    scene->addItem(f);
}
void TemplateWindow::on_btnSink3_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sinks/sink_3.png", 35, 15);
    scene->addItem(f);
}
void TemplateWindow::on_btnSink4_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sinks/sink_4.png", 35, 15);
    scene->addItem(f);
}
void TemplateWindow::on_btnSink5_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sinks/sink_5.png", 25, 17);
    scene->addItem(f);
}
void TemplateWindow::on_btnSink6_clicked() {
    Furniture *f = new Furniture(":/img/furniture/sinks/sink_6.png", 24, 16);
    scene->addItem(f);
}

/* BEDS - category 1 (baby cot) */

void TemplateWindow::on_btnBabyBed_blue_clicked() {
    Furniture *f = new Furniture(":/img/furniture/beds/baby_bed_lightblue.png", 27, 18);
    scene->addItem(f);
}
void TemplateWindow::on_btnBabyBed_yellow_clicked() {
    Furniture *f = new Furniture(":/img/furniture/beds/baby_bed_lightyellow.png", 27, 18);
    scene->addItem(f);
}

/* BEDS - category 2 (single beds 1) */

void TemplateWindow::on_btnSingleBed1_blue_clicked() {
    Furniture *f = new Furniture(":/img/furniture/beds/single_bed_lightblue.png", 40, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnSingleBed1_yellow_clicked() {
    Furniture *f = new Furniture(":/img/furniture/beds/single_bed_lightyellow.png", 40, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnSingleBed1_white_clicked() {
    Furniture *f = new Furniture(":/img/furniture/beds/single_bed_white.png", 40, 25);
    scene->addItem(f);
}

/* BEDS - category 3 (single beds 2) */

void TemplateWindow::on_btnSingleBed2_blue_clicked() {
    Furniture *f = new Furniture(":/img/furniture/beds/single_bed_2_blue.png", 55, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnSingleBed2_green_clicked() {
    Furniture *f = new Furniture(":/img/furniture/beds/single_bed_2_green.png", 55, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnSingleBed2_purple_clicked() {
    Furniture *f = new Furniture(":/img/furniture/beds/single_bed_2_purple.png", 55, 25);
    scene->addItem(f);
}

/* BEDS - category 4 (king beds 1) */

void TemplateWindow::on_btnKingBed1_lightblue_clicked() {
    Furniture *f = new Furniture(":/img/furniture/beds/king_bed_1_lightblue.png", 38, 50);
    scene->addItem(f);
}
void TemplateWindow::on_btnKingBed1_lightred_clicked() {
    Furniture *f = new Furniture(":/img/furniture/beds/king_bed_1_lightred.png", 38, 50);
    scene->addItem(f);
}
void TemplateWindow::on_btnKingBed1_white_clicked() {
    Furniture *f = new Furniture(":/img/furniture/beds/king_bed_1_white.png", 38, 50);
    scene->addItem(f);
}

/* BEDS - category 5 (king beds 2) */

void TemplateWindow::on_btnKingBed2_lightred_clicked() {
    Furniture *f = new Furniture(":/img/furniture/beds/king_bed_2_lightred.png", 42, 50);
    scene->addItem(f);
}
void TemplateWindow::on_btnKingBed2_white_clicked() {
    Furniture *f = new Furniture(":/img/furniture/beds/king_bed_2_white.png", 42, 50);
    scene->addItem(f);
}

/* ED - category 1 (fridges, washing machines, refrigerator, vent, microwave) */

void TemplateWindow::on_btnFridge_dark_clicked() {
    Furniture *f = new Furniture(":/img/furniture/electronic devices/fridge_dark.png", 25, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnFridge_light_clicked() {
    Furniture *f = new Furniture(":/img/furniture/electronic devices/fridge_white.png", 25, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnRefrigerator_clicked() {
    Furniture *f = new Furniture(":/img/furniture/electronic devices/refridgerator.png", 30, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnVent_clicked() {
    Furniture *f = new Furniture(":/img/furniture/electronic devices/vent.png", 30, 20);
    scene->addItem(f);
}
void TemplateWindow::on_btnAirConditioner_clicked() {
    Furniture *f = new Furniture(":/img/furniture/electronic devices/air_conditioner.png", 30, 10);
    scene->addItem(f);
}
void TemplateWindow::on_btnWashingMachine_grey_clicked() {
    Furniture *f = new Furniture(":/img/furniture/electronic devices/washing_machine_grey.png", 25, 20);
    scene->addItem(f);
}
void TemplateWindow::on_btnWashingMachine_white_clicked() {
    Furniture *f = new Furniture(":/img/furniture/electronic devices/washing_machine_white.png", 25, 20);
    scene->addItem(f);
}
void TemplateWindow::on_btnMicrowave_clicked() {
    Furniture *f = new Furniture(":/img/furniture/electronic devices/microwave.png", 17, 10);
    scene->addItem(f);
}

/* ED - TV, laptops and PC */

void TemplateWindow::on_btnTV1_black_clicked() {
    Furniture *f = new Furniture(":/img/furniture/electronic devices/tv_1_black.png", 33, 7);
    scene->addItem(f);
}
void TemplateWindow::on_btnTV1_white_clicked() {
    Furniture *f = new Furniture(":/img/furniture/electronic devices/tv_1_white.png", 33, 7);
    scene->addItem(f);
}
void TemplateWindow::on_btnTV2_black_clicked() {
    Furniture *f = new Furniture(":/img/furniture/electronic devices/tv_2_black.png", 33, 5);
    scene->addItem(f);
}
void TemplateWindow::on_btnTV2_white_clicked() {
    Furniture *f = new Furniture(":/img/furniture/electronic devices/tv_2_white.png", 33, 5);
    scene->addItem(f);
}
void TemplateWindow::on_btnLaptopMac_clicked() {
    Furniture *f = new Furniture(":/img/furniture/electronic devices/laptop_mac.png", 13, 8);
    scene->addItem(f);
}
void TemplateWindow::on_btnLaptop_black_clicked() {
    Furniture *f = new Furniture(":/img/furniture/electronic devices/laptop_black.png", 13, 8);
    scene->addItem(f);
}
void TemplateWindow::on_btnLaptop_white_clicked() {
    Furniture *f = new Furniture(":/img/furniture/electronic devices/laptop_white.png", 13, 8);
    scene->addItem(f);
}
void TemplateWindow::on_btnPC_clicked() {
    Furniture *f = new Furniture(":/img/furniture/electronic devices/pc.png", 22, 12);
    scene->addItem(f);
}

/* ED - speakers */

void TemplateWindow::on_btnSpeakers1_black_clicked() {
    Furniture *f = new Furniture(":/img/furniture/electronic devices/speakers_1_black.png", 23, 10);
    scene->addItem(f);
}
void TemplateWindow::on_btnSpeakers1_brown_clicked() {
    Furniture *f = new Furniture(":/img/furniture/electronic devices/speakers_1_brown.png", 23, 10);
    scene->addItem(f);
}
void TemplateWindow::on_btnSpeakers2_black_clicked() {
    Furniture *f = new Furniture(":/img/furniture/electronic devices/speakers_2_black.png", 10, 10);
    scene->addItem(f);
}
void TemplateWindow::on_btnSpeakers2_brown_clicked() {
    Furniture *f = new Furniture(":/img/furniture/electronic devices/speakers_2_brown.png", 10, 10);
    scene->addItem(f);
}

/* BATHS - category 1 (corner, round, shower) */

void TemplateWindow::on_btnBath1_dark_clicked() {
    Furniture *f = new Furniture(":/img/furniture/bathroom/bath_1_dark.png", 30, 30);
    scene->addItem(f);
}
void TemplateWindow::on_btnBath1_light_clicked() {
    Furniture *f = new Furniture(":/img/furniture/bathroom/bath_1_light.png", 30, 30);
    scene->addItem(f);
}
void TemplateWindow::on_btnBath1_white_clicked() {
    Furniture *f = new Furniture(":/img/furniture/bathroom/bath_1_white.png", 30, 30);
    scene->addItem(f);
}
void TemplateWindow::on_btnBath2_clicked() {
    Furniture *f = new Furniture(":/img/furniture/bathroom/bath_2.png", 40, 25);
    scene->addItem(f);
}
void TemplateWindow::on_btnShower_clicked() {
    Furniture *f = new Furniture(":/img/furniture/bathroom/shower_1.png", 30, 25);
    scene->addItem(f);
}

/* BATHROOM - category 2 (other) */

void TemplateWindow::on_btnBathroomCabinet_clicked() {
    Furniture *f = new Furniture(":/img/furniture/bathroom/cabinet.png", 15, 15);
    scene->addItem(f);
}
void TemplateWindow::on_btnBathSink1_clicked() {
    Furniture *f = new Furniture(":/img/furniture/bathroom/sink_1.png", 20, 15);
    scene->addItem(f);
}
void TemplateWindow::on_btnBathSink2_clicked() {
    Furniture *f = new Furniture(":/img/furniture/bathroom/sink_2.png", 18, 13);
    scene->addItem(f);
}

/* BATH - category 3 (toilets) */

void TemplateWindow::on_btnToilet1_clicked() {
    Furniture *f = new Furniture(":/img/furniture/bathroom/toilet_1.png", 12, 20);
    scene->addItem(f);
}
void TemplateWindow::on_btnToilet2_white_clicked() {
    Furniture *f = new Furniture(":/img/furniture/bathroom/toilet_2_white.png", 12, 15);
    scene->addItem(f);
}
void TemplateWindow::on_btnToilet2_grey_clicked() {
    Furniture *f = new Furniture(":/img/furniture/bathroom/toilet_2_grey.png", 12, 15);
    scene->addItem(f);
}

/* DOORS */
void TemplateWindow::on_btnDoor1_clicked()
{
    Furniture *f = new Furniture(":/img/furniture/doors/doors_1.png", 20, 30);
    scene->addItem(f);
}
void TemplateWindow::on_btnDoor2_clicked()
{
    Furniture *f = new Furniture(":/img/furniture/doors/doors_2.png", 20, 30);
    scene->addItem(f);
}

void TemplateWindow::on_btnDoor3_clicked()
{
    Furniture *f = new Furniture(":/img/furniture/doors/doors_3.png", 20, 30);
    scene->addItem(f);
}

void TemplateWindow::on_btnDoor4_clicked()
{
    Furniture *f = new Furniture(":/img/furniture/doors/doors_4.png", 20, 30);
    scene->addItem(f);
}

void TemplateWindow::on_btnDoor5_clicked()
{
    Furniture *f = new Furniture(":/img/furniture/doors/doors_5.png", 20, 30);
    scene->addItem(f);
}

void TemplateWindow::on_btnDoor6_clicked()
{
    Furniture *f = new Furniture(":/img/furniture/doors/doors_6.png", 20, 30);
    scene->addItem(f);
}

/* CARPETS - category 1 (round) */

void TemplateWindow::on_btnCarpet1_dark_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/carpet_1_dark.png", 35, 35);
    scene->addItem(f);
}
void TemplateWindow::on_btnCarpet1_brown_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/carpet_1_brown.png", 35, 35);
    scene->addItem(f);
}
void TemplateWindow::on_btnCarpet1_blue_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/carpet_1_blue.png", 35, 35);
    scene->addItem(f);
}
void TemplateWindow::on_btnCarpet1_purple_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/carpet_1_purple.png", 35, 35);
    scene->addItem(f);
}

/* CARPETS - category 2 (square) */

void TemplateWindow::on_btnCarpet2Col2_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/carpet_2_colorful_2.png", 40, 30);
    scene->addItem(f);
}
void TemplateWindow::on_btnCarpet2Col1_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/carpet_2_colorful_1.png", 40, 30);
    scene->addItem(f);
}
void TemplateWindow::on_btnCarpet2Col3_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/carpet_2_colorful_3.png", 40, 30);
    scene->addItem(f);
}

/* PIANOS */

void TemplateWindow::on_btnPiano_black_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/piano_black.png", 30, 10);
    scene->addItem(f);
}
void TemplateWindow::on_btnPiano_brown_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/piano_brown.png", 30, 10);
    scene->addItem(f);
}

/* EXERCISE */

void TemplateWindow::on_btnBenchPress_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/bench_press.png", 27, 30);
    scene->addItem(f);
}
void TemplateWindow::on_btnExerciseBike_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/exercise_bicycle.png", 12, 25);
    scene->addItem(f);
}

/* PLANTS */

void TemplateWindow::on_btnChristmasTree_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/christmas_tree.png", 22, 22);
    scene->addItem(f);
}
void TemplateWindow::on_btnPlant_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/plant.png", 18, 18);
    scene->addItem(f);
}

/* SHELVES */

void TemplateWindow::on_btnShelf_dark_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/shelf_dark.png", 50, 7);
    scene->addItem(f);
}
void TemplateWindow::on_btnShelf_light_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/shelf_light.png", 50, 7);
    scene->addItem(f);
}
void TemplateWindow::on_btnShelf_grey_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/shelf_grey.png", 50, 7);
    scene->addItem(f);
}
void TemplateWindow::on_btnShelf_white_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/shelf_white.png", 50, 7);
    scene->addItem(f);
}

/* LAMPS */

void TemplateWindow::on_btnLamp1_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/lamp_1.png", 12, 12);
    scene->addItem(f);
}
void TemplateWindow::on_btnLamp2_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/lamp_2.png", 11, 11);
    scene->addItem(f);
}
void TemplateWindow::on_btnLamp3_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/lamp_3.png", 7, 9);
    scene->addItem(f);
}

/* OTHER */

void TemplateWindow::on_btnIroning_white_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/ironing_board_white.png", 40, 13);
    scene->addItem(f);
}
void TemplateWindow::on_btnIroning_lightblue_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/ironing_board_lightblue.png", 40, 13);
    scene->addItem(f);
}
void TemplateWindow::on_btnBin_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/bin.png", 17, 14);
    scene->addItem(f);
}
void TemplateWindow::on_btnFireplace_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/fireplace.png", 45, 20);
    scene->addItem(f);
}
void TemplateWindow::on_btnBooks_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/books.png", 7, 6);
    scene->addItem(f);
}
void TemplateWindow::on_btnBowl_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/fruit_bowl.png", 9, 9);
    scene->addItem(f);
}
void TemplateWindow::on_btnDuckie_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/rubber_duck.png", 3, 5);
    scene->addItem(f);
}
void TemplateWindow::on_btnCat_clicked() {
    Furniture *f = new Furniture(":/img/furniture/other/cat.png", 8, 13);
    scene->addItem(f);
}
