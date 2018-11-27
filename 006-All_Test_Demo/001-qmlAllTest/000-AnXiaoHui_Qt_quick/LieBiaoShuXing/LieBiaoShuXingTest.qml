import QtQuick 2.0

Item {  // 列表内只能包含QML对象，不能包含任何基本类型的字面量（如 8, ture,如果非要包含，需要使用var变量）。
        // 这点和JSON是不一样的
    children: [ //
        Text {
            x: 0
            text: "textOne"
        },
        Text {
            x: 70
            text: "TextTwo"
        }
    ]
    Component.onCompleted: {
        for (var i = 0; i < children.length; ++i)
        {
            console.log("text of label ", i, " : ", children[i].text);
        }
    }
}
