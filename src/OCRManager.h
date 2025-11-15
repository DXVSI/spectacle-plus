/* SPDX-FileCopyrightText: 2025
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include <QImage>
#include <QObject>
#include <QString>

class OCRManager : public QObject
{
    Q_OBJECT

    public:

    static OCRManager *instance();

    QString recognizeText(const QImage &image, const QString &lang = QStringLiteral("eng+rus"));

    Q_SIGNALS:

    void textRecognized(const QString &text);
    void errorOccurred(const QString &error);

    private:

    explicit OCRManager(QObject *parent = nullptr);
    ~OCRManager() override = default;

    OCRManager(OCRManager const &) = delete;
    void operator=(OCRManager const &) = delete;
};
