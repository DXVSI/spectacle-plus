/* SPDX-FileCopyrightText: 2025
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "OCRManager.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <QBuffer>
#include <QDebug>

OCRManager::OCRManager(QObject *parent)
    : QObject(parent)
{
}

OCRManager *OCRManager::instance()
{
    static OCRManager instance;
    return &instance;
}

QString OCRManager::recognizeText(const QImage &image, const QString &lang)
{
    if (image.isNull()) {
        Q_EMIT errorOccurred(QStringLiteral("Image is null"));
        return QString();
    }

    // Конвертируем QImage в формат для tesseract
    QImage img = image.convertToFormat(QImage::Format_RGB888);

    // Увеличиваем изображение в 2 раза для лучшего распознавания мелкого текста
    if (img.width() < 1000 || img.height() < 500) {
        img = img.scaled(img.width() * 2, img.height() * 2,
                         Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();

    // Инициализируем tesseract
    if (api->Init(nullptr, lang.toStdString().c_str())) {
        Q_EMIT errorOccurred(QStringLiteral("Could not initialize tesseract"));
        delete api;
        return QString();
    }

    // Настраиваем tesseract для распознавания кода
    // PSM 6 = Assume a single uniform block of text (best for code blocks)
    api->SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);

    // Включаем улучшенное распознавание
    api->SetVariable("preserve_interword_spaces", "1");
    api->SetVariable("tessedit_char_whitelist", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_.:;(){}[]<>+-=*/\\\"'#@!?&|~`$%^ \t\n");

    // Устанавливаем изображение
    api->SetImage(img.bits(), img.width(), img.height(),
                  3, img.bytesPerLine());

    // Распознаем текст
    char* outText = api->GetUTF8Text();
    QString result = QString::fromUtf8(outText);

    delete[] outText;
    api->End();
    delete api;

    Q_EMIT textRecognized(result);
    return result;
}
