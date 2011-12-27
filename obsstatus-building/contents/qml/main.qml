import QtQuick 1.0
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.core 0.1 as PlasmaCore

QGraphicsWidget{
	id:page
	property string username
	property string password
	property string activeUrl
	
	Component.onCompleted:{
		plasmoid.configurationRequired=true
		username=plasmoid.readConfig("uname")
		password=plasmoid.readConfig("pwd")
		
		//Connect to Data Source
		activeUrl="2https://"+username+":"+password+"@api.opensuse.org/build/_workerstatus"
		print(activeUrl)
		dataSource.connectSource(activeUrl)
		timer.start()
		plasmoid.busy=true
	}
	
	PlasmaWidgets.WebView{
		id:serviceStatus
		anchors.fill:page
	}
	
	Item{
		PlasmaCore.DataSource{
			id:dataSource
			engine: "obsdataengine"
			
			onSourceAdded:{
				print("Source Added")
				connectSource("Building")

			}
			
			onDataChanged:{
				print("Data Changed")
				if(timer.running=true){
					serviceStatus.html+="<b>Building: </b><br /><br />"
					for(i in data["Building"]){
						serviceStatus.html+="<b>Worker ID: "+i+":</b><br />"+data["Building"][i]+"<br /><br />"
					}											
					plasmoid.busy=false
					timer.stop()
				}
			}
		}
	}
	
	Item{
		Timer{
			id:timer
			interval:20000
			running:false
			triggeredOnStart:false
			onTriggered:{
					if(plasmoid.busy=true){
						plasmoid.busy=false
						serviceStatus.html="Could not Connect to Source"
						running=false
				}
			}
		}
	}
	
	PlasmaWidgets.PushButton{
		id:refreshButton
		text:"Refresh"
		anchors{
			bottom:serviceStatus.top
			rightMargin:5
		}
		onClicked:{
			dataSource.connectSource(activeUrl);
			timer.start()
			plasmoid.busy=true
		}
	}
}
