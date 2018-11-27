/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 1.0

Component {
    Item {
        id: wrapper; width: wrapper.ListView.view.width; height: 86
        Item {
            id: moveMe
            Rectangle { color: "black"; opacity: index % 2 ? 0.2 : 0.4; height: 84; width: wrapper.width; y: 1 }
            Rectangle {
                x: 6; y: 4; width: 77; height: 77; color: "white"; smooth: true

                Image { source: imagePath; x: 1; y: 1 }
                Image { source: "images/gloss.png" }
            }
            Column {
                x: 92; width: wrapper.ListView.view.width - 95; y: 15; spacing: 2
                Text { text: title; color: "white"; width: parent.width; font.pixelSize: 14; font.bold: true; elide: Text.ElideRight; style: Text.Raised; styleColor: "black" }
                Text { text: photoAuthor; width: parent.width; font.pixelSize: 14; elide: Text.ElideLeft; color: "#cccccc"; style: Text.Raised; styleColor: "black" }
                Text { text: photoDate; width: parent.width; font.pixelSize: 14; elide: Text.ElideRight; color: "#cccccc"; style: Text.Raised; styleColor: "black" }
            }
        }
    }
}
