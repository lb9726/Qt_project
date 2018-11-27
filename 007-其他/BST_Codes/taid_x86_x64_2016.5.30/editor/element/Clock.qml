import QtQuick 2.5
import "../editor.js" as Editor

Element{
    id:id_Colock
    property string dateString: "2015-10-25"
    property string timeString: "19:20:20"
    property bool l_ShowSecond: panelAttr.m_ShowSecond  //bool
    property string l_DateStyle:panelAttr.m_DateStyle

    property string l_dResPath:panelAttr.dResPath
    property string l_tResPath:panelAttr.tResPath
    property bool l_editable:panelAttr.m_editable

    onL_DateStyleChanged: {
        paraClockDate();
    }

    function paraClockDate(){
        var date = new Date()
        var mt = ((date.getMonth()+1) >9 ? (date.getMonth()+1):("0"+(date.getMonth()+1)))
        var d = (date.getDate() >9 ? date.getDate():("0"+(date.getDate())));

        if(l_DateStyle=="yyyy-MM-dd"){
            dateString = date.getFullYear()+"-"+mt+"-"+d;
        }else if(l_DateStyle == "yyyy-dd-MM"){
            dateString = date.getFullYear()+"-"+d+"-"+mt;
        }else{
        }

        paraData(dateString);

    }

    function paraClockTime(){
        var date = new Date()
        var h = date.getHours() > 9 ? date.getHours(): ("0"+date.getHours())
        var m = date.getMinutes() > 9 ? date.getMinutes(): ("0"+date.getMinutes())
        var s = date.getSeconds() > 9 ? date.getSeconds(): ("0"+date.getSeconds())
        timeString = h+":"+m+":"+s;
        paraTime(timeString);
    }

    Timer{
        interval: 1000
        running: true
        repeat: true
        onTriggered:{
            paraClockTime();
            paraClockDate();
        }
    }

    function paraData(dateStr){
        if(dateStr.length != 10)
            return
        //console.log(dateStr)
        for(var i=0;i<4;i++){
            var charValue = parseInt(dateStr.charAt(i));
            id_dateShow.set(i,{"pPath":id_dateRes.get(charValue).kFront})
        }

        id_dateShow.set(4,{"pPath":id_dateRes.get(10).kFront})
        id_dateShow.set(7,{"pPath":id_dateRes.get(10).kFront})

        for(i=5;i<7;i++){
            charValue = parseInt(dateStr.charAt(i));
            id_dateShow.set(i,{"pPath":id_dateRes.get(charValue).kFront})
        }

        for(i=8;i<10;i++){
            charValue = parseInt(dateStr.charAt(i));
            id_dateShow.set(i,{"pPath":id_dateRes.get(charValue).kFront})
        }

        //        for(var j=0;j<id_dateShow.count;j++){
        //            var sdf = id_dateShow.get(j);
        //            console.log(sdf.pPath)
        //        }
    }

    function paraTime(timeString){
        if(timeString.length != 8)
            return

        for(var i=0;i<2;i++){
            var charValue = parseInt(timeString.charAt(i));
            id_timeShow.set(i,{"pPath":id_timeRes.get(charValue).kFront})
        }

        id_timeShow.set(2,{"pPath":id_timeRes.get(10).kFront})
        id_timeShow.set(5,{"pPath":id_timeRes.get(10).kFront})

        for(i=3;i<5;i++){
            charValue = parseInt(timeString.charAt(i));
            id_timeShow.set(i,{"pPath":id_timeRes.get(charValue).kFront})
        }

        for(i=6;i<8;i++){
            charValue = parseInt(timeString.charAt(i));
            id_timeShow.set(i,{"pPath":id_timeRes.get(charValue).kFront})
        }
    }

    property int timeWidth: l_ShowSecond ? id_timeArea.width/14 : id_timeArea.width/9

    Item{
        id:id_clockArea
        clip:true
        anchors.fill: parent
        z:10
        enabled: {
            if(!selecteFlag || !elementFocus ||!l_editable || previewModel){
                return false;
            }
            return true;
        }

        Item {
            id:id_timeArea
            Drag.active: dragArea.drag.active
            Drag.hotSpot.x: id_timeArea.width/2
            Drag.hotSpot.y: id_timeArea.height/2

            ListView{
                clip: true
                enabled: false
                orientation:ListView.Horizontal
                anchors.fill: parent
                model: id_timeShow
                delegate: Image {
                    height: parent.height
                    width: {
                        if(index == 2||index == 5)
                            return timeWidth
                        else
                            return timeWidth*2
                    }
                    source: pPath == "" ? "" :l_tResPath + pPath
                }
            }

            Item{
                width: id_timeArea.width
                height: id_timeArea.height
                visible:{
                    if(!selecteFlag || !elementFocus ||!l_editable || previewModel){
                        return false;
                    }
                    return true;
                }

                Rectangle{
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: -3
                    width: 6
                    height: 6
                    color: "blue"
                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled:true
                        property int tempMousex: 0;

                        onPressed: {
                            tempMousex = mouse.x
                        }

                        onPositionChanged:{
                            if(pressed){
                                ctrl_tx(mouse.x - tempMousex);
                            }

                        }

                        onEntered: {
                            cursorShape = Qt.SizeHorCursor
                        }
                    }
                }

                Rectangle{
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: -3
                    width: 6
                    height: 6
                    color: "blue"
                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled:true
                        property int tempMousey: 0;

                        onPressed: {
                            tempMousey = mouse.y
                        }
                        onPositionChanged:{
                            if(pressed){
                                ctrl_ty(mouse.y - tempMousey);
                            }
                        }
                        onEntered: {
                            cursorShape = Qt.SizeVerCursor
                        }
                    }
                }

                Rectangle{
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: -3
                    anchors.bottomMargin: -3
                    width: 6
                    height: 6
                    color: "blue"
                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled:true
                        property int tempMousex: 0;
                        property int tempMousey: 0;

                        onPressed: {
                            tempMousex = mouse.x
                            tempMousey = mouse.y
                        }

                        onPositionChanged:{
                            if(pressed){
                                ctrl_ty(mouse.y - tempMousey);
                                ctrl_tx(mouse.x - tempMousex);
                            }
                        }

                        onEntered: {
                            cursorShape = Qt.SizeFDiagCursor
                        }
                    }
                }
            }

            MouseArea{
                id:dragArea
                anchors.fill: parent
                drag.target: id_timeArea
                onClicked: {

                }
                z:-10
            }

            onXChanged: {
                if(x < 0){
                    x = 0;
                }

                if(x + width > parent.width){
                    x = parent.width - width;
                }
                if(x>=0 && x <= parent.width)
                    sig_tRect(Qt.rect(id_timeArea.x, id_timeArea.y, id_timeArea.width, id_timeArea.height));
            }

            onYChanged: {
                if(y < 0)
                    y = 0
                if(y+height > parent.height){
                    y = parent.height - height
                }
                if(y>=0 && x <= parent.height)
                    sig_tRect(Qt.rect(id_timeArea.x, id_timeArea.y, id_timeArea.width, id_timeArea.height));
            }

        }

        Item {
            id:id_dateArea
            Drag.active: dragArea2.drag.active
            Drag.hotSpot.x: id_timeArea.width/2
            Drag.hotSpot.y: id_timeArea.height/2
            ListView{
                clip: true
                enabled: false
                orientation:ListView.Horizontal
                anchors.fill: parent
                model: id_dateShow
                delegate: Image {
                    height: parent.height
                    width: {
                        if(index == 4||index == 7)
                            return id_dateArea.width/18
                        else
                            return id_dateArea.width/9
                    }
                    source: pPath == "" ? "" :l_dResPath + pPath
                }
            }

            Item{
                width: id_dateArea.width
                height: id_dateArea.height
                visible:{
                    if(!selecteFlag || !elementFocus ||!l_editable || previewModel){
                        return false;
                    }
                    return true;
                }

                Rectangle{
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: -3
                    width: 6
                    height: 6
                    color: "blue"
                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled:true
                        property int tempMousex: 0;

                        onPressed: {
                            tempMousex = mouse.x
                        }

                        onPositionChanged:{
                            if(pressed){
                                ctrl_dx(mouse.x - tempMousex);
                            }

                        }

                        onEntered: {
                            cursorShape = Qt.SizeHorCursor
                        }
                    }
                }

                Rectangle{
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: -3
                    width: 6
                    height: 6
                    color: "blue"
                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled:true
                        property int tempMousey: 0;

                        onPressed: {
                            tempMousey = mouse.y
                        }
                        onPositionChanged:{
                            if(pressed){
                                ctrl_dy(mouse.y - tempMousey);
                            }
                        }
                        onEntered: {
                            cursorShape = Qt.SizeVerCursor
                        }
                    }
                }

                Rectangle{
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: -3
                    anchors.bottomMargin: -3
                    width: 6
                    height: 6
                    color: "blue"
                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled:true
                        property int tempMousex: 0;
                        property int tempMousey: 0;

                        onPressed: {
                            tempMousex = mouse.x
                            tempMousey = mouse.y
                        }

                        onPositionChanged:{
                            if(pressed){
                                ctrl_dy(mouse.y - tempMousey);
                                ctrl_dx(mouse.x - tempMousex);
                            }
                        }

                        onEntered: {
                            cursorShape = Qt.SizeFDiagCursor
                        }
                    }
                }
            }

            MouseArea{
                id:dragArea2
                anchors.fill: parent
                drag.target: id_dateArea
                onClicked: {

                }
                z:-10
            }

            onXChanged: {
                if(x < 0){
                    x = 0;
                }
                if(x + width > parent.width){
                    x = parent.width - width;
                }
                if(x>=0 && x <= parent.width)
                    sig_dRect(Qt.rect(x,y,width,height));
            }

            onYChanged: {
                if(y < 0)
                    y = 0
                if(y+height > parent.height){
                    y= parent.height - height
                }
                if(y>=0 && x <= parent.height)
                    sig_dRect(Qt.rect(x,y,width,height));
            }
        }

        Image{
            anchors.fill: parent
            source: coverImag
            z:-10
        }

        MouseArea{
            anchors.fill: parent
            z:-1

        }
    }

    ListModel{
        id:id_timeShow
        ListElement{pPath:""}
        ListElement{pPath:""}
        ListElement{pPath:""}
        ListElement{pPath:""}
        ListElement{pPath:""}
        ListElement{pPath:""}
        ListElement{pPath:""}
        ListElement{pPath:""}
    }

    ListModel{
        id:id_dateShow
        ListElement{pPath:""}
        ListElement{pPath:""}
        ListElement{pPath:""}
        ListElement{pPath:""}
        ListElement{pPath:""}
        ListElement{pPath:""}
        ListElement{pPath:""}
        ListElement{pPath:""}
        ListElement{pPath:""}
        ListElement{pPath:""}
    }

    ListModel{
        id:id_dateRes
        ListElement{kIndex:"0";kFront:""}
        ListElement{kIndex:"1";kFront:""}
        ListElement{kIndex:"2";kFront:""}
        ListElement{kIndex:"3";kFront:""}
        ListElement{kIndex:"4";kFront:""}
        ListElement{kIndex:"5";kFront:""}
        ListElement{kIndex:"6";kFront:""}
        ListElement{kIndex:"7";kFront:""}
        ListElement{kIndex:"8";kFront:""}
        ListElement{kIndex:"9";kFront:""}
        ListElement{kIndex:"56";kFront:""}
    }

    ListModel{
        id:id_timeRes
        ListElement{kIndex:"0";kFront:""}
        ListElement{kIndex:"1";kFront:""}
        ListElement{kIndex:"2";kFront:""}
        ListElement{kIndex:"3";kFront:""}
        ListElement{kIndex:"4";kFront:""}
        ListElement{kIndex:"5";kFront:""}
        ListElement{kIndex:"6";kFront:""}
        ListElement{kIndex:"7";kFront:""}
        ListElement{kIndex:"8";kFront:""}
        ListElement{kIndex:"9";kFront:""}
        ListElement{kIndex:"56";kFront:""}
    }

    function slt_dgetRes(){
        var list = new Array

        list.push(l_dResPath);
        var tempString,curItem,i;
        for(i =0;i<id_dateRes.count;i++){
            curItem = id_dateRes.get(i);
            tempString = curItem.kFront;
            list.push(tempString);
        }
        return list;
    }
    function slt_dgetRect(){
        return Qt.rect(id_dateArea.x,id_dateArea.y,
                       id_dateArea.width,id_dateArea.height);
    }
    function slt_dsetRect(rectstring){
        var rect = Editor.getRectFromString(rectstring);
        //console.log(rect);
        if(rect != null){
            id_dateArea.x = rect.x;
            id_dateArea.y = rect.y;
            id_dateArea.width = rect.width;
            id_dateArea.height = rect.height;
            sig_dRect(rect);
        }
    }

    function slt_tgetRes(){
        var list = new Array

        list.push(l_tResPath);
        var tempString,curItem,i;
        for(i =0;i<id_timeRes.count;i++){
            curItem = id_timeRes.get(i);
            tempString = curItem.kFront;
            list.push(tempString);
        }
        return list;
    }
    function slt_tgetRect(){
        return Qt.rect(id_timeArea.x,id_timeArea.y,
                       id_timeArea.width,id_timeArea.height);
    }
    function slt_tsetRect(rectstring){
        var rect = Editor.getRectFromString(rectstring);
        //console.log(rect);
        if(rect != null){
            id_timeArea.x = rect.x;
            id_timeArea.y = rect.y;
            id_timeArea.width = rect.width;
            id_timeArea.height = rect.height;
            sig_tRect(rect)
        }

    }

    signal sig_tRect(rect rt);
    signal sig_dRect(rect rt);

    function ctrl_tx(deltX){
        deltX = Math.floor(deltX)
        if(deltX > 0){
            if(id_timeArea.x + id_timeArea.width + deltX <= parent.width)
                id_timeArea.width  += deltX
        }else{
            if((id_timeArea.width  + deltX) > 10){
                id_timeArea.width  += deltX
            }
        }
        sig_tRect(Qt.rect(id_timeArea.x, id_timeArea.y, id_timeArea.width, id_timeArea.height));
    }
    function ctrl_ty(deltY){
        deltY = Math.floor(deltY)
        if(deltY > 0){
            if(id_timeArea.y + id_timeArea.height  + deltY <= parent.height)
                id_timeArea.height  += deltY
        }else{
            if((id_timeArea.height  + deltY) > 10){
                id_timeArea.height  += deltY
            }
        }
        sig_tRect(Qt.rect(id_timeArea.x, id_timeArea.y, id_timeArea.width, id_timeArea.height));
    }

    function ctrl_dx(deltX){
        deltX = Math.floor(deltX)
        if(deltX > 0){
            if(id_dateArea.x + id_dateArea.width + deltX <= parent.width)
                id_dateArea.width  += deltX
        }else{
            if((id_dateArea.width  + deltX) > 10){
                id_dateArea.width  += deltX
            }
        }
        sig_dRect(Qt.rect(id_dateArea.x, id_dateArea.y, id_dateArea.width, id_dateArea.height));
    }
    function ctrl_dy(deltY){
        deltY = Math.floor(deltY)
        if(deltY > 0){
            if(id_dateArea.y + id_dateArea.height  + deltY <= parent.height)
                id_dateArea.height  += deltY
        }else{
            if((id_dateArea.height  + deltY) > 10){
                id_dateArea.height  += deltY
            }
        }
        sig_dRect(Qt.rect(id_dateArea.x, id_dateArea.y, id_dateArea.width, id_dateArea.height));
    }

    Component.onCompleted:{
        if(panelAttr != null)
        {
            id_Colock.sig_tRect.connect(panelAttr.slt_tRect);
            id_Colock.sig_dRect.connect(panelAttr.slt_dRect);
        }
    }

    function slt_tResource(fileList){
        for(var i = 0; i < fileList.length;i++){
            for(var j=0;j< id_timeRes.count;j++){
                var oPath= "t"+id_dateRes.get(j).kIndex +".png";
                var newPath = fileList[i];
                if(oPath == newPath.toLowerCase()){
                    id_timeRes.set(j,{kFront:fileList[i]});
                    break;
                }
            }
        }
    }

    function slt_dResource(fileList){
        for(var i = 0; i < fileList.length;i++){
            var newPath = fileList[i];
            for(var j=0;j< id_dateRes.count;j++){
                var oPath= "d"+id_dateRes.get(j).kIndex +".png";
                if(oPath == newPath.toLowerCase()){
                    id_dateRes.set(j,{kFront:newPath});
                    break;
                }
            }
        }
    }

}
