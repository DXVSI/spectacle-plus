/* SPDX-FileCopyrightText: 2025
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class TranslateManager : public QObject
{
    Q_OBJECT

    public:

    static TranslateManager *instance();

    void translateText(const QString &text,
                       const QString &sourceLang = QStringLiteral("auto"),
                       const QString &targetLang = QStringLiteral("ru"));

    Q_SIGNALS:

    void translationReady(const QString &translatedText);
    void errorOccurred(const QString &error);

    private:

    explicit TranslateManager(QObject *parent = nullptr);
    ~TranslateManager() override = default;

    TranslateManager(TranslateManager const &) = delete;
    void operator=(TranslateManager const &) = delete;

    void onTranslationReply(QNetworkReply *reply);

    QNetworkAccessManager *mNetworkManager;
};
