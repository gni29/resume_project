#include "write_resume_window.h"
#include "num_issue_window.h"

#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>

WriteResumeWindow::WriteResumeWindow(QWidget* parent, const QString& company_dir)
    : QDialog(parent),
      company_directory(company_dir),
      num_issues(1),
      check_button(nullptr),
      adapt_button(nullptr),
      retouch_button(nullptr),
      cancel_button(nullptr),
      page_stack(nullptr),
      prev_button(nullptr),
      next_button(nullptr),
      page_indicator(nullptr),
      auto_save_timer(nullptr) {

    setWindowTitle(tr("자기소개서 작성"));

    createResumeDirectory();

    QDir resumeDir(company_directory);
    QStringList txtFiles = resumeDir.entryList(QStringList() << "issue_*.txt", QDir::Files);
    num_issues = txtFiles.size();

    num_issues = qBound(1, num_issues, 10);

    setupUi();
    loadResume();

    QTimer* autoSaveTimer = new QTimer(this);
    connect(autoSaveTimer, &QTimer::timeout, this, &WriteResumeWindow::autoSaveResume);
    autoSaveTimer->start(100000);
}
WriteResumeWindow::~WriteResumeWindow() {
    // 필요한 정리 작업이나 리소스 해제 수행
    // 예를 들어, 동적으로 할당된 메모리 해제 등
    
    // 동적 할당된 타이머 정리
    if (auto_save_timer) {
        auto_save_timer->stop();
        delete auto_save_timer;
        auto_save_timer = nullptr;
    }
    
    // 벡터의 동적 할당된 위젯들 정리
    for (QLineEdit* input : issue_inputs) {
        delete input;
    }
    for (QTextEdit* input : write_issue_inputs) {
        delete input;
    }
    for (QLabel* counter : character_counters) {
        delete counter;
    }
}
void WriteResumeWindow::setupUi() {
    setStyleSheet(
        "QDialog {"
        "    background-color: #2b2b2b;"
        "    color: white;"
        "}"
        "QLabel {"
        "    color: white;"
        "    font-weight: bold;"
        "}"
        "QLineEdit, QTextEdit {"
        "    background-color: #3c3f41;"
        "    color: white;"
        "    border: 1px solid #5a5a5a;"
        "    padding: 5px;"
        "    border-radius: 3px;"
        "}"
        "QLineEdit::placeholder, QTextEdit::placeholder {"
        "    color: #a0a0a0;"
        "}"
        "QPushButton {"
        "    background-color: #4a6984;"
        "    color: white;"
        "    border: none;"
        "    padding: 6px 12px;"
        "    border-radius: 3px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #5a7fa0;"
        "}"
    );

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    page_stack = new QStackedWidget();

    for (int i = 0; i < num_issues; i++) {
        QWidget* page = new QWidget();
        QVBoxLayout* pageLayout = new QVBoxLayout(page);

        QLabel* questionLabel = new QLabel(tr("문항 질문"));
        QLineEdit* issueTitle = new QLineEdit();
        issueTitle->setPlaceholderText(tr("문항 질문을 입력하세요"));

        QLabel* answerLabel = new QLabel(tr("답변"));
        QTextEdit* issueContent = new QTextEdit();
        issueContent->setPlaceholderText(tr("답변을 작성하세요 (최대 2000자)"));
        issueContent->setMinimumHeight(200);

        QLabel* counter = new QLabel("0자");
        counter->setAlignment(Qt::AlignRight);

        pageLayout->addWidget(questionLabel);
        pageLayout->addWidget(issueTitle);
        pageLayout->addWidget(answerLabel);
        pageLayout->addWidget(issueContent);
        pageLayout->addWidget(counter);

        issue_inputs.append(issueTitle);
        write_issue_inputs.append(issueContent);
        character_counters.append(counter);

        connect(issueTitle, &QLineEdit::textChanged, this, &WriteResumeWindow::onContentChanged);
        connect(issueContent, &QTextEdit::textChanged, this, &WriteResumeWindow::onContentChanged);

        page_stack->addWidget(page);
    }

    mainLayout->addWidget(page_stack);

    QHBoxLayout* navigationLayout = new QHBoxLayout();
    prev_button = new QPushButton(tr("이전 문항"));
    next_button = new QPushButton(tr("다음 문항"));
    page_indicator = new QLabel();
    navigationLayout->addWidget(prev_button);
    navigationLayout->addStretch();
    navigationLayout->addWidget(page_indicator);
    navigationLayout->addStretch();
    navigationLayout->addWidget(next_button);
    mainLayout->addLayout(navigationLayout);

    connect(prev_button, &QPushButton::clicked, this, &WriteResumeWindow::showPreviousPage);
    connect(next_button, &QPushButton::clicked, this, &WriteResumeWindow::showNextPage);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    QPushButton* applyButton = new QPushButton(tr("확인"));
    connect(applyButton, &QPushButton::clicked, this, &WriteResumeWindow::onCheckClicked);
    buttonLayout->addWidget(applyButton);

    QPushButton* okButton = new QPushButton(tr("적용"));
    connect(okButton, &QPushButton::clicked, this, &WriteResumeWindow::onAdaptClicked);
    buttonLayout->addWidget(okButton);

    QPushButton* cancelButton = new QPushButton(tr("취소"));
    connect(cancelButton, &QPushButton::clicked, this, &WriteResumeWindow::onCancelClicked);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
    resize(600, 500);

    updateNavigationButtons();
}

void WriteResumeWindow::showNextPage() {
    int currentIndex = page_stack->currentIndex();
    if (currentIndex < page_stack->count() - 1) {
        page_stack->setCurrentIndex(currentIndex + 1);
        updateNavigationButtons();
    }
}

void WriteResumeWindow::showPreviousPage() {
    int currentIndex = page_stack->currentIndex();
    if (currentIndex > 0) {
        page_stack->setCurrentIndex(currentIndex - 1);
        updateNavigationButtons();
    }
}

void WriteResumeWindow::updateNavigationButtons() {
    int currentIndex = page_stack->currentIndex();
    int pageCount = page_stack->count();

    prev_button->setEnabled(currentIndex > 0);
    next_button->setEnabled(currentIndex < pageCount - 1);

    page_indicator->setText(QString("%1 / %2").arg(currentIndex + 1).arg(pageCount));
}

void WriteResumeWindow::autoSaveResume() {
    saveResume(true);
}

void WriteResumeWindow::onContentChanged() {
    if (!autoSaveTimer()->isActive()) {
        autoSaveTimer()->start(3000);
    }

    for (int i = 0; i < write_issue_inputs.size(); i++) {
        QString text = write_issue_inputs[i]->toPlainText().trimmed();
        int count = text.length();
        int koreanCount = 0;
        for (const QChar& ch : text) {
            if ((ch.unicode() >= 0xAC00 && ch.unicode() <= 0xD7A3)) {
                koreanCount++;
            }
        }
        character_counters[i]->setText(QString("%1자").arg(koreanCount > 0 ? koreanCount : count));
    }
}

void WriteResumeWindow::onAdaptClicked() {
    saveResume(false);
}

void WriteResumeWindow::onCheckClicked() {
    saveResume(false);
    accept();
}

void WriteResumeWindow::onRetouchClicked() {
    openNumIssueWindow();
}

void WriteResumeWindow::onCancelClicked() {
    loadResume();
    reject();
}

void WriteResumeWindow::openNumIssueWindow() {
    try {
        NumIssueWindow* num_issue_win = new NumIssueWindow(this, company_directory, this);
        if (!num_issue_win) {
            throw std::runtime_error("문항 수 입력 창 생성 실패");
        }

        num_issue_win->exec();
    } catch (const std::exception& e) {
        qCritical() << "문항 수 입력 창 열기 오류:" << e.what();
        QMessageBox::warning(this, tr("오류"), tr("문항 수 입력 창을 열 수 없습니다: %1").arg(e.what()));
    }
}

void WriteResumeWindow::setNumIssues(int num) {
    if (num > 0 && num <= 10) {
        num_issues = num;
        setupUi();
        loadResume();
    }
}

void WriteResumeWindow::loadResume() {
    for (int i = 0; i < num_issues; i++) {
        loadIssueFromFile(i);
    }
}

void WriteResumeWindow::saveResume(bool silent) {
    for (int i = 0; i < num_issues; i++) {
        if (i < issue_inputs.size() && i < write_issue_inputs.size()) {
            saveIssueToFile(i,
                issue_inputs[i]->text(),
                write_issue_inputs[i]->toPlainText());
        }
    }

    if (!silent) {
        QMessageBox::information(this, tr("저장 완료"), tr("자기소개서가 저장되었습니다."));
    }
}

void WriteResumeWindow::createResumeDirectory() {
    QDir dir(company_directory);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            QMessageBox::warning(this, tr("디렉토리 오류"),
                tr("디렉토리를 생성할 수 없습니다: %1").arg(company_directory));
        }
    }
}

void WriteResumeWindow::saveIssueToFile(int issueNumber, const QString& title, const QString& content) {
    createResumeDirectory();

    QString filename = QString("%1/issue_%2.txt").arg(company_directory).arg(issueNumber + 1);

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "문항 제목:\n" << (title.isEmpty() ? QString("질문 %1").arg(issueNumber + 1) : title) << "\n\n";
        out << "답변:\n" << content;
        file.close();

        qDebug() << "파일 저장 성공:" << filename;
    } else {
        qDebug() << "파일 저장 실패:" << filename << "오류:" << file.errorString();
        QMessageBox::warning(this, tr("저장 오류"),
            tr("파일을 저장할 수 없습니다: %1\n오류: %2").arg(filename, file.errorString()));
    }
}

void WriteResumeWindow::loadIssueFromFile(int issueNumber) {
    QString filename = QString("%1/issue_%2.txt").arg(company_directory).arg(issueNumber + 1);

    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString fileContent = in.readAll();
        file.close();

        qDebug() << "파일 내용:" << fileContent;

        int titleStartIndex = fileContent.indexOf("문항 제목:") + 9;
        int titleEndIndex = fileContent.indexOf("\n\n", titleStartIndex);
        QString title = fileContent.mid(titleStartIndex, titleEndIndex - titleStartIndex).trimmed();

        int contentStartIndex = fileContent.indexOf("답변:") + 5;
        QString content = fileContent.mid(contentStartIndex).trimmed();

        if (issueNumber < issue_inputs.size()) {
            issue_inputs[issueNumber]->setText(title);
        }

        if (issueNumber < write_issue_inputs.size()) {
            write_issue_inputs[issueNumber]->setPlainText(content);

            if (issueNumber < character_counters.size()) {
                character_counters[issueNumber]->setText(
                    QString("%1자").arg(content.length()));
            }
        }
    } else {
        qDebug() << "파일 로드 실패:" << filename << "오류:" << file.errorString();

        if (issueNumber < issue_inputs.size()) {
            issue_inputs[issueNumber]->setText(tr("질문 %1").arg(issueNumber + 1));
        }

        if (issueNumber < write_issue_inputs.size()) {
            write_issue_inputs[issueNumber]->clear();

            if (issueNumber < character_counters.size()) {
                character_counters[issueNumber]->setText("0자");
            }
        }
    }
}
