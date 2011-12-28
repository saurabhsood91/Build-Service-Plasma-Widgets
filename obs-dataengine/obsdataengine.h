#ifndef OBSDATAENGINE_H
#define OBSDATAENGINE_H

#include<plasma/dataengine.h>
#include<QtNetwork/QNetworkAccessManager>
#include<QtNetwork/QNetworkReply>
#include<QtNetwork/QNetworkRequest>
#include<QtXml/QXmlStreamReader>
class ObsDataEngine:public Plasma::DataEngine{
	Q_OBJECT
	
	public:
		ObsDataEngine(QObject *parent,const QVariantList &args);
		bool sourceRequestEvent(const QString &source);
		bool updateSourceEvent(const QString &source);
	private:
		QString url;
		QNetworkAccessManager *manager;
		QString data;
		QString details;
		QString username;
		QString password;
		~ObsDataEngine();
	
	public slots:
		void buildResults(QNetworkReply*);
		void serviceStatus(QNetworkReply*);
		void requestLists(QNetworkReply*);
				
};

#endif
