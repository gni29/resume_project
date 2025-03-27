//
//  company_model.h
//  qt_resume
//
//  Created by Pyoung Jin Ji on 3/26/25.
//

#ifndef company_model_h
#define company_model_h


#include "database_utils.h"
#include <QString>
#include <QVector>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

// 데이터 구조체
struct Qualification {
    int id;
    int company_id;
    QString description;
};

struct ResumeIssue {
    int id;
    int company_id;
    int issue_number;
    QString title;
    QString content;
};

struct Company {
    int id;
    QString name;
    QString job_description;
    QDateTime created_at;
    QVector<Qualification> qualifications;
    QVector<ResumeIssue> resume_issues;
};

namespace CompanyModel {
    // SQL 인젝션 방지를 위한 문자열 이스케이프
    inline QString escapeString(const QString& str) {
    QString result = str;
    result.replace("'", "''");
    return result;
}
    // 전방 선언
    bool getQualifications(int companyId, QVector<Qualification>& qualifications);
    bool getResumeIssues(int companyId, QVector<ResumeIssue>& issues);
    
    // 회사 추가
    inline bool addCompany(const std::string& name, const std::string& jobDescription, int& newId) {
        try {
            QString qName = QString::fromStdString(name);
            QString qJobDesc = QString::fromStdString(jobDescription);
            qint64 now = QDateTime::currentSecsSinceEpoch();
            
            QSqlDatabase db = QSqlDatabase::database("default");
            QSqlQuery query(db);
            
            // 준비된 쿼리 사용 (SQL 인젝션 방지)
            query.prepare("INSERT OR IGNORE INTO companies (name, job_description, created_at) "
                          "VALUES (:name, :jobDesc, :created)");
            query.bindValue(":name", qName);
            query.bindValue(":jobDesc", qJobDesc);
            query.bindValue(":created", now);
            
            if (!query.exec()) {
                qCritical() << "회사 추가 SQL 오류:" << query.lastError().text();
                return false;
            }
            
            // 이미 존재하는 경우 해당 ID를 찾아야 함
            if (query.numRowsAffected() == 0) {
                query.prepare("SELECT id FROM companies WHERE name = :name AND job_description = :jobDesc");
                query.bindValue(":name", qName);
                query.bindValue(":jobDesc", qJobDesc);
                
                if (!query.exec() || !query.next()) {
                    qCritical() << "기존 회사 검색 오류:" << query.lastError().text();
                    return false;
                }
                
                newId = query.value(0).toInt();
                return true;
            }
            
            newId = DatabaseUtils::getLastInsertRowId();
            return true;
        } catch (const std::exception& e) {
            qCritical() << "회사 추가 중 오류:" << e.what();
            return false;
        }
    }
    
    // 회사 목록 가져오기
    inline QVector<Company> getCompanies() {
        QVector<Company> companies;
        
        QSqlDatabase db = QSqlDatabase::database("default");
        QSqlQuery query(db);
        
        if (!query.exec("SELECT id, name, job_description, created_at FROM companies ORDER BY name")) {
            qCritical() << "회사 목록 조회 오류:" << query.lastError().text();
            return companies;
        }
        
        while (query.next()) {
            Company company;
            company.id = query.value(0).toInt();
            company.name = query.value(1).toString();
            company.job_description = query.value(2).toString();
            company.created_at = QDateTime::fromSecsSinceEpoch(query.value(3).toLongLong());
            companies.append(company);
        }
        
        return companies;
    }
    
    // 회사 정보 가져오기
    inline bool getCompany(int id, Company& company) {
        QSqlDatabase db = QSqlDatabase::database("default");
        QSqlQuery query(db);
        
        query.prepare("SELECT id, name, job_description, created_at FROM companies WHERE id = :id");
        query.bindValue(":id", id);
        
        if (!query.exec() || !query.next()) {
            qCritical() << "회사 정보 조회 오류:" << query.lastError().text();
            return false;
        }
        
        company.id = query.value(0).toInt();
        company.name = query.value(1).toString();
        company.job_description = query.value(2).toString();
        company.created_at = QDateTime::fromSecsSinceEpoch(query.value(3).toLongLong());
        
        // 자격요건 가져오기
        getQualifications(id, company.qualifications);
        
        // 이력서 가져오기
        getResumeIssues(id, company.resume_issues);
        
        return true;
    }
    
    // 자격요건 추가
    inline bool addQualification(int companyId, const QString& description) {
        QSqlDatabase db = QSqlDatabase::database("default");
        QSqlQuery query(db);
        
        query.prepare("INSERT INTO qualifications (company_id, description) VALUES (:companyId, :description)");
        query.bindValue(":companyId", companyId);
        query.bindValue(":description", description);
        
        if (!query.exec()) {
            qCritical() << "자격요건 추가 오류:" << query.lastError().text();
            return false;
        }
        
        return true;
    }
    
    // 자격요건 목록 가져오기
    inline bool getQualifications(int companyId, QVector<Qualification>& qualifications) {
        qualifications.clear();
        
        QSqlDatabase db = QSqlDatabase::database("default");
        QSqlQuery query(db);
        
        query.prepare("SELECT id, company_id, description FROM qualifications WHERE company_id = :companyId");
        query.bindValue(":companyId", companyId);
        
        if (!query.exec()) {
            qCritical() << "자격요건 목록 조회 오류:" << query.lastError().text();
            return false;
        }
        
        while (query.next()) {
            Qualification qual;
            qual.id = query.value(0).toInt();
            qual.company_id = query.value(1).toInt();
            qual.description = query.value(2).toString();
            qualifications.append(qual);
        }
        
        return true;
    }
    
    // 이력서 이슈 추가/업데이트
    inline bool saveResumeIssue(int companyId, int issueNumber, const QString& title, const QString& content) {
        QSqlDatabase db = QSqlDatabase::database("default");
        QSqlQuery checkQuery(db);
        
        // 먼저 존재하는지 확인
        checkQuery.prepare("SELECT id FROM resumes WHERE company_id = :companyId AND issue_number = :issueNumber");
        checkQuery.bindValue(":companyId", companyId);
        checkQuery.bindValue(":issueNumber", issueNumber);
        
        if (!checkQuery.exec()) {
            qCritical() << "이력서 이슈 확인 오류:" << checkQuery.lastError().text();
            return false;
        }
        
        QSqlQuery query(db);
        
        if (checkQuery.next()) {
            // 기존 이슈 업데이트
            int existingId = checkQuery.value(0).toInt();
            query.prepare("UPDATE resumes SET issue_title = :title, issue_content = :content WHERE id = :id");
            query.bindValue(":title", title);
            query.bindValue(":content", content);
            query.bindValue(":id", existingId);
        } else {
            // 새 이슈 추가
            query.prepare("INSERT INTO resumes (company_id, issue_number, issue_title, issue_content) "
                         "VALUES (:companyId, :issueNumber, :title, :content)");
            query.bindValue(":companyId", companyId);
            query.bindValue(":issueNumber", issueNumber);
            query.bindValue(":title", title);
            query.bindValue(":content", content);
        }
        
        if (!query.exec()) {
            qCritical() << "이력서 저장 오류:" << query.lastError().text();
            return false;
        }
        
        return true;
    }
    
    // 이력서 이슈 목록 가져오기
    inline bool getResumeIssues(int companyId, QVector<ResumeIssue>& issues) {
        issues.clear();
        
        QSqlDatabase db = QSqlDatabase::database("default");
        QSqlQuery query(db);
        
        query.prepare("SELECT id, company_id, issue_number, issue_title, issue_content FROM resumes "
                     "WHERE company_id = :companyId ORDER BY issue_number");
        query.bindValue(":companyId", companyId);
        
        if (!query.exec()) {
            qCritical() << "이력서 이슈 목록 조회 오류:" << query.lastError().text();
            return false;
        }
        
        while (query.next()) {
            ResumeIssue issue;
            issue.id = query.value(0).toInt();
            issue.company_id = query.value(1).toInt();
            issue.issue_number = query.value(2).toInt();
            issue.title = query.value(3).toString();
            issue.content = query.value(4).toString();
            issues.append(issue);
        }
        
        return true;
    }
    
    // 회사 삭제
    inline bool deleteCompany(int id) {
        QSqlDatabase db = QSqlDatabase::database("default");
        QSqlQuery query(db);
        
        query.prepare("DELETE FROM companies WHERE id = :id");
        query.bindValue(":id", id);
        
        if (!query.exec()) {
            qCritical() << "회사 삭제 오류:" << query.lastError().text();
            return false;
        }
        
        return true;
    }
    
    // 자격요건 삭제
    inline bool deleteQualification(int id) {
        QSqlDatabase db = QSqlDatabase::database("default");
        QSqlQuery query(db);
        
        query.prepare("DELETE FROM qualifications WHERE id = :id");
        query.bindValue(":id", id);
        
        if (!query.exec()) {
            qCritical() << "자격요건 삭제 오류:" << query.lastError().text();
            return false;
        }
        
        return true;
    }
    
    // 이력서 이슈 삭제
    inline bool deleteResumeIssue(int id) {
        QSqlDatabase db = QSqlDatabase::database("default");
        QSqlQuery query(db);
        
        query.prepare("DELETE FROM resumes WHERE id = :id");
        query.bindValue(":id", id);
        
        if (!query.exec()) {
            qCritical() << "이력서 이슈 삭제 오류:" << query.lastError().text();
            return false;
        }
        
        return true;
    }
    
} // namespace CompanyModel

#endif /* company_model_h */
