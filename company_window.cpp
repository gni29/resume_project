//
//  company_window.cpp
//  qt_resume
//
//  Created by Pyoung Jin Ji on 3/26/25.
//

#include <stdio.h>
#include "company_window.h"
#include "write_resume_window.h"
#include "type_qualification_window.h"
#include "num_issue_window.h"
#include "app_utils.h"
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QMenu>
#include <QAction>

CompanyWindow::CompanyWindow(QWidget* parent, int id, const QString& company, const QString& jd)
    : QDialog(parent),
      company_id(id),
      company_name(company),
      job_description(jd),
      print_company_name(nullptr),
      print_jd(nullptr),
      check_button(nullptr),
      adapt_button(nullptr),
      cancel_button(nullptr),
      write_resume_button(nullptr),
      open_add_qual_button(nullptr),
      open_jd_button(nullptr),
      qualifications_browser(nullptr),
      qualifications_context_menu(nullptr),
      write_resume_win(nullptr),
      type_qualification_win(nullptr),
      has_resume(false) {
    
    // 타이틀 설정
    setWindowTitle(tr("공고 관리"));
    
    // UI 설정
    setupUi();
    setupQualificationsContextMenu();
    
    // 데이터 로드
    loadCompanyInfo();
    
    // 이력서 존재 여부 확인
    has_resume = checkForResume();
}

CompanyWindow::~CompanyWindow() {
    // 하위 창들 정리
    if (write_resume_win) {
        write_resume_win->close();
        delete write_resume_win;
        write_resume_win = nullptr;
    }
    
    if (type_qualification_win) {
        type_qualification_win->close();
        delete type_qualification_win;
        type_qualification_win = nullptr;
    }
    
    // 컨텍스트 메뉴 정리
    if (qualifications_context_menu) {
        delete qualifications_context_menu;
        qualifications_context_menu = nullptr;
    }
    
    // 나머지 위젯들은 Qt의 부모-자식 관계로 자동 정리됨
}

void CompanyWindow::setupQualificationsContextMenu() {
    // 컨텍스트 메뉴 생성
    qualifications_context_menu = new QMenu(this);
    QAction* deleteAction = qualifications_context_menu->addAction(tr("지원자격 삭제"));
    
    // 시그널-슬롯 연결
    connect(deleteAction, &QAction::triggered, this, &CompanyWindow::onDeleteQualificationClicked);
    
    // 컨텍스트 메뉴 정책 설정
    qualifications_browser->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(qualifications_browser, &QListWidget::customContextMenuRequested, [this](const QPoint& pos) {
        QListWidgetItem* item = qualifications_browser->itemAt(pos);
        if (item) {
            qualifications_browser->setCurrentItem(item);
            qualifications_context_menu->exec(qualifications_browser->mapToGlobal(pos));
        }
    });
}

void CompanyWindow::onDeleteQualificationClicked() {
    QListWidgetItem* selectedItem = qualifications_browser->currentItem();
    if (!selectedItem) return;
    
    int selectedIndex = qualifications_browser->row(selectedItem);
    
    // 확인 대화상자
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        tr("지원자격 삭제"),
        tr("이 지원자격을 정말 삭제하시겠습니까?"),
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        // 데이터베이스에서 삭제
        if (selectedIndex >= 0 && selectedIndex < qualifications.size()) {
            int qualId = qualifications[selectedIndex].id;
            if (CompanyModel::deleteQualification(qualId)) {
                // UI에서 삭제
                delete qualifications_browser->takeItem(selectedIndex);
                
                // 내부 데이터 구조에서도 삭제
                qualifications.removeAt(selectedIndex);
                
                QMessageBox::information(this, tr("삭제 완료"), tr("지원자격이 삭제되었습니다."));
            } else {
                QMessageBox::warning(this, tr("삭제 실패"), tr("지원자격 삭제 중 오류가 발생했습니다."));
            }
        }
    }
}

void CompanyWindow::setupUi() {
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
        "QLineEdit {"
        "    background-color: #3c3f41;"
        "    color: white;"
        "    border: 1px solid #5a5a5a;"
        "    padding: 5px;"
        "    border-radius: 3px;"
        "}"
        "QLineEdit::placeholder {"
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
        "QListWidget {"
        "    background-color: #3c3f41;"
        "    color: white;"
        "    border: 1px solid #5a5a5a;"
        "    border-radius: 3px;"
        "    min-height: 100px;"
        "    max-height: 200px;"
        "}"
        "QListWidget::item {"
        "    background-color: #3c3f41;"
        "    color: white;"
        "    padding: 5px;"
        "    border-bottom: 1px solid #5a5a5a;"
        "}"
        "QListWidget::item:selected {"
        "    background-color: #4a6984;"
        "    color: white;"
        "}"
        "QGroupBox {"
        "    color: white;"
        "    border: 1px solid #5a5a5a;"
        "    border-radius: 5px;"
        "    margin-top: 10px;"
        "}"
        "QGroupBox::title {"
        "    color: white;"
        "    subcontrol-origin: margin;"
        "    left: 10px;"
        "    padding: 0 3px;"
        "}"
    );
    
    // 메인 레이아웃
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // 회사 정보 영역
    QFormLayout* formLayout = new QFormLayout();
    
    // 회사명
    QLabel* companyLabel = new QLabel(tr("회사"));
    print_company_name = new QLineEdit();
    print_company_name->setReadOnly(true);
    print_company_name->setText(company_name);
    formLayout->addRow(companyLabel, print_company_name);
    
    // 직무 (가로 레이아웃 사용)
    QLabel* jdLabel = new QLabel(tr("직무"));
    QHBoxLayout* jdLayout = new QHBoxLayout();
    
    print_jd = new QLineEdit();
    print_jd->setReadOnly(true);
    print_jd->setText(job_description);
    jdLayout->addWidget(print_jd);
    
    formLayout->addRow(jdLabel, jdLayout);
    mainLayout->addLayout(formLayout);
    
    // 지원자격 영역
    QGroupBox* qualGroup = new QGroupBox(tr("지원자격"));
    QVBoxLayout* qualLayout = new QVBoxLayout(qualGroup);
    
    // 자격요건 목록
    qualifications_browser = new QListWidget();
    qualLayout->addWidget(qualifications_browser);
    
    // 자격요건 추가 버튼
    open_add_qual_button = new QPushButton(tr("지원자격 추가"));
    connect(open_add_qual_button, &QPushButton::clicked, this, &CompanyWindow::onAddQualificationClicked);
    qualLayout->addWidget(open_add_qual_button, 0, Qt::AlignLeft);
    
    mainLayout->addWidget(qualGroup);
    
    // 자기소개서 버튼
    write_resume_button = new QPushButton(tr("자기소개서 작성"));
    connect(write_resume_button, &QPushButton::clicked, this, &CompanyWindow::onWriteResumeClicked);
    mainLayout->addWidget(write_resume_button, 0, Qt::AlignCenter);
    
    // 하단 버튼 영역
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    // 적용 버튼
    check_button = new QPushButton(tr("확인"));
    connect(check_button, &QPushButton::clicked, this, &CompanyWindow::onCheckClicked);
    buttonLayout->addWidget(check_button);
    
    // 확인 버튼
    adapt_button = new QPushButton(tr("적용"));
    connect(adapt_button, &QPushButton::clicked, this, &CompanyWindow::onAdaptClicked);
    buttonLayout->addWidget(adapt_button);
    
    // 취소 버튼
    cancel_button = new QPushButton(tr("취소"));
    connect(cancel_button, &QPushButton::clicked, this, &CompanyWindow::onCancelClicked);
    buttonLayout->addWidget(cancel_button);
    
    // 버튼 레이아웃 추가
    mainLayout->addLayout(buttonLayout);
    
    // 창 크기 설정
    resize(600, 400);
}

void CompanyWindow::onCheckClicked() {
    saveCompanyInfo();
    accept(); // 창 닫기
}

void CompanyWindow::onAdaptClicked() {
    saveCompanyInfo();
    accept(); // 창 닫기
}

void CompanyWindow::onCancelClicked() {
    reject(); // 변경사항 무시하고 창 닫기
}

void CompanyWindow::onWriteResumeClicked() {
    openResumeWindow();
}

void CompanyWindow::onAddQualificationClicked() {
    openQualificationWindow();
}

void CompanyWindow::onOpenJdClicked() {
    QMessageBox::information(this, tr("직무 설명"), job_description);
}

void CompanyWindow::loadCompanyInfo() {
    try {
        // 데이터베이스에서 회사 정보 가져오기
        Company company;
        if (CompanyModel::getCompany(company_id, company)) {
            // 필요한 경우 회사명과 직무 업데이트
            company_name = company.name;
            job_description = company.job_description;
            
            if (print_company_name) print_company_name->setText(company_name);
            if (print_jd) print_jd->setText(job_description);
            
            // 자격요건 저장
            qualifications = company.qualifications;
            
            // 이력서 이슈 저장
            resume_issues = company.resume_issues;
            
            // 브라우저에 자격요건 표시
            updateQualifications();
        } else {
            qCritical() << "회사 정보를 찾을 수 없음: ID=" << company_id;
            QMessageBox::warning(this, tr("데이터 오류"), tr("회사 정보를 데이터베이스에서 찾을 수 없습니다."));
        }
    } catch (const std::exception& e) {
        qCritical() << "회사 정보 로딩 중 오류:" << e.what();
        QMessageBox::warning(this, tr("로드 오류"), tr("회사 정보를 로드하는 중 오류가 발생했습니다: %1").arg(e.what()));
    }
}

void CompanyWindow::saveCompanyInfo() {
    try {
        // 현재 입력된 회사명과 직무 정보 저장
        QString current_name = print_company_name->text();
        QString current_jd = print_jd->text();
        
        // 데이터베이스 업데이트 (필요한 경우)
        if (current_name != company_name || current_jd != job_description) {
            // 업데이트 로직 (필요하면 구현)
            // CompanyModel::updateCompany(company_id, current_name, current_jd);
            
            company_name = current_name;
            job_description = current_jd;
        }
        
        QMessageBox::information(this, tr("저장 완료"), tr("회사 정보가 저장되었습니다."));
    } catch (const std::exception& e) {
        qCritical() << "회사 정보 저장 중 오류:" << e.what();
        QMessageBox::warning(this, tr("저장 오류"), tr("회사 정보를 저장하는 중 오류가 발생했습니다: %1").arg(e.what()));
    }
}

void CompanyWindow::updateQualifications() {
    if (!qualifications_browser) return;
    
    qualifications_browser->clear();
    for (const auto& qual : qualifications) {
        qualifications_browser->addItem(qual.description);
    }
}

void CompanyWindow::addQualification(const QString& qualification) {
    if (!qualification.isEmpty()) {
        try {
            // 데이터베이스에 자격요건 추가
            if (CompanyModel::addQualification(company_id, qualification)) {
                // 자격요건 목록 업데이트
                loadCompanyInfo(); // 전체 회사 정보 다시 로드
            } else {
                qCritical() << "자격요건 추가 실패";
                QMessageBox::warning(this, tr("추가 실패"), tr("자격요건을 추가하는데 실패했습니다."));
            }
        } catch (const std::exception& e) {
            qCritical() << "자격요건 추가 중 오류:" << e.what();
            QMessageBox::warning(this, tr("오류"), tr("자격요건 추가 중 오류가 발생했습니다: %1").arg(e.what()));
        }
    }
}
    
// CompanyWindow.cpp의 openResumeWindow 함수 수정 (app_utils.h 사용)

// 먼저 #include "app_utils.h"를 헤더 포함 목록에 추가하세요

void CompanyWindow::openResumeWindow() {
    try {
        // 회사별 디렉토리 경로 가져오기
        QString resumeDir = getCompanyDir(company_id);
        if (resumeDir.isEmpty()) {
            QMessageBox::warning(this, tr("디렉토리 오류"),
                tr("회사 디렉토리를 생성할 수 없습니다. 권한을 확인하세요."));
            return;
        }
        
        // 이미 이력서 파일이 존재하는지 확인
        if (NumIssueWindow::isIssuesSet(resumeDir)) {
            // 이미 이력서 파일이 존재하면 바로 이력서 창 열기
            WriteResumeWindow* write_resume_win = new WriteResumeWindow(this, resumeDir);
            if (!write_resume_win) {
                throw std::runtime_error("자기소개서 창 생성 실패");
            }
            write_resume_win->show();
        } else {
            // 이력서 파일이 없으면 문항 수 입력 창 열기
            NumIssueWindow* num_issue_win = new NumIssueWindow(this, resumeDir, nullptr);
            if (!num_issue_win) {
                throw std::runtime_error("문항 수 입력 창 생성 실패");
            }
            
            num_issue_win->exec();
        }
    } catch (const std::exception& e) {
        qCritical() << "이력서 창 열기 중 오류:" << e.what();
        QMessageBox::warning(this, tr("오류"), tr("이력서 창을 열 수 없습니다: %1").arg(e.what()));
    }
}
    void CompanyWindow::openQualificationWindow() {
        try {
            // 이미 창이 열려 있으면 먼저 닫기
            if (type_qualification_win) {
                type_qualification_win->close();
                delete type_qualification_win;
                type_qualification_win = nullptr;
            }
            
            type_qualification_win = new TypeQualificationWindow(this, this);
            if (!type_qualification_win) {
                throw std::runtime_error("지원자격 창 생성 실패");
            }
            
            type_qualification_win->exec();
        } catch (const std::exception& e) {
            qCritical() << "자격 요건 창 열기 중 오류:" << e.what();
            QMessageBox::warning(this, tr("오류"), tr("자격 요건 창을 열 수 없습니다: %1").arg(e.what()));
        }
    }
    
    bool CompanyWindow::checkForResume() {
        try {
            // 데이터베이스에서 이력서 존재 여부 확인
            return !resume_issues.isEmpty();
        } catch (const std::exception& e) {
            qCritical() << "이력서 확인 중 오류:" << e.what();
            return false;
        }
    }
    
    // 전역 함수 정의 - CompanyWindow 생성
    CompanyWindow* create_company_window(int id, const std::string& company, const std::string& jd) {
        try {
            QString qCompany = QString::fromStdString(company);
            QString qJd = QString::fromStdString(jd);
            CompanyWindow* win = new CompanyWindow(nullptr, id, qCompany, qJd);
            if (!win) {
                qCritical() << "공고 창 생성 실패";
                return nullptr;
            }
            return win;
        } catch (const std::exception& e) {
            qCritical() << "공고 창 생성 중 오류:" << e.what();
            return nullptr;
        }
    }
