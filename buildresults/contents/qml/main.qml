import QtQuick 1.0
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.core 0.1 as PlasmaCore

QGraphicsWidget {
	id:page
    property string activeSource
	property string username
	property string passwd
    Component.onCompleted: {
		plasmoid.addEventListener('ConfigChanged', configChanged);
        plasmoid.configurationRequired = true

    }
    
    function configChanged(){
			username=plasmoid.readConfig("uname");
			passwd=plasmoid.readConfig("pwd");
	}

    PlasmaWidgets.LineEdit{
		id:repositoryName
		width:200
		y:parent.top+5
		
	}
	Item{
	 PlasmaCore.DataSource {
		id: dataSource
		engine: "obsdataengine"
		interval:0
		onDataChanged:{
			print("inside datachanged");
			plasmoid.busy=false
			for(i in data["Results"]){
				textBrowser.html+="<br>"+data["Results"][i]+"<br><br>"
			}
			disconnectSource("Results")
		}
        
		onSourceAdded:{
			print("Source added: "+dataSource.sources[0]);
			connectSource("Results");
		}
        
        onSourceRemoved:{
            print("Source Removed")
            disconnectSource("Results")
        }
 }
 
 
}


    PlasmaWidgets.PushButton {
		id:getDetails
		text:"Click"
		x:parent.top+5
		y:repositoryName.right+20
		anchors{
			left:repositoryName.right
			leftMargin:repositoryName.y+10
		}
		onClicked:{
            textBrowser.html=""
			activeSource="1https://"+username+":"+passwd+"@api.opensuse.org/build/"+repositoryName.text+"/_result";
			print(activeSource);
			dataSource.connectedSources=[activeSource];
			plasmoid.busy=true
			}
		
		
    }
    
    PlasmaWidgets.WebView{
		id:textBrowser
		x:repositoryName.x+25
		y:parent.left+15
		width:page.width
		height:page.height
		html:"Build Results:"
		anchors{
			top:repositoryName.bottom
		}
	}

}
