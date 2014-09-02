/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *      Alexis López Zubieta
 *
 * This file is part of Moonlight Desktop Environment.
 *
 * Moonlight Desktop Environment is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moonlight Desktop Environment is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moonlight Desktop Environment. If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef MOONLIGHTPANELMODULE_H
#define	MOONLIGHTPANELMODULE_H

#include "ilxqtpanelplugin.h"
#include <QAbstractNativeEventFilter>
#include <QPointer>
#include <QDebug>

class QScreen;

class LxQtPanel;
namespace LxQt {
    class Settings;
}

class MoonLightPanelModule : public QObject, public QAbstractNativeEventFilter {
    Q_OBJECT
public:

    void init(); 
    virtual ~MoonLightPanelModule();
    virtual bool nativeEventFilter(const QByteArray & eventType, void * message, long * result);

    static MoonLightPanelModule * instance() {
        if (mSelf == NULL) {
            qDebug() << "Instancing MoonLightPanelModule.";
            mSelf = new MoonLightPanelModule();
        }

        return mSelf;
    }

    int count() {
        return mPanels.count();
    }

    LxQt::Settings *settings() {
        return mSettings;
    }

public slots:
    void addNewPanel();

private:
    QList<LxQtPanel*> mPanels;

    LxQtPanel* addPanel(const QString &name);

private slots:
    void removePanel(LxQtPanel* panel);

    // These slots are for Qt 5 onlt, but the stupid Qt moc cannot do conditional compilation
    // so we have to provide them for Qt 4 as well.
    void handleScreenAdded(QScreen* newScreen);
    void screenDestroyed(QObject* screenObj);
    void reloadPanelsAsNeeded();
    // end Qt5 only slots

private:
    MoonLightPanelModule();
    MoonLightPanelModule(const MoonLightPanelModule& orig);
    

    LxQt::Settings *mSettings;
    static MoonLightPanelModule* mSelf;
};

#endif	/* MOONLIGHTPANELMODULE_H */

