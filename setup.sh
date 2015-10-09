#!/usr/bin/sh

SCRIPT_PATH=$(cd $(dirname "${BASH_SOURCE[0]}"); pwd)

SQLITE_ZIP_FILE="external/sqlite.zip"
SQLITE_SRC_DL="https://www.sqlite.org/snapshot/sqlite-amalgamation-201510071324.zip"

CATCH_FILE="external/catch/catch.hpp"
CATCH_SRC_DL="https://raw.githubusercontent.com/philsquared/Catch/master/single_include/catch.hpp"

function cleanup()
{
	if [[ -f external/sqlite.zip ]]; then
		rm "$SQLITE_ZIP_FILE"
	fi
}

function install_sqlite_src()
{
	if [[ -d external/sqlite ]]; then
		return
	fi
	
	echo "Downloading SQLite source"
	
	curl "$SQLITE_SRC_DL" -o "$SQLITE_ZIP_FILE"
	unzip "$SQLITE_ZIP_FILE" -d $(dirname "$SQLITE_ZIP_FILE")/sqlite
}

function install_catch()
{
	if [[ -f "$CATCH_FILE" ]]; then
		return
	fi
	
	echo "Downloading Catch source"
	
	local catch_dir=$(dirname "$CATCH_FILE")
	mkdir -p "$catch_dir"
	curl "$CATCH_SRC_DL" -o "$CATCH_FILE"
}

function main()
{
	mkdir -p external
	
	install_sqlite_src
	install_catch
}

trap cleanup EXIT
pushd "$SCRIPT_PATH" > /dev/null
main "$@"
popd > /dev/null