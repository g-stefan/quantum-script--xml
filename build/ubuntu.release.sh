#!/bin/sh
# Public domain
# http://unlicense.org/
# Created by Grigore Stefan <g_stefan@yahoo.com>

echo "-> release quantum-script-extension-xml"

cmdX(){
	if ! "$@" ; then
		echo "Error: release"
		exit 1
	fi
}

export XYO_PATH_RELEASE=release

if ! xyo-cc quantum-script-extension-xml --has-archived-release --version-file=version.ini; then
	exit 0
fi
cmdX /bin/sh ./build/ubuntu.sh make
cmdX /bin/sh ./build/ubuntu.sh install
cmdX /bin/sh ./build/ubuntu.sh install-release
cmdX xyo-cc quantum-script-extension-xml --archive-release-sha512 --version-file=version.ini
