/* SPDX-FileCopyrightText: 2025
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "TranslateDialog.h"
#include "settings.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QSettings>

using namespace Qt::StringLiterals;

TranslateDialog::TranslateDialog(const QString &recognizedText, QWidget *parent)
    : QDialog(parent)
    , mRecognizedText(recognizedText)
{
    setupUi();
    loadSettings();

    // Если текст уже есть, показываем его
    if (!recognizedText.isEmpty()) {
        mRecognizedTextEdit->setPlainText(recognizedText);
    }
}

void TranslateDialog::setupUi()
{
    setWindowTitle(QStringLiteral("OCR and Translation"));
    setMinimumWidth(650);
    setMinimumHeight(550);

    auto *mainLayout = new QVBoxLayout(this);

    // OCR Language selection
    auto *ocrGroup = new QGroupBox(QStringLiteral("OCR Settings"));
    auto *ocrLayout = new QHBoxLayout();

    auto *ocrLabel = new QLabel(QStringLiteral("OCR Language:"));
    mOcrLanguageCombo = new QComboBox();
    populateOcrLanguages();
    mRecognizeButton = new QPushButton(QStringLiteral("Re-recognize"));

    ocrLayout->addWidget(ocrLabel);
    ocrLayout->addWidget(mOcrLanguageCombo, 1);
    ocrLayout->addWidget(mRecognizeButton);

    ocrGroup->setLayout(ocrLayout);
    mainLayout->addWidget(ocrGroup);

    // Recognized text
    auto *recognizedGroup = new QGroupBox(QStringLiteral("Recognized Text"));
    auto *recognizedLayout = new QVBoxLayout();
    mRecognizedTextEdit = new QTextEdit();
    mRecognizedTextEdit->setReadOnly(false);
    mRecognizedTextEdit->setMaximumHeight(150);
    mRecognizedTextEdit->setPlaceholderText(QStringLiteral("Press 'Re-recognize' to change OCR language..."));
    recognizedLayout->addWidget(mRecognizedTextEdit);
    recognizedGroup->setLayout(recognizedLayout);
    mainLayout->addWidget(recognizedGroup);

    // Translation Language selection
    auto *translateGroup = new QGroupBox(QStringLiteral("Translation Settings"));
    auto *translateLayout = new QFormLayout();

    mSourceLanguageCombo = new QComboBox();
    mTargetLanguageCombo = new QComboBox();
    populateLanguages();

    translateLayout->addRow(QStringLiteral("Source Language:"), mSourceLanguageCombo);
    translateLayout->addRow(QStringLiteral("Target Language:"), mTargetLanguageCombo);

    translateGroup->setLayout(translateLayout);
    mainLayout->addWidget(translateGroup);

    // Translated text
    auto *translatedGroup = new QGroupBox(QStringLiteral("Translation"));
    auto *translatedLayout = new QVBoxLayout();
    mTranslatedTextEdit = new QTextEdit();
    mTranslatedTextEdit->setReadOnly(true);
    mTranslatedTextEdit->setPlaceholderText(QStringLiteral("Press 'Translate' button to get translation..."));
    translatedLayout->addWidget(mTranslatedTextEdit);
    translatedGroup->setLayout(translatedLayout);
    mainLayout->addWidget(translatedGroup);

    // Buttons
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    mTranslateButton = new QPushButton(QStringLiteral("Translate"));
    mTranslateButton->setDefault(true);
    mCloseButton = new QPushButton(QStringLiteral("Close"));

    buttonLayout->addWidget(mTranslateButton);
    buttonLayout->addWidget(mCloseButton);
    mainLayout->addLayout(buttonLayout);

    // Connections
    connect(mRecognizeButton, &QPushButton::clicked, this, [this]() {
        mRecognizedTextEdit->setPlainText(QStringLiteral("Recognizing..."));
        Q_EMIT ocrRequested(ocrLanguage());
    });

    connect(mTranslateButton, &QPushButton::clicked, this, [this]() {
        if (mRecognizedTextEdit->toPlainText().isEmpty()) {
            return;
        }
        mTranslatedTextEdit->setPlainText(QStringLiteral("Translating..."));
        Q_EMIT translateRequested(sourceLanguage(), targetLanguage());
    });

    connect(mCloseButton, &QPushButton::clicked, this, &QDialog::accept);

    connect(mOcrLanguageCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TranslateDialog::saveSettings);
    connect(mSourceLanguageCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TranslateDialog::saveSettings);
    connect(mTargetLanguageCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TranslateDialog::saveSettings);
}

void TranslateDialog::populateLanguages()
{
    // Список популярных языков LibreTranslate
    mLanguages = {
        {QStringLiteral("auto"), QStringLiteral("Auto Detect")},
        {QStringLiteral("en"), QStringLiteral("English")},
        {QStringLiteral("ru"), QStringLiteral("Russian")},
        {QStringLiteral("es"), QStringLiteral("Spanish")},
        {QStringLiteral("fr"), QStringLiteral("French")},
        {QStringLiteral("de"), QStringLiteral("German")},
        {QStringLiteral("it"), QStringLiteral("Italian")},
        {QStringLiteral("pt"), QStringLiteral("Portuguese")},
        {QStringLiteral("pl"), QStringLiteral("Polish")},
        {QStringLiteral("tr"), QStringLiteral("Turkish")},
        {QStringLiteral("ja"), QStringLiteral("Japanese")},
        {QStringLiteral("zh"), QStringLiteral("Chinese")},
        {QStringLiteral("ar"), QStringLiteral("Arabic")},
        {QStringLiteral("hi"), QStringLiteral("Hindi")},
        {QStringLiteral("ko"), QStringLiteral("Korean")},
        {QStringLiteral("uk"), QStringLiteral("Ukrainian")},
        {QStringLiteral("cs"), QStringLiteral("Czech")},
        {QStringLiteral("nl"), QStringLiteral("Dutch")},
        {QStringLiteral("sv"), QStringLiteral("Swedish")},
        {QStringLiteral("da"), QStringLiteral("Danish")},
    };

    for (const auto &lang : mLanguages) {
        mSourceLanguageCombo->addItem(lang.name, lang.code);
        // Target language не должен иметь "Auto Detect"
        if (lang.code != QStringLiteral("auto")) {
            mTargetLanguageCombo->addItem(lang.name, lang.code);
        }
    }
}

void TranslateDialog::populateOcrLanguages()
{
    // Tesseract language codes
    mOcrLanguageCombo->addItem(QStringLiteral("English + Russian"), QStringLiteral("eng+rus"));
    mOcrLanguageCombo->addItem(QStringLiteral("English"), QStringLiteral("eng"));
    mOcrLanguageCombo->addItem(QStringLiteral("Russian"), QStringLiteral("rus"));
    mOcrLanguageCombo->addItem(QStringLiteral("Spanish"), QStringLiteral("spa"));
    mOcrLanguageCombo->addItem(QStringLiteral("French"), QStringLiteral("fra"));
    mOcrLanguageCombo->addItem(QStringLiteral("German"), QStringLiteral("deu"));
    mOcrLanguageCombo->addItem(QStringLiteral("Italian"), QStringLiteral("ita"));
    mOcrLanguageCombo->addItem(QStringLiteral("Portuguese"), QStringLiteral("por"));
    mOcrLanguageCombo->addItem(QStringLiteral("Chinese Simplified"), QStringLiteral("chi_sim"));
    mOcrLanguageCombo->addItem(QStringLiteral("Japanese"), QStringLiteral("jpn"));
    mOcrLanguageCombo->addItem(QStringLiteral("Korean"), QStringLiteral("kor"));
    mOcrLanguageCombo->addItem(QStringLiteral("Arabic"), QStringLiteral("ara"));
}

void TranslateDialog::loadSettings()
{
    QSettings settings;
    settings.beginGroup(QStringLiteral("Translation"));

    QString ocrLang = settings.value(QStringLiteral("ocrLanguage"), QStringLiteral("eng+rus")).toString();
    QString sourceLang = settings.value(QStringLiteral("sourceLanguage"), QStringLiteral("auto")).toString();
    QString targetLang = settings.value(QStringLiteral("targetLanguage"), QStringLiteral("ru")).toString();

    // Устанавливаем сохраненные языки
    int ocrIndex = mOcrLanguageCombo->findData(ocrLang);
    if (ocrIndex >= 0) {
        mOcrLanguageCombo->setCurrentIndex(ocrIndex);
    }

    int sourceIndex = mSourceLanguageCombo->findData(sourceLang);
    if (sourceIndex >= 0) {
        mSourceLanguageCombo->setCurrentIndex(sourceIndex);
    }

    int targetIndex = mTargetLanguageCombo->findData(targetLang);
    if (targetIndex >= 0) {
        mTargetLanguageCombo->setCurrentIndex(targetIndex);
    }

    settings.endGroup();
}

void TranslateDialog::saveSettings()
{
    QSettings settings;
    settings.beginGroup(QStringLiteral("Translation"));
    settings.setValue(QStringLiteral("ocrLanguage"), ocrLanguage());
    settings.setValue(QStringLiteral("sourceLanguage"), sourceLanguage());
    settings.setValue(QStringLiteral("targetLanguage"), targetLanguage());
    settings.endGroup();
}

QString TranslateDialog::ocrLanguage() const
{
    return mOcrLanguageCombo->currentData().toString();
}

QString TranslateDialog::sourceLanguage() const
{
    return mSourceLanguageCombo->currentData().toString();
}

QString TranslateDialog::targetLanguage() const
{
    return mTargetLanguageCombo->currentData().toString();
}

QString TranslateDialog::recognizedText() const
{
    return mRecognizedTextEdit->toPlainText();
}

void TranslateDialog::setTranslatedText(const QString &text)
{
    mTranslatedTextEdit->setPlainText(text);
}

void TranslateDialog::setRecognizedText(const QString &text)
{
    mRecognizedText = text;
    mRecognizedTextEdit->setPlainText(text);
}
