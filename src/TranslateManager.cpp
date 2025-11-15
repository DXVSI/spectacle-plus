/* SPDX-FileCopyrightText: 2025
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "TranslateManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QSslError>

TranslateManager::TranslateManager(QObject *parent)
    : QObject(parent)
    , mNetworkManager(new QNetworkAccessManager(this))
{
    connect(mNetworkManager, &QNetworkAccessManager::finished,
            this, &TranslateManager::onTranslationReply);

    // Игнорируем SSL ошибки для libretranslate
    connect(mNetworkManager, &QNetworkAccessManager::sslErrors,
            this, [](QNetworkReply *reply, const QList<QSslError> &) {
        reply->ignoreSslErrors();
    });
}

TranslateManager *TranslateManager::instance()
{
    static TranslateManager instance;
    return &instance;
}

void TranslateManager::translateText(const QString &text,
                                     const QString &sourceLang,
                                     const QString &targetLang)
{
    if (text.isEmpty()) {
        Q_EMIT errorOccurred(QStringLiteral("Text is empty"));
        return;
    }

    // Используем MyMemory API - бесплатный сервис перевода
    // Преобразуем коды языков для MyMemory
    QString fromLang = sourceLang;
    QString toLang = targetLang;

    // MyMemory использует двухбуквенные коды
    if (fromLang == QStringLiteral("auto")) {
        fromLang = QStringLiteral("en"); // По умолчанию английский
    }

    // Формируем URL с параметрами
    QString urlString = QStringLiteral("https://api.mymemory.translated.net/get");
    QUrl url(urlString);
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("q"), text);
    query.addQueryItem(QStringLiteral("langpair"), fromLang + QStringLiteral("|") + toLang);
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, QStringLiteral("Spectacle/1.0"));

    mNetworkManager->get(request);
}

void TranslateManager::onTranslationReply(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        QString errorMsg = QStringLiteral("Network error: ") + reply->errorString();
        errorMsg += QStringLiteral("\nHTTP Status: ") + QString::number(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
        Q_EMIT errorOccurred(errorMsg);
        return;
    }

    QByteArray response = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(response);

    if (!doc.isObject()) {
        Q_EMIT errorOccurred(QStringLiteral("Invalid response format. Response: ") + QString::fromUtf8(response));
        return;
    }

    QJsonObject obj = doc.object();

    // MyMemory API возвращает структуру:
    // {
    //   "responseData": {
    //     "translatedText": "перевод",
    //     "match": 1.0
    //   },
    //   "responseStatus": 200,
    //   "respondDetails": null
    // }

    // Проверяем статус
    if (obj.contains(QStringLiteral("responseStatus"))) {
        int status = obj[QStringLiteral("responseStatus")].toInt();
        if (status != 200) {
            QString errorDetails = obj[QStringLiteral("responseDetails")].toString();
            Q_EMIT errorOccurred(QStringLiteral("API Error (status %1): %2").arg(status).arg(errorDetails));
            return;
        }
    }

    // Получаем перевод из responseData
    if (obj.contains(QStringLiteral("responseData"))) {
        QJsonObject responseData = obj[QStringLiteral("responseData")].toObject();
        if (responseData.contains(QStringLiteral("translatedText"))) {
            QString translated = responseData[QStringLiteral("translatedText")].toString();
            Q_EMIT translationReady(translated);
            return;
        }
    }

    Q_EMIT errorOccurred(QStringLiteral("No translation in response. Response: ") + QString::fromUtf8(response));
}
