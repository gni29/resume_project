//
//  first_window.cpp
//  qt_resume
//
//  Created by Pyoung Jin Ji on 3/26/25.
//

#include <stdio.h>
#include "first_window.h"
#include "company_add_window.h"
#include "company_window.h"
#include "database_utils.h"
#include "company_model.h"

#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QApplication>
#include <QMenu>
#include <QContextMenuEvent>
#include <QDir>

// 싱글톤 인스턴스
FirstWindow* FirstWindow::instance_ = nullptr;

// 전역 함수 - 싱글톤 인스턴스 접근
FirstWindow* get_main_window() {
    if (!FirstWindow::instance_) {
        try {
            FirstWindow::instance_ = new FirstWindow();
        } catch (const std::exception& e) {
            qCritical() << "메인 창 생성 오류:" << e.what();
            return nullptr;
        }
    }
    return FirstWindow::instance_;
}

FirstWindow::FirstWindow(QWidget* parent)
    : QMainWindow(parent),
      add_company_button(nullptr),
      close_button(nullptr),
      company_browser(nullptr),
      company_context_menu(nullptr) {
    
    // 타이틀 설정
    setWindowTitle(tr("공고 목록"));
    
    // UI 설정
    setupUi();
    setupContextMenu();
    
    // 데이터 초기화 타이머 설정 (약간 지연시켜 UI가 먼저 나타나도록)
    QTimer::singleShot(300, this, [this]() {
        // 데이터베이스 초기화
        if (!initializeDatabase()) {
            QMessageBox::warning(this, tr("초기화 실패"),
                tr("데이터베이스 초기화 실패: 프로그램이 제대로 작동하지 않을 수 있습니다."));
        }
        
        if (company_browser) {
            company_browser->addItem(tr("로딩 중..."));
        }
        
        // 약간 지연시켜 공고 목록 로드 (UI가 먼저 나타나도록)
        QTimer::singleShot(200, this, &FirstWindow::updateCompanyList);
    });
}

FirstWindow::~FirstWindow() {
    // 데이터베이스 연결 종료
    try {
        DatabaseUtils::closeDatabase();
    } catch (...) {
        // 오류 무시
    }
    
    // 싱글톤 인스턴스 해제
    if (instance_ == this) {
        instance_ = nullptr;
    }
}

void FirstWindow::setupContextMenu() {
    // 컨텍스트 메뉴 생성
    company_context_menu = new QMenu(this);
    QAction* deleteAction = company_context_menu->addAction(tr("공고 삭제"));
    
    // 시그널-슬롯 연결
    connect(deleteAction, &QAction::triggered, this, &FirstWindow::onDeleteCompanyAction);
    
    // 컨텍스트 메뉴 정책 설정
    company_browser->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(company_browser, &QListWidget::customContextMenuRequested,
            this, &FirstWindow::onCompanyItemRightClicked);
}

void FirstWindow::setupUi() {
    // 중앙 위젯 설정
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // 메인 레이아웃
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // 상단 레이아웃
    QHBoxLayout* topLayout = new QHBoxLayout();
    
    // 타이틀 라벨
    QLabel* titleLabel = new QLabel(tr("공고 목록"), this);
    topLayout->addWidget(titleLabel, 1);
    
    // 공고 추가 버튼
    add_company_button = new QPushButton(tr("공고 추가"), this);
    topLayout->addWidget(add_company_button);
    
    // 상단 레이아웃 추가
    mainLayout->addLayout(topLayout);
    
    // 회사 목록 브라우저
    company_browser = new QListWidget(this);
    mainLayout->addWidget(company_browser);
    
    // 하단 레이아웃
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch(1);
    
    // 닫기 버튼
    close_button = new QPushButton(tr("닫기"), this);
    bottomLayout->addWidget(close_button);
    
    // 하단 레이아웃 추가
    mainLayout->addLayout(bottomLayout);
    
    // 신호-슬롯 연결
    connect(add_company_button, &QPushButton::clicked, this, &FirstWindow::onAddCompanyClicked);
    connect(close_button, &QPushButton::clicked, this, &FirstWindow::onCloseClicked);
    connect(company_browser, &QListWidget::itemDoubleClicked, this, &FirstWindow::onCompanyItemDoubleClicked);
    
    // 크기 설정
    resize(400, 300);
}

void FirstWindow::onCompanyItemRightClicked(const QPoint& pos) {
    // 마우스 위치의 아이템 선택
    QListWidgetItem* item = company_browser->itemAt(pos);
    if (item) {
        company_browser->setCurrentItem(item);
        // 컨텍스트 메뉴 표시
        company_context_menu->exec(company_browser->mapToGlobal(pos));
    }
}

void FirstWindow::onDeleteCompanyAction() {
    // 현재 선택된 항목 확인
    QListWidgetItem* selectedItem = company_browser->currentItem();
    if (!selectedItem) return;
    
    int selected = company_browser->row(selectedItem);
    
    if (companies.isEmpty() || selected < 0 || selected >= companies.size()) {
        return;
    }
    
    // 삭제 확인 대화상자
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        tr("공고 삭제"),
        tr("정말로 이 공고를 삭제하시겠습니까?"),
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        const auto& company = companies[selected];
        
        // 데이터베이스에서 회사 삭제
        if (CompanyModel::deleteCompany(company.id)) {
            // 로컬 목록에서 제거
            companies.removeAt(selected);
            
            // 회사 디렉토리 삭제
            QString companyDir = QString("./job_postings/%1").arg(company.id);
            QDir dir(companyDir);
            if (dir.exists()) {
                dir.removeRecursively();
            }
            
            // UI 목록 업데이트
            updateCompanyList();
            
            QMessageBox::information(this, tr("삭제 완료"), tr("공고가 성공적으로 삭제되었습니다."));
        } else {
            QMessageBox::warning(this, tr("삭제 실패"), tr("공고를 삭제하는 중 오류가 발생했습니다."));
        }
    }
}

bool FirstWindow::initializeDatabase() {
    try {
        QString dbPath = "./job_applications.db";
        return DatabaseUtils::initializeDatabase(dbPath);
    } catch (const std::exception& e) {
        qCritical() << "데이터베이스 초기화 오류:" << e.what();
        return false;
    }
}

void FirstWindow::onAddCompanyClicked() {
    openCompanyAddWindow();
}

void FirstWindow::onCloseClicked() {
    close();
}

void FirstWindow::onCompanyItemDoubleClicked(QListWidgetItem* item) {
    int selected = company_browser->row(item);
    
    if (companies.isEmpty()) {
        return;
    }
    
    if (selected >= 0 && selected < companies.size()) {
        try {
            const auto& company = companies[selected];
            openCompanyWindow(company.id, company.name, company.job_description);
        } catch (const std::exception& e) {
            qCritical() << "공고 창 열기 오류:" << e.what();
        }
    }
}

void FirstWindow::updateCompanyList() {
    if (!company_browser) return;
    
    try {
        company_browser->clear();
        companies.clear();
        
        // 데이터베이스에서 회사 목록 가져오기
        companies = CompanyModel::getCompanies();
        
        // 회사가 없으면 메시지 표시
        if (companies.isEmpty()) {
            company_browser->addItem(tr("저장된 공고가 없습니다. '공고 추가' 버튼을 눌러 공고를 추가하세요."));
            return;
        }
        
        // 브라우저에 회사 정보 추가
        for (const auto& company : companies) {
            QString display_text = company.name + " - " + company.job_description;
            company_browser->addItem(display_text);
        }
        
        // 첫 번째 항목 선택
        if (company_browser->count() > 0) {
            company_browser->setCurrentRow(0);
        }
    } catch (const std::exception& e) {
        company_browser->clear();
        company_browser->addItem(tr("회사 목록을 불러오는 중 오류가 발생했습니다."));
        qCritical() << "공고 목록 업데이트 오류:" << e.what();
    }
}

void FirstWindow::openCompanyAddWindow() {
    try {
        // 회사 추가 창 생성
        CompanyAddWindow* add_win = company_add();
        if (!add_win) {
            throw std::runtime_error("공고 추가 창 생성 실패");
        }
        
        // 부모 창 참조 설정
        add_win->parent_window = this;
        
        add_win->exec(); // modal 대화상자로 실행
    } catch (const std::exception& e) {
        qCritical() << "공고 추가 창 열기 오류:" << e.what();
        QMessageBox::critical(this, tr("오류"), tr("공고 추가 창을 열 수 없습니다."));
    }
}

void FirstWindow::openCompanyWindow(int id, const QString& company_name, const QString& jd) {
    try {
        if (id <= 0 || company_name.isEmpty() || jd.isEmpty()) {
            return;
        }
        
        // 회사 창 생성
        CompanyWindow* company_win = create_company_window(id, company_name.toStdString(), jd.toStdString());
        if (company_win) {
            company_win->show();
        }
    } catch (const std::exception& e) {
        qCritical() << "공고 창 열기 오류:" << e.what();
        QMessageBox::critical(this, tr("오류"), tr("공고 창을 열 수 없습니다."));
    } catch (const std::exception& e) {
            qCritical() << "공고 창 열기 오류:" << e.what();
            QMessageBox::critical(this, tr("오류"), tr("공고 창을 열 수 없습니다."));
        }
    }
