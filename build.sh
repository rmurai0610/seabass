#!/usr/bin/env bash
if [ -d "./imgui" ]; then
  echo "Checking if there are latest version of Dear ImGui";
  cd imgui;
  git fetch;
  git checkout `git tag | sort -V | grep -v "\-rc" | tail -1`;
  if [ $? -ne 0  ]; then
    exit 1;
  fi
  cd ..;
else
  git clone https://github.com/ocornut/imgui.git
  cd imgui;
  git checkout `git tag | sort -V | grep -v "\-rc" | tail -1`;
  cd ..;
fi
