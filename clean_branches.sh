#!/bin/bash

for b in $(git branch --format='%(refname:short)' | grep -v main); do
    if ! [[ $(git ls-remote --head origin "refs/heads/$b") ]]; then
        false
#        echo "$b does not exist in origin"
    elif [[ $(git diff "$b" "origin/$b") ]]; then
        false
#        echo "$b differs from origin branch"
    elif ! [[ $(git show "$b" --format=%H --no-notes --no-patch) == $(git show "origin/$b" --format=%H --no-notes --no-patch) ]]; then
        false
#        echo "$b different commit hash with origin"
    else
        git push origin "$b" -u
        git branch -d "$b"
    fi
done
