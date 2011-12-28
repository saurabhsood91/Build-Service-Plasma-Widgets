#include "obsdataengine.h"

ObsDataEngine::ObsDataEngine(QObject *parent,const QVariantList &args):Plasma::DataEngine(parent,args)
{
    manager=new QNetworkAccessManager(this);
}

bool ObsDataEngine::updateSourceEvent(const QString &source)
{
	if(source.at(0)=='1'){
		url=source;
		url.remove(0,1);
		manager->get(QNetworkRequest(QUrl(url)));
		connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(buildResults(QNetworkReply*)));
	}
	else if(source.at(0)=='2'){
		url=source;
		url.remove(0,1);
		manager->get(QNetworkRequest(QUrl(url)));
		connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(serviceStatus(QNetworkReply*)));
	}
	else if(source.at(0)=='3'){
		url=source;
		url.remove(0,1);
		manager->get(QNetworkRequest(QUrl(url)));
		connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(requestLists(QNetworkReply*)));
	}
	return true;
	
}

bool ObsDataEngine::sourceRequestEvent(const QString& source)
{
	if(source.at(0)=='1'){
		url=source;
		url.remove(0,1);
		manager->get(QNetworkRequest(QUrl(url)));
		connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(buildResults(QNetworkReply*)));
	}
	else if(source.at(0)=='2'){
		url=source;
		url.remove(0,1);
		manager->get(QNetworkRequest(QUrl(url)));
		connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(serviceStatus(QNetworkReply*)));
	}
	
	else if(source.at(0)=='3'){
		url=source;
		url.remove(0,1);
		manager->get(QNetworkRequest(QUrl(url)));
		connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(requestLists(QNetworkReply*)));
	}
	return true;
}



void ObsDataEngine::buildResults(QNetworkReply *reply)
{
	QString data=(QString)reply->readAll();
	QXmlStreamReader xml(data);
	QXmlStreamAttributes attrib;
	QString finalData="";
	qint32 i=0;
        while(!xml.atEnd()){
            xml.readNextStartElement();
            if(xml.name()=="result" && !xml.isEndElement()){
                attrib=xml.attributes();
                QString projectName=attrib.value("project").toString();
                QString repositoryName=attrib.value("repository").toString();
                QString arch=attrib.value("arch").toString();
                QString state=attrib.value("state").toString();
                finalData.append("Project: ").append(projectName).append("<br />Repository: ").append(repositoryName).append("<br />Architecture: ").append(arch).append("<br />State: ").append(state);
            }
            else if(xml.name()=="status" && !xml.isEndElement()){
				QString key=""+(++i);
                attrib=xml.attributes();
                QString packageName=attrib.value("package").toString();
                QString code=attrib.value("code").toString();
                finalData.append("<br />Package: ").append(packageName).append("<br />Code: ").append(code);
                if(code=="failed" || code=="unresolvable"){
                    xml.readNextStartElement();
                    QString reason=xml.readElementText();
                    finalData.append("<br />Reason: ").append(reason);
                }
                finalData.append("<br />");
                setData("Results",key,finalData);
            }
        }
}

void ObsDataEngine::serviceStatus(QNetworkReply *reply)
{
	QString data=(QString)reply->readAll();
	QXmlStreamReader xml(data);
    while(!xml.atEnd()){
        xml.readNextStartElement();

        if(xml.name()=="idle" && !xml.isEndElement()){
            QXmlStreamAttributes attrib=xml.attributes();
            QString workerId=attrib.value("workerid").toString();
            QString hostarch=attrib.value("hostarch").toString();
            QString finalData="WorkerId: ";
            finalData.append(workerId).append("<<br />Host Architecture: ").append(hostarch);
            setData("Idle",workerId,finalData);
        }
        else if(xml.name()=="building" && !xml.isEndElement()){
            QXmlStreamAttributes attrib=xml.attributes();
            QString repository=attrib.value("repository").toString();
            QString arch=attrib.value("arch").toString();
            QString project=attrib.value("project").toString();
            QString package=attrib.value("package").toString();
            QString starttime=attrib.value("starttime").toString();
            QString workerId=attrib.value("workerid").toString();
            QString hostarch=attrib.value("hostarch").toString();
            QString finalData="Repository: ";
            finalData.append(repository).append("<br />Architecture: ").append(arch).append("<br />Project:").append(project).append("<br />Package: ").append(package).append("<br />Start Time: ").append(starttime).append("<br />Worker ID: ").append(workerId).append("<br />Host Architecture: ").append(hostarch);
            setData("Building",workerId,finalData);
        }
        else if(xml.name()=="waiting" && !xml.isEndElement()){
            QXmlStreamAttributes attrib=xml.attributes();
            QString arch=attrib.value("arch").toString();
            QString jobs=attrib.value("jobs").toString();
            QString finalData="Architecture: ";
            finalData.append(arch).append("<br />Jobs: ").append(jobs);
        }
        else if(xml.name()=="buildavg" && !xml.isEndElement()){
            QXmlStreamAttributes attrib=xml.attributes();
            QString arch=attrib.value("arch").toString();
            QString buildavg=attrib.value("buildavg").toString();
            QString finalData="Architecture: ";
            finalData.append(arch).append("<br />Average: ").append(buildavg);
        }
        else if(xml.name()=="scheduler" && !xml.isEndElement()){
            QXmlStreamAttributes attrib=xml.attributes();
            QString arch=attrib.value("arch").toString();
            QString starttime=attrib.value("starttime").toString();
            QString state=attrib.value("state").toString();
            QString finalData="Architecture: ";
            finalData.append(arch).append("<br />Start Time: ").append(starttime).append("<br />State: ").append(state);
            xml.readNextStartElement();
            if(xml.name()=="queue" && !xml.isEndElement()){
                QXmlStreamAttributes subAttrib=xml.attributes();
                QString med=subAttrib.value("med").toString();
                QString high=subAttrib.value("high").toString();
                QString low=subAttrib.value("low").toString();
                finalData.append("<br />Low: ").append(low).append("<br />Med: ").append(med).append("<br />High: ").append(high);
            }
            setData("Scheduler",arch,finalData);
        }
    }
}

void ObsDataEngine::requestLists(QNetworkReply *reply){
	QString data=(QString)reply->readAll();
	QString requestBy,timeOfRequest,sourceProject,sourcePackage,targetPackage,targetProject,sourceUpdate,finalData,requestId;
	QString description;
	QXmlStreamReader xml(data);
	
	while(!xml.atEnd()){
		xml.readNext();
		
		if(xml.name()=="request" && !xml.isEndElement()){
			QXmlStreamAttributes attrib=xml.attributes();
			requestId=attrib.value("id").toString();
		}
			
			if(xml.name()=="action" && !xml.isEndElement()){
				QXmlStreamAttributes attrib=xml.attributes();
				QString actionType=attrib.value("type").toString();
			}
			
            if(xml.name()=="source" && !xml.isEndElement()){
                QXmlStreamAttributes attrib=xml.attributes();
				sourceProject=attrib.value("project").toString();
				sourcePackage=attrib.value("package").toString();
            }
				
				
			if(xml.name()=="target" && !xml.isEndElement()){
                QXmlStreamAttributes attrib=xml.attributes();
				targetProject=attrib.value("project").toString();
                targetPackage=attrib.value("package").toString();
            }
				
				
            if(xml.name()=="options" && !xml.isEndElement()){
                QXmlStreamAttributes attrib=xml.attributes();
				xml.readNext();
				sourceUpdate=xml.readElementText();
            }
			
			if(xml.name()=="state" && !xml.isEndElement()){
				QXmlStreamAttributes attrib=xml.attributes();
				requestBy=attrib.value("who").toString();
				timeOfRequest=attrib.value("when").toString();
			}

			if(xml.name()=="description" && !xml.isEndElement()){
				description=xml.readElementText();
			}

			if(xml.name()=="request" && xml.isEndElement()){
				QString finalData="";
				finalData.append("Description: ").append(description).append("<br />")
				.append("By: ").append(requestBy).append("<br />").append("Time: ").append(timeOfRequest).append("<br />")
				.append("Source Project: ").append(sourceProject).append("<br />").append("Source Package: ").append(sourcePackage).append("<br />")
				.append("Target Project: ").append(targetProject).append("<br />").append("Target Package: ").append(targetPackage).append("<br />")
				.append("Source Update: ").append(sourceUpdate).append("<br /><br />");
			
				setData("Requests",requestId,finalData);
			}
	}
}

ObsDataEngine::~ObsDataEngine()
{
}


K_EXPORT_PLASMA_DATAENGINE(obsdataengine, ObsDataEngine)

#include "obsdataengine.moc"
