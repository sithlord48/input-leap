/*
 * InputLeap -- mouse and keyboard sharing utility
 * Copyright (C) 2023-2024 InputLeap Developers
 * Copyright (C) 2012-2016 Symless Ltd.
 * Copyright (C) 2008 Volker Lanz (vl@fidra.de)
 *
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file LICENSE that should have accompanied this file.
 *
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "KeySequence.h"

#include <QString>
#include <QStringList>
#include <QList>

class ActionDialog;
class QSettings;
class QTextStream;

namespace SETTINGS {
    static const QString ACTION_TYPE = QStringLiteral("type");
    static const QString SCREEN_NAMES = QStringLiteral("typeScreenNames");
    static const QString SCREEN_NAME = QStringLiteral("typeScreenName");
    static const QString SWITCH_TO_SCREEN = QStringLiteral("switchScreenName");
    static const QString SWITCH_DIRECTION = QStringLiteral("switchInDirection");
    static const QString LOCKTOSCREEN = QStringLiteral("lockCursorToScreen");
    static const QString ACTIVEONRELEASE = QStringLiteral("activeOnRelease");
    static const QString HASSCREENS = QStringLiteral("hasScreens");
};

class Action
{
    public:
        enum ActionType { keyDown, keyUp, keystroke,
                          switchToScreen, toggleScreen, switchInDirection,
                          lockCursorToScreen, mouseDown, mouseUp, mousebutton };
        enum SwitchDirection { switchLeft, switchRight, switchUp, switchDown };
        enum LockCursorMode { lockCursorToggle, lockCursonOn, lockCursorOff  };

    public:
        Action();

    public:
        QString text() const;
        const KeySequence& keySequence() const { return keySequence_; }
        void setKeySequence(const KeySequence& seq) { keySequence_ = seq; }

        void loadSettings(QSettings& settings);
        void saveSettings(QSettings& settings) const;

        int type() const { return type_; }
        void setType(int t) { type_ = t; }

        const QStringList& typeScreenNames() const { return typeScreenNames_; }
        void appendTypeScreenName(QString name) { typeScreenNames_.append(name); }
        void clearTypeScreenNames() { typeScreenNames_.clear(); }

        const QString& switchScreenName() const { return switchScreenName_; }
        void setSwitchScreenName(const QString& n) { switchScreenName_ = n; }

        int switchDirection() const { return switchDirection_; }
        void setSwitchDirection(int d) { switchDirection_ = d; }

        int lockCursorMode() const { return lockCursorMode_; }
        void setLockCursorMode(int m) { lockCursorMode_ = m; }

        bool activeOnRelease() const { return activeOnRelease_; }
        void setActiveOnRelease(bool b) { activeOnRelease_ = b; }

        bool haveScreens() const { return hasScreens_; }
        void setHaveScreens(bool b) { hasScreens_ = b; }

    private:
        KeySequence keySequence_;
        int type_;
        QStringList typeScreenNames_;
        QString switchScreenName_;
        int switchDirection_;
        int lockCursorMode_;
        bool activeOnRelease_;
        bool hasScreens_;

        static const char* actionTypeNames_[];
        static const char* switchDirectionNames_[];
        static const char* lockCursorModeNames_[];
};

QTextStream& operator<<(QTextStream& outStream, const Action& action);
