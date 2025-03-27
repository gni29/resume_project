#!/bin/bash

# 애플리케이션 이름
APP_NAME="JobApplicationManager"

# 빌드된 앱 경로
APP_PATH="./Release/JobApplicationManager.app"

# 임시 디렉토리 생성
mkdir -p tmp_dmg

# 애플리케이션 복사
cp -R "${APP_PATH}" tmp_dmg/

# .dmg 파일 생성
hdiutil create -fs HFS+ -volname "${APP_NAME}" -srcfolder tmp_dmg "${APP_NAME}.dmg"

# 임시 디렉토리 정리
rm -rf tmp_dmg
