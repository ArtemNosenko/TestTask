#include "fileanalyzer.h"
#include <QFile>
#include <iostream>
#include <QtDebug>

#include <vector>

FileAnalyzer::FileAnalyzer(QObject *parent) : QObject(parent)
{

}

void FileAnalyzer::analyzeFile(const QString &fileName) {

    QFile file(fileName);
    QString cutedString;
    if (file.open(QIODevice::ReadOnly))
    {
        while (true) {
            //Если слово разделилось на несколько частей, то от предыдущей итерации вставляем начало
            const QString &data = cutedString +  file.read(countSymbolsToRead).simplified();

            if (data.isEmpty())
                break;

            auto result = getCutedStrAndWords(data);
            cutedString = result.first;

            for (const auto & word: qAsConst(result.second))
                ++map[word];

            std::vector<std::pair<QString,int>> vec;
            vec.reserve(map.size());
            std::copy(map.begin(),map.end(), std::back_inserter(vec));

            std::size_t countElementsToPartialSort = map.size() > 15 ? 15 : map.size();

            std::partial_sort(vec.begin(), vec.begin() + countElementsToPartialSort, vec.end(),[](const auto &lhs, const auto &rhs){
                return lhs.second > rhs.second;
            });
            vec.resize(countElementsToPartialSort);
            //Сортировка по алфавиту. Если закоментить, то будет отсортированно по кол-ву вхождения
            std::sort(vec.begin(), vec.end(),[](const auto &lhs, const auto &rhs){
                return lhs.first < rhs.first;
            });

            QStringList words;
            QVariantList counts;
            for ( const auto & [word, count] : vec)
            {
                //  words.prepend(word);
                // counts.prepend(count);
                words.append(word);
                counts.append(count);
            }
            emit newTop(words,counts);
            //Если файл очень большой, то сортировка каждые 10000 элементов будет занимать много времени
            //Для огромных файлов нужно будет поставить ограничение countSymbolsToRead, после которого не будет увеличиваться,
            //иначе прогреcc будет не виден
            if (countSymbolsToRead < 10000000)
                countSymbolsToRead *=2;
        }
        map.clear();
        countSymbolsToRead = 10000;
    }
    else
        std::cerr<<"Cant open file " << fileName.toStdString()<<". "<<file.errorString().toStdString();
}

std::pair<QString,QStringList > FileAnalyzer::getCutedStrAndWords(const QString &str) const
{
    QString cutedString;
    QStringList words = str.split(' ');

    int lastSpace = str.lastIndexOf(' ');

    //-1 и + 1для устранения лишних пробелов
    if (lastSpace != -1 && lastSpace + 1 != countSymbolsToRead)
    {
        cutedString = words.last();
        words.pop_back();
    }
    return  {cutedString,words};
}
