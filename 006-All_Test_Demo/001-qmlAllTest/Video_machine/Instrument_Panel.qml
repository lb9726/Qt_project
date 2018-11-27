import QtQuick 2.0

Item {
    property string mWeekDayStr: ""
    property string mCurrentFloor: ""
    property string imagesource: "qrc:/images/arrow/ArrowUp_Red.png"
    property bool arrowDirectionUp: false
    property string mFloorPath: "qrc:/images/Floor_Panel/"
    property int mRotation: 0
    Image {
        x: 4
        id: panelBlackBg
        source: "qrc:/images/panel_Background/PanelBackground.png"
    }
    Image {
        id: frontWhite_ID
        x: 120
        source: "qrc:/images/panel_Background/Front_WhitePicture.png"
    }

    Item {
        width: 295
        height: 30
        x: 362
        y: 418 - 370

        Item {
            id: week_ID_Item
            width: 94
            height: parent.height
            Text {
                id: week_ID_text
                text: DateAndTime.getWeekDay()
                anchors.centerIn: parent
                color: "white"
                font.bold: true
                font.pixelSize: 16
            }
        }

        Image {
            id: timeBg_ID
            x: week_ID_Item.width
            width: 99
            height: parent.height
            source: "qrc:/images/panel_Background/Time_Bg.png"
            Item {
                width: 97
                height: parent.height
                Text {
                    id: time_TextID
                    text: DateAndTime.getTimeFormat(false)
                    anchors.centerIn: parent
                    color: "white"
                    font.bold: true
                    font.pixelSize: 20
                }
            }
        }
        Item {
            x: week_ID_Item.width + timeBg_ID.width
            width: parent.width - week_ID_Item.width - timeBg_ID.width
            height: parent.height
            Image {
                id: weather_ID
                source: "qrc:/images/TimeAndWeather/Cloundy.png"
                anchors.centerIn: parent
            }
        }
    }
    Image {
        x: 120
        y: -50
        source: "qrc:/images/panel_Background/Front_GreyPicture.png"
    }
    Image {
        id: blackBg_ID
        x: 24
        y: 36 // 406 - 370 //  x: 24 //   y: 406
        width: 335
        height: 335
        source: "qrc:/images/floor/floor_Bg.png"
    }

    Rectangle {
        x: 24 + (335 - 245) / 2.0
        y: 36 + (335 - 245) / 2.0
        width: 245/*335*/
        height: 245/*335*/
        color: "transparent"
//        color: "red"
        radius: width/2
        clip: true
        Item {
//            x: (335 - 245) / 2.0
//            y: (335 - 245) / 2.0 // 406 - 370 //  x: 24 //   y: 406
            width: 245
            height: 245
            id: showArrowArea_ID
            Image {
                width: showArrowArea_ID.width
                height: showArrowArea_ID.height
                source: imagesource
                rotation: mRotation
            }
            Image {
                width: showArrowArea_ID.width
                height: showArrowArea_ID.height
                source: imagesource
                rotation: mRotation
                y: showArrowArea_ID.height
            }
            NumberAnimation {
                id: arrowUp_ID
                target: showArrowArea_ID
                property: "y"
                from: 0; to: -showArrowArea_ID.height ;
                alwaysRunToEnd: true
                duration: 1000
                loops: Animation.Infinite
            }

            NumberAnimation {
                id: arrowDown_ID
                target: showArrowArea_ID
                property: "y"
                from: -showArrowArea_ID.height; to: 0;
                duration: 1000
                loops: Animation.Infinite
            }
        }
    }

//    Item {
//        x: 24 + (335 - 245) / 2.0
//        y: 36 + (335 - 245) / 2.0 // 406 - 370 //  x: 24 //   y: 406
//        width: 245
//        height: 245
//        id: showArrowArea_ID
//        Image {
//            width: showArrowArea_ID.width
//            height: showArrowArea_ID.height
//            source: imagesource
//        }
//        Image {
//            clip: true
//            width: showArrowArea_ID.width
//            height: showArrowArea_ID.height
//            source: imagesource
//            y: showArrowArea_ID.height
//        }
//        NumberAnimation {
//            id: arrowUp_ID
//            target: showArrowArea_ID
//            property: "y"
//            from: 0; to: -showArrowArea_ID.height ;
//            alwaysRunToEnd: true
//            duration: 1000
//            loops: Animation.Infinite
//        }

//        NumberAnimation {
//            id: arrowDown_ID
//            target: showArrowArea_ID
//            property: "y"
//            from: -showArrowArea_ID.height; to: 0;
//            duration: 1000
//            loops: Animation.Infinite
//        }
//    }

    Image {
        id: frame_blue
        x: 24
        y: 36 // 406 - 370 //  x: 24 //   y: 406
        width: 336
        height: 335
        source: "qrc:/images/panel_Background/Arrow_Frame_Blue.png"
        transform: Rotation {
            id: minuteRotation
            origin.x: 336 / 2;
            origin.y: 335 / 2.0;
            NumberAnimation on angle {
                from: 0
                to: 360
                duration: 6000
                loops: Animation.Infinite
            }
        }
        Image {
            id: img_CircleDot
            source: "qrc:/images/arrow/circle_dot.png"
            anchors.centerIn: parent
        }
        Image {
            id: img_Circle
            source: "qrc:/images/arrow/circle.png"
            anchors.centerIn: parent
        }
    }

    Image {
        id: floorblackBg_ID
        x: 666
        y: 36
        width: 335
        height: 335
        source: "qrc:/images/floor/floor_Bg.png"
    }
    Image {
        // x: 666  y: 366
        id: floor_img
        x: 666
        y: 36
        width: 336
        height: 335
        source: "qrc:/images/panel_Background/Arrow_Frame_Blue.png"
        transform: Rotation {
            id: minuteRotation2
            origin.x: 336 / 2;
            origin.y: 335 / 2.0;
            NumberAnimation on angle {
                from: 0
                to: 360
                duration: 6000
                loops: Animation.Infinite
            }
        }

        Image {
            id: img_CircleDot2
            source: "qrc:/images/arrow/circle_dot.png"
            anchors.centerIn: parent
        }

        Image {
            id: img_Circle2
            source: "qrc:/images/arrow/circle.png"
            anchors.centerIn: parent
        }

        Image {
            id: small_circle
            source: "qrc:/images/floor/small_circle.png"
            anchors.centerIn: parent
        }

        Image {
            id: keli
            source: "qrc:/images/floor/keli.png"
            anchors.centerIn: parent
        }

        Image {
            id: half_circle
            source: "qrc:/images/floor/half_cricle.png"
            anchors.centerIn: parent
        }
    }

    Item {
        x: 666
        y: 36 - 14
        width: 336
        height: 335
        Image {            
            id: showFloor
            anchors.centerIn: parent
            source: mFloorPath + "Floor_AllBlack.png"
        }
    }

    Image {
        id: earth
        x: 462 - 30
        y: 468 - 370
        source: "qrc:/images/earth/earth.png"
        transform: Rotation {             // (a)
            origin.x : 139 / 2.0
            origin.y : 139 / 2.0
            axis {
                x: 0
                y: 1
                z: 0                      // 绕y轴转动
            }
            NumberAnimation on angle {    // 定义角度angle上的动画
                from: 0
                to: 360
                duration: 20000
                loops: Animation.Infinite
            }
        }
    }
//    AnimatedImage {
//        id: earth
//        x: 462
//        y: 488 - 370
//        source: "qrc:/images/earth/gif_earth.gif"
//    }

    Image {
        id: current_floor_Text
        x: 796
        y: 662 - 370 // 662 - 370
        source: "qrc:/images/floor/current_floor.png"
    }

    Image {
        id: name
        x: 380 - 35
        y: 618 - 370
        source: "qrc:/images/floorMark/floorMark.png"
    }
    Image {
        id : logo_OTIS
        x: 276
        y: 702 - 370
        source: "qrc:/images/OTIS/OTIS_logoBg.png"
        Image {
            anchors.centerIn: parent
            source: "qrc:/images/OTIS/OTIS_logo.png"
            transform: Rotation {             // (a)
                origin.x : 80 / 2.0
                origin.y : 25 / 2.0
                axis {
                    x: 0
                    y: 1
                    z: 0                      // 绕y轴转动
                }
                NumberAnimation on angle {    // 定义角度angle上的动画
                    from: 0
                    to: 360
                    duration: 5000
                    loops: Animation.Infinite
                }
            }
        }
    }

    Connections {
        target: DateAndTime
        onSignalTimeAndDate: {
            time_TextID.text = pTime;
        }
    }

    Connections {
        target: DateAndTime
        onSigWeekChanged: {
            week_ID_text.text = pWeekDay;
        }
    }

    Connections {
        target: DateAndTime
        onSigAutoTestChanged: {
            if (arrowDirectionUp != pArrowUp)
            {
                arrowDirectionUp = pArrowUp;
                if (arrowDirectionUp)
                {
                    mRotation = 0;
                    imagesource = "qrc:/images/arrow/ArrowUp_Red.png"
                }
                else
                {
                    mRotation = 180
                    imagesource = "qrc:/images/arrow/ArrowUp_Blue.png"
                }
                ctrlArrowScroll();
            }
        }
    }

    function changeFloorPicture(indexStr)
    {
        if ("B1" === indexStr)
        {
            showFloor.source = mFloorPath + "Floor_B1.png"
        }
        else if ("1" === indexStr)
        {
            showFloor.source = mFloorPath + "Floor_1F.png"
        }
        else if ("2" === indexStr)
        {
            showFloor.source = mFloorPath + "Floor_2F.png"
        }
        else if ("3" === indexStr)
        {
            showFloor.source = mFloorPath + "Floor_3F.png"
        }
        else if ("4" === indexStr)
        {
            showFloor.source = mFloorPath + "Floor_4F.png"
        }
        else if ("5" === indexStr)
        {
            showFloor.source = mFloorPath + "Floor_5F.png"
        }
        else if ("6" === indexStr)
        {
            showFloor.source = mFloorPath + "Floor_6F.png"
        }
    }

    function ctrlArrowScroll()
    {
        if(arrowDirectionUp)
        {
            arrowDown_ID.stop();
            arrowUp_ID.start();
        }
        else
        {
            arrowUp_ID.stop();
            arrowDown_ID.start();
        }
    }
    Connections {
        target: DateAndTime
        onSigAutoTestChanged: {
            changeFloorPicture(pFloor);
        }
    }
}
