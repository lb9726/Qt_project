import QtQuick.Window 2.2
import QtQuick.Controls 2.0

Window {
    id: id_window
    property int mMiniWidth: 500
    property int mMiniHeight: 550
    property int mHasSaveFlag: 0
    visible: false
    title: qsTr("参数配置")
    modality: Qt.WindowModal
    width: mMiniWidth
    height: mMiniHeight
    minimumHeight: mMiniHeight
    maximumHeight: mMiniHeight
    minimumWidth: mMiniWidth
    maximumWidth: mMiniWidth
    onVisibleChanged: {
        console.log("ParaMeterConfigWindow.qml visible is ", visible);
    }
}
