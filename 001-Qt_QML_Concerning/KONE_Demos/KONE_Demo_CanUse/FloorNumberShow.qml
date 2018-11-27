import QtQuick 2.0
//import lb2616.tools.ShowFloor 1.0
// 主画面坐标为： x:18 ;  y: 451
Item {
    property alias floornum: floornum
    property string text: floornum.text
    property int upindex:1
    property int dnindex:9
    property bool upflag: true
    signal floorChanged(int fnum, bool upflag)
    id: floor
//    width: 351; height: 173

//    ShowFloor { // 导入C++ 类对象
//        id: showfloor
//    }

    Text {
        id: floornum
        font.family: "KONE Information_v12"
        font.pixelSize: 260
        color: "white"
        text: "1"
        anchors.centerIn: floor
    }

//    Connections {
//        target: showfloor
//        onSendSignalFloor: {
//            floornum.text = index
//            emit: floorChanged(index)
//        }
//    }
    Timer {
        interval: 1500; running: true; repeat: true
        onTriggered: {
            changeFloornum();
        }
    }

    function changeFloornum()
    {
        if (upflag)  // 箭头向上
        {
            if (upindex <= 9)
            {
//                floorChanged(upindex, upflag);
                floornum.text = upindex
                upindex ++;
            }
            else
            {
                upflag = false;
                upindex = 1;
                floorChanged(upindex, upflag);
            }
//            console.log("upindex = ", upindex);
        }
        else //箭头向下
        {
            if (dnindex >= 1)
            {
//                floorChanged(dnindex, upflag);
                floornum.text = dnindex
                dnindex--;
            }
            else
            {
                upflag = true;
                dnindex = 9;
                floorChanged(dnindex, upflag);
            }
//            console.log("dnindex = ", dnindex);
        }
    }
}

