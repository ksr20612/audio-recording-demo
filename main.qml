import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    id : mainWin
    visible: true
    width: 350
    height: 400
    title: qsTr("녹음기")

    signal startRecord(var intData);
    signal startPlay(var intData);

    Text {
        x : 148
        y : 50
        id : text
        font.pointSize : 18
        text : "녹음기"
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
            startRecord(1);
        }
    }

    Button {
        id : playButton
        visible : true;
        width : 100
        height : 100
        text : qsTr("PLAYBACK")
        anchors.left:recordStartButton.right
        anchors.top:recordStartButton.top
        anchors.leftMargin : 50
        onClicked: {
            startPlay(1);
        }
    }
}
