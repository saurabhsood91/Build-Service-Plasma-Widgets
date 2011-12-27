import QtQuick 1.0
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.core 0.1 as PlasmaCore

QGraphicsWidget{
	property string username
	property string password
	property string activeUrl
	property int count
	Component.onCompleted:{
		plasmoid.configurationRequired=true
		username=plasmoid.readConfig("uname")
		password=plasmoid.readConfig("pwd")
		activeUrl="3https://"+username+":"+password+"@api.opensuse.org/request?view=collection&states=new&roles=maintainer&user=wstephenson"
		print(activeUrl)
		dataSource.connectSource(activeUrl)
		timer.start()
		plasmoid.busy=true

		//timer.running=true
	}
	
	PlasmaWidgets.WebView{
		id:htmlContent
		anchors.top:refreshButton.bottom
		width:parent.width
		height:parent.height-refreshButton.height
		}
	
	PlasmaWidgets.PushButton{
		id:refreshButton
		text:"Refresh"
		anchors{
			bottom:htmlContent.top
			rightMargin:5
		}
		onClicked:{
			dataSource.connectSource(activeUrl);
			timer.start()
			plasmoid.busy=true
		}
	}
	Item{
		PlasmaCore.DataSource{
			id: dataSource
			engine: "obsdataengine"
			
			onSourceAdded:{
				print('Source Added')
				connectSource("Requests")
			}
			
			onDataChanged:{
				if(timer.running==true){
					print('Data Changed')
					plasmoid.busy=false
					htmlContent.html="<center><font size=8><b>Requests</b></font></center>"
				
					for(i in dataSource.data["Requests"]){
						htmlContent.html+="<b>Request ID: "+i+"</b> <br /> "+dataSource.data["Requests"][i]+"<br /><br />"
					}
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
						htmlContent.html="Could not Connect to Source"
						running=false
				}
			}
		}
	}
}
