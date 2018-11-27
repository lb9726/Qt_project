var parentWindow = null;
var DragItemType = null;
var dragItem = null;
var startingMouse;
var posnInWindow;
var dragItemImagePath = "";

var c_paneltype = "";
var panelIndexNum = 0;
//var comobject = null;

function test(){
    console.log(dropArea)
}

function getRectFromString(rectString,spliter){
    if(!arguments[1])
        spliter = ",";
    var reclist = rectString.split(spliter);
    if(reclist.length !== 4)
        return null;
    else
        return Qt.rect(parseInt(reclist[0]),parseInt(reclist[1]),
                       parseInt(reclist[2]),parseInt(reclist[3]));
}

function directCreatePanel(panelName,mrectH,mrectV){
    c_paneltype = panelName;
    var component = Qt.createComponent("element/"+c_paneltype+".qml");
    if(component.status === Component.Ready)
    {
        var comobject = component.createObject(dropArea, {
                                                   "rectH":mrectH,
                                                   "rectV":mrectV,
                                                   "elementType":panelName
                                               });

        selectAction(c_paneltype,panelIndex);
        id_toolbox.createdPanel(c_paneltype);
        //console.log(panelName,mrectH,mrectV)
    }else{
        console.log(panelName,mrectH,mrectV)
    }
}

function switchHV(eleList){

}

function selectAction(Type,index){
    for(var item in dropArea.children){
        var elementItem = dropArea.children[item];
        if(elementItem == null){
            continue;
        }
        if(elementItem.elementType == null || elementItem.selecteFlag == null){
            continue;
        }
        if(Type == elementItem.elementType){
            elementItem.selecteFlag = true;
            elementItem.panelAttr.visible = true
        }else{
            elementItem.selecteFlag = false;
            elementItem.panelAttr.visible = false
        }
    }
}

function setPanelName(pName,index){
    panelIndex = index;
    switch(pName){
    case "Arrow":
    case "Floor":
    case "Function":
    case "Clock":
    case "Text":
    case "FlrBtn":
    case "CallBtn":
    case "KeyboardBtn":
    case "Media":
    case "Picture":
    case "Video":
    case "Title":
    case "Date":
    case "Time":
        c_paneltype = pName;
        break;
    default:
        c_paneltype = "";
    }
}

function panelIndex(mouse){

}

function destroyCreatedPanel(panelName){
    id_toolbox.destroyPanel(panelName);
    //if(GUiEditor)
        //GUiEditor.removeUIElement(panelName);
}

function init(parent, dragQmlFileUrl, dragImage) {
    if (parentWindow == null) {
        parentWindow = parent;
        DragItemType = Qt.createComponent(dragQmlFileUrl);
        dragItemImagePath = dragImage;
    }
}

function startDrag(mouse) {
    posnInWindow = paletteItem.mapToItem(parentWindow, mouse.x, mouse.y);
    startingMouse = {
        x: mouse.x,
        y: mouse.y
    }
    createDragItem();
}

function createDragItem() {
    if (DragItemType.status === Component.Loading) {
        component.statusChanged.connect(createItem);
    } else {
        createItem();
    }
}

function createItem() {
    if (DragItemType.status === Component.Ready && dragItem == null) {
        dragItem = DragItemType.createObject(
                    parentWindow,
                    {
                        "x": posnInWindow.x,
                        "y": posnInWindow.y,
                        "z": 3,
                        "image": dragItemImagePath,
                        "toHeight": parentWindow.y / 2
                    }
                    );
    } else if (DragItemType.status === Component.Error) {
        dragItem = null;
        console.log(DragItemType.errorString());
    }
}

function continueDrag(mouse) {
    if (dragItem == null)
        return;
    dragItem.x = mouse.x + posnInWindow.x - startingMouse.x;
    dragItem.y = mouse.y + posnInWindow.y - startingMouse.y;
}

function endDrag(mouse) {
    if (dragItem == null)
        return;

    if((c_paneltype != "")&&(dragItem.x > 0) && (dragItem.y >0) && (dragItem.x < parentWindow.width) && (dragItem.y < parentWindow.height))
    {

        var component = Qt.createComponent("element/"+c_paneltype+".qml");

        if (component.status === Component.Ready)
        {
            var mRectV,mRectH;
            var xCor,yCor;
            xCor = dragItem.x;
            yCor = dragItem.y;
            if(xCor > dropArea.width*2/3)
                xCor = dropArea.width*2/3
            if(yCor > dropArea.height*2/3)
                yCor = dropArea.height*2/3

            if(gLayoutOriental){
                mRectV = Qt.rect(xCor,yCor,dropArea.width/3,dropArea.height/3)
                mRectH = Qt.rect(0,0,dropArea.width/3,dropArea.height/3)
            }else{
                mRectH = Qt.rect(xCor,yCor,dropArea.width/3,dropArea.height/3)
                mRectV = Qt.rect(0,0,dropArea.width/3,dropArea.height/3)
            }

            var comobject = component.createObject(dropArea, {
                                                       "rectH":mRectH,
                                                       "rectV":mRectV,
                                                       "elementType":c_paneltype
                                                   });
            selectAction(c_paneltype,panelIndex);

            var ret = GUiEditor.createUICom(c_paneltype,mRectH,mRectV);
            if(!ret){
                comobject.destroy()
                console.log("Create Failed.")
            }else{
                id_toolbox.createdPanel(c_paneltype);
            }
        }
    }
    dragItem.destroy();
    dragItem = null;
}
