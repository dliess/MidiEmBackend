#!/bin/bash

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
REPO_DIR=$(readlink -f $SCRIPT_DIR/..)


AUDIO_GROUP_ID=$(getent group audio | awk -F: '{printf $3}')

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
source "$DIR/common.sh"

docker run \
  --privileged \
  --group-add $AUDIO_GROUP_ID \
  --network=host \
  --mount type=bind,source=$HOME/.ssh,target=/home/build-user/.ssh \
  --mount type=bind,source=/dev,target=/dev \
  --mount type=bind,source=/opt,target=/opt \
  --mount type=bind,source=$HOME/.gitconfig,target=/home/build-user/.gitconfig \
  --mount type=bind,source=$HOME/.config/nvim,target=/home/$BUILD_USER/.config/nvim \
  --mount type=bind,source=$HOME/.config/github-copilot,target=/home/$BUILD_USER/.config/github-copilot \
  --mount type=bind,source=$HOME/nvim-linux64,target=/home/$BUILD_USER/nvim-linux64 \
  --mount type=bind,source=$REPO_DIR,target=/home/$BUILD_USER/nomidi-backend \
  -ti $DOCKER_IMAGE_TAG:latest /bin/zsh

## logging in to a running container:
## get id with: docker ps
## docker exec -it <id> /bin/bash
