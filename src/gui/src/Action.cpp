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
                text += "(";
                text += keySequence_.toString();

                if (!keySequence_.isMouseButton())
                {
                    const QStringList& screens = typeScreenNames();
                    if (haveScreens() && !screens.isEmpty())
                    {
                        text += ",";

                        for (int i = 0; i < screens.size(); i++)
                        {
                            text += screens[i];
                            if (i != screens.size() - 1)
                                text += ":";
                        }
                    }
                    else
                        text += ",*";
                }
                text += ")";
            }
            break;

        case switchToScreen:
            text += "(";
            text += switchScreenName();
            text += ")";
            break;

        case toggleScreen:
            break;

        case switchInDirection:
            text += "(";
            text += switchDirectionNames_[switchDirection_];
            text += ")";
            break;

        case lockCursorToScreen:
            text += "(";
            text += lockCursorModeNames_[lockCursorMode_];
            text += ")";
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
    setType(settings.value("type", keyDown).toInt());

    typeScreenNames_.clear();
    int numTypeScreens = settings.beginReadArray("typeScreenNames");
    for (int i = 0; i < numTypeScreens; i++)
    {
        settings.setArrayIndex(i);
        typeScreenNames_.append(settings.value("typeScreenName").toString());
    }
    settings.endArray();

    setSwitchScreenName(settings.value("switchScreenName").toString());
    setSwitchDirection(settings.value("switchInDirection", switchLeft).toInt());
    setLockCursorMode(settings.value("lockCursorToScreen", lockCursorToggle).toInt());
    setActiveOnRelease(settings.value("activeOnRelease", false).toBool());
    setHaveScreens(settings.value("hasScreens", false).toBool());
}

void Action::saveSettings(QSettings& settings) const
{
    keySequence_.saveSettings(settings);
    settings.setValue("type", type());

    settings.beginWriteArray("typeScreenNames");
    for (int i = 0; i < typeScreenNames().size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("typeScreenName", typeScreenNames()[i]);
    }
    settings.endArray();

    settings.setValue("switchScreenName", switchScreenName());
    settings.setValue("switchInDirection", switchDirection());
    settings.setValue("lockCursorToScreen", lockCursorMode());
    settings.setValue("activeOnRelease", activeOnRelease());
    settings.setValue("hasScreens", haveScreens());
}

QTextStream& operator<<(QTextStream& outStream, const Action& action)
{
    if (action.activeOnRelease())
        outStream << ";";

    outStream << action.text();

    return outStream;
}
