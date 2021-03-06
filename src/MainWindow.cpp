/*****************************************************************************
 * MainWindow.cpp
 *
 * Created: 05/10 2011 by uranusjr
 *
 * Copyright 2013 uranusjr. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/

#include "MainWindow.h"
#include <QApplication>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QTabBar>
#include <QToolBar>
#include "Globals.h"
#include "SharedMenuBar.h"
#include "SharedPreferences.h"
#include "TabWidget.h"
#include "View.h"

namespace UJ
{

namespace Qelly
{

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    _tabs = new TabWidget(this);
    _tabs->setTabPosition(QTabWidget::North);
    _prefs = SharedPreferences::sharedInstance();

    buildToolBar();
    setUnifiedTitleAndToolBarOnMac(true);
    setMenuBar(SharedMenuBar::sharedInstance());
    setCentralWidget(_tabs);
    setWindowTitle("Qelly");

    restoreGeometry(_prefs->windowGeometry());
}

void MainWindow::setToolbarVisible(bool visible)
{
    if (_toolbar)
        _toolbar->setVisible(visible);
}

void MainWindow::buildToolBar()
{
    QWidget *stretch = new QWidget(this);
    stretch->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    _address = new QLineEdit();
    _address->setPlaceholderText(tr("Input site address"));
    _address->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    _address->setFocusPolicy(Qt::ClickFocus);
    QLabel *inputLabel = new QLabel(tr("Address"));
    inputLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    inputLabel->setAlignment(Qt::AlignCenter);
    inputLabel->setFocusPolicy(Qt::ClickFocus);
    _inputFrame = new QWidget();
    _inputFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    QVBoxLayout *inputLayout = new QVBoxLayout(_inputFrame);
    inputLayout->addWidget(_address);
    inputLayout->addWidget(inputLabel);
    _inputFrame->setLayout(inputLayout);

    QAction *action;
    _toolbar = addToolBar(tr("General"));
    _toolbar->installEventFilter(this);
    _toolbar->addAction(QIcon(":/images/bookmarks-organize.png"),
                        tr("Sites"), this, SIGNAL(siteManageShouldOpen()));
    _toolbar->addAction(QIcon(":/images/view-refresh.png"),
                        tr("Reconnect"), this, SIGNAL(reconnect()));
    _toolbar->addAction(QIcon(":/images/list-add.png"),
                        tr("Add"), this, SIGNAL(addCurrentSite()));
    _toolbar->addWidget(_inputFrame);
    _toolbar->addWidget(stretch);
    _toolbar->addAction(QIcon(":/images/face-wink.png"), tr("Emoticons"),
                        this, SIGNAL(emoticonViewerShouldOpen()));
    action = _toolbar->addAction(QIcon(":/images/player-time.png"),
                                 tr("Anti-Idle"));
    action->setCheckable(true);
    action->setChecked(_prefs->isAntiIdleActive());
    connect(action, SIGNAL(triggered(bool)), SIGNAL(antiIdleTriggered(bool)));
    action->connect(_prefs, SIGNAL(antiIdleChanged(bool)),
                    SLOT(setChecked(bool)));
    action = _toolbar->addAction(QIcon(":/images/strigi.png"),
                                 tr("Peek"));
    action->setCheckable(true);
    action->setChecked(_prefs->showHiddenText());
    connect(action, SIGNAL(triggered(bool)),
            SIGNAL(showHiddenTextTriggered(bool)));
    action->connect(_prefs, SIGNAL(showHiddenTextChanged(bool)),
                    SLOT(setChecked(bool)));
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    _toolbar->setVisible(_prefs->isToolbarVisible());
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    e->ignore();
    emit windowShouldClose();
}

bool MainWindow::event(QEvent *e)
{
    switch (e->type())
    {
    case QEvent::Resize:
    case QEvent::Move:
        if (geometry() != QDesktopWidget().availableGeometry())
            _prefs->setWindowGeometry(saveGeometry());
        break;
    case QEvent::WindowStateChange:
    {
#ifdef Q_OS_WIN
        // When return to normal state from maximized state, automatically
        // adapt the recommended size. We do this only on Windows because it is
        // the only OS that really has a distinctive "maximized" state, and the
        // auto-resizing feature doesn't work well with many geometry restoring
        // process and `setUnifiedTitleAndToolBarOnMac`.
        QWindowStateChangeEvent *ce = static_cast<QWindowStateChangeEvent *>(e);
        if ((ce->oldState() & Qt::WindowMaximized) && !isMaximized())
            resize(sizeHint());
#endif
        _prefs->setMaximized(isMaximized());
        break;
    }
    default:
        break;
    }
    return QMainWindow::event(e);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == _toolbar)
    {
        switch (e->type())
        {
        case QEvent::Hide:
            _prefs->setToolbarVisible(false);
            break;
        case QEvent::Show:
            _prefs->setToolbarVisible(true);
            break;
        default:
            break;
        }

    }
    return false;
}

}   // namespace Qelly

}   // namespace UJ
