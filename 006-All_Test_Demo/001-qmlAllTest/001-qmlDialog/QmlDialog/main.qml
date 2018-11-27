import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    ParaMeterConfigDialog {
        id: id_Dialog
    }

    RectBtn {
        y: 0
        id: id_btn1
        onButtonClicked: {
//            id_window_test.visible = true;
            id_TestD.open()
        }
    }

    RectBtn {
        y: 80
        id: id_btn2
        onButtonClicked: {
            id_window_test2.visible = true;
        }
    }
    RectBtn {
        y: 160
        id: id_btn3
        onButtonClicked: {
            id_window_test3.visible = true;
        }
    }
    Window_Test {
        id: id_window_test;
        visible: false;
    }
    Window_Test {
        id: id_window_test2;
        visible: false;
    }
    Window_Test {
        id: id_window_test3;
        visible: false;
    }
    TestDailog {
        id: id_TestD
    }

}
