
WorkerScript.onMessage = function(msg) {
    var model = msg[0].action
    for(var i = 1;i<msg.length;i++){
        model.append({'resource': msg[i].action, 'indexNumber': msg[i].index,'tipName':getFileName(msg[i].action)});
    }
    model.sync();
}

function getFileName(path){
    var pos = path.lastIndexOf("/");
    if(pos < 0)
        pos = path.lastIndexOf("\\")
    if(pos >= 0)
        return path.substring(pos+1);
    else
        return "";
}
