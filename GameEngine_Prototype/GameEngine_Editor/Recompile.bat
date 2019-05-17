@Echo OFF
REM Arguments: devenv path, solution path, project path.
Echo "Building solution/project file using batch file"
SET PATH=%1
SET SolutionPath=%2
REM Echo Start Time - %Time%
Call Devenv %SolutionPath% /Build Debug /project %3 
REM Echo End Time - %Time%
REM Set /p Wait=Build Process Completed...
REM exit /B