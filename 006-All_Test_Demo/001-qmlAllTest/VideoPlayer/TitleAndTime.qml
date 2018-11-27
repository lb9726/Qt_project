import QtQuick 2.0

Item
{
    property string strTime: ""
    property string strDate: ""
    Column
    {
        Text
        {
            id: time
            text: strTime
            color: "white"
            font.family: "KONE Information_v12"
            font.pixelSize: 56
            anchors.left: date.left
        }
        Text
        {
            id: date
            text: strDate
            color: "white"
            font.family: "KONE Information_v12"
            font.pixelSize: 30
        }
    }

    function getTimeAndDate()
    {
        strTime = GetTimeAndDate.getTimeFormat(false);
        strDate = GetTimeAndDate.getDateFormat(false);
    }

    Connections
    {
        target: GetTimeAndDate
        onSignalTimeAndDate:
        {
            strTime = pStringTime
            strDate = pStringDate
        }
    }

    Component.onCompleted: getTimeAndDate()
}

