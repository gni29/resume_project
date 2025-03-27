//
//  num_issue_window.cpp
//  qt_resume
//
//  Created by Pyoung Jin Ji on 3/26/25.
//

#include <stdio.h>
#include "num_issue_window.h"
#include "write_resume_window.h"

#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDebug>
#include <QDir>
#include <QFile>

NumIssueWindow::NumIssueWindow(QWidget* parent, const QString& company_dir, WriteResumeWindow* parent_win)
    : QDialog(parent),
      company_directory(company_dir),
      parent_window(parent_win),
      num_issue_input(nullptr),
      check(nullptr),
      cancel(nullptr) {
    
    // 타이틀 설정
    setWindowTitle(tr("문항 수 입력"));
    
    // UI 설정
    setupUi();
}

NumIssueWindow::~NumIssueWindow() {
    // Qt에서는 부모-자식 관계로 자동 메모리 관리
}

bool NumIssueWindow::isIssuesSet(const QString& companyDir) {
    // 디렉토리 확인
    QDir dir(companyDir);
    QStringList txtFiles = dir.entryList(QStringList() << "issue_*.txt", QDir::Files);
    
    // 파일이 존재하면 이미 문항 수가 설정된 것으로 간주
    return !txtFiles.isEmpty();
}

void NumIssueWindow::setupUi() {
    // 다크 모드 스타일시트
    setStyleSheet(
        "QDialog {"
        "    background-color: #2b2b2b;"
        "    color: white;"
        "}"
        "QLabel {"
        "    color: white;"
        "    font-weight: bold;"
        "}"
        "QComboBox {"
        "    background-color: #3c3f41;"
        "    color: white;"
        "    border: 1px solid #5a5a5a;"
        "    padding: 5px;"
        "    border-radius: 3px;"
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
    
    // 메인 레이아웃
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // 폼 레이아웃
    QFormLayout* formLayout = new QFormLayout();
    
    // 문항 수 라벨 및 선택 콤보박스
    QLabel* label = new QLabel(tr("문항 수"));
    num_issue_input = new QComboBox();
    
    // 문항 수 선택 항목 추가 (1-10)
    for (int i = 1; i <= 10; i++) {
        num_issue_input->addItem(QString::number(i));
    }
    
    formLayout->addRow(label, num_issue_input);
    mainLayout->addLayout(formLayout);
    
    // 버튼 레이아웃
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    // 확인 버튼
    check = new QPushButton(tr("확인"));
    buttonLayout->addWidget(check);
    
    // 취소 버튼
    cancel = new QPushButton(tr("취소"));
    buttonLayout->addWidget(cancel);
    
    // 버튼 레이아웃 추가
    mainLayout->addLayout(buttonLayout);
    
    // 시그널-슬롯 연결
    connect(check, &QPushButton::clicked, this, &NumIssueWindow::onCheckClicked);
    connect(cancel, &QPushButton::clicked, this, &NumIssueWindow::onCancelClicked);
    
    // 창 크기 설정
    setMinimumSize(300, 150);
    resize(300, 150);
}

void NumIssueWindow::onCheckClicked() {
    setNumIssues();
}

void NumIssueWindow::onCancelClicked() {
    reject();
}

void NumIssueWindow::setNumIssues() {
    try {
        if (!num_issue_input) {
            qCritical() << "Error: num_issue_input is null";
            return;
        }
        
        int num = num_issue_input->currentIndex() + 1; // +1 because indexes start at 0
        
        // 새 WriteResumeWindow 창 생성
        WriteResumeWindow* write_resume_win = new WriteResumeWindow(nullptr, company_directory);
        if (!write_resume_win) {
            throw std::runtime_error("자기소개서 창 생성 실패");
        }
        
        write_resume_win->setNumIssues(num);
        write_resume_win->show();
        
        // 이 창 닫기
        accept();
    } catch (const std::exception& e) {
        qCritical() << "Error setting number of issues:" << e.what();
    }
}
