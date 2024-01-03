#/bin/sh

if [ "$CI_SERVER_NAME" = "GitLab" ]; then
	echo "Test $0 skipped; current librtpi GitLab CI pipeline doesn't have sudo support"
	exit 77
fi

sudo ./tst-condpi2-cpp
