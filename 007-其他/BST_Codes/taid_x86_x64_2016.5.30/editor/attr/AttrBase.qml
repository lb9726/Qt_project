import QtQuick 2.5

Item {
    id:root
    anchors.fill: parent
    property string elementName: ""
    signal sig_attr_resize(int x,int y,int w,int h)

    Column{
        id:base_area
        Text{
            id:testpanel
            width: root.width
            height: root.height/10
            text:elementName
            font.family:"微软雅黑"
            color: "white"
            font.pixelSize: height/2
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Rectangle{
            width: root.width
            height:2
            opacity: 0.5
        }

        Row{
            width: root.width
            height:40
            Item{
                width: parent.width/10
                height: parent.height
            }

            Text{
                width: parent.width*2/5
                height: parent.height
                text:"x : "
                font.family:"微软雅黑"
                color: "white"
                //horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Rectangle{
                width: parent.width*2/5 -10
                height: 30
                anchors.verticalCenter: parent.verticalCenter
                TextInput{
                    id:pro_x
                    font.family:"微软雅黑"
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    maximumLength:4
                    //inputMask:"9999"
                    Keys.onEnterPressed: {
                        paraChange();
                    }
                    onFocusChanged: {
                        paraChange();
                    }
                }
            }
        }
        Row{
            width: root.width
            height:40
            //spacing: 10
            Item{
                width: parent.width/10
                height: parent.height
            }
            Text{
                font.family:"微软雅黑"
                width: parent.width*2/5
                height: parent.height
                text:"y : "
                color: "white"
                //horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Rectangle{
                width: parent.width*2/5 -10
                height: 30
                anchors.verticalCenter: parent.verticalCenter
                TextInput{
                    id:pro_y
                    anchors.fill: parent
                    font.family:"微软雅黑"
                    verticalAlignment: Text.AlignVCenter
                    maximumLength:4
                    //inputMask:"9999"
                    Keys.onEnterPressed: {
                        paraChange();
                    }
                    onFocusChanged: {
                        paraChange();
                    }
                }
            }
        }
        Row{
            width: root.width
            height:40
            //spacing: 10
            Item{
                width: parent.width/10
                height: parent.height
            }
            Text{
                font.family:"微软雅黑"
                width: parent.width*2/5
                height: parent.height
                text:"width : "
                color: "white"
                //horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Rectangle{
                width: parent.width*2/5 -10
                height: 30
                anchors.verticalCenter: parent.verticalCenter
                TextInput{
                    id:pro_width
                    anchors.fill: parent
                    font.family:"微软雅黑"
                    verticalAlignment: Text.AlignVCenter
                    maximumLength:4
                    //inputMask:"9999"
                    Keys.onEnterPressed: {
                        paraChange();
                    }
                    onFocusChanged: {
                        paraChange();
                    }
                }
            }
        }
        Row{
            width: root.width
            height:40
            //spacing: 10
            Item{
                width: parent.width/10
                height: parent.height
            }
            Text{
                width: parent.width*2/5
                height: parent.height
                font.family:"微软雅黑"
                text:"height : "
                color: "white"
                //horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Rectangle{
                width: parent.width*2/5 -10
                height: 30
                anchors.verticalCenter: parent.verticalCenter

                TextInput{
                    id:pro_height
                    anchors.fill: parent
                    font.family:"微软雅黑"
                    verticalAlignment: Text.AlignVCenter
                    maximumLength:4
                    //inputMask:"9999"
                    Keys.onEnterPressed: {
                        paraChange();
                    }
                    onFocusChanged: {
                        paraChange();
                    }
                }
            }
        }
    }

    function paraChange(){
        var pX = pro_x.text
        var pY = pro_y.text
        var pWidth = pro_width.text
        var pHeight = pro_height.text
        if(pX==0||pY==0||pWidth==0||pHeight==0){
            return;
        }else{
            sig_attr_resize(pX,pY,pWidth,pHeight);
        }


    }
    function setcord(xcord,ycord){
        pro_x.text = xcord
        pro_y.text = ycord
    }

    function setsize(w,h){
        pro_width.text = w
        pro_height.text = h
    }
}

