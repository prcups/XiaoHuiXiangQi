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
// // Portions created by ______________________ are Copyright (C) ______
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
    Q_UNUSED(event)
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}

BoardView::BoardView()
{
    setAcceptDrops(true);
}

LogWindow::LogWindow(): edit(new QTextEdit)
{
    setVisible(false);
    setWindowTitle(tr("日志"));
    setAllowedAreas(Qt::LeftDockWidgetArea);
    setFeatures(QDockWidget::DockWidgetClosable);
    edit->setReadOnly(true);
    setWidget(edit.get());
}

void LogWindow::clear()
{
    edit->clear();
}

void LogWindow::onLogReceived(const QString& str)
{
    edit->insertPlainText(str);
}

MainWindow::MainWindow()
{
    setWindowTitle(tr("小卉象棋"));
    setWindowIcon(QIcon("icon.jpg"));

    addDockWidget(Qt::LeftDockWidgetArea, &logWindow);
    connect(&log(), &Log::NewLogOutput, &logWindow, &LogWindow::onLogReceived);
    connect(&bar(), &Log::NewLogOutput, this, &MainWindow::onStatusUpdated);
    connect(&dialog(), &Log::NewLogOutput, this, &MainWindow::onDialogWanted);

    auto menubar = menuBar();

    auto gameMenu = menubar->addMenu(tr("对局"));
    gameMenu->addAction(tr("新建"), this, &MainWindow::onCreateTriggered);
    gameMenu->addAction(tr("查看日志"), this, &MainWindow::onShowLogTriggered);
    gameMenu->addSeparator();
    gameMenu->addAction(tr("退出"), this, &MainWindow::close);

    auto operationMenu = menubar->addMenu(tr("操作"));
    undo = operationMenu->addAction(tr("上一步"), this, &MainWindow::onUndoTriggered);
    undo->setDisabled(true);
    redo = operationMenu->addAction(tr("下一步"), this, &MainWindow::onRedoTriggered);
    redo->setDisabled(true);
    draw = operationMenu->addAction(tr("求和"));
    draw->setDisabled(true);
    resign = operationMenu->addAction(tr("认输"));
    resign->setDisabled(true);
    pause = operationMenu->addAction(tr("暂停"), this, &MainWindow::onPauseTriggered);
    pause->setDisabled(true);
    operationMenu->addSeparator();
    operationMenu->addAction(tr("设置"));

    auto aboutMenu = menubar->addMenu(tr("关于"));
    aboutMenu->addAction(tr("关于"), this, &MainWindow::onAboutTriggered);
    aboutMenu->addAction(tr("关于Qt"), this, &MainWindow::onAboutQtTriggered);

    this->setMinimumSize(560, 640);
    this->setCentralWidget(&boardView);

    statusBar()->addWidget(&status);
}

MainWindow::~MainWindow()
{
    delete board;
}

void MainWindow::onDialogWanted(const QString& str)
{
    QMessageBox::information(this, "", str);
}

void MainWindow::onStatusUpdated(const QString& str)
{
    status.setText(str);
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

    logWindow.clear();

    auto newBoard = new Board(type);

    boardView.setScene(newBoard);
    delete board;
    board = newBoard;
    connect(board, &Board::BoardInfoChanged, this, &MainWindow::onBoardInfoChanged);

    board->Start();
}

void MainWindow::onAboutTriggered()
{
    QMessageBox::about(this, tr("关于"), tr("小卉象棋 0.1\n一个简单的象棋界面"));
}

void MainWindow::onAboutQtTriggered()
{
    QMessageBox::aboutQt(this, tr("关于Qt"));
}

void MainWindow::onShowLogTriggered()
{
    logWindow.setVisible(true);
}

void MainWindow::onPauseTriggered()
{
    board->ChangePaused();
}

void MainWindow::onUndoTriggered()
{
    board->Undo();
}

void MainWindow::onRedoTriggered()
{
    board->Redo();
}

void MainWindow::onBoardInfoChanged(const BoardInfo& info)
{
    draw->setDisabled(info.isEnd);
    resign->setDisabled(info.isEnd);

    undo->setEnabled(info.hasPrev);
    redo->setEnabled(info.hasNext);

    if (info.ifJiangjun)
        bar() << (info.isBlack ? tr("红方") : tr("黑方")) + tr("将军");
    if (info.isEnd)
    {
        pause->setDisabled(true);
        bar() << (info.isBlack ? tr("红方") : tr("黑方")) + tr("胜利");
        boardView.resetTransform();
        board->Rotate(false);
        return;
    }

    pause->setEnabled(true);
    if (info.isPaused)
    {
        bar() << tr("游戏已暂停");
        pause->setText(tr("继续"));
    }
    else pause->setText(tr("暂停"));

    if (info.isHuman && info.isBlack)
    {
        boardView.resetTransform();
        boardView.rotate(180);
        board->Rotate(true);
    }
    else
    {
        boardView.resetTransform();
        board->Rotate(false);
    }
}
