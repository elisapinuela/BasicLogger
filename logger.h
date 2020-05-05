#pragma once

#include <QObject>
#include <QPlainTextEdit>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

typedef enum LOGGER_LEVEL {
    LOGGER_LEVEL_DEBUG = 0,
    LOGGER_LEVEL_INFO,
    LOGGER_LEVEL_WARNING,
    LOGGER_LEVEL_ERROR,
    LOGGER_LEVEL_CRITICAL,
    LOGGER_LEVEL_UNKNOWN = 0xFFFFFFFF
}LOGGER_LEVEL;

class Logger
{
public:
    ~Logger();
    void setShowDateTime(bool value);

    static Logger & GetInstance()
    {
        static Logger instance;
        return instance;
    }
    static void SetConfig(LOGGER_LEVEL eLevel = LOGGER_LEVEL_INFO,
                          QString filename = "",
                          bool bLogConsole = true,
                          QPlainTextEdit * editor=nullptr);

    void Debug(const QString &value);
    void Info(const QString &value);
    void Warning(const QString &value);
    void Error(const QString &value);
    void Critical(const QString &value);
private:
    static QFile m_file;
    static QPlainTextEdit * m_editor;
    static bool m_bLogConsole;
    static void SetDefaultFilename();
    static LOGGER_LEVEL m_eLogLevel;

    explicit Logger();
    QString BuildLogMessage(LOGGER_LEVEL eLevel, QString &value);
    void Log(LOGGER_LEVEL eLevel, const QString &value);

};

