import QtQuick 2.2

Rectangle {
    id: root
    Component.onCompleted: {
        var obj = new Object();
        console.log(obj.toString());
        console.log(obj.constructor);
        console.log(root.hasOwnProperty("width"));
        console.log(Item.isPrototypeOf(root));
        console.log(root.propertyIsEnumerable("children"));
        console.log(root.toString());
        console.log(root.valueOf());

        var person = new Object();
        person.name = "zhangsan";
        person.year = 20;

        /*
          var person = {
              "name": "zhangsan",
              "year": 20
          }
上面的代码和前面先构造Object再添加属性的方式，得到的是一样的对象。而且，这样定义的对象，还可以为其添加新的属性和方法
也可以使用数组下标的形式访问其属性和方法。
    对象的字面量表示法，语法是这样的： 使用一对花括号表示一个对象，其属性以Key:Value对形式放在花括号内，多个属性之间
使用逗号分隔：如果一个属性本身是对象，还可以使用字面量表示法来嵌套描述。
    对象的字面量表示法，加上后面介绍数组时要讲的数组的字面量表示法，就构成了JSON（JavaScript Object Notation）的核心语法。
    当我们在应用中使用QML的组件和动态对象时，经常会使用到对象的字面量表示法。
        */
        person.printInfo = function printInfo()
        {
            console.log("name = ", this.name, "year = ", this.year);
        }
        console.log(person["name"]);
        person["printInfo"]();

        for (var prop in person)
        {
            console.log(prop, ",", person[prop]);
        }
    }
}
