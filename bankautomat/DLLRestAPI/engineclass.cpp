#include "engineclass.h"

EngineClass::EngineClass(QObject *parent) : QObject(parent)
{
    objectMyUrl = new MyURL;
    base_url = objectMyUrl->getBase_url();
}

void EngineClass::loginRequest()
{
    qDebug()<<"base_url"+base_url; //Url debug

    QJsonObject jsonObj; //Json object with login credentials
    jsonObj.insert("Korttinumero", _testKorttiNum);
    jsonObj.insert("pin", _testPin); //Insert login credentials


    QNetworkRequest request((base_url+"/login")); //Url for login request
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json"); //Data form in body

    loginManager = new QNetworkAccessManager(this);
    connect(loginManager, SIGNAL(finished (QNetworkReply*)), this, SLOT(loginSlot(QNetworkReply*)));

    loginReply = loginManager->post(request, QJsonDocument(jsonObj).toJson()); //Login post
}

void EngineClass::GetKorttiInfo()
{
    QNetworkRequest request((base_url+"/kortti/info/"+_testKorttiNum)); //Url for kortti info request
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json"); //Data form in body

    request.setRawHeader(QByteArray("Authorization"),(token)); //WEBTOKEN

    korttiInfoManager = new QNetworkAccessManager(this);
    connect(korttiInfoManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(korttiInfoSlot(QNetworkReply*)));

    korttiInfoReply = korttiInfoManager->get(request);
}

void EngineClass::loginSlot(QNetworkReply *loginReply) //Login slot is triggered when finish signal gets raised from login request
{
    response_data=loginReply->readAll(); //Insert reply data to variable
    qDebug()<<response_data; //Debug response data
    if(response_data!="false")
    {
        token="Bearer "+response_data; //Set token to be Bearer token with response data
    }
    else
    {
        qDebug()<<"Login failed";
    }
}

void EngineClass::korttiInfoSlot(QNetworkReply *korttiInfoReply)
{
    response_data=korttiInfoReply->readAll();
    qDebug()<<response_data;
}
