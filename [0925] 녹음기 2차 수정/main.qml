import QtQuick 2.12
import QtQuick 2.9
import QtQml.Models 2.1
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls 1.4
import "./"

Window {
    id : mainWin
    visible: true
    width: 350
    height: 450
    title: qsTr("녹음기")

    signal startRecord(var chan, var rate);
    signal startPlay(var name);
    signal stopRecord();
    signal stopPlay();
    signal conveyName(var name);
    signal conveyNameforPlay(var name);

    Popup{
        id: fileNamePopup
        width : 350
        height : 450
        leftPadding: 0
        rightPadding: 0
        topPadding: 0
        bottomPadding: 0
        background: Rectangle{
            color : "black"
            opacity : 0.7
        }
        closePolicy: Popup.CloseOnEscape

        Rectangle{
            x:(parent.width - width)/2
            y:(parent.height - height)/2
            width: 200
            height: 150
            color: "gray"
            MouseArea{
                anchors.fill: parent
            }
            Text{
                id : popupInfo
                anchors.horizontalCenter: parent.horizontalCenter
                text: "파일명을 입력하세요"
                color: "black"
                anchors.topMargin: 40
                font.pixelSize: 15
            }

            TextField{
                id : fileName
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top : popupInfo.bottom
                anchors.topMargin : 10
                placeholderText: qsTr("Enter file name")
            }

            Button{
                id: confirmButton
                width: 100
                height: 50
                anchors.top: fileName.bottom
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                text: "confirm"
                onClicked: {
                    if(fileName.text!="") {
                        fileNamePopup.close();
                        conveyName(fileName.text);
                        showBoard.text="저장완료";
                        timer.start();
                    }
                }
            }
        }
    }

    Popup{
        id: fileNamePopupForPlay
        width : 350
        height : 450
        leftPadding: 0
        rightPadding: 0
        topPadding: 0
        bottomPadding: 0
        background: Rectangle{
            color : "black"
            opacity : 0.7
        }
        closePolicy: Popup.CloseOnEscape

        Rectangle{
            x:(parent.width - width)/2
            y:(parent.height - height)/2
            width: 200
            height: 150
            color: "gray"
            MouseArea{
                anchors.fill: parent
            }
            Text{
                id : popupInfoForPlay
                anchors.horizontalCenter: parent.horizontalCenter
                text: "파일명을 입력하세요"
                color: "black"
                anchors.topMargin: 40
                font.pixelSize: 15
            }

            TextField{
                id : fileNameForPlay
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top : popupInfoForPlay.bottom
                anchors.topMargin : 10
                placeholderText: qsTr("Enter file name")
            }

            Button{
                id: confirmButtonForPlay
                width: 80
                height: 40
                anchors.top: fileNameForPlay.bottom
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                text: "confirm"
                onClicked: {
                    if(fileNameForPlay.text!="") {
                        fileNamePopupForPlay.close();
                        startPlay(fileNameForPlay.text);
                    }
                }
            }
        }
    }

    Popup{
        id: loadFailPopup
        width : 350
        height : 450
        leftPadding: 0
        rightPadding: 0
        topPadding: 0
        bottomPadding: 0
        background: Rectangle{
            color : "black"
            opacity : 0.7
        }
        closePolicy: Popup.CloseOnEscape

        Rectangle{
            x:(parent.width - width)/2
            y:(parent.height - height)/2
            width: 200
            height: 150
            color: "gray"
            MouseArea{
                anchors.fill: parent
            }
            Text{
                id : loadFailInfo
                anchors.horizontalCenter: parent.horizontalCenter
                text: "해당 파일이 없습니다"
                color: "black"
                anchors.topMargin: 40
                font.pixelSize: 15
            }

            Button{
                id: confirmButtonForloadFail
                width: 80
                height: 40
                anchors.top: loadFailInfo.bottom
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                text: "confirm"
                onClicked: {
                    loadFailPopup.close();
                }
            }
        }
    }

    Timer{
        id: timer
        interval: 2000
        running: true
        repeat: false
        onTriggered:{
             showBoard.text="";
        }
    }

    Text {
        y: 20
        anchors.horizontalCenter: parent.horizontalCenter
        id : title
        font.pointSize : 18
        text : "녹음기"
    }

    Rectangle {
        width : 260
        height : 50
        id : messageBox
        anchors.top : title.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
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

    Rectangle{
        width : 260
        height : 140
        id : parameterBox
        anchors.top : messageBox.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        border.width : 1
        border.color : "black"

        Rectangle {
            width : 260
            height : 70
            id : channelBox
            anchors.top : parameterBox.top
            anchors.horizontalCenter : parent.horizontalCenter

            GroupBox {
                title: "Channel"
                ExclusiveGroup { id: channelGroup }

                RadioButton {
                    id : mono
                    text: "Mono"
                    checked: true
                    exclusiveGroup: channelGroup
                }
                RadioButton {
                    id : stereo
                    text: "Stereo"
                    exclusiveGroup: channelGroup
                    anchors.left : mono.right
                    anchors.leftMargin: 10
                }
            }
        }

        Rectangle {
            width : 260
            height : 70
            id : sampleRateBox
            anchors.top : channelBox.bottom
            anchors.horizontalCenter : parent.horizontalCenter

            GroupBox {
                title: "Sampling rate"
                ExclusiveGroup { id: sampleRateGroup }

                RadioButton {
                    id : option1
                    text: "8000"
                    checked: true
                    exclusiveGroup: sampleRateGroup
                }
                RadioButton {
                    id : option2
                    text: "16000"
                    exclusiveGroup: sampleRateGroup
                    anchors.left : option1.right
                    anchors.leftMargin: 10
                }
            }
        }
    }

    Button {
        id : recordStartButton
        visible : true
        x : 50
        y : 320
        width : 100
        height : 100
        text : qsTr("RECORD") 
        onClicked: {
            recordStartButton.visible = false;
            recordStopButton.visible = true;
            startRecord(getChannel(),getRate());
        }
    }

    Button {
        id : recordStopButton
        visible : false
        x : 50
        y : 320
        width : 100
        height : 100
        text : qsTr("STOP")
        onClicked: {
            recordStartButton.visible = true;
            recordStopButton.visible = false;
            messageBox.border.color = "black";
            fileNamePopup.open();
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
//            playStartButton.visible = false;
//            playStopButton.visible = true;
            fileNamePopupForPlay.open();
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
        messageBox.border.color = "green";
    }

    function showName(name){
        showBoard.text = name;
        messageBox.border.color = "green";
    }

    function loadFail(){
        loadFailPopup.open();
    }

    function getChannel(){
        if(mono.checked == true) return 1;
        else return 2;
    }

    function getRate(){
        if(option1.checked == true) return 8000;
        else return 16000;
    }

    function closeName(){
        timer.start();
        messageBox.border.color = "black";
    }

}
