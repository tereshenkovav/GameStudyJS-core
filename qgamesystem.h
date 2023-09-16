#ifndef QGAMESYSTEM_H
#define QGAMESYSTEM_H

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>

class TimerEvent
{
public:
    TimerEvent(QScriptEngine * engine, QString code, float secs, bool isonce);
    void update(float dt) ;
private:
    QScriptEngine * engine ;
    QString code ;
    bool isonce ;
    float left ;
    float secs ;
};

class QGameSystem : public QObject
{
    Q_OBJECT 
public:
    explicit QGameSystem(QScriptEngine * engine);

    void clearTimers() ;

    static QString ScriptValue2String(const QScriptValue & val) ;
    QString getFilenameByLanguageIfExist(QString filename) ;

private:
    QScriptEngine * engine ;
    QList<TimerEvent> timers ;

signals:
    void writeMessage(QString msg) ;
    void writePair(QString name, QVariant value) ;
    void showCursor(bool showed) ;
    void switchSoundOn(bool ison) ;

public slots:
    void saveObject(QString filename, QScriptValue obj) ;
    QScriptValue loadObject(QString filename) ;
    void saveObjectToAppData(QString filename, QScriptValue obj) ;
    QScriptValue loadObjectFromAppData(QString filename) ;
    void setTimeout(QString code, int ms) ;
    void setInterval(QString code, int ms) ;
    void setUsedLanguages(QScriptValue arr) ;
    void setCurrentLanguage(QString lang) ;
    void switchCurrentLanguage() ;
    QString getCurrentLanguage() ;
    void setSoundOn(bool ison) ;
    bool isSoundOn() ;
    void setDifficult(int code) ;
    int getDifficult() const ;
    void switchDifficult() ;
    void setDifficultCount(int count) ;
    void setCloseHandlerScript(const QString & script) ;
    QString getCloseHandlerScript() const ;
    void update(float dt) ;
};

#endif // QGAMESYSTEM_H
