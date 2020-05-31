#!/bin/bash
export LANG="en_US.UTF8"
rm -rf build/
mkdir build
time lualatex --draftmode -interaction=nonstopmode -file-line-error -shell-escape -recorder -output-directory="build" -jobname="report" "report.tex" | awk 'BEGIN{IGNORECASE = 1}/warning|!/,/^$/;'
time biber --isbn-normalise --validate_datamodel build/report
cd build
time makeglossaries report
cd ..
time lualatex --draftmode -interaction=nonstopmode -file-line-error -shell-escape -recorder -output-directory="build" -jobname="report" "report.tex" | awk 'BEGIN{IGNORECASE = 1}/warning|!/,/^$/;'
time lualatex -interaction=nonstopmode -file-line-error -shell-escape -recorder -output-directory="build" -jobname="report" "report.tex" | awk 'BEGIN{IGNORECASE = 1}/warning|!/,/^$/;'
mv build/report.pdf .
cp build/report.glsdefs .
okular report.pdf &
