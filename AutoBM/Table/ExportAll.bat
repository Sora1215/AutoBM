@setlocal enableextensions

FOR %%a IN (*.xlsx) DO (
::echo VespaExporter.exe "%%a"
VespaExporter.exe -exportjson -useconfig -targetPath:TableData "%%a" 
)


FOR %%a IN (*.xlsm) DO (
::echo VespaExporter.exe "%%a"
VespaExporter.exe -exportjson -useconfig -targetPath:TableData "%%a"
)

echo Finished.
pause
