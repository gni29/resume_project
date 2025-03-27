//
//  company_add_window.cpp
//  qt_resume
//
//  Created by Pyoung Jin Ji on 3/26/25.
//

#include <stdio.h>
#include "company_add_window.h"
#include "first_window.h"
#include "company_window.h"
#include "company_model.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QString>
#include <QDebug>

// 전역 변수로 인스턴스 관리
static CompanyAddWindow* g_company_add_window = nullptr;

// 전역 함수 - CompanyAddWindow 인스턴스 생성/접근
CompanyAddWindow* company_add() {
    if (!g_company_add_window) {
        g_company_add_window = new CompanyAddWindow();
    }
    return g_company_add_window;
}

CompanyAddWindow::CompanyAddWindow(QWidget* parent)
    : QDialog(parent),
      print_company(nullptr),
      JD(nullptr),
      check_button(nullptr),
      cancel(nullptr),
      parent_window(nullptr) {
    
    // 타이틀 설정
    setWindowTitle(tr("공고 추가"));
    
    // UI 설정
    setupUi();
}

CompanyAddWindow::~CompanyAddWindow() {
    // Qt의 부모-자식 관계로 위젯 자동 정리
    
    // g_company_add_window 변수 정리
    if (g_company_add_window == this) {
        g_company_add_window = nullptr;
    }
}

void CompanyAddWindow::setupUi() {
    try {
        // 스타일시트 개선 - 높은 대비와 가독성
        setStyleSheet(
            "QDialog {"
            "    background-color: #f0f0f0;"
            "    color: black;"
            "}"
            "QLabel {"
            "    color: #333333;"
            "    font-weight: bold;"
            "}"
            "QLineEdit {"
            "    background-color: white;"
            "    color: black;"
            "    border: 1px solid #a0a0a0;"
            "    padding: 5px;"
            "    border-radius: 3px;"
            "}"
            "QPushButton {"
            "    background-color: #4a90e2;"
            "    color: white;"
            "    border: none;"
            "    padding: 6px 12px;"
            "    border-radius: 3px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #357abd;"
            "}"
        );
        
        // 레이아웃 설정 - FLTK의 명시적 위치 지정 대신 Qt 레이아웃 사용
        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        QFormLayout* formLayout = new QFormLayout();
        
        // 회사명 입력
        QLabel* companyLabel = new QLabel(tr("회사"));
        print_company = new QLineEdit();
        print_company->setPlaceholderText(tr("회사명을 입력하세요"));
        formLayout->addRow(companyLabel, print_company);
        
        // 직무 입력
        QLabel* jdLabel = new QLabel(tr("직무"));
        JD = new QLineEdit();
        JD->setPlaceholderText(tr("직무를 입력하세요"));
        formLayout->addRow(jdLabel, JD);
        
        // 폼 레이아웃 추가
        mainLayout->addLayout(formLayout);
        
        // 버튼 레이아웃
        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch();
        
        // 확인 버튼
        check_button = new QPushButton(tr("확인"));
        buttonLayout->addWidget(check_button);
        
        // 취소 버튼
        cancel = new QPushButton(tr("취소"));
        buttonLayout->addWidget(cancel);
        
        // 버튼 레이아웃 추가
        mainLayout->addLayout(buttonLayout);
        
        // 시그널-슬롯 연결 (FLTK 콜백 대체)
        connect(check_button, &QPushButton::clicked, this, &CompanyAddWindow::onCheckClicked);
        connect(cancel, &QPushButton::clicked, this, &CompanyAddWindow::onCancelClicked);
        
        // 폼에 여백 추가
        setLayout(mainLayout);
        
        // 창 크기 설정
        resize(400, 200);
        
    } catch (const std::exception& e) {
        qCritical() << "CompanyAddWindow 생성 오류:" << e.what();
        throw;
    }
}

void CompanyAddWindow::onCheckClicked() {
    saveCompany();
}

void CompanyAddWindow::onCancelClicked() {
    close();
}

void CompanyAddWindow::saveCompany() {
    try {
        QString company_name = print_company->text();
        QString job_desc = JD->text();
        
        if (company_name.isEmpty() || job_desc.isEmpty()) {
            qWarning() << "회사명 또는 직무가 입력되지 않았습니다.";
            QMessageBox::warning(this, tr("입력 오류"), tr("회사명과 직무를 모두 입력해주세요."));
            return;
        }
        
        // 데이터베이스에 회사 정보 저장
        int company_id = -1;
        if (!CompanyModel::addCompany(company_name.toStdString(), job_desc.toStdString(), company_id)) {
            qCritical() << "회사 추가 실패";
            QMessageBox::critical(this, tr("저장 오류"), tr("회사 정보를 저장하는데 실패했습니다."));
            return;
        }
        
        // 성공적으로 추가된 경우
        qDebug() << "회사 추가 성공: ID=" << company_id << ", 이름=" << company_name << ", 직무=" << job_desc;
        
        // 창 닫기
        accept();
        
        // 부모 창의 공고 목록 업데이트
        if (parent_window) {
            parent_window->updateCompanyList();
        }
        
        // 회사 창 열기
        CompanyWindow* comp_win = create_company_window(company_id, company_name.toStdString(), job_desc.toStdString());
        if (comp_win) {
            comp_win->show();
        } else {
            throw std::runtime_error("공고 창 생성 실패");
        }
        
    } catch (const std::exception& e) {
        qCritical() << "Error creating company:" << e.what();
        QMessageBox::critical(this, tr("오류"), tr("회사 정보 저장 중 오류: %1").arg(e.what()));
    }
}
