#include "logger.h"

QFile Logger::m_file;
QPlainTextEdit * Logger::m_editor = nullptr;
bool Logger::m_bLogConsole = true;
LOGGER_LEVEL Logger::m_eLogLevel = LOGGER_LEVEL_INFO;

void Logger::SetConfig(LOGGER_LEVEL eLevel,
                       QString filename,
                       bool bLogConsose,
                       QPlainTextEdit * editor)
{
    if (!filename.isEmpty())
    {
        m_file.setFileName(filename);
    }
    else
    {
        SetDefaultFilename();
    }
    m_bLogConsole = bLogConsose;
    m_editor = editor;
    m_eLogLevel = eLevel;
}


Logger::Logger()
{
    if (!m_file.fileName().length())
    {
        SetDefaultFilename();
    }
    QFile * file = new QFile;
    file->setFileName(m_file.fileName());
    file->open(QIODevice::Append | QIODevice::Text);
    bool bOpenSuccess = m_file.open(QIODevice::Append | QIODevice::Text);
    if (!bOpenSuccess)
    {
        QString log_text = QString("Error opening logfile. Maybe invalid filename %1").arg(m_file.fileName());
        QString log_message = BuildLogMessage(LOGGER_LEVEL_CRITICAL, log_text);
        QTextStream(stdout) << log_message << endl;
    }
}

void Logger::Log(LOGGER_LEVEL eLevel, const QString &value)
{
    if (!m_file.isOpen())
    {
        return;
    }
    // Check log level
    if (eLevel < m_eLogLevel)
    {
        return;
    }
    QString new_value = value;
    QString log_message = BuildLogMessage(eLevel, new_value);
    QTextStream outStream(&m_file);
    outStream.setCodec("UTF-8");
    outStream << log_message << endl;
    if (m_editor != 0)
    {
       m_editor->appendPlainText(log_message);
    }
    if (m_bLogConsole)
    {
        QTextStream(stdout) << log_message << endl;
    }
}

Logger::~Logger()
{
    m_file.close();
}

void Logger::SetDefaultFilename()
{
    QString filename= "LOG_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".txt";
    m_file.setFileName(filename);
}

QString Logger::BuildLogMessage(LOGGER_LEVEL eLevel, QString &value)
{
    QString level_label;
    switch(eLevel)
    {
    case LOGGER_LEVEL_INFO:
        level_label = "INFO";
        break;
    case LOGGER_LEVEL_DEBUG:
        level_label = "DEBUG";
        break;
    case LOGGER_LEVEL_ERROR:
        level_label = "ERROR";
        break;
    case LOGGER_LEVEL_WARNING:
        level_label = "WARNING";
        break;
    case LOGGER_LEVEL_CRITICAL:
        level_label = "CRITICAL";
        break;
    default:
        // Set critical label to generate special error log message indicating unknown log level
        level_label = "CRITICAL";
        value = QString("Unknown argument. Invalid logger level %1").arg(eLevel);
        break;
    }

    QString log_message = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") + "\t" +
            level_label + "\t" + value;
    return log_message;
}

void Logger::Debug(const QString &value)
{
    Log(LOGGER_LEVEL_DEBUG, value);
}

void Logger::Info(const QString &value)
{
    Log(LOGGER_LEVEL_INFO, value);
}

void Logger::Warning(const QString &value)
{
    Log(LOGGER_LEVEL_WARNING, value);
}

void Logger::Error(const QString &value)
{
    Log(LOGGER_LEVEL_ERROR, value);
}

void Logger::Critical(const QString &value)
{
    Log(LOGGER_LEVEL_CRITICAL, value);
}

