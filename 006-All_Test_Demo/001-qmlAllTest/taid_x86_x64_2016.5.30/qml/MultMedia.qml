import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2
import QtQml.Models 2.1
import QtQuick.Dialogs 1.2
import QtMultimedia 5.5

Item {
    id: root
    property int tabindex: 0
    property alias nameFilter : fileDialog.nameFilters
    property int pcurrentIndex : 0
    property var img_array: new Array
    property var video_array: new Array
    property var music_array: new Array

    Button {
        width: windowWidth/20 > 50 ? 50: windowWidth/20
        height: windowWidth/20 > 50 ? 50: windowWidth/20
        opacity: pressed ? themeOpacity : 1

        anchors {
            left: parent.left
            top: parent.top
            margins: windowWidth > windowHeight  ? windowHeight/20 : windowWidth/20
        }

        style: ButtonStyle {
            background: Image {
                source: "qrc: /images/back.png"
            }

        }

        onClicked: {
            uiLogicalEngine(7,0)
        }
    }

    Button {
        id: bt_publish
        width: windowWidth/20 > 50 ? 50: windowWidth/20
        height: windowWidth/20 > 50 ? 50: windowWidth/20

        opacity: pressed ? themeOpacity : 1

        anchors {
            right: parent.right
            top: parent.top
            margins: windowWidth > windowHeight  ? windowHeight/20 : windowWidth/20
        }

        style: ButtonStyle {
            background: Image {
                source: "qrc: /images/publish.png"
            }

        }

        onClicked: {
            uiLogicalEngine(7,1)
        }
    }

    Column {
        width: parent.width*4/5
        height: parent.height*5/6
        anchors.centerIn: parent

        Column {
            width: parent.width
            height: parent.height/6

            Row {
                width: parent.width
                height: parent.height*3/4

                Item {
                    width: parent.width/3
                    height: parent.height
                    Rectangle {
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        color: viceColor
                        Text {
                            text: "图片"
                            color: "white"
                            font.pixelSize: parent.height/4
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                tabSelect(0)
                            }

                        }
                    }
                }

                Item {
                    width: parent.width/3
                    height: parent.height
                    Rectangle {
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        color: viceColor

                        Text {
                            text: "视频"
                            color: "white"
                            font.pixelSize: parent.height/4
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                tabSelect(1)
                            }
                        }
                    }
                }

                Item {
                    width: parent.width/3
                    height: parent.height
                    Rectangle {
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        color: viceColor
                        Text {
                            text: "语音报站"
                            color: "white"
                            font.pixelSize: parent.height/4
                            anchors.centerIn: parent
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                tabSelect(2)
                            }
                        }
                    }
                }
            }

            Item {
                width: 30
                height: parent.height/4
                Image {
                    width: parent.width
                    height: parent.height/2
                    anchors.centerIn: parent
                    source: "qrc: /images/trangle.png"
                }
                x: parent.width*(2*tabindex+1)/6-width/2
                Behavior on x { SpringAnimation {spring: 2; damping: 0.2 }}
            }
        }

        Loader {
            id: multmedia_OperateLoader
            width: parent.width
            height: parent.height*5/6
        }
    }

    Column {
        anchors.verticalCenter: parent.verticalCenter
        anchors.right:  parent.right
        anchors.rightMargin: windowWidth/40
        spacing: windowWidth/20
        Repeater {
            model: ["add","remove","clear"]
            Button {
                width: windowWidth/25 > 50 ? 50: windowWidth/25
                height: windowWidth/25 > 50 ? 50: windowWidth/25
                opacity: pressed ? themeOpacity : 1

                style: ButtonStyle {
                    background: Image {
                        source: "qrc: /images/"+modelData+".png"
                    }
                }

                onClicked: {
                    mediaOperat(index+1)
                }
            }
        }
    }

    Item {
        id: id_preview
        anchors.fill: parent
        z: 100
        Rectangle {
            anchors.fill: parent
            color: "black"
            z: -1
            MouseArea {
                anchors.fill: parent
                onDoubleClicked: {
                    id_preview.visible = false;
                    id_previewlist.model = null
                }
            }
        }
        ListView {
            id: id_previewlist
            width: parent.width*4/5
            height: parent.height*4/5
            anchors.centerIn: parent
            clip: true
            delegate: Image {
                width: id_previewlist.width
                height: id_previewlist.height
                source: modelData
            }
            orientation: ListView.Horizontal
            snapMode: ListView.SnapOneItem
            highlightRangeMode: ListView.StrictlyEnforceRange
            cacheBuffer: 3
            boundsBehavior: Flickable.StopAtBounds
            highlightFollowsCurrentItem: true
        }
        visible: false
    }

    Item {
        id: id_videoPlay
        anchors.fill: parent
        z: 100
        Rectangle {
            anchors.fill: parent
            color: "black"
            z: -1
            MouseArea {
                anchors.fill: parent
                onDoubleClicked: {
                    id_videoPlay.visible = false;
                    id_mediaplayer.stop();
                }
            }
        }

        MediaPlayer {
            id: id_mediaplayer
            autoLoad: false
        }

        VideoOutput {
            width: parent.width*4/5
            height: parent.height*4/5
            //anchors.fill: parent
            anchors.centerIn: parent
            source: id_mediaplayer
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if(id_mediaplayer.playbackState == 1)
                        id_mediaplayer.pause();
                    else if(id_mediaplayer.playbackState == 2)
                        id_mediaplayer.play();
                    else
                        id_mediaplayer.play();
                }
            }
        }
        visible: false
    }

    function showPreview(index) {
        //console.log(index);
        id_previewlist.model = img_array
        id_previewlist.forceLayout();
        id_previewlist.currentIndex = index
        id_preview.visible = true;
    }

    function playVideo(path) {
        id_mediaplayer.source = path
        id_mediaplayer.play();
        id_videoPlay.visible = true;
    }

    Component {
        id: img_panel
        Rectangle {
            color: mainColor
            opacity: themeOpacity
            Text {
                anchors.centerIn: parent
                text: "pls select the img."
                color: "white"
                font.pixelSize: 30
                visible: id_imgView.count>0 ? false : true
            }

            GridView {
                id: id_imgView
                anchors.fill: parent
                cellWidth: parent.width/8
                cellHeight: parent.width/8

                displaced: Transition {
                    NumberAnimation { properties: "x,y"; easing.type: Easing.OutQuad }
                }

                model: DelegateModel {
                    id: id_img_delegateModel
                    model: id_img_model
                    delegate: MouseArea {
                        id: img_delegateRoot
                        property int visualIndex: DelegateModel.itemsIndex
                        onClicked: {
                            id_imgView.currentIndex = visualIndex;
                        }
                        onDoubleClicked: {
                            showPreview(index);
                        }

                        width: id_imgView.cellWidth; height: id_imgView.cellHeight
                        drag.target: img_icon

                        Image {
                            id: img_icon
                            width: id_imgView.cellWidth*3/4; height: id_imgView.cellHeight*3/4
                            anchors {
                                horizontalCenter: parent.horizontalCenter;
                                verticalCenter: parent.verticalCenter
                            }
                            source: resource

                            Drag.active: img_delegateRoot.drag.active
                            Drag.source: img_delegateRoot
                            Drag.hotSpot.x: width/2
                            Drag.hotSpot.y: height/2

                            states: [
                                State {
                                    when: img_icon.Drag.active
                                    ParentChange {
                                        target: img_icon
                                        parent: root
                                    }

                                    AnchorChanges {
                                        target: img_icon;
                                        anchors.horizontalCenter: undefined;
                                        anchors.verticalCenter: undefined
                                    }
                                }
                            ]
                        }

                        DropArea {
                            anchors { fill: parent; margins: 15 }
                            onEntered: {
                                id_img_delegateModel.items.move(drag.source.visualIndex, img_delegateRoot.visualIndex)
                                getImgList();
                            }
                        }
                    }
                }
                highlightFollowsCurrentItem: true
                snapMode: GridView.SnapToRow
                highlight: Rectangle {
                    opacity: 0.2
                    width: id_imgView.cellWidth; height: id_imgView.cellHeight
                    color: "red"; radius: 5
                    x: id_imgView.currentItem.x
                    y: id_imgView.currentItem.y
                    Behavior on x { NumberAnimation { easing.type: Easing.OutQuad } }
                    Behavior on y { NumberAnimation { easing.type: Easing.OutQuad } }
                }

                onCurrentIndexChanged: {
                    pcurrentIndex = currentIndex
                }
            }

            function getImgList() {
                img_array = new Array;
                for(var i =0;i<id_img_delegateModel.count;i++) {
                    img_array.push(id_img_delegateModel.items.get(i).model.resource)
                }
            }

            Component.onCompleted: {
                //console.log("Img Panel")
            }
        }
    }

    Component {
        id: video_panel
        Rectangle {
            color: mainColor
            opacity: themeOpacity
            Text {
                anchors.centerIn: parent
                text: "pls select the video."
                color: "white"
                font.pixelSize: 30
                visible: id_videoView.count>0 ? false : true
            }
            GridView {
                id: id_videoView
                anchors.fill: parent
                cellWidth: parent.width/4
                cellHeight: parent.width/4

                displaced: Transition {
                    NumberAnimation { properties: "x,y"; easing.type: Easing.OutQuad }
                }

                model: DelegateModel {
                    id: id_video_delegateModel
                    model: id_video_model
                    delegate: MouseArea {
                        id: delegateRoot
                        property int visualIndex: DelegateModel.itemsIndex
                        onClicked: {
                            id_videoView.currentIndex = visualIndex;
                        }
                        onDoubleClicked: {
                            playVideo(resource);
                        }

                        clip: true
                        width: id_videoView.cellWidth; height: id_videoView.cellHeight
                        drag.target: video_icon

                        Item {
                            id: video_icon
                            width: id_videoView.cellWidth*3/4; height: id_videoView.cellHeight*3/4
                            anchors {
                                horizontalCenter: parent.horizontalCenter;
                                top: parent.top
                            }

                            Image {
                                id: coverImg
                                anchors.fill: parent
                                source: "qrc: /images/video.png"
                                z: -1

                                SequentialAnimation {
                                    running: id_videoView.currentIndex == index ? true : false
                                    loops: Animation.Infinite
                                    NumberAnimation { target: coverImg; property: "opacity"; to: 0.2; duration: 1000 }
                                    NumberAnimation { target: coverImg; property: "opacity"; to: 1; duration: 1000 }
                                }
                            }

                            Drag.active: delegateRoot.drag.active
                            Drag.source: delegateRoot
                            Drag.hotSpot.x: width/2
                            Drag.hotSpot.y: height/2
                            states: [
                                State {
                                    when: video_icon.Drag.active
                                    ParentChange {
                                        target: video_icon
                                        parent: root
                                    }
                                    AnchorChanges {
                                        target: video_icon;
                                        anchors.horizontalCenter: undefined;
                                        anchors.top: undefined
                                    }
                                }
                            ]
                        }

                        Text {
                            id: id_videoName
                            width: id_videoView.cellWidth; height: id_videoView.cellHeight/4
                            anchors.bottom: parent.bottom
                            text: tipName
                            font.pointSize: 20;//id_videoName.height/2
                            color: "white"
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: id_videoName.contentWidth > id_videoView.cellWidth ? Text.AlignLeft: Text.AlignHCenter
                            NumberAnimation {
                                id: animText
                                target: id_videoName
                                property: "x"
                                duration: 5000
                                from: 0
                                to: -(id_videoName.contentWidth - id_videoView.cellWidth )
                                running: id_videoName.contentWidth > id_videoView.cellWidth && id_videoView.currentIndex == index ? true: false
                                loops: Animation.Infinite
                                onStopped: {
                                    id_videoName.x = 0;
                                }
                            }
                        }

                        DropArea {
                            anchors { fill: parent; margins: 15 }
                            onEntered: {
                                id_video_delegateModel.items.move(drag.source.visualIndex, delegateRoot.visualIndex)
                                getVideoList();
                            }
                        }
                        Component.onCompleted: {
                            //id_videoName.text = resource
                        }
                    }
                }
                highlightFollowsCurrentItem: true
                snapMode: GridView.SnapToRow
                highlight: Rectangle {
                    opacity: 0.2
                    width: id_videoView.cellWidth; height: id_videoView.cellHeight
                    color: "red"; radius: 5
                    x: id_videoView.currentItem.x
                    y: id_videoView.currentItem.y
                    Behavior on x { NumberAnimation { easing.type: Easing.OutQuad } }
                    Behavior on y { NumberAnimation { easing.type: Easing.OutQuad } }
                }

                onCurrentIndexChanged: {
                    pcurrentIndex = currentIndex
                }
            }

            function getVideoList() {
                video_array = []
                for(var i =0;i<id_video_delegateModel.count;i++) {
                    video_array.push(id_video_delegateModel.items.get(i).model.resource)
                }
                //console.log(video_array)
            }
            Component.onCompleted: {
                //getVideoList();
            }
        }
    }

    Component {
        id: music_panel
        Rectangle {
            color: mainColor
            opacity: themeOpacity
            Text {
                anchors.centerIn: parent
                text: "pls select the music."
                color: "white"
                font.pixelSize: 30
                visible: id_musicView.count>0 ? false : true
            }
            GridView {
                id: id_musicView
                anchors.fill: parent
                cellWidth: parent.width/2
                cellHeight: 40
                displaced: Transition {
                    NumberAnimation { properties: "x,y"; easing.type: Easing.OutQuad }
                }

                model: DelegateModel {
                    id: id_music_delegateModel
                    model: id_music_model
                    delegate: MouseArea {
                        id: music_delegateRoot
                        property int visualIndex: DelegateModel.itemsIndex
                        onClicked: {
                            id_musicView.currentIndex = visualIndex;
                        }
                        onDoubleClicked: {
                            playMusic(resource)
                        }

                        onPressAndHold: {
                            sampleMusicPlayer.stop();
                        }

                        width: id_musicView.cellWidth; height: id_musicView.cellHeight
                        drag.target: music_icon

                        Item {
                            id: music_icon
                            width: id_musicView.cellWidth; height: id_musicView.cellHeight
                            anchors {
                                horizontalCenter: parent.horizontalCenter;
                                verticalCenter: parent.verticalCenter
                            }
                            clip: true
                            Item {
                                height: id_musicView.cellHeight
                                width: id_musicView.cellWidth/4
                                Image {
                                    height: parent.height*3/4
                                    width: parent.height*3/4
                                    anchors.margins: 4
                                    anchors.centerIn: parent
                                    //anchors.verticalCenter: parent.verticalCenter
                                    source: "qrc: /images/icon/music_logo.png"
                                }
                            }


                            Text {
                                id: id_musicName
                                width: id_musicView.cellWidth*3/4; height: id_musicView.cellHeight
                                anchors.right: parent.right
                                text: tipName
                                font.pointSize: 14;
                                color: "white"
                                verticalAlignment: Text.AlignVCenter

                                NumberAnimation {
                                    target: id_musicName
                                    property: "x"
                                    duration: 5000
                                    from: 0
                                    to: -(id_musicName.contentWidth - id_musicView.cellWidth )
                                    running: id_musicName.contentWidth > id_musicView.cellWidth ? true: false
                                    loops: Animation.Infinite
                                }
                            }

                            Drag.active: music_delegateRoot.drag.active
                            Drag.source: music_delegateRoot
                            Drag.hotSpot.x: width/2
                            Drag.hotSpot.y: height/2

                            states: [
                                State {
                                    when: music_icon.Drag.active
                                    ParentChange {
                                        target: music_icon
                                        parent: root
                                    }

                                    AnchorChanges {
                                        target: music_icon;
                                        anchors.horizontalCenter: undefined;
                                        anchors.verticalCenter: undefined
                                    }
                                }
                            ]
                        }

                        DropArea {
                            anchors { fill: parent; margins: 15 }
                            onEntered: {
                                id_music_delegateModel.items.move(drag.source.visualIndex, music_delegateRoot.visualIndex)
                                getMusicList();
                            }
                        }
                    }
                }
                highlightFollowsCurrentItem: true
                snapMode: GridView.SnapToRow
                highlight: Rectangle {
                    opacity: 0.2
                    width: id_musicView.cellWidth; height: id_musicView.cellHeight
                    color: "red"; radius: 5
                    x: id_musicView.currentItem.x
                    y: id_musicView.currentItem.y
                    Behavior on x { NumberAnimation { easing.type: Easing.OutQuad } }
                    Behavior on y { NumberAnimation { easing.type: Easing.OutQuad } }
                }

                onCurrentIndexChanged: {
                    pcurrentIndex = currentIndex
                }
            }

            function getMusicList() {
                music_array = []
                for(var i =0;i<id_music_delegateModel.count;i++) {
                    music_array.push(id_music_delegateModel.items.get(i).model.resource)
                }

            }
            Component.onCompleted: {
                //getMusicList();
            }
        }
    }

    ListModel {
        id: id_img_model
        //        ListElement {resource: "qrc: /images/icon/1.png";indexNumber: 0;tipName: ""}
        //        ListElement {resource: "qrc: /images/icon/2.png";indexNumber: 0;tipName: ""}
        //        ListElement {resource: "qrc: /images/icon/3.png";indexNumber: 0;tipName: ""}
        //        ListElement {resource: "qrc: /images/icon/4.png";indexNumber: 0;tipName: ""}
        //        ListElement {resource: "qrc: /images/icon/5.png";indexNumber: 0;tipName: ""}
        //        ListElement {resource: "qrc: /images/icon/6.png";indexNumber: 0;tipName: ""}
        //        ListElement {resource: "qrc: /images/icon/7.png";indexNumber: 0;tipName: ""}
    }

    ListModel {
        id: id_video_model
        //        ListElement {resource: "file: ///home/lisx/Desktop/GUI demo_1.mp4";indexNumber: 0;tipName: ""}
        //        ListElement {resource: "file: ///home/lisx/Desktop/GUI demo_2.mp4";indexNumber: 0;tipName: ""}
        //        ListElement {resource: "file: ///home/lisx/Desktop/GUI demo_3.mp4";indexNumber: 0;tipName: ""}
        //        ListElement {resource: "file: ///home/lisx/Desktop/GUI demo_4.mp4";indexNumber: 0;tipName: ""}
        //        ListElement {resource: "file: ///home/lisx/Desktop/GUI demo_5.mp4";indexNumber: 0;tipName: ""}
    }

    ListModel {
        id: id_music_model
        //        ListElement {resource: "file: ///home/lisx/Desktop/mp3/CH_1.mp3";indexNumber: 0;tipName: ""}
        //        ListElement {resource: "file: ///home/lisx/Desktop/mp3/CH_2.mp3";indexNumber: 0;tipName: ""}
        //        ListElement {resource: "file: ///home/lisx/Desktop/mp3/CH_3.mp3";indexNumber: 0;tipName: ""}
        //        ListElement {resource: "file: ///home/lisx/Desktop/mp3/CH_4.mp3";indexNumber: 0;tipName: ""}
    }

    WorkerScript {
        id: myWorker
        source: "qrc: /js/ViewWorker.js"
        onMessage: {
            //console.log(messageObject.reply);
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        //folder: shortcuts.home
        selectMultiple: true
        onAccepted: {
            var i = 0;
            var fileArray = new Array;
            if(tabindex == 0) {
                img_array = []
                fileArray[0] = {'action': id_img_model, "index": 0}
                for( i=0;i<fileDialog.fileUrls.length;i++)
                {
                    fileArray[i+1] = {'action': fileDialog.fileUrls[i],"index": i}
                    img_array.push(fileDialog.fileUrls[i]);
                }
                myWorker.sendMessage(fileArray);
            }else if(tabindex == 1) {
                video_array = []
                fileArray[0] = {'action': id_video_model, "index": 0}
                for( i=0;i<fileDialog.fileUrls.length;i++)
                {
                    fileArray[i+1] = {'action': fileDialog.fileUrls[i],"index": i}
                    video_array.push(fileDialog.fileUrls[i]);
                }
                myWorker.sendMessage(fileArray);
            }else if(tabindex == 2) {
                music_array =[]
                fileArray[0] = {'action': id_music_model, "index": 0}
                for( i=0;i<fileDialog.fileUrls.length;i++)
                {
                    fileArray[i+1] = {'action': fileDialog.fileUrls[i],"index": i}
                    music_array.push(fileDialog.fileUrls[i]);
                }
                myWorker.sendMessage(fileArray);
            }else {

            }
            fileDialog.nameFilters =[]
        }
    }

    Audio {
        id: sampleMusicPlayer
    }

    Component.onCompleted: {
        tabSelect(0)
    }

    function tabSelect(index) {
        switch(index) {
        case 0:
            multmedia_OperateLoader.sourceComponent = img_panel
            tabindex = 0
            break;
        case 1:
            multmedia_OperateLoader.sourceComponent = video_panel
            tabindex = 1
            break;
        case 2:
            multmedia_OperateLoader.sourceComponent = music_panel
            tabindex = 2
            break;
        default:
            return
        }
    }

    function playMusic(resource) {
        sampleMusicPlayer.source = resource;
        sampleMusicPlayer.play();
    }

    function mediaOperat(op_type) {
        switch(op_type) {
        case 1:
            add_media();
            break;
        case 2:
            delete_media();
            break;
        case 3:
            clear_media();
            break;
        default:
            return;
        }
    }

    function add_media() {
        if(tabindex == 0)
            fileDialog.nameFilters = ["Image files (*.jpg *.png)", "All files (*)"]
        else if(tabindex == 1)
            fileDialog.nameFilters = ["Video files (*.mp4 *.rmvb *.avi)", "All files (*)"]
        else if(tabindex == 2)
            fileDialog.nameFilters = ["Audio files (*.wav *.mp3)", "All files (*)"]
        else
            return
        fileDialog.open();
    }

    function delete_media() {
        if(pcurrentIndex <0 )
            return;
        if(tabindex == 0 &&(pcurrentIndex < id_img_model.count)) {
            id_img_model.remove(pcurrentIndex);
        }else if(tabindex == 1 &&(pcurrentIndex < id_video_model.count)) {
            id_video_model.remove(pcurrentIndex);
        }else if(tabindex == 2 &&(pcurrentIndex < id_music_model.count)) {
            id_music_model.remove(pcurrentIndex);
        }else
            return
    }

    function clear_media() {
        if(tabindex == 0 ) {
            id_img_model.clear();
        }else if(tabindex == 1) {
            id_video_model.clear();
        }else if(tabindex == 2) {
            id_music_model.clear();
        }else
            return
    }

    Timer {
        interval: 1000; running: true; repeat: false
        onTriggered: pullResource()
    }

    function pullResource() {
        var i = 0;
        var mediaArray = new Array;
        var pictureArray = new Array;
        var videoArray = new Array;
        var res = GUiEditor.pullResource("Media");

        mediaArray[0] = {'action': id_music_model, "index": 0}
        music_array = []
        for( i=0;i<res.length;i++)
        {
            mediaArray[i+1] = {'action': res[i],"index": i}
            music_array.push(res[i]);
        }
        myWorker.sendMessage(mediaArray);

        res = GUiEditor.pullResource("Picture");
        img_array = []
        pictureArray[0] = {'action': id_img_model, "index": 0}
        for( i=0;i<res.length;i++)
        {
            pictureArray[i+1] = {'action': res[i],"index": i}
            img_array.push(res[i])
        }
        myWorker.sendMessage(pictureArray);

        res = GUiEditor.pullResource("Video");
        video_array = []
        videoArray[0] = {'action': id_video_model, "index": 0}
        for( i=0;i<res.length;i++)
        {
            videoArray[i+1] = {'action': res[i],"index": i}
            video_array.push(res[i]);
        }
        myWorker.sendMessage(videoArray);
    }

    Button {
        width: 50
        height: 50
        onClicked: {
            pushResource ();
        }
    }

    function pushResource() {
        GUiEditor.pushResource("Media",music_array);
        GUiEditor.pushResource("Picture",img_array);
        GUiEditor.pushResource("Video",video_array);
    }
}
