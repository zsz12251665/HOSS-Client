import QtQuick 2.9
import QtQuick.Controls 2.2

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("To-do list")

    ToDoList{
        //center around parent
        anchors.centerIn: parent
    }
}
