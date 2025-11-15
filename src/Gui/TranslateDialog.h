/* SPDX-FileCopyrightText: 2025
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include <QDialog>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>

class TranslateDialog : public QDialog
{
    Q_OBJECT

    public:

    explicit TranslateDialog(const QString &recognizedText, QWidget *parent = nullptr);
    ~TranslateDialog() override = default;

    QString ocrLanguage() const;
    QString sourceLanguage() const;
    QString targetLanguage() const;
    QString recognizedText() const;

    void setTranslatedText(const QString &text);
    void setRecognizedText(const QString &text);

    Q_SIGNALS:

    void translateRequested(const QString &sourceLanguage, const QString &targetLanguage);
    void ocrRequested(const QString &ocrLanguage);

    private:

    void setupUi();
    void loadSettings();
    void saveSettings();
    void populateLanguages();
    void populateOcrLanguages();

    struct LanguageInfo {
        QString code;
        QString name;
    };

    QComboBox *mOcrLanguageCombo;
    QComboBox *mSourceLanguageCombo;
    QComboBox *mTargetLanguageCombo;
    QTextEdit *mRecognizedTextEdit;
    QTextEdit *mTranslatedTextEdit;
    QPushButton *mRecognizeButton;
    QPushButton *mTranslateButton;
    QPushButton *mCloseButton;
    QString mRecognizedText;
    QList<LanguageInfo> mLanguages;
};
