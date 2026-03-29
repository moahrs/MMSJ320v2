@echo off
setlocal
cd /d d:\Projetos\MMSJ320v2

echo [1/5] Set remote origin URL...
git remote set-url origin https://github.com/moahrs/MMSJ320v2.git 2>nul
if errorlevel 1 git remote add origin https://github.com/moahrs/MMSJ320v2.git

echo [2/5] Apply ignore rules and untrack ignored files if needed...
git rm -r --cached . >nul 2>nul

echo [3/5] Stage all files...
git add .

echo [4/5] Commit...
git commit -m "Integrate MMSJ320v2 workspace; ignore .o/.exe/.prj" >nul 2>nul
if errorlevel 1 echo No new commit created (possibly no staged changes or commit already up to date).

echo [5/5] Push to origin...
git push -u origin HEAD

echo Done.
