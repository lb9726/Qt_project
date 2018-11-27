import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Switch {
      id: control
      text: qsTr("Switch")

      indicator: Rectangle {
          implicitWidth: 60
          implicitHeight: 26
          x: control.leftPadding
          y: parent.height / 2 - height / 2
          radius: 13  // 让矩形变成椭圆
          color: control.checked ? "#17a81a" : "#ffffff"
          border.color: control.checked ? "#17a81a" : "#cccccc"

          Rectangle {
              x: control.checked ? parent.width - width : 0
              Behavior on x {
                        NumberAnimation { duration: 300 }
                    } // 小开关的滑动动画控制
              width: 26
              height: 26
              radius: 13 // 使得正方形变成圆形的开关滑动 control.down 压下为真时为灰色，否则白色
              color: control.down ? "#cccccc" : "#ffffff"
              border.color: control.checked ? (control.down ? "#17a81a" : "#21be2b") : "#999999"
          }
      }

      contentItem: Text {
          text: control.text
          font: control.font
          opacity: enabled ? 1.0 : 0.3
          color: control.checked ? "#ffffff" : "black"
//          color: control.checked ? "red" : "white"
          enabled: control.checked
          horizontalAlignment: Text.AlignHCenter
          verticalAlignment: Text.AlignVCenter
          leftPadding: control.indicator.width + control.spacing// 距离左侧开关距离
      }
}
