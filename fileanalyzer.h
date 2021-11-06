#ifndef FILEANALYZER_H
#define FILEANALYZER_H

#include <QObject>
#include <unordered_map>
class FileAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit FileAnalyzer(QObject *parent = nullptr);
public slots:
    void analyzeFile(const QString &fileName);

signals:
    void newTop(const QStringList &top,QVariantList);
private:
    //Обрезает часть слова в конце и возвращает
    //также возвращает stringlist с целыми словами
   std::pair<QString,QStringList > getCutedStrAndWords(const QString & str) const;

    int countSymbolsToRead = 10000;
    std::unordered_map<QString,int> map;

};

#endif // FILEANALYZER_H
