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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QGraphicsView>
#include <QPointer>
#include <QMessageBox>
#include <QDockWidget>
#include <QTextEdit>
#include <QStatusBar>
#include <QLabel>

#include "board.h"
#include "gamestartdialog.h"
#include "log.h"

class BoardView : public QGraphicsView
{
    void resizeEvent ( QResizeEvent * event ) override;
public:
    BoardView();
};

class LogWindow: public QDockWidget
{
    Q_OBJECT

    QScopedPointer <QTextEdit> edit;
public:
    LogWindow();
    void clear();
public slots:
    void onLogReceived(const QString & str);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    LogWindow logWindow;
    QPointer <Board> board;
    BoardView boardView;
    QLabel status;
    QAction *prevStep, *nextStep, *draw, *resign;
private slots:
    void onStatusUpdated(const QString & str);
    void onDialogWanted(const QString & str);
    void onCreateTriggered();
    void onAboutTriggered();
    void onAboutQtTriggered();
    void onShowLogTriggered();

public:
    /**
     * Default constructor
     */
    MainWindow();

    /**
     * Destructor
     */
    ~MainWindow();

};

#endif // MAINWINDOW_H
