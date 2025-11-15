/* SPDX-FileCopyrightText: 2025
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick.Templates as T
import org.kde.spectacle.private

T.Action {
    enabled: !SpectacleCore.videoMode && contextWindow.image
    icon.name: "globe"
    text: i18nc("@action", "Translate")
    onTriggered: contextWindow.translateSelectedArea()
}
