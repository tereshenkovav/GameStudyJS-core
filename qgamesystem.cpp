#include "qgamesystem.h"
#include <QScriptValueIterator>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include <QTimer>
#include <QStandardPaths>
#include <QDir>

QJsonValue script2json(const QScriptValue & val) {
    return QJsonValue::fromVariant(val.toVariant()) ;
}

QScriptValue json2script(QScriptEngine *engine, const QJsonValue & val) {
    return engine->newVariant(val.toVariant()) ;
}

QString currentlang="" ;
QStringList languages ;
bool soundon=true ;
bool fullscreen=false ;
bool oldfullscreen=false ;
int tekdifficult=0 ;
int difficultcount=1 ;
QString closehandlerscript="" ;

QGameSystem::QGameSystem(QScriptEngine *engine)
{
    this->engine = engine ;    
}

void QGameSystem::clearTimers()
{    
    timers.clear() ;
}

QString QGameSystem::ScriptValue2String(const QScriptValue &obj)
{
    if (obj.isObject()||obj.isArray()) {
        QJsonDocument doc ;
        if (obj.isObject()) {
            QJsonObject json ;
            QScriptValueIterator it(obj);
            while(it.hasNext()) {
                it.next();
                json.insert(it.name(),script2json(it.value())) ;
            }
            doc.setObject(json);
        }
        if (obj.isArray()) {
            QJsonDocument doc ;
            QJsonArray json ;
            for (int i=0; i<obj.property("length").toInt32(); i++)
                json.append(script2json(obj.property(i)));
            doc.setArray(json);
        }
        return doc.toJson() ;
    }
    if (obj.isString()) return "\""+obj.toString()+"\"" ;
    if (obj.isNumber()) return QString::number(obj.toNumber()) ;
    if (obj.isBool()) return obj.toBool()?"true":"false" ;
    if (obj.isNull()) return "null" ;
    return "" ;
}

QString QGameSystem::getFilenameByLanguageIfExist(QString filename)
{
    if (currentlang=="") return filename ;
    QFileInfo fi(filename) ;
    QString ext = fi.suffix() ;
    QString langfilename = filename.left(filename.length()-ext.length()-1)+"."+currentlang+"."+ext ;
    if (QFile::exists(langfilename)) return langfilename ;
    return filename ;
}

void QGameSystem::saveObjectToAppData(QString filename, QScriptValue obj)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation) ;
    QDir dir(path) ;
    if (!dir.exists()) dir.mkpath(path) ;
    saveObject(path+"/"+filename,obj) ;
}

void QGameSystem::saveObject(QString filename, QScriptValue obj)
{
    QFile file(getFilenameByLanguageIfExist(filename)) ;
    file.open(QIODevice::WriteOnly);
    file.write(ScriptValue2String(obj).toUtf8()) ;
    file.close() ;
}

QScriptValue QGameSystem::loadObjectFromAppData(QString filename)
{
    return loadObject(QStandardPaths::writableLocation(QStandardPaths::DataLocation)+"/"+filename) ;
}

QScriptValue QGameSystem::loadObject(QString filename)
{
    QFile file(getFilenameByLanguageIfExist(filename));
    if (file.exists()&&file.open(QIODevice::ReadOnly)) {
        QString cmd = "var v = "+file.readAll()+"; v ;" ;
        QScriptValue v = engine->evaluate(cmd) ;
        //qDebug()<<v.toString() ;
        return v ;

        /*
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll()) ;
        file.close() ;

        QScriptValue val = engine->newObject();

        if (doc.isObject()) {
            foreach (QString key, doc.object().keys())
                val.setProperty(key,json2script(engine,doc.object().value(key)));
        }
        if (doc.isArray()) {
            for (int i=0; i<doc.array().count(); i++)
                val.setProperty(i,json2script(engine,doc.array().at(i))) ;
            val.setProperty("length",doc.array().count());
        }
        return val ;
        */

    }
    else
        return QScriptValue::NullValue ;
}

void QGameSystem::setTimeout(QString code, int ms)
{
    timers.append(TimerEvent(engine,code,((float)ms)/1000.0f,true)) ;
}

void QGameSystem::setInterval(QString code, int ms)
{
    timers.append(TimerEvent(engine,code,((float)ms)/1000.0f,false)) ;
}

void QGameSystem::setUsedLanguages(QScriptValue arr)
{
    languages.clear() ;
    for (int i=0; i<arr.property("length").toInt32(); i++)
       languages.append(arr.property(i).toString()) ;
    if (languages.count()>0) currentlang=languages[0] ;
}

void QGameSystem::setCurrentLanguage(QString lang)
{
    if (languages.contains(lang)) currentlang = lang ;
}

void QGameSystem::switchCurrentLanguage()
{
    if (languages.count()==0) return ;
    int idx = languages.indexOf(currentlang) ;
    idx++ ;
    if (idx>=languages.count()) idx=0 ;
    currentlang = languages[idx] ;
}

QString QGameSystem::getCurrentLanguage()
{
    return currentlang ;
}

void QGameSystem::setSoundOn(bool ison)
{
    soundon = ison ;
    emit switchSoundOn(soundon) ;
}

bool QGameSystem::isSoundOn()
{
    return soundon ;
}

void QGameSystem::switchFullScreen()
{
    fullscreen=!fullscreen ;
}

bool QGameSystem::isFullScreen() const
{
    return fullscreen ;
}

void QGameSystem::setDifficult(int code)
{
    tekdifficult = code ;
    if (tekdifficult<0) tekdifficult = 0 ;
    if (tekdifficult>=difficultcount) tekdifficult = difficultcount-1 ;
}

int QGameSystem::getDifficult() const
{
    return tekdifficult ;
}

void QGameSystem::switchDifficult()
{
    tekdifficult++ ;
    if (tekdifficult>=difficultcount) tekdifficult = 0 ;
}

void QGameSystem::setDifficultCount(int count)
{
    difficultcount = count ;
}

void QGameSystem::setCloseHandlerScript(const QString &script)
{
    closehandlerscript = script ;
}

QString QGameSystem::getCloseHandlerScript() const
{
    return closehandlerscript ;
}

void QGameSystem::update(float dt)
{
    for (int i=0; i<timers.length(); i++)
        timers[i].update(dt) ;
}
TimerEvent::TimerEvent(QScriptEngine * engine, QString code, float secs, bool isonce)
{
    this->engine = engine ;
    this->code = code ;
    this->isonce = isonce ;
    this->secs = secs ;
    this->left = secs ;
}

void TimerEvent::update(float dt)
{
    if (left<=0) return ;
    left-=dt ;
    if (left<=0) {
        engine->evaluate(code) ;
        if (!isonce) left=secs ;
    }
}

bool isFullScreen() {
    return fullscreen ;
}

bool onceTestFullScreenSwitch() {
    if (oldfullscreen!=fullscreen) {
        oldfullscreen = fullscreen ;
        return true ;
    }
    else
        return false ;
}
