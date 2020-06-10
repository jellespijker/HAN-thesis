#!/bin/bash
export LANG="en_US.UTF8"
rm -rf build/
mkdir build
time lualatex -interaction=nonstopmode -file-line-error -shell-escape -recorder -output-directory="build" -jobname="report" "report.tex"
time biber --isbn-normalise --validate_datamodel build/report
cd build
time makeglossaries report
cd ..
time lualatex -interaction=nonstopmode -file-line-error -shell-escape -recorder -output-directory="build" -jobname="report" "report.tex"
time lualatex -interaction=nonstopmode -file-line-error -shell-escape -recorder -output-directory="build" -jobname="report" "report.tex"
mv build/report.pdf .
cp build/report.glsdefs .
okular report.pdf &
