/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2011 Razor team
 * Authors:
 *   Alexander Sokoloff <sokoloff.a@gmail.com>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */


#ifndef LXQTTASKBUTTON_H
#define LXQTTASKBUTTON_H

#include <QToolButton>
#include <QProxyStyle>
#include "../panel/ilxqtpanel.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include "../panel/fixx11h.h"

class QPainter;
class QPalette;
class QMimeData;

class ElidedButtonStyle: public QProxyStyle
{
public:
    ElidedButtonStyle(QStyle* style=0): QProxyStyle(style) {}

    void drawItemText(QPainter* painter, const QRect& rect, int flags,
                      const QPalette & pal, bool enabled, const QString & text,
                      QPalette::ColorRole textRole = QPalette::NoRole ) const;
};


class LxQtTaskButton : public QToolButton
{
    Q_OBJECT
public:
    explicit LxQtTaskButton(const Window window, QWidget *parent = 0);
    virtual ~LxQtTaskButton();

    bool isAppHidden() const;
    bool isApplicationActive() const;
    Window windowId() const { return mWindow; }

    bool hasUrgencyHint() const { return mUrgencyHint; }
    void setUrgencyHint(bool set);

    int desktopNum() const;
    void updateText();
    void updateIcon();

public slots:
    void raiseApplication();
    void minimizeApplication();
    void maximizeApplication();
    void deMaximizeApplication();
    void shadeApplication();
    void unShadeApplication();
    void closeApplication();
    void moveApplicationToDesktop();
    void setApplicationLayer();

    void handlePropertyNotify(XEventType* event);

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragLeaveEvent(QDragLeaveEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

    void contextMenuEvent( QContextMenuEvent* event);

private:
    Window mWindow;
    bool mUrgencyHint;
    const QMimeData *mDraggableMimeData;
    QPoint mDragStartPosition;

private slots:
    void activateWithDraggable();
};

typedef QHash<Window,LxQtTaskButton*> LxQtTaskButtonHash;

#endif // LXQTTASKBUTTON_H
