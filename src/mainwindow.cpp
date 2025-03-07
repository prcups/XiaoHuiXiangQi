// The contents of this file are subject to the Mozilla Public License
// Version 1.1 (the "License"); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://www.mozilla.org/MPL/
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
// License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code is ______________________________________.
// 
// The Initial Developer of the Original Code is ________________________.
// Portions created by ______________________ are Copyright (C) ______
// _______________________. All Rights Reserved.
// 
// Contributor(s): ______________________________________.
// 
// Alternatively, the contents of this file may be used under the terms
// of the _____ license (the  "[___] License"), in which case the
// provisions of [______] License are applicable instead of those
// above.  If you wish to allow use of your version of this file only
// under the terms of the [____] License and not to allow others to use
// your version of this file under the MPL, indicate your decision by
// deleting  the provisions above and replace  them with the notice and
// other provisions required by the [___] License.  If you do not delete
// the provisions above, a recipient may use your version of this file
// under either the MPL or the [___] License."

#include "mainwindow.h"

void BoardView::resizeEvent(QResizeEvent* event)
{
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}

BoardView::BoardView(Board* board)
:QGraphicsView(board)
{
    setAcceptDrops(true);
}

MainWindow::MainWindow()
{
    setWindowTitle(tr("小卉象棋"));
    setWindowIcon(QIcon("icon.jpg"));

    auto menubar = new QMenuBar;

    auto gameMenu = menubar->addMenu(tr("对局"));
    gameMenu->addAction(tr("新建"), this, &MainWindow::onCreateTriggered);
    gameMenu->addSeparator();
    gameMenu->addAction(tr("退出"), this, &MainWindow::close);

    auto operationMenu = menubar->addMenu(tr("操作"));
    operationMenu->addAction(tr("上一步"));
    operationMenu->addAction(tr("下一步"));
    operationMenu->addAction(tr("求和"));
    operationMenu->addAction(tr("认输"));
    operationMenu->addSeparator();
    operationMenu->addAction(tr("显示日志"));
    operationMenu->addAction(tr("设置"));

    auto aboutMenu = menubar->addMenu(tr("关于"));
    aboutMenu->addAction(tr("关于"), this, &MainWindow::onAboutTriggered);
    aboutMenu->addAction(tr("关于Qt"), this, &MainWindow::onAboutQtTriggered);

    this->setMenuBar(menubar);
    this->setMinimumSize(560, 640);
}

MainWindow::~MainWindow()
{
    delete boardView;
    delete board;
}

void MainWindow::onCreateTriggered()
{
    PlayerType type[2];
    GameStartDialog gameStartDialog;
    if (!gameStartDialog.exec()) return;
    for (int i = 0; i < 2; ++i)
    {
        if (gameStartDialog.GetPlayerSelection(PieceColor(i)) == 0)
            type[i] = Human;
        else type[i] = Computer;
    }
    delete boardView;
    delete board;
    board = new Board(type);
    boardView = new BoardView(board);
    this->setCentralWidget(boardView);
}

void MainWindow::onAboutTriggered()
{
    QMessageBox::about(this, tr("关于"), tr("小卉象棋 0.1\n一个简单的象棋界面"));
}

void MainWindow::onAboutQtTriggered()
{
    QMessageBox::aboutQt(this, tr("关于Qt"));
}
