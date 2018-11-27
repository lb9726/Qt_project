import QtQuick 2.0

Item {
    Component.onCompleted: {
        var str = new String("I\'m a string");
        console.log(str.length);
        console.log("I\'m a string".length);

        var str2 = new String("I\'m a string");
        console.log(str.charAt(2)); // 输出 m
        console.log(str[0]);        // 输出 I
        console.log(str.charCodeAt(1));     // 输出 39

        console.log(str.indexOf("ing"));    // 输出9
        console.log(str.indexOf("ing", 4))  // 输出 9
        console.log(str.search(/String/));  // 输出 -1
        console.log(str.lastIndexOf(" "));  // 输出5
        console.log(str.search(/String/i)); // 忽略大小写，输出6
        console.log(str.search("i"));
        console.log(str.match("tri"));
        var numberSource = new String("2014-08-18, I got 96");
        var results = numberSource.match(/\d+/g);
        console.log(results.length); // 输出4
        console.log(results);        // [2014, 08, 18, 96]

        var str1 = "Qt Quick";
        var str4 = "qt quick";
        var str5 = "ok";
        console.log(str1 === str4); // 输出 false
        console.log(str1 < str4);   // 输出 true
        console.log(str1.localeCompare(str4)); // 输出 1
        console.log(str5.localeCompare(str4)); // 输出 -1

        /*
        注意， 无论是concact()还是"+", 都不会在字符串之间插入填充字符。如果你想连接多个字符串并且指定它们之间
        的分隔符（填充字符),则可以使用Array对象存储字符串，然后调用Array的join()方法。
        */
        var strOne = new String("Qt");
        var strTwo = "Quick";
        var strResult = strOne.concat(strTwo, " is ", " great!");
        console.log(strResult);
        console.log(strOne.concat(" widgets"));
        console.log(strOne + strTwo);
        /*
提取字串
ECMAScript String提供了slice()和substring()两个方法来提取子串，它们接受一个或两个参数，第一个参数指定起始位置，
第二个参数指定结束位置(提取出的子串不包含该位置的字符),如果忽略第二个参数，结束位置默认为字符串的长度。
slice()和subStirng()这两个方法的不同之处在于对负数参数的处理。slice()遇到负数参数时，会应用"从串尾倒数"这种策略，
比如：slice(-3),会提取母串的后三个字符作为子串;而substring()方法会把负数参数作为0处理，还有一点，substring()总
是把较小的参数作为起始位置，比如substring(3,1)与substring(1,3)效果一样。
还有一个方法，substr(),也可以提取子串，它的第一个参数指定起始位置，第二个参数指定要提取的字符个数，如果不指定，substr()
会返回从起始位置到母串结束的所有字符。
          */
        var source = new String("I like QML");
        console.log(source.slice(-3));          // QML
        console.log(source.slice(2, 6));        // like
        console.log(source.substring(0, 6));    // I like
        console.log(source.substring(-3));      // I like QML
        console.log(source.slice(4, -3));       // ke
        console.log(source.slice(2, 4));        // like
/*
        大小写转换
        如果你想将一个字符串转换为小写，可以使用toLowerCase()或toLocaleLowerCase()方法;
        转换为大写，可以使用toUpperCase()或toLocaleUpperCase()方法。其中toLocaleXXX方法基于
        特定的区域实现，与localeCompare()类似。*/
        var orig = "Qt QuIck";
        console.log(orig.toLocaleLowerCase());
        console.log(orig.toLowerCase());
        console.log(orig.toLocaleUpperCase());
        console.log(orig.toUpperCase());

        var strSource = new String("Android, best");
        console.log(strSource.replace("Android", "iOS"));
        console.log(strSource.replace(/best/, "worst"));

/*
QML在实现ECMAScript 时对String对象做了扩充，加入了arg()方法。如果你用过Qt C++中的QString类，应该不陌生。
那你也应该想到了，QML中的String和Qt C++中的QString是对应关系，当使用C++与QML混合编程时，C++对象中的
QString类型会映射到QML中的String上，反之亦然。arg()的语法： string arg(value)。其中value可以是数字，字符串
，布尔值，对象等。它用来替换发出调用的字符串对象内的%1,%2,%N等占位符：布尔值会被替换为1或0;对于对象，则使用toString()
方法转换后的结果来替换对应的占位符。arg()会返回一个新的字符串对象。

值得注意的是，String类型的对象是只读的，字符串的提取，查找，替换等操作。凡是牵涉到修改的动作，都返回新的字符串原始值或对象，
而母串本身不会改变。另外，字符串原始值是伪对象，他可以使用String类的所有属性和方法。
*/
        var expression = "%1 < %2 = %3";
        var result = expression.arg(7).arg(8).arg("true");
        console.log(result);
        console.log(expression.arg(10).arg(6).arg(false));
        console.log("I am %1 years old.".arg(10));
    }
}
