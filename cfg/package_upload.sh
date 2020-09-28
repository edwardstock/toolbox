#!/usr/bin/env bash

if [ "$1" == "github" ]; then
  # github uploaded

  if [ ! -f "/tmp/ghr.tar.gz" ]; then
    wget -O /tmp/ghr.tar.gz https://github.com/tcnksm/ghr/releases/download/v0.12.2/ghr_v0.12.2_linux_amd64.tar.gz
  fi

  if [ ! -f "/usr/bin/ghr" ] && [ ! -f "/usr/local/bin/ghr" ]; then
    tar -xvf /tmp/ghr.tar.gz -C /tmp
    cp /tmp/ghr_*/ghr /usr/bin
  fi

  echo "Upload release @PROJECT_VERSION@: @UPLOAD_FILE_NAME@"
  ghr @PROJECT_VERSION@ @UPLOAD_FILE_NAME@

  exit 0
fi

UNAME=$BINTRAY_USER
PASS=$BINTRAY_API_KEY
BASE_URL=https://api.bintray.com/content/edwardstock

if [ "$1" == "dry" ]; then
  echo "curl -T @UPLOAD_FILE_NAME@ -u${UNAME}:${PASS} \"${BASE_URL}/@REPO_NAME@/@PROJECT_NAME@/@PROJECT_VERSION@/@TARGET_PATH@/@UPLOAD_FILE_NAME@@URL_SUFFIX@\""
  exit 0
fi

curl -T @UPLOAD_FILE_NAME@ -u${UNAME}:${PASS} "${BASE_URL}/@REPO_NAME@/@PROJECT_NAME@/@PROJECT_VERSION@/@TARGET_PATH@/@UPLOAD_FILE_NAME@@URL_SUFFIX@"
