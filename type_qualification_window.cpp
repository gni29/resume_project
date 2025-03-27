//
//  type_qualification_window.cpp
//  qt_resume
//
//  Created by Pyoung Jin Ji on 3/26/25.
//

#include <stdio.h>
#include "type_qualification_window.h"
#include "company_window.h"

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDebug>

TypeQualificationWindow::TypeQualificationWindow(QWidget* parent, CompanyWindow* companyParent)
    : QDialog(parent),
      parent_window(companyParent),
      qualification_input(nullptr),
      check(nullptr),
      cancel(nullptr) {
    
    // 타이틀 설정
    setWindowTitle(tr("지원자격"));
    
    // UI 설정
    setupUi();
}

TypeQualificationWindow::~TypeQualificationWindow() {
    // Qt에서는 부모-자식 관계로 자동 메모리 관리
}

void TypeQualificationWindow::setupUi() {
    // 배경색 설정
    setStyleSheet("background-color: lightgray;");
    
    // 메인 레이아웃
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // 폼 레이아웃
    QFormLayout* formLayout = new QFormLayout();
    
    // 지원자격 라벨 및 입력 필드
    QLabel* label = new QLabel(tr("지원자격"));
    qualification_input = new QLineEdit();
    
    formLayout->addRow(label, qualification_input);
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
    connect(check, &QPushButton::clicked, this, &TypeQualificationWindow::onCheckClicked);
    connect(cancel, &QPushButton::clicked, this, &TypeQualificationWindow::onCancelClicked);
    
    // 창 크기 설정
    setMinimumSize(400, 150);
    resize(400, 150);
}

void TypeQualificationWindow::onCheckClicked() {
    addQualification();
}

void TypeQualificationWindow::onCancelClicked() {
    reject();
}

void TypeQualificationWindow::addQualification() {
    try {
        if (!qualification_input) {
            qCritical() << "Error: qualification_input is null";
            return;
        }
        
        QString qual = qualification_input->text();
        
        if (!qual.isEmpty() && parent_window) {
            // Add qualification to parent window
            parent_window->addQualification(qual);
            
            // Close this window
            accept();
        }
    } catch (const std::exception& e) {
        qCritical() << "Error adding qualification:" << e.what();
    }
}
