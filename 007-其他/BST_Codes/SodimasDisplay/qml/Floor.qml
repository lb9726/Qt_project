import QtQuick 1.1

UiElement {

    property string baiImage: ""
    property string shiImage: ""
    property string geImage: ""
    clip: true

    MouseArea{
        anchors.fill: parent
//        onClicked: {
//            setFloor("",
//                     "file:///C:/Users/lishaoxiang.SHBST/Desktop/theme/sodimas/floor/T50.png",
//                     "file:///C:/Users/lishaoxiang.SHBST/Desktop/theme/sodimas/floor/T51.png")
//        }
        onClicked: {
            setFloor("",
                     "file:///home/libo/Desktop/Repository/Qt_project/BST_Codes/SodimasDisplay/theme/sodimas/floor/T50.png",
                     "file:///home/libo/Desktop/Repository/Qt_project/BST_Codes/SodimasDisplay/theme/sodimas/floor/T51.png")
        }
    }

    Row{
        anchors.fill: parent
        Image{
          id:id_floor_bai
          height: parent.height
          width: parent.width/3
          fillMode: Image.PreserveAspectFit
          source: baiImage
          opacity: shiImage == "" ? 0 : 1
        }
        Image{
          id:id_floor_shi
          height: parent.height
          width: parent.width/3
          fillMode: Image.PreserveAspectFit
          source: shiImage
          opacity: shiImage == "" ? 0 : 1
        }
        Image{
          id:id_floor_ge
          height: parent.height
          width: parent.width/3
          fillMode: Image.PreserveAspectFit
          source: geImage
          opacity: geImage == "" ? 0 : 1
        }
    }

    function setFloor(ge,shi,bai){
        baiImage = bai;
        shiImage = shi;
        geImage = ge;
    }
}
