# SettingPNG
Setting PNG

https://github.com/SEB103/SettingPNG.git

## Git global setup

* git config --global user.name "SB"

## Create a new repository

* git clone https://github.com/SEB103/SettingPNG.git
* cd provis
* touch README.md
* git add README.md
* git commit -m "add README"
* git push -u origin master

## Push an existing folder

* cd existing_folder
* git init
* git remote add origin https://github.com/SEB103/SettingPNG.git
* git add .
* git commit -m "Initial commit"
* git push -u origin master

## Push an existing Git repository

* cd existing_repo
* git remote rename origin old-origin
* git remote add origin https://github.com/SEB103/SettingPNG.git
* git push -u origin --all
* git push -u origin --tags
