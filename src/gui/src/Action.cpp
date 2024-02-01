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

#include "Action.h"

#include <QSettings>
#include <QTextStream>

const char* Action::actionTypeNames_[] =
{
    "keyDown", "keyUp", "keystroke",
    "switchToScreen", "toggleScreen",
    "switchInDirection", "lockCursorToScreen",
    "mouseDown", "mouseUp", "mousebutton"
};

const char* Action::switchDirectionNames_[] = { "left", "right", "up", "down" };
const char* Action::lockCursorModeNames_[] = { "toggle", "on", "off" };
const QString Action::commandTemplate_ = QStringLiteral("(%1)");

Action::Action() :
    keySequence_(),
    type_(keystroke),
    typeScreenNames_(),
    switchScreenName_(),
    switchDirection_(switchLeft),
    lockCursorMode_(lockCursorToggle),
    activeOnRelease_(false),
    hasScreens_(false)
{
}

QString Action::text() const
{
    /* This function is used to save to config file which is for InputLeap server to
     * read. However the server config parse does not support functions with ()
     * in the end but now argument inside. If you need a function with no
     * argument, it can not have () in the end.
     */
    QString text = QString(actionTypeNames_[keySequence_.isMouseButton() ?
                                             type() + int(mouseDown) : type()]);

    switch (type())
    {
        case keyDown:
        case keyUp:
        case keystroke:
            {
                QString commandArgs = keySequence_.toString();
                if (!keySequence_.isMouseButton())
                {
                    const QStringList& screens = typeScreenNames();
                    if (haveScreens() && !screens.isEmpty())
                    {
                        QString screenList;
                        for (int i = 0; i < screens.size(); i++)
                        {
                            screenList.append(screens[i]);
                            if (i != screens.size() - 1)
                                screenList.append(QStringLiteral(":"));
                        }
                        commandArgs.append(QStringLiteral(",%1").arg(screenList));
                    }
                    else
                    {
                        commandArgs.append(QStringLiteral(",*"));
                    }
                }
                text.append(commandTemplate_.arg(commandArgs));
            }
            break;

        case switchToScreen:
            text.append(commandTemplate_.arg(switchScreenName()));
            break;

        case toggleScreen:
            break;

        case switchInDirection:
            text.append(commandTemplate_.arg(switchDirectionNames_[switchDirection_]));
            break;

        case lockCursorToScreen:
            text.append(commandTemplate_.arg(lockCursorModeNames_[lockCursorMode_]));
            break;

        default:
            Q_ASSERT(0);
            break;
    }


    return text;
}

void Action::loadSettings(QSettings& settings)
{
    keySequence_.loadSettings(settings);
    setType(settings.value(SETTINGS::ACTION_TYPE, keyDown).toInt());

    typeScreenNames_.clear();
    int numTypeScreens = settings.beginReadArray(SETTINGS::SCREEN_NAMES);
    for (int i = 0; i < numTypeScreens; i++)
    {
        settings.setArrayIndex(i);
        typeScreenNames_.append(settings.value(SETTINGS::SCREEN_NAME).toString());
    }
    settings.endArray();

    setSwitchScreenName(settings.value(SETTINGS::SWITCH_TO_SCREEN).toString());
    setSwitchDirection(settings.value(SETTINGS::SWITCH_DIRECTION, switchLeft).toInt());
    setLockCursorMode(settings.value(SETTINGS::LOCKTOSCREEN, lockCursorToggle).toInt());
    setActiveOnRelease(settings.value(SETTINGS::ACTIVEONRELEASE, false).toBool());
    setHaveScreens(settings.value(SETTINGS::HASSCREENS, false).toBool());
}

void Action::saveSettings(QSettings& settings) const
{
    keySequence_.saveSettings(settings);
    settings.setValue(SETTINGS::ACTION_TYPE, type());

    settings.beginWriteArray(SETTINGS::SCREEN_NAMES);
    for (int i = 0; i < typeScreenNames().size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue(SETTINGS::SCREEN_NAME, typeScreenNames()[i]);
    }
    settings.endArray();

    settings.setValue(SETTINGS::SWITCH_TO_SCREEN, switchScreenName());
    settings.setValue(SETTINGS::SWITCH_DIRECTION, switchDirection());
    settings.setValue(SETTINGS::LOCKTOSCREEN, lockCursorMode());
    settings.setValue(SETTINGS::ACTIVEONRELEASE, activeOnRelease());
    settings.setValue(SETTINGS::HASSCREENS, haveScreens());
}

QTextStream& operator<<(QTextStream& outStream, const Action& action)
{
    if (action.activeOnRelease())
        outStream << ";";

    outStream << action.text();

    return outStream;
}
