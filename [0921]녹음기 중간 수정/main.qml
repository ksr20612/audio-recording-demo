import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls 1.4

Window {
    id : mainWin
    visible: true
    width: 350
    height: 400
    title: qsTr("녹음기")

    signal startRecord(var intData);
    signal startPlay(var intData);
    signal stopRecord();
    signal stopPlay();

    Text {
        y: 50
        anchors.horizontalCenter: parent.horizontalCenter
        id : title
        font.pointSize : 18
        text : "녹음기"
    }

    Rectangle {
        width : 300
        height : 50
        id : subTitle
        anchors.top : title.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 30
        border.width : 1
        border.color : "black"

        Text{
            id : showBoard
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize : 18
            text : ""
        }
    }

    Button {
        id : recordStartButton
        visible : true
        x : 50
        y : 250
        width : 100
        height : 100
        text : qsTr("RECORD") 
        onClicked: {
            recordStartButton.visible = false;
            recordStopButton.visible = true;
            startRecord(1);
        }
    }

    Button {
        id : recordStopButton
        visible : false
        x : 50
        y : 250
        width : 100
        height : 100
        text : qsTr("STOP")
        onClicked: {
            recordStartButton.visible = true;
            recordStopButton.visible = false;
            subTitle.border.color = "black";
            stopRecord();
        }
    }

    Button {
        id : playStartButton
        visible : true;
        width : 100
        height : 100
        text : qsTr("PLAYBACK")
        anchors.left:recordStartButton.right
        anchors.top:recordStartButton.top
        anchors.leftMargin : 50
        onClicked: {
            playStartButton.visible = false;
            playStopButton.visible = true;
            startPlay(1);
        }
    }

    Button {
        id : playStopButton
        visible : false;
        width : 100
        height : 100
        text : qsTr("STOP")
        anchors.left:recordStartButton.right
        anchors.top:recordStartButton.top
        anchors.leftMargin : 50
        onClicked: {
            playStartButton.visible = true;
            playStopButton.visible = false;
            stopPlay();
        }
    }

    function showCount(second){
        showBoard.text = second;
        subTitle.border.color = "green";
    }

    function showName(name){
        console.log("name is delievered");
        showBoard.text = name;
    }
}
