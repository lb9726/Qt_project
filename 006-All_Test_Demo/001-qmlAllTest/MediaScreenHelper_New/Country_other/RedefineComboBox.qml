import QtQuick 2.7
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.3

Item {
    signal selfCurrentIndexChanged(int currentIndex)
    property alias newmodel: cbox.model
    property alias newwidth: cbox.width
    ComboBox {
        id: cbox
        width: 140
        style: ComboBoxStyle {
            label: Text {
                id: cBoxtext
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: "black"
                text: cbox.currentText
            }
        }
        onCurrentIndexChanged:  {
            selfCurrentIndexChanged(currentIndex);
        }
    }
}
