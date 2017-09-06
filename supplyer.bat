@echo off
chdir $(ProjectDir)
chdir ..
del         /q  PlanetViewer\assets
del         /q  PlanetViewer\map
del         /q  PlanetViewer\config.ini
robocopy    Road_of_Gold\assets     PlanetViewer\assets /MIR /COPY:D    > nul
robocopy    Road_of_Gold\map        PlanetViewer\map /MIR /COPY:D       > nul
copy        Road_of_Gold\config.ini PlanetViewer\config.ini             > nul

del         /q  MapEditor\assets
del         /q  MapEditor\map
del         /q  MapEditor\config.ini
robocopy    Road_of_Gold\assets     MapEditor\assets /MIR /COPY:D       > nul
robocopy    Road_of_Gold\map        MapEditor\map /MIR /COPY:D          > nul
copy        Road_of_Gold\config.ini MapEditor\config.ini                > nul