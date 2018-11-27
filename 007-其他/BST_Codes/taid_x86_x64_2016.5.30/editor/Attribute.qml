import QtQuick 2.5

Item {
    state:gAutoMode ? "Auto" : "Normal"
    states: [
        State {
            name: "Normal"
            PropertyChanges { target: id_attrArea; enabled: true;opacity:1 }
        },
        State {
            name: "Auto"
            PropertyChanges { target: id_attrArea; enabled: false;opacity:0.6 }
        }
    ]
}
