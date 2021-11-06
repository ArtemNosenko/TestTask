#include "controller.h"

#include "fileanalyzer.h"
#include <QFileDialog>
#include <QDir>

Controller::Controller(QObject *parent) : QObject(parent)
{
    FileAnalyzer *worker = new FileAnalyzer;
    worker->moveToThread(&_fileAnalyzerThread);
    connect(&_fileAnalyzerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &Controller::sendFileNameToAnalyze, worker, &FileAnalyzer::analyzeFile);
    connect(worker, &FileAnalyzer::newTop, this, &Controller::handleResults);
    _fileAnalyzerThread.start();
}



Controller::~Controller() {
    _fileAnalyzerThread.quit();
    _fileAnalyzerThread.wait();
}

bool Controller::openFileDialog()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open file"), QDir::currentPath(), tr("Text Files (*.txt)"));
    if (!fileName.isEmpty())
    {
        setTopWordsCount({});
        setTop({});
        emit updateHistogramm();
        emit sendFileNameToAnalyze(fileName);
        return true;
    }
    return  false;
}

void Controller::handleResults(const QStringList & top, QVariantList topWordsCount)
{
    setTopWordsCount(topWordsCount);
    setTop(top);
    emit updateHistogramm();
}

const QStringList &Controller::top() const
{
    return m_top;
}

void Controller::setTop(const QStringList &newTop)
{
    if (m_top == newTop)
        return;
    m_top = newTop;
    emit topChanged();
}


const QVariantList &Controller::topWordsCount() const
{
    return m_topWordsCount;
}

void Controller::setTopWordsCount(const QVariantList &newTopWordsCount)
{
    if (m_topWordsCount == newTopWordsCount)
        return;
    m_topWordsCount = newTopWordsCount;
    emit topWordsCountChanged();
}

