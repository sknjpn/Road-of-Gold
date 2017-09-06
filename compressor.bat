@echo off
chdir $(ProjectDir)
chdir ..
del         /q  uploadingFiles
robocopy    Road_of_Gold\assets    uploadingFiles\assets /MIR /COPY:D   > nul
robocopy    Road_of_Gold\map       uploadingFiles\map /MIR /COPY:D      > nul
robocopy    Road_of_Gold\engine    uploadingFiles\engine /MIR /COPY:D   > nul
robocopy    PlanetViewer\Engine    uploadingFiles\Engine /MIR /COPY:D   > nul
copy        Road_of_Gold\Road_of_Gold\Release-x86\Road_of_Gold.exe  uploadingFiles\Road_of_Gold_x86.exe > nul
copy        Road_of_Gold\Road_of_Gold\Release-x64\Road_of_Gold.exe  uploadingFiles\Road_of_Gold_x64.exe > nul
copy        Road_of_Gold\MapEditor\Release-x86\MapEditor.exe        uploadingFiles\MapEditor_x86.exe    > nul
copy        Road_of_Gold\MapEditor\Release-x64\MapEditor.exe        uploadingFiles\MapEditor_x64.exe    > nul
copy        Road_of_Gold\Release\PlanetViewer.exe                   uploadingFiles\PlanetViewer_x86.exe > nul
copy        Road_of_Gold\x64\Release\PlanetViewer.exe               uploadingFiles\PlanetViewer_x64.exe > nul
copy        Road_of_Gold\config.ini                                 uploadingFiles\config.ini           > nul