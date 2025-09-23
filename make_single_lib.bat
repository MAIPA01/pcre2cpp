@echo off
cd .\out\build\x64-Debug\_deps\pcre2-build
lib.exe /OUT:.\..\..\..\..\..\pcre2cpp.lib pcre2-8-staticd.lib pcre2-16-staticd.lib pcre2-32-staticd.lib