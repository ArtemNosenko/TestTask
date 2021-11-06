#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

#include <QThread>
#include <QVariant>

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList top READ top WRITE setTop NOTIFY topChanged)
    Q_PROPERTY(QVariantList topWordsCount READ topWordsCount WRITE setTopWordsCount NOTIFY topWordsCountChanged)

public:
    explicit Controller(QObject *parent = nullptr);
    virtual  ~Controller() override;


    Q_INVOKABLE bool openFileDialog();

    const QStringList &top() const;


    const QVariantList &topWordsCount() const;



public slots:
    void handleResults(const QStringList & top, QVariantList topWordCount);
    void setTopWordsCount(const QVariantList &newTopWordsCount);
    void setTop(const QStringList &newTop);
signals:
    void sendFileNameToAnalyze(const QString &);
    void topChanged();
    void topWordsCountChanged();
    void updateHistogramm();


private:
    QThread _fileAnalyzerThread;
    QStringList m_top;
    QVariantList m_topWordsCount;
};

#endif // CONTROLLER_H
