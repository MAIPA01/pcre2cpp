@echo off
cd .\build\x64-debug-cpp17\_deps
lib.exe /OUT:.\..\..\..\..\pcre2cpp\pcre2cpp.lib pcre2-build\pcre2-8d.lib pcre2-build\pcre2-16d.lib pcre2-build\pcre2-32d.lib fmt-build\fmtd.lib