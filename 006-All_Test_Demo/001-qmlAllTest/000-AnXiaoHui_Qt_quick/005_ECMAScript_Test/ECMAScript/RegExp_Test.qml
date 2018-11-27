import QtQuick 2.0

Item {

    Component.onCompleted: {
        var str = new String("I\'m a string");
        str.match(/String/i);
        str.search(new RegExp("String", "i"));
        str.search(RegExp("String", "i"));

        var numbeSource = new String("2014-08-18, I got 96");
        var results = numbeSource.match(/\d+/g);
        console.log("results = ", results);
/*
1.修饰符
在QML中可以使用的修饰符有三个：
"i", 匹配时忽略大小写
"g", 查找所有匹配而非在找到第一个匹配后停止，即全局匹配
"m", 执行多行匹配。

(2) 元字符
元字符是拥有特殊含义的字符。
先解释一下"\d+"这个模式。\d是元字符，表示查找数字。+ 是量词，表示重复一次或者多次
那"\d+"的意思就是匹配一个或多个连续的数字，所以在前面的示例中，使用console.log打印
results，输出结果是： [2014,08,18,96]
如你所见，它匹配到了原字符串中的4个数字。
*/
        var str1 = "We are dogs;\nYour dogs;\nWe want meat, please.\nPlease.";
        var lines = str1.match(/^We.*/mg);
        console.log(lines.length);
        console.log(lines);
    }
}
