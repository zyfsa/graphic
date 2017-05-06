/*
  界面生成：使用QML进行设计，简单快捷
  包含运行编译直接可视的界面；
  另外点进椭圆生成算法以及真实感图形生成，会有之前隐藏界面呈现，可以选选择调整算法参数
*/

import QtQuick 2.3
import QtQuick.Controls 1.2
import wm.qml.Controls 1.0
import QtQuick.Controls.Styles 1.2

ApplicationWindow {
    visible: true
    width: 1024
    height: 632
    title: qsTr("张一帆 SA16006158 图形学大作业")

    menuBar: MenuBar {
        Menu {
            title: qsTr("图元生成")
            MenuItem {
                text: qsTr("直线")
                onTriggered:
                {
                    painter.func = 1;
                    ellipseTex.visible = false;
                    realTex.visible = false;
                    console.log("直线");
                }
            }

            MenuItem {
                text: qsTr("椭圆")
                onTriggered:
                {
                    painter.func = 2;
                    realTex.visible = false;
                    ellipseTex.visible = true;
                    console.log("椭圆");
                }
            }

            MenuItem {
                text: qsTr("区域填充")
                onTriggered:
                {
                    painter.func = 3;
                    realTex.visible = false;
                    console.log("区域填充");
                }
            }
        }
        Menu {
            title: qsTr("样条曲线")
            MenuItem {
                text: qsTr("Bezier曲线")
                onTriggered:
                {
                    painter.func = 4;
                    ellipseTex.visible = false;
                    realTex.visible = false;
                    console.log("Beizer曲线");
                }
            }
            MenuItem {
                text: qsTr("B-样条")
                onTriggered:
                {
                    painter.func = 5;
                    ellipseTex.visible = false;
                    realTex.visible = false;
                    console.log("B-样条");
                }
            }
        }

        Menu {
            title: qsTr("分形图形")
            MenuItem {
                text: qsTr("Koch");
                onTriggered:
                {
                    painter.func = 6;
                    ellipseTex.visible = false;
                    realTex.visible = false;
                    console.log("Koch");
                }
            }

            MenuItem {
                text: qsTr("fern")
                onTriggered:
                {
                    painter.func = 7;
                    ellipseTex.visible = false;
                    realTex.visible = false;
                    console.log("fern");
                }
            }
        }
        Menu {
            title: "真实感图形";
            MenuItem {
                text: qsTr("球体");
                onTriggered:
                {
                    painter.func = 8;
                    ellipseTex.visible = false;
                    realTex.visible = true;
                    console.log("画球");
                }
            }

            MenuItem {
                text: qsTr("球体纹理映射");
                onTriggered:
                {
                    painter.func = 9;
                    ellipseTex.visible = false;
                    juliaTex.visible = false;
                    realTex.visible = true;
                    console.log("画纹理");
                }
            }
        }

    }

    Rectangle {
        id: options;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.top: menuBar.bottom + 4;
        implicitHeight: 90;
        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: "#ffffff";
            }
            GradientStop {
                position: 0.6;
                color: "#ffffff";
            }
            GradientStop {
                position: 1.00;
                color: "#000000";
            }
        }
        Component {
            id: btnStyle;
            ButtonStyle {
                background: Rectangle {
                    implicitHeight: 35;
                    implicitWidth: 90;
                    border.width: control.hovered ? 2 : 1;
                    border.color: "#888";
                    radius: 4;
                    gradient: Gradient {
                        GradientStop {
                            position: 0.00;
                            color: "#ffffff";
                        }
                        GradientStop {
                            position: 0.70;
                            color: "#ffffff";
                        }
                        GradientStop {
                            position: 1.0;
                            color: "red";
                        }
                    }
                }
                label: Text {
                    text: control.text;
                    font.pointSize: 12;
                    color: "#070842"
                    horizontalAlignment: Text.AlignHCenter;
                    verticalAlignment: Text.AlignVCenter;
                }
            }
        }

        ColorPicker {
            id: foreground;
            anchors.left: parent.left;
            anchors.top: parent.top;
            anchors.leftMargin: 6;
            text: "画笔色";
            selectedColor: "#ff0000";
            onColorPicked:
            {
                painter.penColor = clr;
            }
        }


        Rectangle {
            id: splitter;
            border.width: 1;
            border.color: "gray";
            anchors.left: foreground.right;
            anchors.leftMargin: 12;
            anchors.top: foreground.top;
            width: 1;
            height: foreground.height;
        }

        Slider {
            id: thickness;
            anchors.left: splitter.right;
            anchors.leftMargin: 6;
            anchors.bottom: splitter.bottom;
            minimumValue: 1;
            maximumValue: 20;
            stepSize: 1.0;
            value: 1;
            width: 180;
            height: 24;
            onValueChanged: if (painter != null) painter.penWidth = value;
        }

        Text {
            id: penThickLabel;
            anchors.horizontalCenter: thickness.horizontalCenter;
            anchors.bottom: thickness.top;
            anchors.bottomMargin: 4;
            text: "宽度:%1px".arg(thickness.value);
            font.pointSize: 12;
            color: "#070842";
        }

        Text {
            id: minLabel;
            anchors.left: thickness.left;
            anchors.bottom: thickness.top;
            anchors.bottomMargin: 2;
            text: thickness.minimumValue;
            font.pointSize: 12;
        }

        Text {
            id: maxLabel;
            anchors.right: thickness.right;
            anchors.bottom: thickness.top;
            anchors.bottomMargin: 2;
            text: thickness.maximumValue;
            font.pointSize: 12;
        }

        Rectangle {
            id: splitter2;
            border.width: 1;
            border.color: "gray";
            anchors.left: clear.left;
            anchors.rightMargin: 4;
            anchors.top: foreground.top;
            width: 1;
            height: foreground.height;
        }

        Button {
            id: clear;
            anchors.right: undo.left;
            anchors.rightMargin: 4;
            anchors.verticalCenter: splitter2.verticalCenter;
            width: 70;
            height: 28;
            text: "清除";
            style: btnStyle;
            onClicked: painter.clear();
        }

        Button {
            id: undo;
            anchors.right: exit.left;
            anchors.rightMargin: 4;
            anchors.top: clear.top;
            width: 70;
            height: 28;
            text: "撤销";
            style: btnStyle;
            onClicked: painter.undo();
        }

        Button {
            id: exit;
            anchors.right: parent.right;
            anchors.rightMargin: 4;
            anchors.top: undo.top;
            width: 70;
            height: 28;
            text: "退出";
            style: btnStyle;
            onClicked: Qt.quit();
        }

        Rectangle {
            border.width: 1;
            border.color: "gray";
            width: parent.width;
            height: 2;
            anchors.bottom: parent.bottom;
        }

        Rectangle {
            id: ellipseTex;
            visible: false;
            width: parent.width;
            height: 25
            anchors.left: parent.left;
            anchors.top: foreground.bottom;
        Text {
            id: raTex;
            width: 49;
            height: 25;
            font.pointSize: 10;
            color: "#5500ff";
            anchors.left: parent.left;
            anchors.leftMargin: 4;
            anchors.top: parent.top;
            text: "长轴: ";
        }
        TextInput {
            id: ra;
            width: 40;
            height: 25;
            font.pointSize: 10;
            anchors.left: raTex.right;
            anchors.top: parent.top;
            text: "40";
            selectByMouse: true;
            onTextChanged:
            {
                painter.func = 3;
                painter.ra = parseInt(text);
                console.log("长轴 changed");
            }
        }

        Text {
            id: rbTex;
            width: 40;
            height: 25;
            font.pointSize: 10;
            color: "#5500ff";
            anchors.left: ra.right
            anchors.top: parent.top
            text: "短轴: "
        }
        TextInput {
            id: rb;
            width: 40;
            height: 25;
            font.pointSize: 10;
            anchors.left: rbTex.right
            anchors.top: parent.top
            text: "20"
            selectByMouse: true;
            onTextChanged:
            {
                painter.func = 3;
                painter.rb = parseInt(text);
                console.log("短轴 changed");
            }
        }
        }

        Rectangle {
            id: realTex;
            visible: false;
            width: parent.width;
            height: 25;
            anchors.left: parent.left;
            anchors.top: foreground.bottom;
            Text {
                id: vthetaxLabel;
                anchors.left: parent.left;
                anchors.top: parent.top;
                width: 150;
                height: 25
                text: "观察方向:  x:";
                font.pointSize: 10;
                color: "#5500ff";
            }

            TextInput {
                id: vthetax;
                width: 40;
                height: 25;
                font.pointSize: 10;
                anchors.left: vthetaxLabel.right
                anchors.top: parent.top
                text: "0"
                selectByMouse: true;
                IntValidator {id: intvalvx; bottom: -360; top: 360;}
                onTextChanged:
                {
                    painter.vthetax = text;
                    console.log("图像 changed");
                }
            }
            Text {
                id: vthetayLabel;
                anchors.left: vthetax.right;
                anchors.top: parent.top;
                width: 40;
                height: 25;
                text: "y:";
                font.pointSize: 10;
                color: "#5500ff";
            }

            TextInput {
                id: vthetay;
                width: 40;
                height: 25;
                font.pointSize: 10;
                anchors.left: vthetayLabel.right
                anchors.top: parent.top
                text: "0"
                selectByMouse: true;
                IntValidator {id: intvalvy; bottom: -360; top: 360;}
                onTextChanged:
                {
                    painter.vthetay = text;
                    console.log("图像 changed");
                }
            }
            Text {
                id: vthetazLabel;
                anchors.left: vthetay.right;
                anchors.top: parent.top;
                width: 40;
                height: 25
                text: "z:";
                font.pointSize: 10;
                color: "#5500ff";
            }

            TextInput {
                id: vthetaz;
                width: 40;
                height: 25;
                font.pointSize: 10;
                anchors.left: vthetazLabel.right
                anchors.top: parent.top
                text: "1"
                selectByMouse: true;
                IntValidator {id: intvalvz; bottom: -360; top: 360;}
                onTextChanged:
                {
                    painter.vthetaz = text;
                    console.log("图像 changed");
                }
            }
            Text {
                id: lthetaxLabel;
                anchors.left: vthetaz.right;
                anchors.top: parent.top;
                width: 140;
                height: 25
                text: "光照方向:  x:";
                font.pointSize: 10;
                color: "#5500ff";
            }

            TextInput {
                id: lthetax;
                width: 40;
                height: 25;
                font.pointSize: 10;
                anchors.left: lthetaxLabel.right
                anchors.top: parent.top
                text: "-1"
                selectByMouse: true;
                IntValidator {id: intvallx; bottom: -360; top: 360;}
                onTextChanged:
                {
                    painter.lthetax = text;
                    console.log("图像 changed");
                }
            }

            Text {
                id: lthetayLabel;
                anchors.left: lthetax.right;
                anchors.top: parent.top;
                width: 40;
                height: 25
                text: "y:";
                font.pointSize: 10;
                color: "#5500ff";
            }

            TextInput {
                id: lthetay;
                width: 40;
                height: 25;
                font.pointSize: 10;
                anchors.left: lthetayLabel.right
                anchors.top: parent.top
                text: "-1"
                selectByMouse: true;
                IntValidator {id: intvally; bottom: -360; top: 360;}
                onTextChanged:
                {
                    painter.lthetay = text;
                    console.log("图像 changed");
                }
            }

            Text {
                id: lthetazLabel;
                anchors.left: lthetay.right;
                anchors.top: parent.top;
                width: 40;
                height: 25
                text: "z:";
                font.pointSize: 10;
                color: "#5500ff";
            }

            TextInput {
                id: lthetaz;
                width: 40;
                height: 25;
                font.pointSize: 10;
                anchors.left: lthetazLabel.right
                anchors.top: parent.top
                text: "0"
                selectByMouse: true;
                IntValidator {id: intvallz; bottom: -360; top: 360;}
                onTextChanged:
                {
                    painter.lthetaz = text;
                    console.log("图像 changed");
                }
            }
        }
    }


    APaintedItem {
        id: painter;
        anchors.top: options.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
    }
}
