import QtQuick 2.15
import QtQuick.Window 2.15

import QtCharts 2.0
import QtQuick.Controls 2.12


import io.qt.controller 1.0

Window {
    id: window
    width: 1000
    height: 600
    visible: true
    title: qsTr("Hello World")


    ChartView {
        id: chart
        title: "Топ слов отсортированных по алфавиту"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom:  chooseFile.top
        legend.alignment: Qt.AlignBottom
        antialiasing: true

        HorizontalBarSeries {
            id: hbseries
            axisY: BarCategoryAxis { visible: false; categories:  [ "Top words"  ] }
            onHovered: {
                curWordValue.visible = status
                curWordValue.text = barset.values[0]
            }
        }
    }
    Controller{
        id: controller
        function countDigits(n) {
            for(var i = 0; n > 1; i++) {
                n /= 10;
            }
            return i;
        }
        onUpdateHistogramm:  {
            var maxWordCount = controller.topWordsCount[0]

            hbseries.axisX.min = 0
            for (var i = 0; i < controller.top.length; i++ )
            {
                if (controller.topWordsCount[i] > maxWordCount)
                    maxWordCount = controller.topWordsCount[i]

                var curBar = hbseries.at(i)
                //null может быть только если на графике еще не 15 слов
                if (curBar === null){
                    hbseries.append(controller.top[i],[controller.topWordsCount[i]])
                }
                else
                {

                    curBar.label = controller.top[i]
                    curBar.values = [controller.topWordsCount[i]]
                }
            }
            if (maxWordCount > hbseries.axisX.max)
            {
                var pow = countDigits(maxWordCount)
                hbseries.axisX.max = Math.pow(10,pow - 1) * Math.floor(maxWordCount/Math.pow(10,pow - 1)) * 2
            }

        }
    }

    Button {
        id: chooseFile
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        x: 270
        y: 432
        text: qsTr("Open file")
        onClicked:
        {
            if  (controller.openFileDialog())
            {
                hbseries.clear()
                hbseries.axisX.max = 10
            }
        }

    }
    Text{
        id: curWordValue
        anchors.bottom: parent.bottom
        verticalAlignment: Text.AlignVCenter
        anchors.right: chooseFile.left
        anchors.verticalCenter: chooseFile.verticalCenter

    }

}
