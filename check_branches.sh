#!/bin/bash

for b in $(git branch --format='%(refname:short)' | grep -v main); do
    if ! [[ $(git ls-remote --head origin "refs/heads/$b") ]]; then
        echo "$b does not exist in origin"
    elif [[ $(git diff "$b" "origin/$b") ]]; then
        echo "$b differs from origin branch"
    elif ! [[ $(git show "$b" --format=%H --no-notes --no-patch) == $(git show "origin/$b" --format=%H --no-notes --no-patch) ]]; then
        echo "$b different commit hash with origin"
#    else
#        echo "$b can be deleted"
    fi
done
