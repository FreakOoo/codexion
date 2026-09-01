#! /bin/bash
# for new project run: git remote add github https://github.com/FreakOoo/name of project 
# then use: git remote set-url github git@github.com:FreakOoo/name of project
# to authenticate run: ssh -T git@github.com
git add .
git commit -m "small update"
git push
git push github master
sleep 2 
clear
tree
git status
sleep 3.5
clear
cowsay -f eyes "ALL DONE !!! You fat bastard, get outta here ;) "
sleep 5
clear
cowsay -f eyes "Come on go away..."
sleep 3
clear
cowsay -f dragon-and-cow "getcho bingjao ass tf outta here"

