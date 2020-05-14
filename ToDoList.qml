import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import ToDo 1.0
ColumnLayout{
    Frame{
        Layout.fillHeight: true
        ListView{
            implicitWidth: 250
            implicitHeight: 250
            clip: true
            anchors.fill: parent
            model: ToDoModel {
                list: toDoList
            }
            delegate: RowLayout {

                width:parent.width

                CheckBox {
                    checked: model.done
                    onClicked: model.done = checked
                }
                TextField {
                    text: model.description
                    onEditingFinished: model.description = text
                    Layout.fillWidth: true
                }
            }

        }
    }
    RowLayout{
        Button{
            text: qsTr("增加新项目")
            onClicked: toDoList.appendItem()
            Layout.fillWidth: true
        }
        Button{
            text: qsTr("移除所选项目")
            onClicked: toDoList.removeCompletedItems()
            Layout.fillWidth: true
        }
    }
}
