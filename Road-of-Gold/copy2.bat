rd /s /q Planet-Viewer\assets
rd /s /q Road-of-Gold\assets
rd /s /q Map-Editor\assets
rd /s /q Assets-Manager\assets
robocopy assets Planet-Viewer\assets /MIR /COPY:D
robocopy assets Road-of-Gold\assets /MIR /COPY:D
robocopy assets Map-Editor\assets /MIR /COPY:D
robocopy assets Assets-Manager\assets /MIR /COPY:D